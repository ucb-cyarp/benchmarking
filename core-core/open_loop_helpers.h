#ifndef _H_OPEN_LOOP_HELPERS
    #define _H_OPEN_LOOP_HELPERS

    #include "intrin_bench_default_defines.h"
    #include "results.h"
    #include "mallocHelpers.h"
    #include "reporting_helpers.h"
    #include "fifoless_helpers.h"
    #include "fast_copy.h"

    #include <vector>
    #include <atomic>
    #include <stdlib.h>

    template<typename elementType, typename idType = std::atomic_int32_t, typename indexType = std::atomic_int32_t>
    class OpenLoopBufferArgs : public FifolessConfig{
    public:
        indexType *read_offset_ptr;
        indexType *write_offset_ptr;
        void *array;
        void *local_array_reader;
        std::atomic_flag *start_flag; //Used by the client to signal to the server that it should start writing
        std::atomic_flag *stop_flag; //Used by the client to signal to the server that an error occured and that it should stop
        std::atomic_flag *ready_flag; //Used by the server to signal to the client that it is ready to begin
        int array_length; //In Blocks  //Note: The FIFO Array has an additional block allocated to resolve the Empty/Full Ambiguity
        int64_t max_block_transfers; //The maximum number of blocks to transfer in this test
        int balancing_nops; //The number of NOPs to use for balancing services.  If negative, the server has NOPS, if positive, the client has NOPS
        int blockSize; //The block size (in elementType elements)
        int alignment; //The alignment in bytes of the components within the block (the IDs and the Buffer)
        int core_client; //The core the client is executing on
        int core_server; //The core the server is executing on
        int initialNOPs; //The initial NOPs in the reader and writer.

        FILE* readerInterruptReporter;
        FILE* writerInterruptReporter;

        void printStandaloneTitle(bool report_standalone, std::string title) override; //Prints the standalone title block if standalone results are requested
    protected:
        virtual void printExportNonStandaloneResults(Results &result, bool report_standalone, std::string resultPrintFormatStr, FILE* file, std::ofstream* raw_file) override;
    };

    template<typename elementType, 
         typename idType, 
         typename indexType>
    void OpenLoopBufferArgs<elementType, idType, indexType>::printStandaloneTitle(bool report_standalone, std::string title){
        //Print the title if a standalone report
        #if PRINT_TITLE == 1
        if(report_standalone)
        {
            printf("\n");
            printTitle(title);
            printf("Array Length: %d Blocks, Block Length: %d int32_t Elements, Balancing NOPs: %d, Initial NOPs: %d\n", this->array_length, this->blockSize, this->balancing_nops, this->initialNOPs);
            fflush(stdout);
        }
        #endif
    }

    template<typename elementType, 
         typename idType, 
         typename indexType>
    void OpenLoopBufferArgs<elementType, idType, indexType>::printExportNonStandaloneResults(Results &result, bool report_standalone, std::string resultPrintFormatStr, FILE* file, std::ofstream* raw_file){
            if(!report_standalone){
                //Print the complete results
            double avg_duration_ms = result.avg_duration();
            double stddev_duration_ms = result.stddev_duration();

            #if PRINT_STATS == 1
            printf(resultPrintFormatStr.c_str(), this->array_length, this->blockSize, this->balancing_nops, this->initialNOPs, avg_duration_ms, stddev_duration_ms);
            #endif

            #if WRITE_CSV == 1
            fprintf(file, "%d,%d,%d,%d,%f,%f\n", this->array_length, this->blockSize, this->balancing_nops, this->initialNOPs, avg_duration_ms, stddev_duration_ms);
            result.write_durations_and_benchmark_specific_results(*raw_file, {"", "", ""}, {std::to_string(this->array_length), std::to_string(this->blockSize), std::to_string(this->balancing_nops), std::to_string(this->initialNOPs)}, false);
            #endif
        }
    }

    class FifolessBufferEndCondition : public BenchmarkSpecificResult{
    public:
        int64_t expectedBlockID;
        int64_t startBlockID;
        int64_t endBlockID;
        bool wasErrorSrc;
        bool errored;
        int64_t transaction;

        std::string getTrialResultsHeader() override;
        std::string getTrialResults() override;
        std::string getTrialCSVHeader() override;
        std::string getTrialCSVData() override;

        FifolessBufferEndCondition();
    };

    template<typename elementType, typename atomicIdType>
    void getBlockSizing(int blockLength, int alignment, int &blockArrayBytes, int &blockArrayPaddingBytes, int &blockArrayCombinedBytes,
                        int &idBytes, int &idPaddingBytes, int &idCombinedBytes, int &blockSizeBytes){
        blockArrayBytes = sizeof(elementType)*blockLength;
        blockArrayPaddingBytes = (blockArrayBytes % alignment == 0) ? 0 : alignment - blockArrayBytes % alignment;
        blockArrayCombinedBytes = blockArrayBytes+blockArrayPaddingBytes;
        idBytes = sizeof(atomicIdType);
        idPaddingBytes = (idBytes % alignment == 0) ? 0 : alignment - idBytes % alignment;
        idCombinedBytes = idBytes+idPaddingBytes;
        blockSizeBytes = blockArrayCombinedBytes+idCombinedBytes*2;
    }

    template<typename elementType>
    void getBlockSizing(int blockLength, int alignment, int &blockArrayBytes, int &blockArrayPaddingBytes, int &blockArrayCombinedBytes, int &blockSizeBytes){
        blockArrayBytes = sizeof(elementType)*blockLength;
        blockArrayPaddingBytes = (blockArrayBytes % alignment == 0) ? 0 : alignment - blockArrayBytes % alignment;
        blockArrayCombinedBytes = blockArrayBytes+blockArrayPaddingBytes;
        blockSizeBytes = blockArrayCombinedBytes;
    }

    //The vectors include:
    //shared_array_locs:    the array for each communicating buffer
    //shared_write_id_locs: the write indexes for each buffer
    //shared_read_id_locs:  The read indexes for each buffer
    //ready_flags:          The first flag is for the primary and should be a dummy.  The next (cpus-1) flags are real.  The next (cpus-1)*(cpus-1) flags are dummies
    //start_flags:          The first flag is for the primary and is real.  The next num_buffers flags are dummies.
    //stop_flag:            There is a single stop flag shared by all threads which is real

    template<typename elementType, typename atomicIdType, typename atomicIndexType>
    size_t openLoopAllocate(std::vector<elementType*> &shared_array_locs, std::vector<elementType*> &local_array_locs, std::vector<atomicIndexType*> &shared_write_id_locs, std::vector<atomicIndexType*> &shared_read_id_locs, std::vector<std::atomic_flag*> &ready_flags, std::vector<std::atomic_flag*> &start_flags, std::atomic_flag* &stop_flag, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, std::vector<int> cpus, int alignment, bool circular, bool include_dummy_flags){
        //Find the largest array to allocate
        int maxBufferSize = 0;
        int maxLocalBufferSize = 0;

        for(const size_t& array_length : array_lengths){
            for(const int32_t& block_length : block_lengths){
                int blockArrayBytes;
                int blockArrayPaddingBytes;
                int blockArrayCombinedBytes;
                int idBytes;
                int idPaddingBytes;
                int idCombinedBytes;
                int blockSizeBytes;

                getBlockSizing<elementType, atomicIdType>(block_length, alignment, blockArrayBytes, blockArrayPaddingBytes, 
                blockArrayCombinedBytes, idBytes, idPaddingBytes, idCombinedBytes, blockSizeBytes);

                //Note that there is 1 additional block allocated in the array
                int bufferSize = blockSizeBytes*(array_length+1);

                int localBufferSize = block_length*sizeof(elementType);

                if(bufferSize>maxBufferSize){
                    maxBufferSize = bufferSize;
                }

                if(localBufferSize>maxLocalBufferSize){
                    maxLocalBufferSize = localBufferSize;
                }
            }
        }

        int num_buffers = circular ? cpus.size() : cpus.size()-1;
        
        //Initialize
        for(int buffer = 0; buffer<num_buffers; buffer++){
            size_t amountToAlloc = maxBufferSize;
            if(amountToAlloc % CACHE_LINE_SIZE != 0){
                amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
            }
            elementType *shared_array_loc = (elementType*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpus[buffer]);

            size_t amountToAllocLocal = maxLocalBufferSize+FAST_COPY_ALIGNED_PADDING; //Add padding for fast_copy_aligned
            if(amountToAllocLocal % CACHE_LINE_SIZE != 0){
                amountToAllocLocal += (CACHE_LINE_SIZE - (amountToAllocLocal % CACHE_LINE_SIZE));
            }
            elementType *local_array_loc = (elementType*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocLocal, cpus[(buffer+1)%cpus.size()]);

            size_t amountToAllocCursors = sizeof(atomicIndexType);
            if(amountToAllocCursors % CACHE_LINE_SIZE != 0){
                amountToAllocCursors += (CACHE_LINE_SIZE - (amountToAllocCursors % CACHE_LINE_SIZE));
            }
            atomicIndexType *shared_write_id_loc = (atomicIndexType*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpus[buffer]);
            atomicIndexType *shared_read_id_loc = (atomicIndexType*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpus[(buffer+1)%cpus.size()]);
            atomicIndexType *shared_write_id_loc_constructed = new (shared_write_id_loc) atomicIndexType();
            std::atomic_init(shared_write_id_loc, 0); //Init of the shared IDs occurs here
            if(!std::atomic_is_lock_free(shared_write_id_loc)){
                printf("Atomic is not lock free and was expected to be");
                exit(1);
            }
            atomicIndexType *shared_read_id_loc_constructed = new (shared_read_id_loc) atomicIndexType();
            std::atomic_init(shared_read_id_loc, 0); //Now Initialized in reset
            if(!std::atomic_is_lock_free(shared_read_id_loc)){
                printf("Atomic is not lock free and was expected to be");
                exit(1);
            }

            shared_array_locs.push_back(shared_array_loc);
            local_array_locs.push_back(local_array_loc);
            shared_write_id_locs.push_back(shared_write_id_loc);
            shared_read_id_locs.push_back(shared_read_id_loc);

            //Init of array occurs in the reset function because the location of the atomics moves as the block size changes
        }

        //Allocate flags
        for(int i = include_dummy_flags ? 0 : 1; i<cpus.size(); i++){
            std::atomic_flag *ready_flag = (std::atomic_flag*) aligned_alloc_core(CACHE_LINE_SIZE, sizeof(std::atomic_flag), cpus[i]);
            std::atomic_flag *ready_flag_constructed = new (ready_flag) std::atomic_flag();
            ready_flags.push_back(ready_flag);
        }

        if(include_dummy_flags){
            for(int i = 1; i<cpus.size(); i++){
                for(int j = 0; j<cpus.size()-1; j++){
                    std::atomic_flag *ready_flag = (std::atomic_flag*) aligned_alloc_core(CACHE_LINE_SIZE, sizeof(std::atomic_flag), cpus[i]);
                    std::atomic_flag *ready_flag_constructed = new (ready_flag) std::atomic_flag();
                    ready_flags.push_back(ready_flag);
                }
            }
        }

        for(int i = 0; i < (include_dummy_flags ? cpus.size() : 1); i++){
            std::atomic_flag *start_flag = (std::atomic_flag*) aligned_alloc_core(CACHE_LINE_SIZE, sizeof(std::atomic_flag), cpus[i]);
            std::atomic_flag *start_flag_constructed = new (start_flag) std::atomic_flag();
            start_flags.push_back(start_flag);
        }

        stop_flag = (std::atomic_flag*) aligned_alloc_core(CACHE_LINE_SIZE, sizeof(std::atomic_flag), cpus[0]);
        std::atomic_flag *stop_flag_constructed = new (stop_flag) std::atomic_flag();

        return maxBufferSize;
    }

    template<typename atomicIndexType>
    void destructSharedIDs(std::vector<atomicIndexType*> &shared_write_id_locs, std::vector<atomicIndexType*> &shared_read_id_locs, std::vector<std::atomic_flag*> &ready_flags, std::vector<std::atomic_flag*> &start_flags, std::atomic_flag* &stop_flag){
        for(int i = 0; i<shared_write_id_locs.size(); i++){
            shared_write_id_locs[i]->~atomicIndexType();
        }

        for(int i = 0; i<shared_read_id_locs.size(); i++){
            shared_read_id_locs[i]->~atomicIndexType();
        }

        for(int i = 0; i<ready_flags.size(); i++){
            ready_flags[i]->~atomic_flag();
        }

        for(int i = 0; i<start_flags.size(); i++){
            start_flags[i]->~atomic_flag();
        }

        stop_flag->~atomic_flag();
    }

    template<typename T>
    void freeVectorContents(std::vector<T*> vec){
        for(unsigned long i = 0; i<vec.size(); i++){
            free(vec[i]);
        }
    }

#endif
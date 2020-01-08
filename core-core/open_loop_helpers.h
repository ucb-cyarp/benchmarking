#ifndef _H_OPEN_LOOP_HELPERS
    #define _H_OPEN_LOOP_HELPERS

    #include "intrin_bench_default_defines.h"
    #include "results.h"
    #include "mallocHelpers.h"

    #include <vector>
    #include <atomic>
    #include <stdlib.h>

    class OpenLoopBufferEndCondition : public BenchmarkSpecificResult{
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

        OpenLoopBufferEndCondition();
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

    //The vectors include:
    //shared_array_locs:    the array for each communicating buffer
    //shared_write_id_locs: the write indexes for each buffer
    //shared_read_id_locs:  The read indexes for each buffer
    //ready_flags:          The first flag is for the primary and should be a dummy.  The next (cpus-1) flags are real.  The next (cpus-1)*(cpus-1) flags are dummies
    //start_flags:          The first flag is for the primary and is real.  The next num_buffers flags are dummies.
    //stop_flag:            There is a single stop flag shared by all threads which is real

    template<typename elementType, typename atomicIdType, typename atomicIndexType>
    size_t openLoopAllocate(std::vector<elementType*> &shared_array_locs, std::vector<atomicIndexType*> &shared_write_id_locs, std::vector<atomicIndexType*> &shared_read_id_locs, std::vector<std::atomic_flag*> &ready_flags, std::vector<std::atomic_flag*> &start_flags, std::atomic_flag* &stop_flag, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, std::vector<int> cpus, int alignment, bool circular, bool include_dummy_flags){
        //Find the largest array to allocate
        int maxBufferSize = 0;

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

                    if(bufferSize>maxBufferSize){
                        maxBufferSize = bufferSize;
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

            size_t amountToAllocCursors = sizeof(atomicIndexType);
            if(amountToAllocCursors % CACHE_LINE_SIZE != 0){
                amountToAllocCursors += (CACHE_LINE_SIZE - (amountToAllocCursors % CACHE_LINE_SIZE));
            }
            atomicIndexType *shared_write_id_loc = (atomicIndexType*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpus[buffer]);
            atomicIndexType *shared_read_id_loc = (atomicIndexType*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpus[buffer%num_buffers]);
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
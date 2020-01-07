#ifndef _H_OPEN_LOOP_HELPERS
    #define _H_OPEN_LOOP_HELPERS

    #include "results.h"

    #include <vector>
    #include <atomic>
    #include <stdlib.h>

    class OpenLoopBufferEndCondition : public BenchmarkSpecificResult{
    public:
        int64_t expectedBlockID;
        int64_t startBlockID;
        int64_t endBlockID;
        bool wasErrorSrc;

        std::string getTrialResultsHeader() override;
        std::string getTrialResults() override;
        std::string getTrialCSVHeader() override;
        std::string getTrialCSVData() override;
    };

    template<typename elementType, typename atomicIndexType>
    void getBlockSizing(int blockLength, int allignement, int &blockArrayBytes, int &blockArrayPaddingBytes, int &blockArrayCombinedBytes,
                        int &idBytes, int &idPaddingBytes, int &idCombinedBytes, int &blockSizeBytes){
        blockArrayBytes = sizeof(elementType)*blockLenth;
        blockArrayPaddingBytes = (blockArrayBytes % alignment == 0) ? 0 : alignment - blockArrayBytes % alignment;
        blockArrayCombinedBytes = blockArrayBytes+blockArrayPaddingBytes;
        idBytes = sizeof(idType);
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

    template<typename elementType, typename atomicIndexType>
    size_t openLoopAllocate(std::vector<elementType*> &shared_array_locs, std::vector<atomicIndexType*> &shared_write_id_locs, std::vector<atomicIndexType*> &shared_read_id_locs, std::vector<std::atomic_flag*> ready_flags, std::vector<std::atomic_flag*> start_flags, std::atomic_flag* &stop_flag, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, std::vector<int> cpus, int alignment, bool circular, bool include_dummy_flags){
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

                    getBlockSizing<elementType, atomicIndexType>(block_length, alignement, blockArrayBytes, blockArrayPaddingBytes, 
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
            elementType *shared_write_id_loc = (atomicIndexType*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpus[buffer]);
            elementType *shared_read_id_loc = (atomicIndexType*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpus[buffer%num_buffers]);
            std::atomic_init(*shared_write_id_loc, 0);
            if(!std::atomic_is_lock_free(*shared_write_id_loc)){
                printf("Atomic is not lock free and was expected to be");
                exit(1);
            }
            std::atomic_init(*shared_read_id_loc, 0);
            if(!std::atomic_is_lock_free(*shared_read_id_loc)){
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
            ready_flags.push_back(ready_flag);
        }

        if(include_dummy_flags){
            for(int i = 1; i<cpus.size(); i++){
                for(int j = 0; j<cpus.size()-1; j++){
                    std::atomic_flag *ready_flag = (std::atomic_flag*) aligned_alloc_core(CACHE_LINE_SIZE, sizeof(std::atomic_flag), cpus[i]);
                    ready_flags.push_back(ready_flag);
                }
            }
        }

        for(int i = 0; i < (include_dummy_flags ? cpus.size() : 1); i++){
            std::atomic_flag start_flag = (std::atomic_flag*) aligned_alloc_core(CACHE_LINE_SIZE, sizeof(std::atomic_flag), cpus[i]);
            start_flags.push_back(start_flag);
        }

        stop_flag = (std::atomic_flag*) aligned_alloc_core(CACHE_LINE_SIZE, sizeof(std::atomic_flag), cpus[i]);

        return amountToAlloc;
    }

    template<typename T>
    void freeVectorContents(std::vector<T*> vec){
        for(unsigned long i = 0; i<vec.size(); i++){
            free(vec[i]);
        }
    }

#endif
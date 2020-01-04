#ifndef _H_OPEN_LOOP_HELPERS
    #define _H_OPEN_LOOP_HELPERS

    #include <vector>
    #include <atomic>
    #include <stdlib.h>

    template<typename elementType, typename atomicIndexType>
    void getBlockSizing(int blockLength, int alignement, int &blockArrayBytes, int &blockArrayPaddingBytes, int &blockArrayCombinedBytes,
                        int &idBytes, int &idPaddingBytes, int &idCombinedBytes, int &blockSizeBytes){
        blockArrayBytes = sizeof(elementType)*blockLenth;
        blockArrayPaddingBytes = (blockArrayBytes % allignment == 0) ? 0 : allignment - blockArrayBytes % allignment;
        blockArrayCombinedBytes = blockArrayBytes+blockArrayPaddingBytes;
        idBytes = sizeof(idType);
        idPaddingBytes = (idBytes % allignment == 0) ? 0 : allignment - idBytes % allignment;
        idCombinedBytes = idBytes+idPaddingBytes;
        blockSizeBytes = blockArrayCombinedBytes+idCombinedBytes*2;
    }

    //The vectors include:
    //shared_array_locs:    the array for each communicating buffer
    //shared_write_id_locs: the write indexes for each buffer
    //shared_read_id_locs:  The read indexes for each buffer
    //ready_flags:          The first flag is for the primary and should be a dummy.  The next (num_buffers-1) flags are real.  The next (num_buffers-1)*(num_buffers-1) flags are dummies
    //start_flags:          The first flag is for the primary and is real.  The next num_buffers flags are dummies.
    //stop_flag:            There is a single stop flag shared by all threads which is real

    template<typename elementType, typename atomicIndexType>
    size_t openLoopCycle2Allocate(std::vector<elementType*> &shared_array_locs, std::vector<atomicIndexType*> &shared_write_id_locs, std::vector<atomicIndexType*> &shared_read_id_locs, std::vector<std::atomic_flag*> ready_flags, std::vector<std::atomic_flag*> start_flags, std::atomic_flag* &stop_flag, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, int cpu_a, int cpu_b, int num_buffers, bool include_dummy_flags){
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
        
        //Initialize
        for(int buffer = 0; buffer<buffers; buffer++){
            size_t amountToAlloc = maxBufferSize;
            if(amountToAlloc % CACHE_LINE_SIZE != 0){
                amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
            }
            elementType *shared_array_loc = (elementType*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);

            size_t amountToAllocCursors = sizeof(atomicIndexType);
            if(amountToAllocCursors % CACHE_LINE_SIZE != 0){
                amountToAllocCursors += (CACHE_LINE_SIZE - (amountToAllocCursors % CACHE_LINE_SIZE));
            }
            elementType *shared_write_id_loc = (atomicIndexType*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_a);
            elementType *shared_read_id_loc = (atomicIndexType*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_b);
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



        return amountToAlloc;
    }

#endif
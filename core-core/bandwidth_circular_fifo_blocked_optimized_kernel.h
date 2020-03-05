#ifndef _H_BANDWIDTH_CIRCULAR_FIFO_BLOCKED_OPTIMIZED_KERNEL
    #define _H_BANDWIDTH_CIRCULAR_FIFO_BLOCKED_OPTIMIZED_KERNEL
    #include <stdint.h>
    #include <cstddef>
    #include <atomic>
    #include "fifo_helpers.h"

    class BandwidthCircularFifoBlockedOptimizedKernelArgs
    {
        public:
            int32_t* array_shared_ptr;
            int32_t* local_array_reader;
            std::atomic_int32_t* write_pos_shared_ptr;
            std::atomic_int32_t* read_pos_shared_ptr;
            int32_t block_length;
            size_t length;
    };

    void* bandwidth_circular_fifo_blocked_optimized_server_kernel(void* arg);

    void* bandwidth_circular_fifo_blocked_optimized_client_kernel(void* arg);

    void* bandwidth_circular_fifo_blocked_optimized_kernel_reset(void* arg);

    //In this case, array_length is in blocks
    template<typename elementType, typename atomicIndexType>
    size_t fifoOptimizedAllocate(elementType* &shared_array_loc, elementType* &local_array_loc, atomicIndexType* &shared_write_id_loc, atomicIndexType* &shared_read_id_loc, int array_length, int block_length, int cpu_a, int cpu_b){
        size_t size = fifoAllocate(shared_array_loc, shared_write_id_loc, shared_read_id_loc, array_length*block_length, cpu_a, cpu_b); //Array length units changed here to elements.  Outside, array length is in blocks

        //Allocate the local array (only a block's worth)
        size_t amountToAlloc = block_length*sizeof(elementType);
        if(amountToAlloc % CACHE_LINE_SIZE != 0){
            amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
        }
        local_array_loc = (elementType*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_b);

        return size;
    }
    
#endif
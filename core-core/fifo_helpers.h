#ifndef FIFO_HELPERS_H
    #define FIFO_HELPERS_H

    #include <atomic>
    #include <vector>
    #include <memory>
    #include "intrin_bench_default_defines_and_imports_cpp.h"
    #include "mallocHelpers.h"

    /**
     * The FIFO benchmarks share the same allocation scheme so it could be factored out
     * 
     * Note that the atomicIndexType must be a stdatomic type
     */
    template<typename elementType, typename atomicIndexType>
    size_t fifoAllocate(elementType **shared_array_loc, atomicIndexType** shared_write_id_loc, atomicIndexType** shared_read_id_loc, int array_length, int cpu_a, int cpu_b){
        //Initialize
        size_t amountToAlloc = array_length*sizeof(elementType);
        if(amountToAlloc % CACHE_LINE_SIZE != 0){
            amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
        }
        *shared_array_loc = (elementType*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);

        size_t amountToAllocCursors = sizeof(atomicIndexType);
        if(amountToAllocCursors % CACHE_LINE_SIZE != 0){
            amountToAllocCursors += (CACHE_LINE_SIZE - (amountToAllocCursors % CACHE_LINE_SIZE));
        }
        *shared_write_id_loc = (atomicIndexType*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_a);
        *shared_read_id_loc = (atomicIndexType*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_b);
        std::atomic_int32_t* shared_write_id_loc_constructed = new (shared_write_id_loc) std::atomic_int32_t();
        std::atomic_init(*shared_write_id_loc, 0);
        if(!std::atomic_is_lock_free(*shared_write_id_loc)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }
        std::atomic_int32_t* shared_read_id_loc_constructed = new (shared_read_id_loc) std::atomic_int32_t();
        std::atomic_init(*shared_read_id_loc, 0);
        if(!std::atomic_is_lock_free(*shared_read_id_loc)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }

        //Init to 0
        for(size_t i = 0; i < array_length; i++)
        {
            (*shared_array_loc)[i] = 0;
        }

        return amountToAlloc;
    }

    int countNumberOfFIFOExperiments(std::vector<size_t> array_lengths, std::vector<int32_t> writes_per_transactions);

#endif
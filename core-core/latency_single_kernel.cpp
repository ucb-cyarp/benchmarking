/*
 * Latency (Single Element -> Both Threads Read and Write to Same Address)
 *     Two thread are run on different cores, each accessing a single 
 *     element (4 byte) of shared memory.
 * 
 *     The thread logic is as such:
 *         The thread starts a local timer and then enters a loop
 * 
 *         The thread has a local counter and a pointer to the shared memory location
 *         The thread is polling on the shared memory location until it is greater than its counter (should be 1 greater than its counter)
 *         When this happens, it sets its counter to the value in shared memory +1 (should be +2 from previous counter value)
 *         The thread then writes this value to the shared memory location
 *         If the new counter value is greater than a threshold, the loop terminates
 * 
 *         Note: Counter represents the number of core-core transfers (not round trip)
 * 
 *         When the loop terminates, the timer is stopped.
 *         The round trip latency is calculated as the total time / (counter / 2)
 *         The unidirectional latency is calculated as the total time / counter
 */

#include "latency_single_kernel.h"
#include "intrin_bench_default_defines_and_imports_cpp.h"
#include <cstddef>
#include <atomic>

/*
 * Resets shared ptr to 0
 */
void* latency_single_kernel_reset(void* arg)
{
    LatencySingleKernelResetArgs* args = (LatencySingleKernelResetArgs*) arg;
    std::atomic_int32_t* shared_ptr_int = args->shared_ptr;

    std::atomic_store_explicit(shared_ptr_int, 0, std::memory_order_release);

    return NULL;
}

/*
 * Arg contains a pointer to the shared memory location and its initial counter value (-1 for server, 0 for client)
 * 
 * returns nothing (reporting handled by server wrapper)
 */
void* latency_single_kernel(void* arg)
{
    //Get the shared pointer and the initial counter value
    LatencySingleKernelArgs* kernel_args = (LatencySingleKernelArgs*) arg;
    std::atomic_int32_t* shared_ptr = kernel_args->shared_ptr;
    int32_t counter = kernel_args->init_counter;

    //Execute until the specified number of transactions has occured
    while(counter < STIM_LEN)
    {
        if(std::atomic_load_explicit(shared_ptr, std::memory_order_acquire) == (counter+1))
        {
            counter+=2;
            std::atomic_store_explicit(shared_ptr, counter, std::memory_order_release);
        }

        //Poll on the memory location until the above condition is met or the counter exceeds STIM_LEN
    }

    return NULL;
}

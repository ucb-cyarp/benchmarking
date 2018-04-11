/*
 * Latency (Dual Elements -> Each Thread Reads from One Address and Writes to Another, No Thread Reads/Writes to Same Address)
 *     Two thread are run on different cores, each accessing 2 
 *     elements (1 byte, 2 byte, 4 byte, 8 byte) of shared memory.
 *     Each thead is the primary owner of one of these elements.
 * 
 *     The thread logic is as such:
 *         The thread starts a local timer and then enters a loop
 * 
 *         The thread has a local counter and a pointer to its own piece of shared memory and a pointer to the other thread's piece of shared memory
 *         The thread is polling on the other thread's memory location until it is greater than its counter (should be 1 greater than its counter)
 *         When this happens, it sets its counter to the value in the other thread's shared memory +1 (should be +2 from previous counter value)
 *         The thread then writes this value to its own shared memory location
 *         If the new counter value is greater than a threshold, the loop terminates
 * 
 *         When the loop terminates, the timer is stopped.
 *         The round trip latency is calculated as the total time / (counter / 2)
 *         The unidirectional latency is calculated as the total time / counter
 * 
 */

#include "latency_dual_kernel.h"
#include "intrin_bench_default_defines.h"

/*
 * Resets shared ptr to 0
 */
void* latency_dual_kernel_reset(void* arg)
{
    LatencyDualKernelResetArgs* args = (LatencyDualKernelResetArgs*) arg;
    volatile int32_t* shared_ptr_a_int = args->shared_ptr_a;
    volatile int32_t* shared_ptr_b_int = args->shared_ptr_b;

    *shared_ptr_a_int = 0;
    *shared_ptr_b_int = 0;

    return NULL;
}

/*
 * Arg contains a pointer to the shared memory location and its initial counter value (-1 for server, 0 for client)
 * 
 * returns nothing (reporting handled by server wrapper)
 */
void* latency_dual_kernel(void* arg)
{
    //Get the shared pointer and the initial counter value
    LatencyDualKernelArgs* kernel_args = (LatencyDualKernelArgs*) arg;
    volatile int32_t* my_shared_ptr = kernel_args->my_shared_ptr;
    volatile int32_t* other_shared_ptr = kernel_args->other_shared_ptr;
    int32_t counter = kernel_args->init_counter;

    //Execute until the specified number of transactions has occured
    while(counter < STIM_LEN)
    {
        if(*other_shared_ptr > counter)
        {
            counter = *other_shared_ptr+1;
            *my_shared_ptr = counter;
        }

        //Poll on the memory location until the above condition is met or the counter exceeds STIM_LEN
    }

    return NULL;
}
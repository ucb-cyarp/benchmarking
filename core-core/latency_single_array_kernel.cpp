/*
 * Latency Multiple Elements
 *       Very similar to the latency test except that additional shared memory is more than a single element.
 *       When polling on the array, all elements must be greater than the counter before the counter is incremented and new values are written to shared memory.
 *       The recieving node poles the first memory location until it has the expected value.  It then begins polling the next value.  It does this until it 
 *       reaches the last address
 * 
 *       Note: This may not measure the true peak bandwidth if cache coherence does not guarentee changes are represented in order.
 * 
 *       Note: This also does not account for pipelining of computation and data transfer (ie. if 2 elements can be used while the rest are transfering in)
 *                   * One way to answer this problem may be to have a counter of number of times we need to pole for each element
 */

#include "latency_single_array_kernel.h"
#include "intrin_bench_default_defines_and_imports_cpp.h"
#include <atomic>

/*
 * Resets shared ptr array to 0
 */
void* latency_single_array_kernel_reset(void* arg)
{
    LatencySingleArrayKernelResetArgs* args = (LatencySingleArrayKernelResetArgs*) arg;

    std::atomic_int32_t* shared_ptr_int = args->shared_ptr;
    size_t length = args->length;

    for(size_t i = 0; i<length; i++)
    {
        std::atomic_store_explicit(shared_ptr_int+i, 0, std::memory_order_relaxed);
    }

    //Will allow updates to occur in any order (std::memory_order_relaxed) with the restriction that each must be atomic
    //The fence is required to force the relaxed atomic operations to complete
    std::atomic_thread_fence(std::memory_order_release);

    return NULL;
}

/*
 * Resets shared ptr array to 0
 * 
 * Version for when fanout or fanin is being tested
 */
void* latency_single_array_join_kernel_reset(void* arg)
{
    LatencySingleArrayJoinKernelResetArgs* args = (LatencySingleArrayJoinKernelResetArgs*) arg;

    std::atomic_int32_t* shared_ptr_int_a = args->shared_ptr_a;
    size_t length_a = args->length_a;

    std::atomic_int32_t* shared_ptr_int_b = args->shared_ptr_b;
    size_t length_b = args->length_b;

    for(size_t i = 0; i<length_a; i++)
    {
        std::atomic_store_explicit(shared_ptr_int_a+i, 0, std::memory_order_relaxed);
    }

    for(size_t i = 0; i<length_b; i++)
    {
        std::atomic_store_explicit(shared_ptr_int_b+i, 0, std::memory_order_relaxed);
    }

    //TODO: Compare the use of stdatomic fence to an array of stdatomics
    std::atomic_thread_fence(std::memory_order_release);

    return NULL;
}

/*
 * Arg contains a pointer to the shared memory location, the length of the array, and its initial counter value (-1 for server, 0 for client)
 * 
 * returns nothing (reporting handled by server wrapper)
 */
void* latency_single_array_kernel(void* arg)
{
    //Get the shared pointer and the initial counter value
    LatencySingleArrayKernelArgs* kernel_args = (LatencySingleArrayKernelArgs*) arg;
    std::atomic_int32_t* shared_ptr = kernel_args->shared_ptr;
    size_t length = kernel_args->length;
    int32_t counter = kernel_args->init_counter;

    //Execute until the specified number of transactions has occured
    size_t index = 0;

    while(counter < STIM_LEN)
    {
        //Check all of the 
        if(std::atomic_load_explicit(shared_ptr+index, std::memory_order_acquire) == (counter+1))
        {
            //The current location has incremented
            //Check the next one
            index++;

            if(index >= length) //>= length and not >=length-1 because index is incremented unconditionally
            {
                //Checked the last element in the array
                //Can increment the counter now
                counter+=2;

                //Increment the entire array
                for(size_t i = 0; i<length; i++)
                {
                    std::atomic_store_explicit(shared_ptr+i, counter, std::memory_order_relaxed); //Allow writes to the array to occur out of order so long as individual writes are atomic.  Fences will force all to complete before next transacton
                }

                //TODO: Compare the use of stdatomic fence to an array of stdatomics
                std::atomic_thread_fence(std::memory_order_release);

                //Reset index for next round
                index = 0;
            }
        }

        //Poll on the memory location until the above condition is met or the counter exceeds STIM_LEN
    }

    return NULL;
}

/*
 * Arg contains a pointer to the shared memory location, the length of the array, and its initial counter value (-1 for server, 0 for client)
 * 
 * This version is used for fanout or fanin testing
 * 
 * returns nothing (reporting handled by server wrapper)
 */
void* latency_single_array_join_kernel(void* arg)
{
    //Get the shared pointer and the initial counter value
    LatencySingleArrayJoinKernelArgs* kernel_args = (LatencySingleArrayJoinKernelArgs*) arg;
    std::atomic_int32_t* shared_ptr_a = kernel_args->shared_ptr_a;
    size_t length_a = kernel_args->length_a;
    int32_t counter_a = kernel_args->init_counter_a;

    std::atomic_int32_t* shared_ptr_b = kernel_args->shared_ptr_b;
    size_t length_b = kernel_args->length_b;
    int32_t counter_b = kernel_args->init_counter_b;

    //Execute until the specified number of transactions has occured
    size_t index_a = 0;
    size_t index_b = 0;

    while(counter_a < STIM_LEN || counter_b < STIM_LEN)
    {
        //Handle Connection A
        if(counter_a < STIM_LEN) //Check because loop will run until both connections finish
        {
            //Check all of the 
            if(std::atomic_load_explicit(shared_ptr_a+index_a, std::memory_order_acquire) == (counter_a+1))
            {
                //The current location has incremented
                //Check the next one
                index_a++;

                if(index_a >= length_a) //>= length and not >=length-1 because index is incremented unconditionally
                {
                    //Checked the last element in the array
                    //Can increment the counter now
                    counter_a+=2;

                    //Increment the entire array
                    for(size_t i = 0; i<length_a; i++)
                    {
                        std::atomic_store_explicit(shared_ptr_a+i, counter_a, std::memory_order_relaxed);
                    }
                    
                    //TODO: Compare the use of stdatomic fence to an array of stdatomics
                    std::atomic_thread_fence(std::memory_order_release);

                    //Reset index for next round
                    index_a = 0;
                }
            }
        }

        //Handle Connection B
        if(counter_b < STIM_LEN) //Check because loop will run until both connections finish
        {
            //TODO: Compare the use of stdatomic fence to an array of stdatomics
            std::atomic_thread_fence(std::memory_order_acquire);

            //Check all of the 
            if(std::atomic_load_explicit(shared_ptr_b+index_b, std::memory_order_acquire) == (counter_b+1))
            {
                //The current location has incremented
                //Check the next one
                index_b++;

                if(index_b >= length_b) //>= length and not >=length-1 because index is incremented unconditionally
                {
                    //Checked the last element in the array
                    //Can increment the counter now
                    counter_b+=2;

                    //Increment the entire array
                    for(size_t i = 0; i<length_b; i++)
                    {
                        std::atomic_store_explicit(shared_ptr_b+i, counter_b, std::memory_order_relaxed);
                    }

                    //TODO: Compare the use of stdatomic fence to an array of stdatomics
                    std::atomic_thread_fence(std::memory_order_release);

                    //Reset index for next round
                    index_b = 0;
                }
            }
        }

        //Poll on the memory location until the above condition is met or the counter exceeds STIM_LEN
    }

    return NULL;
}
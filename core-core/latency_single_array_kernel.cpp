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
#include "intrin_bench_default_defines.h"

/*
 * Resets shared ptr array to 0
 */
void* latency_single_array_kernel_reset(void* arg)
{
    LatencySingleArrayKernelResetArgs* args = (LatencySingleArrayKernelResetArgs*) arg;

    volatile int32_t* shared_ptr_int = args->shared_ptr;
    size_t length = args->length;

    for(size_t i = 0; i<length; i++)
    {
        shared_ptr_int[i] = 0;
    }

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
    volatile int32_t* shared_ptr = kernel_args->shared_ptr;
    size_t length = kernel_args->length;
    int32_t counter = kernel_args->init_counter;

    //Execute until the specified number of transactions has occured
    size_t index = 0;

    while(counter < STIM_LEN)
    {
        //Check all of the 
        if(shared_ptr[index] == (counter+1))
        {
            //The current location has incremented
            //Check the next one
            index++;

            if(index >= length) //>= length and not >=length-1 because index is incremented unconditionally
            {
                //Checked the last element in the array
                //Can increment the counter now
                counter = shared_ptr[index-1]+1;

                //Increment the entire array
                for(size_t i = 0; i<length; i++)
                {
                    shared_ptr[i] = counter;
                }

                //Reset index for next round
                index = 0;
            }
        }

        //Poll on the memory location until the above condition is met or the counter exceeds STIM_LEN
    }

    return NULL;
}
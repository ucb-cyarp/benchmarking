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

#include "latency_dual_array_kernel.h"
#include "intrin_bench_default_defines.h"

/*
 * Resets shared ptr array to 0
 */
void* latency_dual_array_kernel_reset(void* arg)
{
    LatencyDualArrayKernelResetArgs* args = (LatencyDualArrayKernelResetArgs*) arg;

    volatile int32_t* shared_ptr_a_int = args->shared_ptr_a;
    volatile int32_t* shared_ptr_b_int = args->shared_ptr_b;
    size_t length = args->length;

    for(size_t i = 0; i<length; i++)
    {
        shared_ptr_a_int[i] = 0;
        shared_ptr_b_int[i] = 0;
    }

    return NULL;
}

/*
 * Resets shared ptr array to 0
 * 
 * Used for fan-in/fan-out tests
 */
void* latency_dual_array_join_kernel_reset(void* arg)
{
    LatencyDualArrayJoinKernelResetArgs* args = (LatencyDualArrayJoinKernelResetArgs*) arg;

    volatile int32_t* shared_ptr_a_int = args->shared_ptr_a;
    volatile int32_t* shared_ptr_b_int = args->shared_ptr_b;
    size_t length_ab = args->length_ab;

    for(size_t i = 0; i<length_ab; i++)
    {
        shared_ptr_a_int[i] = 0;
        shared_ptr_b_int[i] = 0;
    }

    volatile int32_t* shared_ptr_c_int = args->shared_ptr_c;
    volatile int32_t* shared_ptr_d_int = args->shared_ptr_d;
    size_t length_cd = args->length_cd;

    for(size_t i = 0; i<length_cd; i++)
    {
        shared_ptr_c_int[i] = 0;
        shared_ptr_d_int[i] = 0;
    }

    return NULL;
}

/*
 * Arg contains a pointer to the shared memory location, the length of the array, and its initial counter value (-1 for server, 0 for client)
 * 
 * returns nothing (reporting handled by server wrapper)
 */
void* latency_dual_array_kernel(void* arg)
{
    //Get the shared pointer and the initial counter value
    LatencyDualArrayKernelArgs* kernel_args = (LatencyDualArrayKernelArgs*) arg;
    volatile int32_t* my_shared_ptr = kernel_args->my_shared_ptr;
    volatile int32_t* other_shared_ptr = kernel_args->other_shared_ptr;
    size_t length = kernel_args->length;
    int32_t counter = kernel_args->init_counter;

    //Execute until the specified number of transactions has occured
    size_t index = 0;

    while(counter < STIM_LEN)
    {
        //Check all of the 
        if(other_shared_ptr[index] == (counter+1))
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
                    my_shared_ptr[i] = counter;
                }

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
 * This version is used for fan-in/fan-out tests
 * 
 * returns nothing (reporting handled by server wrapper)
 */
void* latency_dual_array_join_kernel(void* arg)
{
    //Get the shared pointer and the initial counter value
    LatencyDualArrayJoinKernelArgs* kernel_args = (LatencyDualArrayJoinKernelArgs*) arg;
    volatile int32_t* my_shared_ptr_a = kernel_args->my_shared_ptr_a;
    volatile int32_t* other_shared_ptr_a = kernel_args->other_shared_ptr_a;
    size_t length_a = kernel_args->length_a;
    int32_t counter_a = kernel_args->init_counter_a;

    volatile int32_t* my_shared_ptr_b = kernel_args->my_shared_ptr_b;
    volatile int32_t* other_shared_ptr_b = kernel_args->other_shared_ptr_b;
    size_t length_b = kernel_args->length_b;
    int32_t counter_b = kernel_args->init_counter_b;

    //Execute until the specified number of transactions has occured
    size_t index_a = 0;
    size_t index_b = 0;

    while(counter_a < STIM_LEN || counter_b < STIM_LEN)
    {
        if(counter_a < STIM_LEN)
        {
            //Check all of the 
            if(other_shared_ptr_a[index_a] == (counter_a+1))
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
                        my_shared_ptr_a[i] = counter_a;
                    }

                    //Reset index for next round
                    index_a = 0;
                }
            }
        }

        if(counter_b < STIM_LEN)
        {
            //Check all of the 
            if(other_shared_ptr_b[index_b] == (counter_b+1))
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
                        my_shared_ptr_b[i] = counter_b;
                    }

                    //Reset index for next round
                    index_b = 0;
                }
            }
        }

        //Poll on the memory location until the above condition is met or the counter exceeds STIM_LEN
    }

    return NULL;
}
/*
 * Latency Flow Control
 *       Very similar to the latency test except that the array transfer only goes from server to client.  An ack is used to signal to the procucer that it is OK to send more data
 *       
 */

#include "latency_flow_ctrl_kernel.h"
#include "intrin_bench_default_defines_and_imports_cpp.h"

/*
 * Resets shared ptr array to 0
 */
void* latency_flow_ctrl_kernel_reset(void* arg)
{
    LatencyFlowCtrlKernelArgs* args = (LatencyFlowCtrlKernelArgs*) arg;

    std::atomic_int32_t* array_shared_ptr_int = args->array_shared_ptr;
    std::atomic_int32_t* ack_shared_ptr_int = args->ack_shared_ptr;
    size_t length = args->length;

    for(size_t i = 0; i<length; i++)
    {
        std::atomic_store_explicit(array_shared_ptr_int+i, 0, std::memory_order_relaxed);
    }

    std::atomic_store_explicit(ack_shared_ptr_int, 0, std::memory_order_release);

    return NULL;
}

/*
 * Resets shared ptr array to 0
 */
void* latency_flow_ctrl_join_kernel_reset(void* arg)
{
    LatencyFlowCtrlJoinKernelArgs* args = (LatencyFlowCtrlJoinKernelArgs*) arg;

    std::atomic_int32_t* array_shared_ptr_int_a = args->array_shared_ptr_a;
    std::atomic_int32_t* ack_shared_ptr_int_a = args->ack_shared_ptr_a;
    size_t length_a = args->length_a;

    std::atomic_int32_t* array_shared_ptr_int_b = args->array_shared_ptr_b;
    std::atomic_int32_t* ack_shared_ptr_int_b = args->ack_shared_ptr_b;
    size_t length_b = args->length_b;

    for(size_t i = 0; i<length_a; i++)
    {
        std::atomic_store_explicit(array_shared_ptr_int_a+i, 0, std::memory_order_relaxed);
    }

    std::atomic_store_explicit(ack_shared_ptr_int_a, 0, std::memory_order_release);

    for(size_t i = 0; i<length_b; i++)
    {
        std::atomic_store_explicit(array_shared_ptr_int_b+i, 0, std::memory_order_relaxed);
    }
    
    std::atomic_store_explicit(ack_shared_ptr_int_b, 0, std::memory_order_release);

    return NULL;
}

/*
 * Arg contains a pointer to the shared memory location, the length of the array, and the pointer to the ack memory location
 * 
 * returns nothing (reporting handled by server wrapper)
 */
void* latency_flow_ctrl_server_kernel(void* arg)
{
    //Get the shared pointer and the initial counter value
    LatencyFlowCtrlKernelArgs* kernel_args = (LatencyFlowCtrlKernelArgs*) arg;
    std::atomic_int32_t* array_shared_ptr = kernel_args->array_shared_ptr;
    std::atomic_int32_t* ack_shared_ptr = kernel_args->ack_shared_ptr;
    size_t length = kernel_args->length;

    int32_t counter = -1; //Server

    //Execute until the specified number of transactions has occured
    size_t index = 0;

    while(counter < STIM_LEN)
    {
        //Check the ack memory location
        if(std::atomic_load_explicit(ack_shared_ptr, std::memory_order_acquire) == (counter+1))
        {
            //Last transaction has been acked, increment counter
            counter+=2;

            //Send next batch
            //Increment the entire array
            for(size_t i = 0; i<length; i++)
            {
                std::atomic_store_explicit(array_shared_ptr+i, counter, std::memory_order_relaxed);
            }

            //TODO: Check if this fence is required.  In other tests (ex. FIFO), there is an implicit barrier with an update to the write pointer
            std::atomic_thread_fence(std::memory_order_release); //Forces writes to occure (should act as a compiler barrier and an instruction fence if the platform requires it - x86 should not)
        }

        //Poll on the memory location until the above condition is met or the counter exceeds STIM_LEN
    }

    return NULL;
}

/*
 * Arg contains a pointer to the shared memory location, the length of the array, and the pointer to the ack memory location
 * 
 * This is a version for testing fan-in
 * 
 * returns nothing (reporting handled by server wrapper)
 */
void* latency_flow_ctrl_server_join_kernel(void* arg)
{
    //Get the shared pointer and the initial counter value
    LatencyFlowCtrlJoinKernelArgs* kernel_args = (LatencyFlowCtrlJoinKernelArgs*) arg;
    std::atomic_int32_t* array_shared_ptr_a = kernel_args->array_shared_ptr_a;
    std::atomic_int32_t* ack_shared_ptr_a = kernel_args->ack_shared_ptr_a;
    size_t length_a = kernel_args->length_a;

    std::atomic_int32_t* array_shared_ptr_b = kernel_args->array_shared_ptr_b;
    std::atomic_int32_t* ack_shared_ptr_b = kernel_args->ack_shared_ptr_b;
    size_t length_b = kernel_args->length_b;

    int32_t counter_a = -1; //Server

    int32_t counter_b = -1; //Server

    //Execute until the specified number of transactions has occured
    size_t index_a = 0;

    size_t index_b = 0;

    while(counter_a < STIM_LEN || counter_b < STIM_LEN)
    {
        if(counter_a < STIM_LEN)
        {
            //Check the ack memory location
            if(std::atomic_load_explicit(ack_shared_ptr_a, std::memory_order_acquire) == (counter_a+1))
            {
                //Last transaction has been acked, increment counter
                counter_a+=2;

                //Send next batch
                //Increment the entire array
                for(size_t i = 0; i<length_a; i++)
                {
                    std::atomic_store_explicit(array_shared_ptr_a+i, counter_a, std::memory_order_relaxed);
                }

                //TODO: Check if this fence is required.  In other tests (ex. FIFO), there is an implicit barrier with an update to the write pointer
                std::atomic_thread_fence(std::memory_order_release);
            }
        }

        if(counter_b < STIM_LEN)
        {
            //Check the ack memory location
            if(std::atomic_load_explicit(ack_shared_ptr_b, std::memory_order_acquire) == (counter_b+1))
            {
                //Last transaction has been acked, increment counter
                counter_b+=2;

                //Send next batch
                //Increment the entire array
                for(size_t i = 0; i<length_b; i++)
                {
                    std::atomic_store_explicit(array_shared_ptr_b+i, counter_b, std::memory_order_relaxed);
                }

                //TODO: Check if this fence is required.  In other tests (ex. FIFO), there is an implicit barrier with an update to the write pointer
                std::atomic_thread_fence(std::memory_order_release);
            }
        }

        //Poll on the memory location until the above condition is met or the counter exceeds STIM_LEN
    }

    return NULL;
}

/*
 * Arg contains a pointer to the shared memory location, the length of the array, and the pointer to the ack memory location
 * 
 * returns nothing (reporting handled by server wrapper)
 */
void* latency_flow_ctrl_client_kernel(void* arg)
{
    //Get the shared pointer and the initial counter value
    LatencyFlowCtrlKernelArgs* kernel_args = (LatencyFlowCtrlKernelArgs*) arg;
    std::atomic_int32_t* array_shared_ptr = kernel_args->array_shared_ptr;
    std::atomic_int32_t* ack_shared_ptr = kernel_args->ack_shared_ptr;
    size_t length = kernel_args->length;

    int32_t counter = 0; //Client

    //Execute until the specified number of transactions has occured
    size_t index = 0;

    while(counter < STIM_LEN)
    {
        //Check all of the memory locations

        if(std::atomic_load_explicit(array_shared_ptr+index, std::memory_order_acquire) == (counter+1))
        {
            //The current location has incremented
            //Check the next one
            index++;

            if(index >= length) //>= length and not >=length-1 because index is incremented unconditionally
            {
                //Checked the last element in the array
                
                //Increment counter and ackowlege
                counter+=2;

                std::atomic_store_explicit(ack_shared_ptr, counter, std::memory_order_release);

                //Reset index for next round
                index = 0;
            }
        }

        //Poll on the memory location until the above condition is met or the counter exceeds STIM_LEN
    }

    return NULL;
}

/*
 * Arg contains a pointer to the shared memory location, the length of the array, and the pointer to the ack memory location
 * 
 * This is a version for testing fan-out
 * 
 * returns nothing (reporting handled by server wrapper)
 */
void* latency_flow_ctrl_client_join_kernel(void* arg)
{
    //Get the shared pointer and the initial counter value
    LatencyFlowCtrlJoinKernelArgs* kernel_args = (LatencyFlowCtrlJoinKernelArgs*) arg;
    std::atomic_int32_t* array_shared_ptr_a = kernel_args->array_shared_ptr_a;
    std::atomic_int32_t* ack_shared_ptr_a = kernel_args->ack_shared_ptr_a;
    size_t length_a = kernel_args->length_a;

    std::atomic_int32_t* array_shared_ptr_b = kernel_args->array_shared_ptr_b;
    std::atomic_int32_t* ack_shared_ptr_b = kernel_args->ack_shared_ptr_b;
    size_t length_b = kernel_args->length_b;

    int32_t counter_a = 0; //Client

    int32_t counter_b = 0; //Client

    //Execute until the specified number of transactions has occured
    size_t index_a = 0;

    size_t index_b = 0;

    while(counter_a < STIM_LEN || counter_b < STIM_LEN)
    {
        if(counter_a < STIM_LEN)
        {
            //Check all of the memory locations
            if(std::atomic_load_explicit(array_shared_ptr_a+index_a, std::memory_order_acquire) == (counter_a+1))
            {
                //The current location has incremented
                //Check the next one
                index_a++;

                if(index_a >= length_a) //>= length and not >=length-1 because index is incremented unconditionally
                {
                    //Checked the last element in the array
                    
                    //Increment counter and ackowlege
                    counter_a+=2;

                    std::atomic_store_explicit(ack_shared_ptr_a, counter_a, std::memory_order_release);

                    //Reset index for next round
                    index_a = 0;
                }
            }
        }

        if(counter_b < STIM_LEN)
        {
            //Check all of the memory locations
            if(std::atomic_load_explicit(array_shared_ptr_b+index_b, std::memory_order_acquire) == (counter_b+1))
            {
                //The current location has incremented
                //Check the next one
                index_b++;

                if(index_b >= length_b) //>= length and not >=length-1 because index is incremented unconditionally
                {
                    //Checked the last element in the array
                    
                    //Increment counter and ackowlege
                    counter_b+=2;

                    std::atomic_store_explicit(ack_shared_ptr_b, counter_b, std::memory_order_release);

                    //Reset index for next round
                    index_b = 0;
                }
            }
        }

        //Poll on the memory location until the above condition is met or the counter exceeds STIM_LEN
    }

    return NULL;
}
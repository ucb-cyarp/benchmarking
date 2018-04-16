/*
 * Latency Flow Control
 *       Very similar to the latency test except that the array transfer only goes from server to client.  An ack is used to signal to the procucer that it is OK to send more data
 *       
 */

#include "latency_flow_ctrl_kernel.h"
#include "intrin_bench_default_defines.h"

/*
 * Resets shared ptr array to 0
 */
void* latency_flow_ctrl_kernel_reset(void* arg)
{
    LatencyFlowCtrlKernelArgs* args = (LatencyFlowCtrlKernelArgs*) arg;

    volatile int32_t* array_shared_ptr_int = args->array_shared_ptr;
    volatile int32_t* ack_shared_ptr_int = args->ack_shared_ptr;
    size_t length = args->length;

    *ack_shared_ptr_int = 0;

    for(size_t i = 0; i<length; i++)
    {
        array_shared_ptr_int[i] = 0;
    }

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
    volatile int32_t* array_shared_ptr = kernel_args->array_shared_ptr;
    volatile int32_t* ack_shared_ptr = kernel_args->ack_shared_ptr;
    size_t length = kernel_args->length;

    int32_t counter = -1; //Server

    //Execute until the specified number of transactions has occured
    size_t index = 0;

    while(counter < STIM_LEN)
    {
        //Check the ack memory location
        if(*ack_shared_ptr == (counter+1))
        {
            //Last transaction has been acked, increment counter
            counter+=2;

            //Send next batch
            //Increment the entire array
            for(size_t i = 0; i<length; i++)
            {
                array_shared_ptr[i] = counter;
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
    volatile int32_t* array_shared_ptr = kernel_args->array_shared_ptr;
    volatile int32_t* ack_shared_ptr = kernel_args->ack_shared_ptr;
    size_t length = kernel_args->length;

    int32_t counter = 0; //Client

    //Execute until the specified number of transactions has occured
    size_t index = 0;

    while(counter < STIM_LEN)
    {
        //Check all of the memory locations
        if(array_shared_ptr[index] == (counter+1))
        {
            //The current location has incremented
            //Check the next one
            index++;

            if(index >= length) //>= length and not >=length-1 because index is incremented unconditionally
            {
                //Checked the last element in the array
                
                //Increment counter and ackowlege
                counter+=2;

                *ack_shared_ptr = counter;

                //Reset index for next round
                index = 0;
            }
        }

        //Poll on the memory location until the above condition is met or the counter exceeds STIM_LEN
    }

    return NULL;
}
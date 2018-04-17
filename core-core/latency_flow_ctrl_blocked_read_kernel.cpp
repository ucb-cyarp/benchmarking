/*
 * Latency Flow Control
 *       Same as flow contol case except that the array is not read until a signal from the writer (the valid signal) is written.
 *       This should limit contention for the cache lines containing the array.
 * 
 *       Note! This assumes memory transactions occur in order.  Specifically, that the array is completely written before the valid
 *             signal is written.  It also assumes that the cache coherency engine guarentees that memory writes are handled in order.
 *             This needs to be confirmed by the intel platform.
 * 
 *             Note: It looks like volitile may enforce the write ordering condition.  Further research required.
 */

#include "latency_flow_ctrl_blocked_read_kernel.h"
#include "intrin_bench_default_defines.h"

/*
 * Resets shared ptr array to 0
 */
void* latency_flow_ctrl_blocked_read_kernel_reset(void* arg)
{
    LatencyFlowCtrlBlockedReadKernelArgs* args = (LatencyFlowCtrlBlockedReadKernelArgs*) arg;

    volatile int32_t* array_shared_ptr_int = args->array_shared_ptr;
    volatile int32_t* ack_shared_ptr_int = args->ack_shared_ptr;
    volatile int32_t* valid_shared_ptr_int = args->valid_shared_ptr;
    size_t length = args->length;

    *ack_shared_ptr_int = 0;
    *valid_shared_ptr_int = 0;

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
void* latency_flow_ctrl_blocked_read_server_kernel(void* arg)
{
    //Get the shared pointer and the initial counter value
    LatencyFlowCtrlBlockedReadKernelArgs* kernel_args = (LatencyFlowCtrlBlockedReadKernelArgs*) arg;
    volatile int32_t* array_shared_ptr = kernel_args->array_shared_ptr;
    volatile int32_t* ack_shared_ptr = kernel_args->ack_shared_ptr;
    volatile int32_t* valid_shared_ptr = kernel_args->valid_shared_ptr;
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

            //Increment the valid memory location
            *valid_shared_ptr = counter;
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
void* latency_flow_ctrl_blocked_read_client_kernel(void* arg)
{
    //Get the shared pointer and the initial counter value
    LatencyFlowCtrlBlockedReadKernelArgs* kernel_args = (LatencyFlowCtrlBlockedReadKernelArgs*) arg;
    volatile int32_t* array_shared_ptr = kernel_args->array_shared_ptr;
    volatile int32_t* ack_shared_ptr = kernel_args->ack_shared_ptr;
    volatile int32_t* valid_shared_ptr = kernel_args->valid_shared_ptr;
    size_t length = kernel_args->length;

    int32_t counter = 0; //Client

    //Execute until the specified number of transactions has occured
    size_t index = 0;

    while(counter < STIM_LEN)
    {
        //Wait until the valid pointer indicates that the array is ready
        if(*valid_shared_ptr == (counter+1))
        {
            //Check the array
            for(size_t i = 0; i<length; i++)
            {
                int32_t read_val = array_shared_ptr[i];
                if(read_val != (counter+1))
                {
                    printf("Unexpected Value Read!\n");
                    exit(1);
                }
            }
            
            //Checked the last element in the array
            
            //Increment counter and ackowlege
            counter+=2;

            *ack_shared_ptr = counter;
        }

        //Poll on the memory location until the above condition is met or the counter exceeds STIM_LEN
    }

    return NULL;
}
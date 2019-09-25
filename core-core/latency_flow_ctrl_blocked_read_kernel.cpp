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
 * Resets shared ptr array to 0
 */
void* latency_flow_ctrl_blocked_read_join_kernel_reset(void* arg)
{
    LatencyFlowCtrlBlockedReadJoinKernelArgs* args = (LatencyFlowCtrlBlockedReadJoinKernelArgs*) arg;

    volatile int32_t* array_shared_ptr_int_a = args->array_shared_ptr_a;
    volatile int32_t* ack_shared_ptr_int_a = args->ack_shared_ptr_a;
    volatile int32_t* valid_shared_ptr_int_a = args->valid_shared_ptr_a;
    size_t length_a = args->length_a;

    volatile int32_t* array_shared_ptr_int_b = args->array_shared_ptr_b;
    volatile int32_t* ack_shared_ptr_int_b = args->ack_shared_ptr_b;
    volatile int32_t* valid_shared_ptr_int_b = args->valid_shared_ptr_b;
    size_t length_b = args->length_b;

    *ack_shared_ptr_int_a = 0;
    *valid_shared_ptr_int_a = 0;

    *ack_shared_ptr_int_b = 0;
    *valid_shared_ptr_int_b = 0;

    for(size_t i = 0; i<length_a; i++)
    {
        array_shared_ptr_int_a[i] = 0;
    }

    for(size_t i = 0; i<length_b; i++)
    {
        array_shared_ptr_int_b[i] = 0;
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
        asm volatile ("" : : : "memory");
        if(*ack_shared_ptr == (counter+1))
        {
            //Last transaction has been acked, increment counter
            counter+=2;

            //Send next batch
            //Increment the entire array
            asm volatile ("" : : : "memory");
            for(size_t i = 0; i<length; i++)
            {
                array_shared_ptr[i] = counter;
            }

            asm volatile ("" : : : "memory");
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
void* latency_flow_ctrl_blocked_read_server_join_kernel(void* arg)
{
    //Get the shared pointer and the initial counter value
    LatencyFlowCtrlBlockedReadJoinKernelArgs* kernel_args = (LatencyFlowCtrlBlockedReadJoinKernelArgs*) arg;
    volatile int32_t* array_shared_ptr_a = kernel_args->array_shared_ptr_a;
    volatile int32_t* ack_shared_ptr_a = kernel_args->ack_shared_ptr_a;
    volatile int32_t* valid_shared_ptr_a = kernel_args->valid_shared_ptr_a;
    size_t length_a = kernel_args->length_a;

    volatile int32_t* array_shared_ptr_b = kernel_args->array_shared_ptr_b;
    volatile int32_t* ack_shared_ptr_b = kernel_args->ack_shared_ptr_b;
    volatile int32_t* valid_shared_ptr_b = kernel_args->valid_shared_ptr_b;
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
             asm volatile ("" : : : "memory");
            if(*ack_shared_ptr_a == (counter_a+1))
            {
                //Last transaction has been acked, increment counter
                counter_a+=2;

                //Send next batch
                //Increment the entire array
                 asm volatile ("" : : : "memory");
                for(size_t i = 0; i<length_a; i++)
                {
                    array_shared_ptr_a[i] = counter_a;
                }

                asm volatile ("" : : : "memory");
                //Increment the valid memory location
                *valid_shared_ptr_a = counter_a;
            }
        }

        if(counter_b < STIM_LEN)
        {
            //Check the ack memory location
             asm volatile ("" : : : "memory");
            if(*ack_shared_ptr_b == (counter_b+1))
            {
                //Last transaction has been acked, increment counter
                counter_b+=2;

                //Send next batch
                //Increment the entire array
                 asm volatile ("" : : : "memory");
                for(size_t i = 0; i<length_b; i++)
                {
                    array_shared_ptr_b[i] = counter_b;
                }

                asm volatile ("" : : : "memory");
                //Increment the valid memory location
                *valid_shared_ptr_b = counter_b;
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

/*
 * Arg contains a pointer to the shared memory location, the length of the array, and the pointer to the ack memory location
 * 
 * returns nothing (reporting handled by server wrapper)
 */
void* latency_flow_ctrl_blocked_read_client_join_kernel(void* arg)
{
    //Get the shared pointer and the initial counter value
    LatencyFlowCtrlBlockedReadJoinKernelArgs* kernel_args = (LatencyFlowCtrlBlockedReadJoinKernelArgs*) arg;
    volatile int32_t* array_shared_ptr_a = kernel_args->array_shared_ptr_a;
    volatile int32_t* ack_shared_ptr_a = kernel_args->ack_shared_ptr_a;
    volatile int32_t* valid_shared_ptr_a = kernel_args->valid_shared_ptr_a;
    size_t length_a = kernel_args->length_a;

    volatile int32_t* array_shared_ptr_b = kernel_args->array_shared_ptr_b;
    volatile int32_t* ack_shared_ptr_b = kernel_args->ack_shared_ptr_b;
    volatile int32_t* valid_shared_ptr_b = kernel_args->valid_shared_ptr_b;
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
            //Wait until the valid pointer indicates that the array is ready
            if(*valid_shared_ptr_a == (counter_a+1))
            {
                //Check the array
                for(size_t i = 0; i<length_a; i++)
                {
                    int32_t read_val = array_shared_ptr_a[i];
                    if(read_val != (counter_a+1))
                    {
                        printf("Unexpected Value Read!\n");
                        exit(1);
                    }
                }
                
                //Checked the last element in the array
                
                //Increment counter and ackowlege
                counter_a+=2;

                *ack_shared_ptr_a = counter_a;
            }
        }

        if(counter_b < STIM_LEN)
        {
            //Wait until the valid pointer indicates that the array is ready
            if(*valid_shared_ptr_b == (counter_b+1))
            {
                //Check the array
                for(size_t i = 0; i<length_b; i++)
                {
                    int32_t read_val = array_shared_ptr_b[i];
                    if(read_val != (counter_b+1))
                    {
                        printf("Unexpected Value Read!\n");
                        exit(1);
                    }
                }
                
                //Checked the last element in the array
                
                //Increment counter and ackowlege
                counter_b+=2;

                *ack_shared_ptr_b = counter_b;
            }
        }

        //Poll on the memory location until the above condition is met or the counter exceeds STIM_LEN
    }

    return NULL;
}
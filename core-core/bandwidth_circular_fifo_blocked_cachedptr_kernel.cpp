/*
 * Bandwidth FIFO
 *     Implements a circular FIFO between a producer thread and a consumer thread.  There is a single shared array that the producer writes data into.
 *     This acts as the buffer for the FIFO.  There is a shared memory location where the producer writes the element ID of the last element entered
 *     into the FIFO.  The ID of the element in the array is the element ID % array length.  Element IDs increase by 1 between elements.
 *     The consumer writes to a seperate shared memory location the ID of the last element it consumed.  The address of this element is the element ID 
 *     % array length.  The producer will not write into the array if the ID of the new element - ID of the last read elemeent > array length.
 *     Likewise, the consumer will not read if ID of the last read element == ID of last written element.
 * 
 *     Note! This assumes memory transactions occur in order.  Specifically, that the array is completely written before the write pointer is updated
 *           It also assumes that the cache coherency engine guarentees that memory writes are handled in order.
 *           This needs to be confirmed by the Intel platform.
 * 
 *           Note: It looks like volitile may enforce the write ordering condition.  Further research required.
 */

#include "bandwidth_circular_fifo_blocked_cachedptr_kernel.h"
#include "intrin_bench_default_defines_and_imports_cpp.h"
#include <cstdlib>
#include <cstdio>

/*
 * Resets shared ptr array to 0
 */
void* bandwidth_circular_fifo_blocked_cachedptr_kernel_reset(void* arg)
{
    BandwidthCircularFifoBlockedCachedPtrKernelArgs* args = (BandwidthCircularFifoBlockedCachedPtrKernelArgs*) arg;

    volatile int32_t* array_shared_ptr_int = args->array_shared_ptr;
    volatile int32_t* write_pos_shared_ptr_int = args->write_pos_shared_ptr;
    volatile int32_t* read_pos_shared_ptr_int = args->read_pos_shared_ptr;
    size_t length = args->length;

    *write_pos_shared_ptr_int = 0;
    *read_pos_shared_ptr_int = 0;

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
void* bandwidth_circular_fifo_blocked_cachedptr_server_kernel(void* arg)
{
    //Get the shared pointer and the initial counter value
    BandwidthCircularFifoBlockedCachedPtrKernelArgs* kernel_args = (BandwidthCircularFifoBlockedCachedPtrKernelArgs*) arg;
    volatile int32_t* array_shared_ptr = kernel_args->array_shared_ptr;
    volatile int32_t* write_pos_shared_ptr = kernel_args->write_pos_shared_ptr;
    volatile int32_t* read_pos_shared_ptr = kernel_args->read_pos_shared_ptr;

    size_t length = kernel_args->length;
    int32_t block_length = kernel_args->block_length;

    int32_t write_id = 0;
    *write_pos_shared_ptr = write_id; //sync write id

    int32_t write_index = 0;

    int32_t writeLim = STIM_LEN*length;

    //Use cached val for read ptr
    int32_t read_id = 0;

    while(write_id < writeLim)
    {
        //Check if we are allowed to write a full block
        if(length - (write_id - read_id) >= block_length)
        {
            //Yes, we are allowed to write a block
            int32_t number_to_write = block_length;

            for(int32_t i = 1; i <= number_to_write; i++) //start at 1 because write_id is for last written element, need to write into following elements
            {
                int32_t new_write_id = write_id + i;

                //Handle the mod with a branch as it should be cheaper 
                write_index++;
                if(write_index == length)
                {
                    write_index = 0;
                }

                //We could write anything but let's write the ID value for this test
                array_shared_ptr[write_index] = new_write_id;
            }
            
            //We now need to update the write_id
            write_id += number_to_write;

            //Let's sync the write pointer to the new tail of the queue
            *write_pos_shared_ptr = write_id;
        }else{
            //The cached read ptr indicates that there is no room
            //Update it for the next round in case things have changed

            //Get the current read_id
            read_id = *read_pos_shared_ptr;
        }

        //Poll until we are able to write again or we have reached the test end
    }

    return NULL;
}

/*
 * Arg contains a pointer to the shared memory location, the length of the array, and the pointer to the ack memory location
 * 
 * returns nothing (reporting handled by server wrapper)
 */
void* bandwidth_circular_fifo_blocked_cachedptr_client_kernel(void* arg)
{
    //Get the shared pointer and the initial counter value
    BandwidthCircularFifoBlockedCachedPtrKernelArgs* kernel_args = (BandwidthCircularFifoBlockedCachedPtrKernelArgs*) arg;
    volatile int32_t* array_shared_ptr = kernel_args->array_shared_ptr;
    volatile int32_t* write_pos_shared_ptr = kernel_args->write_pos_shared_ptr;
    volatile int32_t* read_pos_shared_ptr = kernel_args->read_pos_shared_ptr;

    size_t length = kernel_args->length;
    size_t block_length = kernel_args->block_length;

    int32_t read_id = 0;
    *read_pos_shared_ptr = read_id; //sync read id

    int32_t read_index = 0;

    int32_t readLim = STIM_LEN*length;

    //Use cached val for write ptr
    int32_t write_id = 0;

    while(read_id < readLim)
    {
        //Check if we are allowed to read a full block
        if(write_id - read_id >= block_length)
        {
            //Yes, we can read a full block
            
            int32_t num_to_read = block_length;

            //Let's read all of them then update the read_pos_shared_ptr
            //This avoids creating more for the read_pos_shared_ptr

            //NOTE: The sender in this test is updating the write_pos_shared_ptr
            //      every time an element is enqueued onto the FIFO.  Therefore
            //      the contention reduction will only be in the reverse direction
            //      !! This could potentially be mitigated by allowing multiple
            //         elements to be enqueued before updating the write_pos_shared_ptr.
            //         One question is how quickly this can be expected to happen in normal operation.

            for(int32_t i = 1; i <= num_to_read; i++) //Starting at 1 because ready_id is the last read element, we need to read elements after it
            {
                //Read the value
                int32_t expected_read_id = read_id+i;
                
                //Handle the mod with a branch as it should be cheaper 
                read_index++; //always increments by 1 for each loop itteration
                if(read_index == length)
                {
                    read_index = 0;
                }

                int32_t read_value = array_shared_ptr[read_index];

                //Check that the value is what we expect.
                //NOTE: We are using the fact that in this test, the value should be the same as the ID.
                if(read_value != expected_read_id)
                {
                    printf("Unexpected read value!!!");
                    exit(1);
                }
            }

            //We read and checked all entries, now let's update the read_id
            read_id += num_to_read;

            *read_pos_shared_ptr = read_id; //Sync read ID
        }else{
            //The cached write ptr indicates that there is nothing to read
            //Update it for the next round in case things have changed

            //Get the current read_id
            write_id = *write_pos_shared_ptr; //get the current write ID
        }

        //Poll until there is more data to read or the end of the test
    }

    return NULL;
}
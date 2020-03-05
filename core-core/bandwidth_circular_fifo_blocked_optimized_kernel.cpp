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

#include "bandwidth_circular_fifo_blocked_optimized_kernel.h"
#include "intrin_bench_default_defines_and_imports_cpp.h"
#include <cstdlib>
#include <cstdio>

/*
 * Resets shared ptr array to 0
 */
void* bandwidth_circular_fifo_blocked_optimized_kernel_reset(void* arg)
{
    BandwidthCircularFifoBlockedOptimizedKernelArgs* args = (BandwidthCircularFifoBlockedOptimizedKernelArgs*) arg;

    int32_t* array_shared_ptr_int = args->array_shared_ptr;
    std::atomic_int32_t* write_pos_shared_ptr_int = args->write_pos_shared_ptr;
    std::atomic_int32_t* read_pos_shared_ptr_int = args->read_pos_shared_ptr;
    size_t length = args->length;
    size_t block_length = args->block_length;

    int32_t* array_local_int = args->local_array_reader;

    for(size_t i = 0; i<length*block_length; i++)
    {
        array_shared_ptr_int[i] = 0;
    }

    for(size_t i = 0; i<block_length; i++){
        array_local_int[i] = 0;
    }

    std::atomic_store_explicit(write_pos_shared_ptr_int, 0, std::memory_order_release);
    std::atomic_store_explicit(read_pos_shared_ptr_int, 0, std::memory_order_release);

    return NULL;
}

/*
 * Arg contains a pointer to the shared memory location, the length of the array, and the pointer to the ack memory location
 * 
 * returns nothing (reporting handled by server wrapper)
 */
void* bandwidth_circular_fifo_blocked_optimized_server_kernel(void* arg)
{
    //Get the shared pointer and the initial counter value
    BandwidthCircularFifoBlockedOptimizedKernelArgs* kernel_args = (BandwidthCircularFifoBlockedOptimizedKernelArgs*) arg;
    int32_t* array_shared_ptr = kernel_args->array_shared_ptr;
    std::atomic_int32_t* write_pos_shared_ptr = kernel_args->write_pos_shared_ptr;
    std::atomic_int32_t* read_pos_shared_ptr = kernel_args->read_pos_shared_ptr;

    size_t length = kernel_args->length; //In blocks
    int32_t block_length = kernel_args->block_length; //In Elements

    int32_t write_id = 0;
    std::atomic_store_explicit(write_pos_shared_ptr, write_id, std::memory_order_release); //sync write id

    int32_t write_index = 0;
    int32_t write_offset = 0;

    int32_t sample_vals = (write_offset+1)%2;

    int32_t writeLim = STIM_LEN*length; //In Blocks
    while(write_id < writeLim) //In Blocks
    {
        //Get the current read_id
        int32_t read_id = std::atomic_load_explicit(read_pos_shared_ptr, std::memory_order_acquire); //Should prevent array writes from occuring before this

        //Check if we are allowed to write a full block
        if(length - (write_id - read_id) > 0)
        {
            // printf("Write %d of %d, %d\n", write_id, writeLim, sample_vals);
            //Yes, we are allowed to write a block
            int32_t* array_base = array_shared_ptr + write_offset*block_length;

            for(int32_t i = 0; i < block_length; i++){ //start at 1 because write_id is for last written element, need to write into following elements
                //We could write anything but let's write the ID value for this test
                array_base[i] = sample_vals;
            }

            //We now need to update the write_id
            write_id++;

            //Let's sync the write pointer to the new tail of the queue
            std::atomic_store_explicit(write_pos_shared_ptr, write_id, std::memory_order_release); //Should prevent array writes from occuring after this.  Should force array writes to be visible to consumer by this point
            
            //Check for index wrap arround (no extra element in this test)
            if (write_offset >= length-1)
            {
                write_offset = 0;
            }
            else
            {
                write_offset++;
            }
            sample_vals = (sample_vals+1)%2;
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
void* bandwidth_circular_fifo_blocked_optimized_client_kernel(void* arg)
{
    //Get the shared pointer and the initial counter value
    BandwidthCircularFifoBlockedOptimizedKernelArgs* kernel_args = (BandwidthCircularFifoBlockedOptimizedKernelArgs*) arg;
    int32_t* array_shared_ptr = kernel_args->array_shared_ptr;
    int32_t* local_buffer = kernel_args->local_array_reader;
    std::atomic_int32_t* write_pos_shared_ptr = kernel_args->write_pos_shared_ptr;
    std::atomic_int32_t* read_pos_shared_ptr = kernel_args->read_pos_shared_ptr;

    size_t length = kernel_args->length; //In blocks
    size_t block_length = kernel_args->block_length; //In elements

    int32_t read_id = 0;
    std::atomic_store_explicit(read_pos_shared_ptr, read_id, std::memory_order_release); //sync read id

    int32_t read_offset = 0;

    int32_t expected_sample_vals = (read_offset+1)%2;

    int32_t readLim = STIM_LEN*length;
    while(read_id < readLim)
    {
        
        int32_t write_id = std::atomic_load_explicit(write_pos_shared_ptr, std::memory_order_acquire); //get the current write ID.  Should force all writes to the array to finish before this point and for reads to not occur before this point

        //Check if we are allowed to read a full block
        if(write_id - read_id > 0)
        {
            // printf("Read %d of %d, %d\n", read_id, readLim, expected_sample_vals);
            //Yes, we can read a full block
            int32_t* array_base = array_shared_ptr + read_offset*block_length;

            for(int32_t i = 0; i < block_length; i++)
            {        
                local_buffer[i] = array_base[i];
            }

            //We read and checked all entries, now let's update the read_id
            read_id++;

            std::atomic_store_explicit(read_pos_shared_ptr, read_id, std::memory_order_release); //Sync read ID.  Should force reads from array to finish before this (preventing producer from overwriting it will not write past the read pointer).

            //Check the read values
            for(int32_t i = 0; i < block_length; i++)
            {
                //Check that the value is what we expect.
                //NOTE: We are using the fact that in this test, the value should be the same as the ID.
                if(local_buffer[i] != expected_sample_vals)
                {
                    printf("Unexpected read value!!! ID: %d, %d ~= %d\n", read_id-1, local_buffer[i], expected_sample_vals);
                    exit(1);
                }
            }

            //check for wraparound (no extra element in this test)
            if (read_offset >= length-1)
            {
                read_offset = 0;
            }
            else
            {
                read_offset++;
            }
            expected_sample_vals = (expected_sample_vals+1)%2;
        }

        //Poll until there is more data to read or the end of the test
    }

    return NULL;
}
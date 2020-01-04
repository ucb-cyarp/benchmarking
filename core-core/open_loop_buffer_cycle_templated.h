#ifndef _OPEN_LOOP_BUFFER_CYCLE_TEMPLATED_H
#define _OPEN_LOOP_BUFFER_CYCLE_TEMPLATED_H

#include "open_loop_buffer_templated.h"

#include <atomic>

//In this benchmark, all threads have a buffer they are reading from an a buffer they are writing to
//This allows cycles of data passing to be setup.  All threads execute the same function.  The primary
//thread (reponsible for signaling the others to start) is destinguised by the arguments passed to it.

//This version includes the block size as a template parameter which allows us to define a structure for blocks.
//This is helpful for readable programs but not when varring the block size at runtime.

//An alternative version was written which allows run-time changes of the block type (with some alignment constraints)

template<int blockSize, typename elementType, typename idType = std::atomic_int32_t, typename indexType = std::atomic_int32_t>
struct OpenLoopBufferCycleArgs{
    indexType *read_offset_ptr_in;
    Block<blockSize, elementType, idType> *array_in;
    indexType *write_offset_ptr_out;
    Block<blockSize, elementType, idType> *array_out;
    std::atomic_flag *start_flag_in; //Used by the client to signal to the server that it should start writing
    std::atomic_flag *start_flag_out;
    std::atomic_flag *stop_flag_in; //Used by the client to signal to the server that an error occured and that it should stop
    std::atomic_flag *stop_flag_out;
    std::atomic_flag **ready_flags_in; //Used by the server to signal to the client that it is ready to begin
    std::atomic_flag *ready_flag_out;
    int num_ready_flags_in; //The number of ready flags in
    int array_length; //In Blocks  //Note: The FIFO Array has an additional block allocated to resolve the Empty/Full Ambiguity
    int max_block_transfers; //The maximum number of blocks to transfer in this test
};

template<int blockSize, typename elementType, typename idType = std::atomic_int32_t, typename indexType = std::atomic_int32_t>
struct OpenLoopBufferCycleResetArgs{
    indexType **read_offset_ptrs;
    indexType **write_offset_ptrs;
    Block<blockSize, elementType, idType> **arrays;
    int numBuffers;
    std::atomic_flag **init_set_flags; //Flags to initialize as set
    int num_init_set_flags;
    std::atomic_flag **init_clear_flags; //Flags to initialize as clear
    int num_init_clear_flags;
    int array_length; //In Blocks  //Note: The FIFO Array has an additional block allocated to resolve the Empty/Full Ambiguity
    int max_block_transfers; //The maximum number of blocks to transfer in this test
};

//Note, all threads use the same logic.  The primary thread is responsible for starting the others.  The primary thread is
//provided with the ready flag for each secondary thread.  It is provided with a dummy ready flag for itself.  It is provided
//with a real start out flag and a dummy start in flag which has been pre cleared.

//The secondary flags are provided with real 

//Any thread can pull the stop thread since any thread can experinece an error.  All threads check the stop thread including the primary
//thread.

template<int blockSize, typename elementType, typename idType = std::atomic_int32_t, typename indexType = std::atomic_int32_t>
void* open_loop_buffer_cycle_reset(void* arg){
    OpenLoopBufferResetArgs<blockSize, elementType, idType, indexType> *args = (OpenLoopBufferResetArgs<blockSize, elementType, idType, indexType>) arg;

    std::atomic_thread_fence(std::memory_order_acquire);

    int numberInitBlocks = args->array_length/2; //Initialize FIFO to be half full (round down if odd number)

    //Reset the index pointers
    for(int buffer = 0; buffer<args->numBuffers; buffer++){
        std::atomic_store_explicit(args->read_offset_ptr[buffer], 0, std::memory_order_relaxed); //Initialized to 0.  This is the index last read.  Index 1 contains the first initial value
        std::atomic_store_explicit(args->write_offset_ptr[buffer], numberInitBlocks+1, std::memory_order_relaxed); //Initialized to index after that last initial value in the FIFO (note, the initial elements start at index 1)

        Block<blockSize, elementType, idType> *array = args->arrays[buffers];
        //Reset the block ids and data
        for(int i = 1; i<numberInitBlocks+1; i++){
            std::atomic_store_explicit(array[i].block_id_start, i, std::memory_order_relaxed);

            for(int j = 0; j<blockSize; j++){
                array[i].data[j] = (i+1)%2;
            }
            
            std::atomic_store_explicit(array[i].block_id_stop, i, std::memory_order_relaxed);
        }
    }

    for(int i = 0; i<num_init_set_flags; i++){
        std::atomic_flag_test_and_set_explicit(args->init_set_flags[i], std::memory_order_relaxed);
    }
    
    for(int i = 0; i<init_clear_flags; i++){
        std::atomic_flag_test_and_set_explicit(args->init_clear_flags[i], std::memory_order_relaxed);
    }

    std::atomic_thread_fence(std::memory_order_release);
}


//This thread is the writer.  It will recieve a start signal from the client when it is ready (the client is the primary thread)
template<int blockSize, typename elementType, typename idType = std::atomic_int32_t, typename indexType = std::atomic_int32_t, typename idLocalType = int32_t, typename indexLocalType, int idMax>
void* open_loop_buffer_cycle(void* arg){
    OpenLoopBufferCycleArgs<blockSize, elementType, idType, indexType>* args = (OpenLoopBufferCycleArgs<blockSize, elementType, idType, indexType>*) arg;

    indexType *read_offset_ptr_in = args->read_offset_ptr_in;
    Block<blockSize, elementType, idType> *array_in = args->array_in;
    indexType *write_offset_ptr_out = args->write_offset_ptr_out;
    Block<blockSize, elementType, idType> *array_out = args->array_out;
    std::atomic_flag *start_flag_in = args->start_flag_in;
    std::atomic_flag *start_flag_out = args->start_flag_out;
    std::atomic_flag *stop_flag_in = args->stop_flag_in;
    std::atomic_flag *stop_flag_out = args->stop_flag_out;
    std::atomic_flag **ready_flags_in = args->ready_flags_in;
    std::atomic_flag *ready_flag_out = args->ready_flag_out;
    int num_ready_flags_in = args->num_ready_flags_in;
    int array_length = args->array_length;
    int max_block_transfers = args->max_block_transfers;

    int numberInitBlocks = array_length/2; //Initialize FIFO to be half full (round down if odd number)

    //Load initial write offset
    indexLocalType writeOffset = std::atomic_load_explicit(write_offset_ptr, std::memory_order_acquire);

    //Load the initial read offset
    indexLocalType readOffset = std::atomic_load_explicit(read_offset_ptr, std::memory_order_acquire);

    //Initialize Block Inds
    idLocalType writeBlockInd = writeOffset;
    idLocalType readBlockInd = 0;
    
    //Local Vars
    elementType expectedSampleVals = 1;
    idLocalType newBlockIDStart = -1;
    idLocalType newBlockIDEnd = -1;
    idLocalType expectedBlockID = -1;
    elementType localBuffer[blockSize];
    bool localError = false;

    //Signal This Thread is Ready
    std::atomic_thread_fence(std::memory_order_acquire);
    std::atomic_flag_clear_explicit(ready_flag_out, std::memory_order_release);

    //Wait for other ready signals
    for(int i = 0; i<num_ready_flags_in; i++){
        bool ready = false;
        while(!ready){
            ready = !std::atomic_flag_test_and_set_explicit(ready_flags_in[i], std::memory_order_acq_rel); //Start signal is active low
        }
    }

    //Send Start Signal
    std::atomic_thread_fence(std::memory_order_acquire);
    std::atomic_flag_clear_explicit(start_flag_out, std::memory_order_release);
    
    //Wait for start signal
    bool start = false;
    while(!start){
        start = !std::atomic_flag_test_and_set_explicit(start_flag_in, std::memory_order_acq_rel); //Start signal is active low
    }

    for(int transfer = 0; transfer<max_block_transfers; transfer++){
        //===== Read =====
        //---- Read from Input Buffer Unconditionally ----
        //Load Read Ptr
        if (readOffset >= array_length)
        {
            readOffset = 0;
        }
        else
        {
            readOffset++;
        }

        //+++ Read from array +++
        //The end ID is read first to check that the values being read from the array were completly written before this thread started reading.
        newBlockIDEnd = std::atomic_load_explicit(array_in[readOffset].block_id_stop, std::memory_order_acquire);
        
        //Read (and check) elements
        //Read backwards to avoid cache thrashing
        for(int sample = blockSize-1; sample>=0; sample--){
            localBuffer[sample] = array_in[readOffset].data[sample];
        }

        //The start ID is read last to check that the block was not being overwritten while the data was being read
        std::atomic_signal_fence(std::memory_order_release) //Do not want an actual fence but do not want sample reading to be re-ordered before the end block ID read
        newBlockIDStart = std::atomic_load_explicit(array_in[readOffset].block_id_start, std::memory_order_acquire);
        //+++ End read from array +++

        //Update Read Ptr
        std::atomic_store_explicit(read_offset_ptr_in, readOffset, std::memory_order_release);

        //Check the read block IDs
        expectedBlockID = readBlockInd == UINT32_MAX ? 0 : readBlockInd+1;
        if(expectedBlockID != newBlockIDStart || expectedBlockID != newBlockIDEnd){
            //Will signal failure outside of loop
            localError = true;
            break;
        }
        readBlockInd = expectedBlockID;

        //===== Check elements =====
        for(int sample = 0; sample<blockSize; sample++){
            if(localBuffer.data[sample] != expectedSampleVals){
                std::cerr << "Unexpected array data!" << std::endl;
                exit(1);
            }
        }

        expectedSampleVals = (expectedSampleVals+1)%2;

        //===== Write (Copy) =====
        std::atomic_signal_fence(std::memory_order_acquire) //Do not want an actual fence but do not want the write to the initial block ID to re-ordered before the write to the write ptr
        //---- Write to output buffer unconditionally (order of writing the IDs is important as they are used by the reader to detect partially valid blocks)
        //+++ Write into array +++
        //Write initial block ID
        std::atomic_store_explicit(array_out[writeOffset].block_id_start, writeBlockInd, std::memory_order_release); //Prevents memory access from being reordered after this
        std::atomic_signal_fence(std::memory_order_acquire) //Do not want an actual fence but do not want sample writing to be re-ordered before the initial block ID write

        //Write elements (copy)
        for(int sample = 0; sample<blockSize; sample++){
            array_out[writeOffset].data[sample] = localBuffer[sample];
        }

        //Write final block ID
        std::atomic_store_explicit(array_out[writeOffset].block_id_stop, writeBlockInd, std::memory_order_release); //Prevents memory access from being reordered after this
        //+++ End write into array +++

        //Check for index wrap arround
        if (writeOffset >= array_length)
        {
            writeOffset = 0;
        }
        else
        {
            writeOffset++;
        }

        //Update Write Ptr
        std::atomic_store_explicit(write_offset_ptr_out, writeOffset, std::memory_order_release);

        //Increment block id for next block
        writeBlockInd = writeBlockInd == idMax ? 0 : writeBlockInd+1;

        //Check stop flag
        bool stop = !std::atomic_flag_test_and_set_explicit(stop_flag, std::memory_order_acq_rel);
        if(stop){
            break;
        }
    }

    std::atomic_thread_fence(std::memory_order_acquire);
    std::atomic_flag_clear(stop_flag, std::memory_order_release);

    OpenLoopBufferEndCondition *rtn = malloc(sizeof(OpenLoopBufferEndCondition));
    rtn->expectedBlockID = readBlockInd;
    rtn->startBlockID = newBlockIDStart;
    rtn->startBlockID = newBlockIDEnd;
    rtn->wasErrorSrc = localError;

    return rtn;
}



//==== Benchmark Function ====
//Check Sizes of Block (For Padding and Alignment) and Block Array (For Padding and Alignment)

#endif
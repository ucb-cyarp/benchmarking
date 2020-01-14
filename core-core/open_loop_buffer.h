#ifndef _OPEN_LOOP_BUFFER_H
#define _OPEN_LOOP_BUFFER_H

#include <atomic>
#include "open_loop_helpers.h"

//Note: This benchmark has the potential for asemytric load on the reader and writer threads.  Ballancing in the form of NOPs is required
//to remove this imballance's impact on the measurement of how long the benchmark can run before failing.

//This version does not use a structure for the blocks.  This allos the block size to be changed at runtime.
//There is a catch which is that the blocks must be naturally alligned.

//The Block Format is:
//         ============
// 0 - N   = Start ID =
//         ============
// N+1     =          =
//         =  Buffer  =
// M       =          =
//         ============
// M+1 - L =  End ID  =
//         ============

//The IDs are padded to be multiples of the provided alignment.  The buffer is also padded to be a multiple of the alignment.

//Note: The server and client threads are different instruction streams.  One writes while the other reads and needs to check the block ids (and the block data).
//The writer needs to periodically check the error condition flag provided by the reader.  This can result in the two functions having different
//execution times.  To compensate, nop loops are placed in each thread

//A better compensation mechanism is to force all threads to be both a reader and a writer.  Both threads would experience the same code.
//The start stop flags could be augmented with dummy flags, allowing all of the threads to use the same function.

template<typename elementType, typename idType = std::atomic_int32_t, typename indexType = std::atomic_int32_t>
void open_loop_buffer_reset_internal(OpenLoopBufferArgs<elementType, idType, indexType> *args){
    int blockArrayBytes;
    int blockArrayPaddingBytes;
    int blockArrayCombinedBytes;
    int idBytes;
    int idPaddingBytes;
    int idCombinedBytes;
    int blockSizeBytes;

    getBlockSizing<elementType, indexType>(args->blockSize, args->alignment, blockArrayBytes, blockArrayPaddingBytes, 
    blockArrayCombinedBytes, idBytes, idPaddingBytes, idCombinedBytes, blockSizeBytes);

    std::atomic_thread_fence(std::memory_order_acquire);

    int numberInitBlocks = args->array_length/2; //Initialize FIFO to be half full (round down if odd number)

    //Reset the index pointers (they are initialized outside)
    std::atomic_store_explicit(args->read_offset_ptr, 0, std::memory_order_relaxed); //Initialized to 0.  This is the index last read.  Index 1 contains the first initial value
    std::atomic_store_explicit(args->write_offset_ptr, numberInitBlocks+1, std::memory_order_relaxed); //Initialized to index after that last initial value in the FIFO (note, the initial elements start at index 1)

    //=== Init ===
    //Reset the block ids and data
    //Init ID Zero
    idType *block_id_start = (idType*) (((char*) args->array));
    idType *block_id_start_constructed = new (block_id_start) idType();
    std::atomic_init(block_id_start, 0);
    std::atomic_store_explicit(block_id_start, 0, std::memory_order_relaxed);

    elementType *data_array = (elementType*) (((char*) args->array) + idCombinedBytes);
    for(int j = 0; j<args->blockSize; j++){
        data_array[j] = -1;
    }

    idType *block_id_end = (idType*) (((char*) args->array) + idCombinedBytes + blockArrayCombinedBytes);
    idType *block_id_end_constructed = new (block_id_end) idType();
    std::atomic_init(block_id_end, 0);
    std::atomic_store_explicit(block_id_end, 0, std::memory_order_relaxed);

    //Initial Conditions
    for(int i = 1; i<numberInitBlocks+1; i++){
        idType *block_id_start = (idType*) (((char*) args->array) + i*blockSizeBytes);
        idType *block_id_start_constructed = new (block_id_start) idType();
        std::atomic_init(block_id_start, i);
        std::atomic_store_explicit(block_id_start, i, std::memory_order_relaxed);

        elementType *data_array = (elementType*) (((char*) args->array) + i*blockSizeBytes + idCombinedBytes);
        for(int j = 0; j<args->blockSize; j++){
            data_array[j] = (i+1)%2;
        }
        
        idType *block_id_end = (idType*) (((char*) args->array) + i*blockSizeBytes + idCombinedBytes + blockArrayCombinedBytes);
        idType *block_id_end_constructed = new (block_id_end) idType();
        std::atomic_init(block_id_end, i);
        std::atomic_store_explicit(block_id_end, i, std::memory_order_relaxed);
    }

    //Init After
    for(int i = numberInitBlocks+1; i <= args->array_length; i++){ //Note, there is an extra element in the array
        idType *block_id_start = (idType*) (((char*) args->array) + i*blockSizeBytes);
        idType *block_id_start_constructed = new (block_id_start) idType();
        std::atomic_init(block_id_start, 0);
        std::atomic_store_explicit(block_id_start, 0, std::memory_order_relaxed);

        elementType *data_array = (elementType*) (((char*) args->array) + i*blockSizeBytes + idCombinedBytes);
        for(int j = 0; j<args->blockSize; j++){
            data_array[j] = -1;
        }
        
        idType *block_id_end = (idType*) (((char*) args->array) + i*blockSizeBytes + idCombinedBytes + blockArrayCombinedBytes);
        idType *block_id_end_constructed = new (block_id_end) idType();
        std::atomic_init(block_id_end, 0);
        std::atomic_store_explicit(block_id_end, 0, std::memory_order_relaxed);
    }
    
    //Flags are initialized and destructed outside
    std::atomic_flag_test_and_set_explicit(args->start_flag, std::memory_order_relaxed);
    std::atomic_flag_test_and_set_explicit(args->stop_flag, std::memory_order_relaxed);
    std::atomic_flag_test_and_set_explicit(args->ready_flag, std::memory_order_relaxed);

    std::atomic_thread_fence(std::memory_order_release);
}

template<typename elementType, typename idType = std::atomic_int32_t, typename indexType = std::atomic_int32_t>
void* open_loop_buffer_reset(void* arg){
    OpenLoopBufferArgs<elementType, idType, indexType> *args = (OpenLoopBufferArgs<elementType, idType, indexType>*) arg;
    open_loop_buffer_reset_internal(args);

    return nullptr;
}

template<typename elementType, typename idType = std::atomic_int32_t, typename indexType = std::atomic_int32_t>
void* open_loop_buffer_cleanup(void* arg){
    OpenLoopBufferArgs<elementType, idType, indexType> *args = (OpenLoopBufferArgs<elementType, idType, indexType>*) arg;

    int blockArrayBytes;
    int blockArrayPaddingBytes;
    int blockArrayCombinedBytes;
    int idBytes;
    int idPaddingBytes;
    int idCombinedBytes;
    int blockSizeBytes;

    getBlockSizing<elementType, indexType>(args->blockSize, args->alignment, blockArrayBytes, blockArrayPaddingBytes, 
    blockArrayCombinedBytes, idBytes, idPaddingBytes, idCombinedBytes, blockSizeBytes);

    std::atomic_thread_fence(std::memory_order_acquire);

    int numberInitBlocks = args->array_length/2; //Initialize FIFO to be half full (round down if odd number)

    std::atomic_thread_fence(std::memory_order_acquire);

    //=== Init ===
    //Reset the block ids and data
    //Init ID Zero
    idType *block_id_start = (idType*) (((char*) args->array));
    block_id_start->~idType();

    idType *block_id_end = (idType*) (((char*) args->array) + idCombinedBytes + blockArrayCombinedBytes);
    block_id_end->~idType();

    //Initial Conditions
    for(int i = 1; i<numberInitBlocks+1; i++){
        idType *block_id_start = (idType*) (((char*) args->array) + i*blockSizeBytes);
        block_id_start->~idType();
        
        idType *block_id_end = (idType*) (((char*) args->array) + i*blockSizeBytes + idCombinedBytes + blockArrayCombinedBytes);
        block_id_end->~idType();
    }

    //Init After
    for(int i = numberInitBlocks+1; i <= args->array_length; i++){ //Note, there is an extra element in the array
        idType *block_id_start = (idType*) (((char*) args->array) + i*blockSizeBytes);
        block_id_start->~idType();
        
        idType *block_id_end = (idType*) (((char*) args->array) + i*blockSizeBytes + idCombinedBytes + blockArrayCombinedBytes);
        block_id_end->~idType();
    }

    std::atomic_thread_fence(std::memory_order_release);
}

//This thread is the writer.  It will recieve a start signal from the client when it is ready (the client is the primary thread)
template<typename elementType, typename idType = std::atomic_int32_t, typename indexType = std::atomic_int32_t, typename idLocalType = int32_t, typename indexLocalType = int32_t, int idMax = INT32_MAX>
void* open_loop_buffer_server(void* arg){
    OpenLoopBufferArgs<elementType, idType, indexType>* args = (OpenLoopBufferArgs<elementType, idType, indexType>*) arg;

    indexType *write_offset_ptr = args->write_offset_ptr;
    void *array = args->array;
    int array_length = args->array_length;
    int64_t max_block_transfers = args->max_block_transfers;
    std::atomic_flag *start_flag = args->start_flag;
    std::atomic_flag *stop_flag = args->stop_flag;
    std::atomic_flag *ready_flag = args->ready_flag;
    int blockSize = args->blockSize;
    int allignment = args->alignment;
    int core = args->core_server;

    int balancing_nops = args->balancing_nops;
    int numNops = balancing_nops < 0 ? -balancing_nops : 0;
    numNops += args->initialNOPs;

    //printf("Config: Array Len: %d, Block Size: %d, NOPs %d\n", array_length, blockSize, balancing_nops);

    int numberInitBlocks = array_length/2; //Initialize FIFO to be half full (round down if odd number)

    int blockArrayBytes;
    int blockArrayPaddingBytes;
    int blockArrayCombinedBytes;
    int idBytes;
    int idPaddingBytes;
    int idCombinedBytes;
    int blockSizeBytes;

    getBlockSizing<elementType, idType>(blockSize, args->alignment, blockArrayBytes, blockArrayPaddingBytes, 
    blockArrayCombinedBytes, idBytes, idPaddingBytes, idCombinedBytes, blockSizeBytes);

    //Load initial write offset
    indexLocalType writeOffset = std::atomic_load_explicit(write_offset_ptr, std::memory_order_acquire);

    //Initialize Block ID
    idLocalType writeBlockInd = writeOffset;

    elementType sampleVals = (writeOffset+1)%2;

    //Signal Ready
    std::atomic_thread_fence(std::memory_order_acquire);
    std::atomic_flag_clear_explicit(ready_flag, std::memory_order_release);

    //Begin writing entry (prime access to cache line)
    idType *block_id_start = (idType*) (((char*) array) + writeOffset*blockSizeBytes);
    std::atomic_store_explicit(block_id_start, writeBlockInd, std::memory_order_release); //Prevents memory access from being reordered after this
    
    //Wait for start signal
    bool start = false;
    while(!start){
        start = !std::atomic_flag_test_and_set_explicit(start_flag, std::memory_order_acq_rel); //Start signal is active low
        
        //Begin writing entry (prime access to cache line)
        idType *block_id_start = (idType*) (((char*) array) + writeOffset*blockSizeBytes);
        std::atomic_store_explicit(block_id_start, writeBlockInd, std::memory_order_release); //Prevents memory access from being reordered after this
    }

    bool stop = false;
    int64_t transfer;
    for(transfer = 0; transfer<max_block_transfers; transfer++){
        std::atomic_signal_fence(std::memory_order_acquire); //Do not want an actual fence but do not want the write to the initial block ID to re-ordered before the write to the write ptr
        //---- Write to output buffer unconditionally (order of writing the IDs is important as they are used by the reader to detect partially valid blocks)
        //+++ Write into array +++
        //Write initial block ID
        idType *block_id_start = (idType*) (((char*) array) + writeOffset*blockSizeBytes);
        std::atomic_store_explicit(block_id_start, writeBlockInd, std::memory_order_release); //Prevents memory access from being reordered after this
        std::atomic_signal_fence(std::memory_order_acquire); //Do not want an actual fence but do not want sample writing to be re-ordered before the initial block ID write

        //Write elements
        elementType *data_array = (elementType*) (((char*) array) + writeOffset*blockSizeBytes + idCombinedBytes);
        for(int sample = 0; sample<blockSize; sample++){
            data_array[sample] = sampleVals;
        }

        //Write final block ID
        idType *block_id_end = (idType*) (((char*) array) + writeOffset*blockSizeBytes + idCombinedBytes + blockArrayCombinedBytes);
        std::atomic_store_explicit(block_id_end, writeBlockInd, std::memory_order_release); //Prevents memory access from being reordered after this
        //+++ End write into array +++

        //Check for index wrap arround (note, there is an extra array element)
        if (writeOffset >= array_length)
        {
            writeOffset = 0;
        }
        else
        {
            writeOffset++;
        }

        //Update Write Ptr
        std::atomic_store_explicit(write_offset_ptr, writeOffset, std::memory_order_release);

        //Increment block id for next block
        writeBlockInd = writeBlockInd == idMax ? 0 : writeBlockInd+1;
        sampleVals = (sampleVals+1)%2;

        //Check stop flag
        stop = !std::atomic_flag_test_and_set_explicit(stop_flag, std::memory_order_acq_rel);
        if(stop){
            break;
        }

        //Perform NOPs
        for(int nop = 0; nop<numNops; nop++){
            asm volatile ("nop" : : :);
        }
    }

    FifolessBufferEndCondition *rtn = new FifolessBufferEndCondition;
    rtn->expectedBlockID = -1;
    rtn->startBlockID = -1;
    rtn->startBlockID = -1;
    rtn->wasErrorSrc = false;
    rtn->errored = stop;
    rtn->resultGranularity = HW_Granularity::CORE;
    rtn->granularityIndex = core;
    rtn->transaction = transfer;

    return (void*) rtn;

    // return nullptr;
}

//The client is the one that should be measuring time since it is what detects errors
//Make the client the primary (ie. in the client server runner, swap the functions given as the client and the server)
template<typename elementType, typename idType = std::atomic_int32_t, typename indexType = std::atomic_int32_t, typename idLocalType = int32_t, typename indexLocalType = int32_t, int idMax = INT32_MAX>
void* open_loop_buffer_client(void* arg){
    OpenLoopBufferArgs<elementType, idType, indexType>* args = (OpenLoopBufferArgs<elementType, idType, indexType>*) arg;
    indexType *read_offset_ptr = args->read_offset_ptr;
    void *array = args->array;
    int array_length = args->array_length;
    int64_t max_block_transfers = args->max_block_transfers;
    std::atomic_flag *start_flag = args->start_flag;
    std::atomic_flag *stop_flag = args->stop_flag;
    std::atomic_flag *ready_flag = args->ready_flag;
    int blockSize = args->blockSize;

    int core = args->core_client;

    int balancing_nops = args->balancing_nops;
    int numNops = balancing_nops > 0 ? balancing_nops : 0;
    numNops += args->initialNOPs;

    elementType expectedSampleVals = 0;
    idLocalType readBlockInd = 0;

    idLocalType newBlockIDStart = -1;
    idLocalType newBlockIDEnd = -1;

    idLocalType expectedBlockID = -1;

    elementType localBuffer[blockSize];

    int blockArrayBytes;
    int blockArrayPaddingBytes;
    int blockArrayCombinedBytes;
    int idBytes;
    int idPaddingBytes;
    int idCombinedBytes;
    int blockSizeBytes;

    getBlockSizing<elementType, idType>(args->blockSize, args->alignment, blockArrayBytes, blockArrayPaddingBytes, 
    blockArrayCombinedBytes, idBytes, idPaddingBytes, idCombinedBytes, blockSizeBytes);

    //Load the initial read offset
    indexLocalType readOffset = std::atomic_load_explicit(read_offset_ptr, std::memory_order_acquire);

    int numberInitBlocks = array_length/2; //Initialize FIFO to be half full (round down if odd number)

    //Begin reading entry (prime access to cache line)
    idType *block_id_end = (idType*) (((char*) array) + readOffset*blockSizeBytes + idCombinedBytes + blockArrayCombinedBytes);
    int dummyBlockIDEnd = std::atomic_load_explicit(block_id_end, std::memory_order_acquire);
    asm volatile(""
    : "=r" (dummyBlockIDEnd)
    : "r" (dummyBlockIDEnd)
    : );

    //Wait for ready
    bool ready = false;
    while(!ready){
        ready = !std::atomic_flag_test_and_set_explicit(ready_flag, std::memory_order_acq_rel);

        //Begin reading entry (prime access to cache line)
        idType *block_id_end = (idType*) (((char*) array) + readOffset*blockSizeBytes + idCombinedBytes + blockArrayCombinedBytes);
        int dummyBlockIDEnd = std::atomic_load_explicit(block_id_end, std::memory_order_acquire);
        asm volatile(""
        : "=r" (dummyBlockIDEnd)
        : "r" (dummyBlockIDEnd)
        : );
    }

    //Singal start
    std::atomic_thread_fence(std::memory_order_acquire);
    std::atomic_flag_clear_explicit(start_flag, std::memory_order_release);

    bool failureDetected = false;
    int64_t transfer;
    for(transfer = 0; transfer<(max_block_transfers+numberInitBlocks); transfer++){ //Need to do numberInitBlocks extra reads
        //---- Read from Input Buffer Unconditionally ----
        //Load Read Ptr
        if (readOffset >= array_length) //(note, there is an extra array element)
        {
            readOffset = 0;
        }
        else
        {
            readOffset++;
        }

        //+++ Read from array +++
        //The end ID is read first to check that the values being read from the array were completly written before this thread started reading.
        idType *block_id_start = (idType*) (((char*) array) + readOffset*blockSizeBytes);
        newBlockIDStart = std::atomic_load_explicit(block_id_start, std::memory_order_acquire);
        
        //Read elements
        //Read backwards to avoid cache thrashing
        elementType *data_array = (elementType*) (((char*) array) + readOffset*blockSizeBytes + idCombinedBytes);
        for(int sample = 0; sample<blockSize; sample++){
            localBuffer[sample] = data_array[sample];
        }

        //The start ID is read last to check that the block was not being overwritten while the data was being read
        std::atomic_signal_fence(std::memory_order_release); //Do not want an actual fence but do not want sample reading to be re-ordered before the end block ID read
        idType *block_id_end = (idType*) (((char*) array) + readOffset*blockSizeBytes + idCombinedBytes + blockArrayCombinedBytes);
        newBlockIDEnd = std::atomic_load_explicit(block_id_end, std::memory_order_acquire);
        
        //+++ End read from array +++

        //Update Read Ptr
        std::atomic_store_explicit(read_offset_ptr, readOffset, std::memory_order_release);

        //Check the read block IDs
        expectedBlockID = readBlockInd == UINT32_MAX ? 0 : readBlockInd+1;
        if(expectedBlockID != newBlockIDStart || expectedBlockID != newBlockIDEnd){
            //Will signal failure outside of loop
            failureDetected = true;
            readBlockInd = expectedBlockID;
            break;
        }
        readBlockInd = expectedBlockID;

        //Check elements
        for(int sample = 0; sample<blockSize; sample++){
            if(localBuffer[sample] != expectedSampleVals){
                // std::cerr << "Unexpected array data!" << std::endl;
                // exit(1);
                asm volatile(
                    ""
                    :
                    : "rm" (localBuffer[sample])
                    :
                );
            }
        }

        expectedSampleVals = (expectedSampleVals+1)%2;

        //Perform NOPs
        for(int nop = 0; nop<numNops; nop++){
            asm volatile ("nop" : : :);
        }

    }

    std::atomic_thread_fence(std::memory_order_acquire);
    std::atomic_flag_clear_explicit(stop_flag, std::memory_order_release);

    FifolessBufferEndCondition *rtn = new FifolessBufferEndCondition;
    rtn->expectedBlockID = readBlockInd;
    rtn->startBlockID = newBlockIDStart;
    rtn->endBlockID = newBlockIDEnd;
    rtn->wasErrorSrc = true;
    rtn->errored = failureDetected;
    rtn->resultGranularity = HW_Granularity::CORE;
    rtn->granularityIndex = core;
    rtn->transaction = transfer;

    return (void*) rtn;
}

#endif
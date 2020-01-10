#ifndef _CLOSED_LOOP_BUFFER_H
#define _CLOSED_LOOP_BUFFER_H

#include <atomic>
#include "open_loop_helpers.h"
#include "closed_loop_helpers.h"

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

template<typename elementType, 
         typename idType = std::atomic_int32_t, 
         typename indexType = std::atomic_int32_t, 
         typename nopsClient = std::atomic_int32_t>
struct ClosedLoopBufferArgs{
    indexType *read_offset_ptr;
    indexType *write_offset_ptr;
    void *array;
    std::atomic_flag *start_flag; //Used by the client to signal to the server that it should start writing
    std::atomic_flag *stop_flag; //Used by the client to signal to the server that an error occured and that it should stop
    std::atomic_flag *ready_flag; //Used by the server to signal to the client that it is ready to begin
    int array_length; //In Blocks  //Note: The FIFO Array has an additional block allocated to resolve the Empty/Full Ambiguity
    int64_t max_block_transfers; //The maximum number of blocks to transfer in this test
    int blockSize; //The block size (in elementType elements)
    nopsClient *clientNops; //A pointer to a shared integer which controls how many nops per loop itteration the client uses
    int32_t control_check_period; //The number of itterations of the main loop between control checks on the controller side
    int32_t control_client_check_period; //The number of itterations of the main loop before the client checks for a new control signal
    int32_t control_gain; //The gain of the control system
    int alignment; //The alignment in bytes of the components within the block (the IDs and the Buffer)
    int core_client; //The core the client is executing on
    int core_server; //The core the server is executing on
    int initialNops; //The initial NOPs for the client and server
};

//Note: The server and client threads are different instruction streams.  One writes while the other reads and needs to check the block ids (and the block data).
//The writer needs to periodically check the error condition flag provided by the reader.  This can result in the two functions having different
//execution times.  To compensate, nop loops are placed in each thread

template<typename elementType, 
         typename idType = std::atomic_int32_t, 
         typename indexType = std::atomic_int32_t, 
         typename nopsClientType = std::atomic_int32_t>
void* closed_loop_buffer_reset(void* arg){
    ClosedLoopBufferArgs<elementType, idType, indexType, nopsClientType> *args = (ClosedLoopBufferArgs<elementType, idType, indexType, nopsClientType>*) arg;

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

    //Reset the clientNops control
    std::atomic_store_explicit(args->clientNops, args->initialNops, std::memory_order_relaxed);

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

template<typename elementType, 
         typename idType = std::atomic_int32_t, 
         typename indexType = std::atomic_int32_t, 
         typename nopsClientType = std::atomic_int32_t>
void* closed_loop_buffer_cleanup(void* arg){
    ClosedLoopBufferArgs<elementType, idType, indexType, nopsClientType> *args = (ClosedLoopBufferArgs<elementType, idType, indexType, nopsClientType>*) arg;

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
//The server also acts as the controller in this case.  It is capable of slowing itself down and can command the reciever to slow down if
//nessasary via a shared ptr
template<typename elementType, 
         typename idType = std::atomic_int32_t, 
         typename indexType = std::atomic_int32_t, 
         typename idLocalType = int32_t, 
         typename indexLocalType = int32_t, 
         typename nopsClientType = std::atomic_int32_t,
         typename nopsClientLocalType = int32_t, 
         int idMax = INT32_MAX>
void* closed_loop_buffer_bang_control_server(void* arg){
    ClosedLoopBufferArgs<elementType, idType, indexType, nopsClientType>* args = (ClosedLoopBufferArgs<elementType, idType, indexType, nopsClientType>*) arg;

    indexType *write_offset_ptr = args->write_offset_ptr;
    indexType *read_offset_ptr = args->read_offset_ptr;
    void *array = args->array;
    int array_length = args->array_length;
    int64_t max_block_transfers = args->max_block_transfers;
    std::atomic_flag *start_flag = args->start_flag;
    std::atomic_flag *stop_flag = args->stop_flag;
    std::atomic_flag *ready_flag = args->ready_flag;
    int blockSize = args->blockSize;
    int allignment = args->alignment;
    int core = args->core_server;

    nopsClientType *clientNops = args->clientNops;
    int32_t control_check_period = args->control_check_period;
    int32_t control_gain = args->control_gain;

    nopsClientLocalType nops_server = args->initialNops;
    nopsClientLocalType nops_client_local = std::atomic_load_explicit(clientNops, std::memory_order_acquire);

    int halfFilledPoint = array_length/2; //Initialize FIFO to be half full (round down if odd number)
    int numberInitBlocks = halfFilledPoint;

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
    
    //Wait for start signal
    bool start = false;
    while(!start){
        start = !std::atomic_flag_test_and_set_explicit(start_flag, std::memory_order_acq_rel); //Start signal is active low
    }

    bool stop = false;
    int64_t transfer;
    int32_t controlCheckCounter = 0;
    int64_t controlCheckHist = 0;
    int64_t speedUpCount = 0;
    int64_t slowDownCount = 0;
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

        //==== Control ====
        if(controlCheckCounter < control_check_period){
            controlCheckCounter++;
        }else{
            controlCheckCounter = 0;
            controlCheckHist++;

            //Check Fullness
            //writeOffset is the local copy of the write offset
            //Load the 
            indexLocalType readOffset = std::atomic_load_explicit(read_offset_ptr, std::memory_order_acquire);
            indexLocalType numEntries = getNumItemsInBuffer(readOffset, writeOffset, array_length);

            //Check if the number of entries is above or below the half occupancy point
            if(numEntries>halfFilledPoint){
                //Above the set point, speed up the client or slow down the server
                slowDownCount++;

                //Slow down the server
                nops_server += control_gain;
            }else if(numEntries<halfFilledPoint){
                //Below the set point, speed up the server or slow down the client
                speedUpCount++;

                //First, check if the server can be sped up
                if(nops_server > 0){
                    nops_server -= control_gain;
                    if(nops_server<0){ //If the number of NOPS went negative, shift the difference to the client
                        nops_client_local-=nops_server;
                        nops_server=0;
                        std::atomic_store_explicit(clientNops, nops_client_local, std::memory_order_release);
                    }
                }else{
                    nops_client_local += control_gain;
                    std::atomic_store_explicit(clientNops, nops_client_local, std::memory_order_release);
                }
            }
        }

        //Perform NOPs
        for(nopsClientLocalType nop = 0; nop<nops_server; nop++){
            asm volatile ("nop" : : :);
        }
    }

    ClosedLoopServerEndCondition *rtn = new ClosedLoopServerEndCondition;
    rtn->resultGranularity = HW_Granularity::CORE;
    rtn->granularityIndex = core;

    rtn->controlChecks = controlCheckHist;
    rtn->speed_up_count = speedUpCount;
    rtn->slow_down_count = slowDownCount;
    rtn->serverNops = nops_server;
    rtn->clientNops = nops_client_local;
    rtn->errored = stop;
    rtn->transaction = transfer;

    return (void*) rtn;

    // return nullptr;
}

//The client is the one that should be measuring time since it is what detects errors
//Make the client the primary (ie. in the client server runner, swap the functions given as the client and the server)
template<typename elementType, 
         typename idType = std::atomic_int32_t, 
         typename indexType = std::atomic_int32_t, 
         typename idLocalType = int32_t, 
         typename indexLocalType = int32_t, 
         typename nopsClientType = std::atomic_int32_t, 
         typename nopsClientLocalType = int32_t, 
         int idMax = INT32_MAX>
void* closed_loop_buffer_client(void* arg){
    ClosedLoopBufferArgs<elementType, idType, indexType, nopsClientType>* args = (ClosedLoopBufferArgs<elementType, idType, indexType, nopsClientType>*) arg;
    indexType *read_offset_ptr = args->read_offset_ptr;
    void *array = args->array;
    int array_length = args->array_length;
    int64_t max_block_transfers = args->max_block_transfers;
    std::atomic_flag *start_flag = args->start_flag;
    std::atomic_flag *stop_flag = args->stop_flag;
    std::atomic_flag *ready_flag = args->ready_flag;
    int blockSize = args->blockSize;

    int core = args->core_client;

    int32_t control_client_check_period = args->control_client_check_period;
    nopsClientType *clientNops = args->clientNops;
    nopsClientLocalType clientNopsLocal = std::atomic_load_explicit(clientNops, std::memory_order_acquire);
    
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

    //Wait for ready
    bool ready = false;
    while(!ready){
        ready = !std::atomic_flag_test_and_set_explicit(ready_flag, std::memory_order_acq_rel);
    }

    //Singal start
    std::atomic_thread_fence(std::memory_order_acquire);
    std::atomic_flag_clear_explicit(start_flag, std::memory_order_release);

    bool failureDetected = false;
    int64_t transfer;
    int32_t controlCounter = 0;
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
        idType *block_id_end = (idType*) (((char*) array) + readOffset*blockSizeBytes + idCombinedBytes + blockArrayCombinedBytes);
        newBlockIDEnd = std::atomic_load_explicit(block_id_end, std::memory_order_acquire);
        
        //Read elements
        //Read backwards to avoid cache thrashing
        elementType *data_array = (elementType*) (((char*) array) + readOffset*blockSizeBytes + idCombinedBytes);
        for(int sample = blockSize-1; sample>=0; sample--){
            localBuffer[sample] = data_array[sample];
        }

        //The start ID is read last to check that the block was not being overwritten while the data was being read
        std::atomic_signal_fence(std::memory_order_release); //Do not want an actual fence but do not want sample reading to be re-ordered before the end block ID read
        idType *block_id_start = (idType*) (((char*) array) + readOffset*blockSizeBytes);
        newBlockIDStart = std::atomic_load_explicit(block_id_start, std::memory_order_acquire);
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
                std::cerr << "Unexpected array data!" << std::endl;
                exit(1);
            }
        }

        expectedSampleVals = (expectedSampleVals+1)%2;

        //Check for new control
        if(controlCounter < control_client_check_period){
            controlCounter++;
        }else{
            controlCounter = 0;

            clientNopsLocal = std::atomic_load_explicit(clientNops, std::memory_order_acquire);
        }

        //Perform NOPs
        for(nopsClientLocalType nop = 0; nop<clientNopsLocal; nop++){
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
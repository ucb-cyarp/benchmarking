#ifndef _CLOSED_LOOP_HELPERS_H
#define _CLOSED_LOOP_HELPERS_H

#include "intrin_bench_default_defines.h"
#include "results.h"
#include "mallocHelpers.h"
#include "reporting_helpers.h"
#include "open_loop_helpers.h"
#include "fifoless_helpers.h"
#include "sir.h"
#include "fast_copy.h"

#include <vector>
#include <atomic>
#include <stdlib.h>
#include <stdint.h>
#include <sys/ioctl.h>

// #define CLOSED_LOOP_DISABLE_INTERRUPTS 1

#define TEST_ELEMENT_WRAPAROUND (65536) //2^16

template<typename elementType, 
         typename idType, 
         typename indexType, 
         typename nopsClient,
         typename nopsLocal> //Note, this can be changed to a float for PI controllers
class ClosedLoopBufferArgs : public FifolessConfig{
public:
    indexType *read_offset_ptr;
    indexType *write_offset_ptr;
    void *array;
    void *local_array_reader;
    std::atomic_flag *start_flag; //Used by the client to signal to the server that it should start writing
    std::atomic_flag *stop_flag; //Used by the client to signal to the server that an error occured and that it should stop
    std::atomic_flag *ready_flag; //Used by the server to signal to the client that it is ready to begin
    int array_length; //In Blocks  //Note: The FIFO Array has an additional block allocated to resolve the Empty/Full Ambiguity
    int64_t max_block_transfers; //The maximum number of blocks to transfer in this test
    int blockSize; //The block size (in elementType elements)
    nopsClient *clientNops; //A pointer to a shared integer which controls how many nops per loop itteration the client uses
    int32_t control_check_period; //The number of itterations of the main loop between control checks on the controller side
    int32_t control_client_check_period; //The number of itterations of the main loop before the client checks for a new control signal
    int alignment; //The alignment in bytes of the components within the block (the IDs and the Buffer)
    int core_client; //The core the client is executing on
    int core_server; //The core the server is executing on
    nopsLocal initialNops; //The initial NOPs for the client and server
    FILE* writerSirFile; //The file pointer to the sir driver for the writer (used for disabling interrupts)
    FILE* readerSirFile; //The file pointer to the sir driver for the reader (used for disabling interrupts)

protected:
    virtual void printExportNonStandaloneResults(Results &result, bool report_standalone, std::string resultPrintFormatStr, FILE* file, std::ofstream* raw_file) = 0;
};

//See ==== Entries Available to read ==== In Lockless Thread Crossing FIFO (coppied here for convenience):
/*
 * ==== Entries Available to read ====
 * % 1 2 3 _ _ _ ((readOffset < writeOffset) : writeOffset - readOffset - 1 = 3)
 * r       w
 *
 * 7 _ _ % 4 5 6 ((readOffset >= writeOffset) : arrayLength - (readOffset - writeOffset + 1) = 4) | arrayLength includes the extra entry
 *   w   r
 *
 * 7 8 9 % 4 5 6 ((readOffset >= writeOffset) : arrayLength - (readOffset - writeOffset + 1) = 6)
 *       r
 *       w
 *
 * _ % _ _ _ _ _ ((readOffset < writeOffset) : writeOffset - readOffset - 1 = 0)
 *   r w
 *
 * _ _ _ _ _ _ _ ((readOffset >= writeOffset) : arrayLength - (readOffset - writeOffset + 1) = 0)
 * w           r
 *
 * Avail to read: (readOffset < writeOffset) ? writeOffset - readOffset - 1 : arrayLength - (readOffset - writeOffset + 1)
 * Avail to read: (readOffset < writeOffset) ? writeOffset - readOffset - 1 : arrayLength - readOffset + writeOffset - 1
 */

template<typename T>
inline T getNumItemsInBuffer(T readOffset, T writeOffset, T fifoLength){
    int arrayLength = fifoLength+1;

    return (readOffset < writeOffset) ? writeOffset - readOffset - 1 : arrayLength - readOffset + writeOffset - 1;
}

class ClosedLoopServerEndCondition : public BenchmarkSpecificResult{
public:
    int64_t controlChecks;
    int64_t speed_up_count;
    int64_t slow_down_count;
    float serverNops;
    float clientNops;
    bool errored;
    int64_t transaction;

    std::string getTrialResultsHeader() override;
    std::string getTrialResults() override;
    std::string getTrialCSVHeader() override;
    std::string getTrialCSVData() override;

    ClosedLoopServerEndCondition();
};

template<typename elementType, typename atomicIdType, typename atomicIndexType, typename nopCountType>
size_t closedLoopAllocate(std::vector<elementType*> &shared_array_locs, std::vector<elementType*> &local_array_locs, std::vector<atomicIndexType*> &shared_write_id_locs, std::vector<atomicIndexType*> &shared_read_id_locs, std::vector<std::atomic_flag*> &ready_flags, std::vector<std::atomic_flag*> &start_flags, std::atomic_flag* &stop_flag, std::vector<nopCountType*> &nopControls, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, std::vector<int> cpus, int alignment, bool circular, bool include_dummy_flags){
    int maxBufferSize = openLoopAllocate<elementType, atomicIdType, atomicIndexType>(shared_array_locs, local_array_locs, shared_write_id_locs, shared_read_id_locs, ready_flags, start_flags, stop_flag, array_lengths, block_lengths, cpus, alignment, circular, include_dummy_flags);

    for(int i = 1; i<cpus.size(); i++){//Do not need one for the controller
        nopCountType *nopControl = (nopCountType*) aligned_alloc_core(CACHE_LINE_SIZE, sizeof(nopCountType), cpus[i]);
        nopCountType *nopControlConstructed = new (nopControl) nopCountType(0);
        //std::atomic_init(nopControl, 0); //Init of the shared ptrs
        nopControls.push_back(nopControl);
    }

    return maxBufferSize;
}

template<typename atomicIndexType, typename nopCountType>
void destructSharedIDs(std::vector<atomicIndexType*> &shared_write_id_locs, std::vector<atomicIndexType*> &shared_read_id_locs, std::vector<std::atomic_flag*> &ready_flags, std::vector<std::atomic_flag*> &start_flags, std::atomic_flag* &stop_flag, std::vector<nopCountType*> &nopControl){
    destructSharedIDs(shared_write_id_locs, shared_read_id_locs, ready_flags, start_flags, stop_flag);

    for(int i = 0; i<nopControl.size(); i++){
        nopControl[i]->~nopCountType();
    }
}

template<typename elementType, 
         typename idType, 
         typename indexType, 
         typename nopsClientType,
         typename nopsClientTypeLocal>
void* closed_loop_buffer_reset(void* arg){
    ClosedLoopBufferArgs<elementType, idType, indexType, nopsClientType, nopsClientTypeLocal> *args = (ClosedLoopBufferArgs<elementType, idType, indexType, nopsClientType, nopsClientTypeLocal>*) arg;

    int blockArrayBytes;
    int blockArrayPaddingBytes;
    int blockArrayCombinedBytes;
    int blockSizeBytes;

    getBlockSizing<elementType>(args->blockSize, args->alignment, blockArrayBytes, blockArrayPaddingBytes, blockArrayCombinedBytes, blockSizeBytes);

    std::atomic_thread_fence(std::memory_order_acquire);

    int numberInitBlocks = args->array_length/2; //Initialize FIFO to be half full (round down if odd number)

    //Using memory order released so that initialization occures in the order it occures in steady state operation

    //Reset the reader array
    elementType* local_array = (elementType*) args->local_array_reader;
    for(int i = 0; i<args->blockSize; i++){
        local_array[i] = 0;
    }
    char* local_array_padding = (char*) (local_array+args->blockSize);
    for(int i = 0; i<FAST_COPY_ALIGNED_PADDING; i++){
        local_array_padding[i] = 0;
    }

    //Reset the index pointers (they are initialized outside)
    std::atomic_store_explicit(args->read_offset_ptr, 0, std::memory_order_release); //Initialized to 0.  This is the index last read.  Index 1 contains the first initial value
    std::atomic_store_explicit(args->write_offset_ptr, numberInitBlocks+1, std::memory_order_release); //Initialized to index after that last initial value in the FIFO (note, the initial elements start at index 1)

    //=== Init ===
    //Reset the block ids and data

    //Init Blocks After Initial Conditions (these are initialized first because, after the reset, these will be the first blocks to be written to by the writer)
    for(int i = numberInitBlocks+1; i <= args->array_length; i++){ //Note, there is an extra element in the array
        elementType *data_array = (elementType*) (((char*) args->array) + i*blockSizeBytes);
        for(int j = 0; j<args->blockSize; j++){
            data_array[j] = -1;
        }
    }

    std::atomic_signal_fence(std::memory_order_release);

    //Init ID Zero
    elementType *data_array = (elementType*) (((char*) args->array));
    for(int j = 0; j<args->blockSize; j++){
        data_array[j] = -1;
    }

    std::atomic_signal_fence(std::memory_order_release);

    //Initial Conditions
    for(int i = 1; i<numberInitBlocks+1; i++){
        elementType *data_array = (elementType*) (((char*) args->array) + i*blockSizeBytes);
        for(int j = 0; j<args->blockSize; j++){
            data_array[j] = (i+1)%TEST_ELEMENT_WRAPAROUND;
        }
    }

    //Reset the clientNops control (this is written to after the array is written to in the steady state case)
    std::atomic_store_explicit(args->clientNops, (nopsClientTypeLocal) args->initialNops, std::memory_order_release);
    
    //Flags are initialized and destructed outside
    std::atomic_flag_test_and_set_explicit(args->start_flag, std::memory_order_release);
    std::atomic_flag_test_and_set_explicit(args->stop_flag, std::memory_order_release);
    std::atomic_flag_test_and_set_explicit(args->ready_flag, std::memory_order_release);

    std::atomic_thread_fence(std::memory_order_release);
}

template<typename elementType, 
         typename idType, 
         typename indexType, 
         typename nopsClientType,
         typename nopsLocalType>
void* closed_loop_buffer_cleanup(void* arg){
    ClosedLoopBufferArgs<elementType, idType, indexType, nopsClientType, nopsLocalType> *args = (ClosedLoopBufferArgs<elementType, idType, indexType, nopsClientType, nopsLocalType>*) arg;

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

    std::atomic_thread_fence(std::memory_order_release);
}

//For versions where the number of NOPs is an integer
template<typename elementType, 
         typename idType, 
         typename indexType, 
         typename idLocalType, 
         typename indexLocalType, 
         typename nopsClientType, 
         typename nopsClientLocalType, 
         int idMax = INT32_MAX>
void* closed_loop_buffer_int_client(void* arg){
    ClosedLoopBufferArgs<elementType, idType, indexType, nopsClientType, nopsClientLocalType>* args = (ClosedLoopBufferArgs<elementType, idType, indexType, nopsClientType, nopsClientLocalType>*) arg;
    indexType *read_offset_ptr = args->read_offset_ptr;
    void *array = args->array;
    int array_length = args->array_length;
    int64_t max_block_transfers = args->max_block_transfers;
    std::atomic_flag *start_flag = args->start_flag;
    std::atomic_flag *stop_flag = args->stop_flag;
    std::atomic_flag *ready_flag = args->ready_flag;
    int blockSize = args->blockSize;

    int core = args->core_client;

    FILE* sirFile = args->readerSirFile;

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

    //==== Load the initial read offset ====
    indexLocalType readOffset = std::atomic_load_explicit(read_offset_ptr, std::memory_order_acquire);

    int numberInitBlocks = array_length/2; //Initialize FIFO to be half full (round down if odd number)

    //Disable Interrupts For This Trial (Before Signalling Ready)
    #if CLOSED_LOOP_DISABLE_INTERRUPTS == 1
        int status = ioctl(fileno(sirFile), SIR_IOCTL_DISABLE_INTERRUPT, NULL);
        if(status < 0){
            std::cerr << "Problem accessing /dev/sir0 to Disable Interrupts" << std::endl;
            exit(1);
        }
    #endif

    //==== Wait for ready ====
    bool ready = false;
    while(!ready){
        ready = !std::atomic_flag_test_and_set_explicit(ready_flag, std::memory_order_acq_rel);
    }

    //==== Read the uninitialized entries in the FIFO ====
    //The writer will have written the initial conditions into the FIFO and therefor will have
    //exclusive access to the cache lines in the array.  Reading the entries will force them
    //into the shared state which will force the writer to invalidate there entries in the reader.
    //This is what it needs to do in the steady state case and should do at the start as well.
    
    //Read Elements beyond initial condition
    for(int i = numberInitBlocks+1; i <= array_length+1; i++){ //Note, there is an extra element in the array
        int ind = i<=array_length ? i : 0;
        idType *block_id_end = (idType*) (((char*) array) + ind*blockSizeBytes + idCombinedBytes);
        newBlockIDEnd = std::atomic_load_explicit(block_id_end, std::memory_order_acquire);
        asm volatile(""
        :
        : "r" (newBlockIDEnd)
        :);
        
        //Read elements
        //Read backwards to avoid cache thrashing
        elementType *data_array = (elementType*) (((char*) array) + ind*blockSizeBytes + idCombinedBytes*2);
        for(int sample = 0; sample<blockSize; sample++){
            localBuffer[sample] = data_array[sample];
            asm volatile(""
            :
            : "r" (localBuffer[sample])
            :);
        }

        //The start ID is read last to check that the block was not being overwritten while the data was being read
        std::atomic_signal_fence(std::memory_order_release); //Do not want an actual fence but do not want sample reading to be re-ordered before the end block ID read
        idType *block_id_start = (idType*) (((char*) array) + ind*blockSizeBytes);
        newBlockIDStart = std::atomic_load_explicit(block_id_start, std::memory_order_acquire);
        asm volatile(""
        :
        : "r" (newBlockIDStart)
        :);
    }

    //==== Singal start ====
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
        idType *block_id_end = (idType*) (((char*) array) + readOffset*blockSizeBytes + idCombinedBytes);
        newBlockIDEnd = std::atomic_load_explicit(block_id_end, std::memory_order_acquire);
        
        //Read elements
        //Read backwards to avoid cache thrashing
        //Read forwards to allow compiler to optimize
        elementType *data_array = (elementType*) (((char*) array) + readOffset*blockSizeBytes + idCombinedBytes*2);

        //Need to align localBuffer to data_array so that AVX load instructions can be used
        //localBuffer is oversized by 256 bits (32 bytes) to allow for this shift

        //The localBufferAlignedBase will be set so that it has the same position inside of a 32 byte block as
        //data_array.
        //It will use standard load/store instructions until the cursors are aligned to a 16 byte boundary
        //If the 16 byte boundary is not a 32 byte boundary, it will use a SSE load/store instruction
        //It will then use the AVX load/store instructions to copy data.
        //It will then use an SSE instruction to copy remaining data (if applicable)
        //It will then use standard load/store instructions to copy the remaining data

        for(int sample = 0; sample<blockSize; sample++){
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

    //Re-enable interrupts before processing results (which dynamically allocates memory which can result in a system call)
    #if CLOSED_LOOP_DISABLE_INTERRUPTS == 1
        status = ioctl(fileno(sirFile), SIR_IOCTL_RESTORE_INTERRUPT, NULL);
        if(status < 0){
            std::cerr << "Problem accessing /dev/sir0 to Restore Interrupts" << std::endl;
            exit(1);
        }
    #endif

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

//For versions where the number of NOPs is kept as a float
template<typename elementType, 
         typename idType, 
         typename indexType, 
         typename idLocalType, 
         typename indexLocalType, 
         typename nopsClientType, 
         typename nopsClientLocalType, 
         int idMax = INT32_MAX>
void* closed_loop_buffer_float_client(void* arg){
    ClosedLoopBufferArgs<elementType, idType, indexType, nopsClientType, nopsClientLocalType>* args = (ClosedLoopBufferArgs<elementType, idType, indexType, nopsClientType, nopsClientLocalType>*) arg;
    indexType *read_offset_ptr = args->read_offset_ptr;
    void *array = args->array;
    int array_length = args->array_length;
    int64_t max_block_transfers = args->max_block_transfers;
    std::atomic_flag *start_flag = args->start_flag;
    std::atomic_flag *stop_flag = args->stop_flag;
    std::atomic_flag *ready_flag = args->ready_flag;
    int blockSize = args->blockSize;

    int core = args->core_client;

    FILE* sirFile = args->readerSirFile;

    int32_t control_client_check_period = args->control_client_check_period;
    nopsClientType *clientNops = args->clientNops;
    nopsClientLocalType nops_current = 0; //Is used to carry over residual between nop cycles
    nopsClientLocalType clientNopsLocal = std::atomic_load_explicit(clientNops, std::memory_order_acquire);
    
    elementType expectedSampleVals = (1+1)%TEST_ELEMENT_WRAPAROUND;
    idLocalType readBlockInd = 0;

    idLocalType newBlockIDStart = -1;
    idLocalType newBlockIDEnd = -1;

    idLocalType expectedBlockID = -1;

    elementType* localBufferRaw = (elementType*) args->local_array_reader;

    int blockArrayBytes;
    int blockArrayPaddingBytes;
    int blockArrayCombinedBytes;
    int blockSizeBytes;

    getBlockSizing<elementType>(args->blockSize, args->alignment, blockArrayBytes, blockArrayPaddingBytes, 
    blockArrayCombinedBytes, blockSizeBytes);

    //==== Load the initial read offset ====
    indexLocalType readOffset = std::atomic_load_explicit(read_offset_ptr, std::memory_order_acquire);

    int numberInitBlocks = array_length/2; //Initialize FIFO to be half full (round down if odd number)

    //Disable Interrupts For This Trial (Before Signalling Ready)
    #if CLOSED_LOOP_DISABLE_INTERRUPTS == 1
        int status = ioctl(fileno(sirFile), SIR_IOCTL_DISABLE_INTERRUPT, NULL);
        if(status < 0){
            std::cerr << "Problem accessing /dev/sir0 to Disable Interrupts" << std::endl;
            exit(1);
        }
    #endif

    //==== Wait for ready ====
    bool ready = false;
    while(!ready){
        ready = !std::atomic_flag_test_and_set_explicit(ready_flag, std::memory_order_acq_rel);
    }

    //==== Read the uninitialized entries in the FIFO ====
    //The writer will have written the initial conditions into the FIFO and therefor will have
    //exclusive access to the cache lines in the array.  Reading the entries will force them
    //into the shared state which will force the writer to invalidate there entries in the reader.
    //This is what it needs to do in the steady state case and should do at the start as well.
    
    //Read Elements beyond initial condition
    for(int i = numberInitBlocks+1; i <= array_length+1; i++){ //Note, there is an extra element in the array
        int ind = i<=array_length ? i : 0;
        //Read elements
        elementType *data_array = (elementType*) (((char*) array) + ind*blockSizeBytes);
        for(int sample = 0; sample<blockSize; sample++){
            localBufferRaw[sample] = data_array[sample];
            asm volatile(""
            :
            : "r" (localBufferRaw[sample])
            :);
        }
    }

    //Write to local array (will have been reset by the server and need to re-aquire exclusive access)
    for(int i = 0; i < blockSize; i++){
        localBufferRaw[i] = 0;
    }
    char* local_array_padding = (char*) (localBufferRaw+args->blockSize);
    for(int i = 0; i<FAST_COPY_ALIGNED_PADDING; i++){
        local_array_padding[i] = 0;
    }

    //==== Singal start ====
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

        std::atomic_signal_fence(std::memory_order_acquire);

        //+++ Read from array +++
        //Read elements
        elementType *data_array = (elementType*) (((char*) array) + readOffset*blockSizeBytes);
        // for(int sample = 0; sample<blockSize; sample++){
        //     localBuffer[sample] = data_array[sample];
        // }
        // elementType* localBuffer = fast_copy_aligned(data_array, localBufferRaw, blockSize, FAST_COPY_ALIGNED_PADDING);
        // elementType* localBuffer = fast_copy_semialigned(data_array, localBufferRaw, blockSize);
        // elementType* localBuffer = fast_copy_unaligned(data_array, localBufferRaw, blockSize);
        elementType* localBuffer = fast_copy_unaligned_ramp_in(data_array, localBufferRaw, blockSize);

        //The start ID is read last to check that the block was not being overwritten while the data was being read
        std::atomic_signal_fence(std::memory_order_release); //Do not want an actual fence but do not want sample reading to be re-ordered before the end block ID read
        //+++ End read from array +++

        //Update Read Ptr
        std::atomic_store_explicit(read_offset_ptr, readOffset, std::memory_order_release);

        //Check elements
        for(int sample = 0; sample<blockSize; sample++){
            if(localBuffer[sample] != expectedSampleVals){
                //Will signal failure outside of loop
                // printf("Got %d expected %d\n", localBuffer[sample], expectedSampleVals);
                failureDetected = true;
                break;
            }
        }

        if(failureDetected){
            break;
        }

        expectedSampleVals = (expectedSampleVals+1)%TEST_ELEMENT_WRAPAROUND;

        //Check for new control
        if(controlCounter < control_client_check_period){
            controlCounter++;
        }else{
            controlCounter = 0;

            clientNopsLocal = std::atomic_load_explicit(clientNops, std::memory_order_acquire);
        }

        //Perform NOPs
        nops_current += clientNopsLocal;//nop_current contains the resitual from the last round
        int32_t nops_current_trunk = std::floor(nops_current); //Truncate to a fixed number of NOPs
        for(int32_t nop = 0; nop<nops_current_trunk; nop++){
            asm volatile ("nop" : : :);
        }
        nops_current -= nops_current_trunk;//Get the residual for the next round
    }

    std::atomic_thread_fence(std::memory_order_acquire);
    std::atomic_flag_clear_explicit(stop_flag, std::memory_order_release);

    //Re-enable interrupts before processing results (which dynamically allocates memory which can result in a system call)
    #if CLOSED_LOOP_DISABLE_INTERRUPTS == 1
        status = ioctl(fileno(sirFile), SIR_IOCTL_RESTORE_INTERRUPT, NULL);
        if(status < 0){
            std::cerr << "Problem accessing /dev/sir0 to Restore Interrupts" << std::endl;
            exit(1);
        }
    #endif

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


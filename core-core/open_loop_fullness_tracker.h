#ifndef _H_OPEN_LOOP_FULLNESS_TRACKER
#define _H_OPEN_LOOP_FULLNESS_TRACKER

#include <atomic>
#include "open_loop_helpers.h"
#include "open_loop_buffer.h"
#include "closed_loop_helpers.h"
#include <sys/ioctl.h>
#include "sir.h"
#include "time_helpers.h"

//This benchmarks measures the fullness of the buffer at the start of a reader cycle.  This is slightly
//different from the closed loop test case which measures at the end of a writer cycle.  It is read
//earlier in the cycle so that, in the error case, the measurement had already been made. 


template<typename elementType, typename idType = std::atomic_int32_t, typename indexType = std::atomic_int32_t>
class OpenLoopFullnessTrackerBufferArgs : public OpenLoopBufferArgs<elementType, idType, indexType>{
public:
    int checkPeriod; //Number of cycles between checks
    //These buffers are used to record the fullness values
    int32_t* startTracker;
    int32_t* startInterruptTracker;
    double* startTimingTracker;
    int startTrackerLen;
    int32_t* endTracker;
    int32_t* endInterruptTracker;
    double* endTimingTracker;
    int endTrackerLen;
    FILE* readerInterruptReporter;
    FILE* writerInterruptReporter;

    int32_t* startInterruptTrackerWriter;
    double* startTimingTrackerWriter;
    int32_t* endInterruptTrackerWriter;
    double* endTimingTrackerWriter;

    void printStandaloneTitle(bool report_standalone, std::string title) override; //Prints the standalone title block if standalone results are requested
protected:
    virtual void printExportNonStandaloneResults(Results &result, bool report_standalone, std::string resultPrintFormatStr, FILE* file, std::ofstream* raw_file) override;
};

template<typename elementType, 
        typename idType, 
        typename indexType>
void OpenLoopFullnessTrackerBufferArgs<elementType, idType, indexType>::printStandaloneTitle(bool report_standalone, std::string title){
    //Print the title if a standalone report
    #if PRINT_TITLE == 1
    if(report_standalone)
    {
        printf("\n");
        printTitle(title);
        printf("Array Length: %d Blocks, Block Length: %d int32_t Elements, Balancing NOPs: %d, Initial NOPs: %d, Check Period: %d\n", this->array_length, this->blockSize, this->balancing_nops, this->initialNOPs, this->checkPeriod);
        fflush(stdout);
    }
    #endif
}

template<typename elementType, 
        typename idType, 
        typename indexType>
void OpenLoopFullnessTrackerBufferArgs<elementType, idType, indexType>::printExportNonStandaloneResults(Results &result, bool report_standalone, std::string resultPrintFormatStr, FILE* file, std::ofstream* raw_file){
        if(!report_standalone){
            //Print the complete results
        double avg_duration_ms = result.avg_duration();
        double stddev_duration_ms = result.stddev_duration();

        #if PRINT_STATS == 1
        printf(resultPrintFormatStr.c_str(), this->array_length, this->blockSize, this->balancing_nops, this->initialNOPs, avg_duration_ms, stddev_duration_ms);
        #endif

        #if WRITE_CSV == 1
        fprintf(file, "%d,%d,%d,%d,%d,%f,%f\n", this->array_length, this->blockSize, this->balancing_nops, this->initialNOPs, this->checkPeriod, avg_duration_ms, stddev_duration_ms);
        result.write_durations_and_benchmark_specific_results(*raw_file, {"", "", "", ""}, {std::to_string(this->array_length), std::to_string(this->blockSize), std::to_string(this->balancing_nops), std::to_string(this->initialNOPs), std::to_string(this->checkPeriod)}, false);
        #endif
    }
}

void writeRawHeaderOpenLoopFullnessTracker(std::vector<std::shared_ptr<BenchmarkSpecificResult>> implSpecificResults, std::ofstream* raw_file);

class FifolessBufferFullnessTrackerEndCondition : public FifolessBufferEndCondition{
public:
    std::vector<int32_t> startTracker;
    std::vector<int32_t> endTracker;
    std::vector<int32_t> startInterruptTracker;
    std::vector<int32_t> endInterruptTracker;
    std::vector<double> startTimingTracker;
    std::vector<double> endTimingTracker;

    //The fullness tracking is only reported in the CSV file
    // std::string getTrialResultsHeader() override;
    // std::string getTrialResults() override;
    std::string getTrialCSVHeader() override;
    std::string getTrialCSVData() override;

    FifolessBufferFullnessTrackerEndCondition();
};

//TODO: Bundle this with SIR
inline uint64_t readInterrupts(FILE* interruptReader){
    uint64_t interrupts;

    int status = ioctl(fileno(interruptReader), SIR_IOCTL_GET, &interrupts);
    if(status < 0){
        std::cerr << "Problem reading /dev/sir0" << std::endl;
        exit(1);
    }
    return interrupts;
}

template<typename elementType, typename atomicIdType, typename atomicIndexType>
size_t openLoopFullnessTrackerAllocate(std::vector<elementType*> &shared_array_locs, std::vector<atomicIndexType*> &shared_write_id_locs, std::vector<atomicIndexType*> &shared_read_id_locs, std::vector<std::atomic_flag*> &ready_flags, std::vector<std::atomic_flag*> &start_flags, std::atomic_flag* &stop_flag, std::vector<int32_t*> &startTracker, std::vector<int32_t*> &startInterruptTracker, std::vector<double*> &startTimingTracker, std::vector<int32_t*> &endTracker, std::vector<int32_t*> &endInterruptTracker, std::vector<double*> &endTimingTracker, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, std::vector<int> cpus, int alignment, bool circular, bool include_dummy_flags, int startTrackerLen, int endTrackerLen){
    //Run the standard open loop allocator
    size_t maxBufferSize = openLoopAllocate<elementType, atomicIdType, atomicIndexType>(shared_array_locs, shared_write_id_locs, shared_read_id_locs, ready_flags, start_flags, stop_flag, array_lengths, block_lengths, cpus, alignment, circular, include_dummy_flags);

    //Allocate the tracker buffers
    int num_buffers = circular ? cpus.size() : cpus.size()-1;
    //Initialize
    for(int buffer = 0; buffer<num_buffers; buffer++){
        //Start Tracker
        size_t amountToAllocStartTracker = sizeof(int32_t)*startTrackerLen;
        if(amountToAllocStartTracker % CACHE_LINE_SIZE != 0){
            amountToAllocStartTracker += (CACHE_LINE_SIZE - (amountToAllocStartTracker % CACHE_LINE_SIZE));
        }
        int32_t *startTrackerInst = (int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocStartTracker, cpus[buffer]);
        startTracker.push_back(startTrackerInst);

        //Start Interrupt Tracker
        size_t amountToAllocStartInterruptTracker = sizeof(int32_t)*startTrackerLen;
        if(amountToAllocStartInterruptTracker % CACHE_LINE_SIZE != 0){
            amountToAllocStartInterruptTracker += (CACHE_LINE_SIZE - (amountToAllocStartInterruptTracker % CACHE_LINE_SIZE));
        }
        //There are 2 of these arrays, one for the client and one for the server
        for(int i = 0; i<2; i++){
            int32_t *startInterruptTrackerInst = (int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocStartInterruptTracker, cpus[buffer]);
            startInterruptTracker.push_back(startInterruptTrackerInst);
        }

        //Start Timing 
        size_t amountToAllocStartTimingTracker = sizeof(double)*startTrackerLen;
        if(amountToAllocStartTimingTracker % CACHE_LINE_SIZE != 0){
            amountToAllocStartTimingTracker += (CACHE_LINE_SIZE - (amountToAllocStartTimingTracker % CACHE_LINE_SIZE));
        }
        //There are 2 of these arrays, one for the client and one for the server
        for(int i = 0; i<2; i++){
            double *startTimingTrackerInst = (double*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocStartTimingTracker, cpus[buffer]);
            startTimingTracker.push_back(startTimingTrackerInst);
        }

        //End Tracker
        size_t amountToAllocEndTracker = sizeof(int32_t)*endTrackerLen;
        if(amountToAllocEndTracker % CACHE_LINE_SIZE != 0){
            amountToAllocEndTracker += (CACHE_LINE_SIZE - (amountToAllocEndTracker % CACHE_LINE_SIZE));
        }
        int32_t *endTrackerInst = (int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocEndTracker, cpus[buffer]);
        endTracker.push_back(endTrackerInst);

        //End Interrupt Tracker
        size_t amountToAllocEndInterruptTracker = sizeof(int32_t)*endTrackerLen;
        if(amountToAllocEndInterruptTracker % CACHE_LINE_SIZE != 0){
            amountToAllocEndInterruptTracker += (CACHE_LINE_SIZE - (amountToAllocEndInterruptTracker % CACHE_LINE_SIZE));
        }
        //There are 2 of these arrays, one for the client and one for the server
        for(int i = 0; i<2; i++){
            int32_t *endInterruptTrackerInst = (int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocEndInterruptTracker, cpus[buffer]);
            endInterruptTracker.push_back(endInterruptTrackerInst);
        }

        //End Timing 
        size_t amountToAllocEndTimingTracker = sizeof(double)*endTrackerLen;
        if(amountToAllocEndTimingTracker % CACHE_LINE_SIZE != 0){
            amountToAllocEndTimingTracker += (CACHE_LINE_SIZE - (amountToAllocEndTimingTracker % CACHE_LINE_SIZE));
        }
        //There are 2 of these arrays, one for the client and one for the server
        for(int i = 0; i<2; i++){
            double *endTimingTrackerInst = (double*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocEndTimingTracker, cpus[buffer]);
            endTimingTracker.push_back(endTimingTrackerInst);
        }
    }

    return maxBufferSize;
}

//In addition to typical open loop reset, reset the capture buffers.
template<typename elementType, typename idType = std::atomic_int32_t, typename indexType = std::atomic_int32_t>
void* open_loop_fullness_tracker_buffer_reset(void* arg){
    OpenLoopFullnessTrackerBufferArgs<elementType, idType, indexType> *args = (OpenLoopFullnessTrackerBufferArgs<elementType, idType, indexType>*) arg;
    
    //Perform the original open loop reset
    open_loop_buffer_reset_internal(args);
    
    int32_t* startTracker = args->startTracker;
    int32_t* startInterruptTracker = args->startInterruptTracker;
    double* startTimingTracker = args->startTimingTracker;
    int startTrackerLen = args->startTrackerLen;
    int32_t* endTracker = args->endTracker;
    int32_t* endInterruptTracker = args->endInterruptTracker;
    double* endTimingTracker = args->endTimingTracker;
    int endTrackerLen = args->endTrackerLen;

    for(int i = 0; i<startTrackerLen; i++){
        startTracker[i] = -1;
        startInterruptTracker[i] = -1;
        startTimingTracker[i] = -1;
    }

    for(int i = 0; i<endTrackerLen; i++){
        endTracker[i] = -1;
        endInterruptTracker[i] = -1;
        endTimingTracker[i] = -1;
    }

    std::atomic_thread_fence(std::memory_order_release);

    return nullptr;
}

//The client is the one that should be measuring time since it is what detects errors
//Make the client the primary (ie. in the client server runner, swap the functions given as the client and the server)
template<typename elementType, typename idType = std::atomic_int32_t, typename indexType = std::atomic_int32_t, typename idLocalType = int32_t, typename indexLocalType = int32_t, int idMax = INT32_MAX>
void* open_loop_fullness_tracker_buffer_client(void* arg){
    OpenLoopFullnessTrackerBufferArgs<elementType, idType, indexType>* args = (OpenLoopFullnessTrackerBufferArgs<elementType, idType, indexType>*) arg;
    indexType *read_offset_ptr = args->read_offset_ptr;
    indexType *write_offset_ptr = args->write_offset_ptr;
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

    FILE* interruptReporterFile = args->readerInterruptReporter;

    //Buffers
    int checkPeriod = args->checkPeriod;
    int32_t* startTracker = args->startTracker;
    int32_t* startInterruptTracker = args->startInterruptTracker;
    double* startTimingTracker = args->startTimingTracker;
    int startTrackerLen = args->startTrackerLen;
    int32_t* endTracker = args->endTracker;
    int32_t* endInterruptTracker = args->endInterruptTracker;
    double* endTimingTracker = args->endTimingTracker;
    int endTrackerLen = args->endTrackerLen;
    int endTrackerInd = 0; //This is the index to write next.  It is the tail of the buffer.  When reading back, this address is read first then is incremented until it has been reached again
    int startTrackerInd = 0;
    int checkCounter = 0;

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
        idType *block_id_end = (idType*) (((char*) array) + ind*blockSizeBytes + idCombinedBytes + blockArrayCombinedBytes);
        newBlockIDEnd = std::atomic_load_explicit(block_id_end, std::memory_order_acquire);
        asm volatile(""
        :
        : "r" (newBlockIDEnd)
        :);
        
        //Read elements
        //Read backwards to avoid cache thrashing
        elementType *data_array = (elementType*) (((char*) array) + ind*blockSizeBytes + idCombinedBytes);
        for(int sample = blockSize-1; sample>=0; sample--){
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

    //Get initial time & interrupts
    timespec_t lastTime;
    clock_gettime(CLOCK_MONOTONIC, &lastTime);
    uint64_t lastInterrupts = readInterrupts(interruptReporterFile);

    bool failureDetected = false;
    int64_t transfer;
    for(transfer = 0; transfer<(max_block_transfers+numberInitBlocks); transfer++){ //Need to do numberInitBlocks extra reads
        //---- Check fullness ----
        if(checkCounter>=checkPeriod){
            checkCounter = 0;

            //Get time & number of interrupts
            timespec_t currentTime;
            clock_gettime(CLOCK_MONOTONIC, &currentTime);
            uint64_t currentInterrupts = readInterrupts(interruptReporterFile);

            //Get the number of interrupts
            double timeDiff = difftimespec(&currentTime, &lastTime);
            uint64_t interruptDiff = currentInterrupts - lastInterrupts;
            lastInterrupts = currentInterrupts;
            lastTime = currentTime;

            //Load Write Ptr
            indexLocalType writeOffset = std::atomic_load_explicit(write_offset_ptr, std::memory_order_acquire);

            //Compute the fullness
            indexLocalType numEntries = getNumItemsInBuffer(readOffset, writeOffset, array_length);

            if(startTrackerInd<startTrackerLen){
                startTracker[startTrackerInd] = numEntries;
                startInterruptTracker[startTrackerInd] = interruptDiff;
                startTimingTracker[startTrackerInd] = timeDiff;
                startTrackerInd++;
            }

            endTracker[endTrackerInd] = numEntries;
            endInterruptTracker[endTrackerInd] = interruptDiff;
            endTimingTracker[endTrackerInd] = timeDiff;
            if(endTrackerInd>=(startTrackerLen-1)){
                endTrackerInd = 0;
            }else{
                endTrackerInd++;
            }
        }else{
            checkCounter++;
        }

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

        //Perform NOPs
        for(int nop = 0; nop<numNops; nop++){
            asm volatile ("nop" : : :);
        }

    }

    std::atomic_thread_fence(std::memory_order_acquire);
    std::atomic_flag_clear_explicit(stop_flag, std::memory_order_release);

    FifolessBufferFullnessTrackerEndCondition *rtn = new FifolessBufferFullnessTrackerEndCondition;
    rtn->expectedBlockID = readBlockInd;
    rtn->startBlockID = newBlockIDStart;
    rtn->endBlockID = newBlockIDEnd;
    rtn->wasErrorSrc = true;
    rtn->errored = failureDetected;
    rtn->resultGranularity = HW_Granularity::CORE;
    rtn->granularityIndex = core;
    rtn->transaction = transfer;

    //Copy from buffers
    for(int i = 0; i<startTrackerLen; i++){
        rtn->startTracker.push_back(startTracker[i]);
        rtn->startInterruptTracker.push_back(startInterruptTracker[i]);
        rtn->startTimingTracker.push_back(startTimingTracker[i]);
    }

    int ind = endTrackerInd; //Begin copying from 
    for(int i = 0; i<endTrackerLen; i++){
        rtn->endTracker.push_back(endTracker[ind]);
        rtn->endInterruptTracker.push_back(endInterruptTracker[ind]);
        rtn->endTimingTracker.push_back(endTimingTracker[ind]);

        if(ind >= (endTrackerLen-1)){
            ind = 0;
        }else{
            ind++;
        }
    }

    return (void*) rtn;
}

template<typename elementType, typename idType = std::atomic_int32_t, typename indexType = std::atomic_int32_t, typename idLocalType = int32_t, typename indexLocalType = int32_t, int idMax = INT32_MAX>
void* open_loop_fullness_tracker_buffer_server(void* arg){
    OpenLoopFullnessTrackerBufferArgs<elementType, idType, indexType>* args = (OpenLoopFullnessTrackerBufferArgs<elementType, idType, indexType>*) arg;

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

    //Tracking
    FILE* interruptReporterFile = args->writerInterruptReporter;
    int checkPeriod = args->checkPeriod;
    int32_t* startInterruptTracker = args->startInterruptTrackerWriter;
    double* startTimingTracker = args->startTimingTrackerWriter;
    int startTrackerLen = args->startTrackerLen;
    int32_t* endInterruptTracker = args->endInterruptTrackerWriter;
    double* endTimingTracker = args->endTimingTrackerWriter;
    int endTrackerLen = args->endTrackerLen;
    int endTrackerInd = 0; //This is the index to write next.  It is the tail of the buffer.  When reading back, this address is read first then is incremented until it has been reached again
    int startTrackerInd = 0;
    int checkCounter = 0;

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
    
    //Wait for start signal
    bool start = false;
    while(!start){
        start = !std::atomic_flag_test_and_set_explicit(start_flag, std::memory_order_acq_rel); //Start signal is active low
    }

    //Get initial time & interrupts
    timespec_t lastTime;
    clock_gettime(CLOCK_MONOTONIC, &lastTime);
    uint64_t lastInterrupts = readInterrupts(interruptReporterFile);

    bool stop = false;
    int64_t transfer;
    for(transfer = 0; transfer<max_block_transfers; transfer++){
        //Check Time and Interrupts
        if(checkCounter>=checkPeriod){
            checkCounter = 0;

            //Get time & number of interrupts
            timespec_t currentTime;
            clock_gettime(CLOCK_MONOTONIC, &currentTime);
            uint64_t currentInterrupts = readInterrupts(interruptReporterFile);

            //Get the number of interrupts
            double timeDiff = difftimespec(&currentTime, &lastTime);
            uint64_t interruptDiff = currentInterrupts - lastInterrupts;
            lastInterrupts = currentInterrupts;
            lastTime = currentTime;

            if(startTrackerInd<startTrackerLen){
                startInterruptTracker[startTrackerInd] = interruptDiff;
                startTimingTracker[startTrackerInd] = timeDiff;
                startTrackerInd++;
            }

            endInterruptTracker[endTrackerInd] = interruptDiff;
            endTimingTracker[endTrackerInd] = timeDiff;
            if(endTrackerInd>=(startTrackerLen-1)){
                endTrackerInd = 0;
            }else{
                endTrackerInd++;
            }
        }else{
            checkCounter++;
        }

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

    FifolessBufferFullnessTrackerEndCondition *rtn = new FifolessBufferFullnessTrackerEndCondition;
    rtn->expectedBlockID = -1;
    rtn->startBlockID = -1;
    rtn->startBlockID = -1;
    rtn->wasErrorSrc = false;
    rtn->errored = stop;
    rtn->resultGranularity = HW_Granularity::CORE;
    rtn->granularityIndex = core;
    rtn->transaction = transfer;

    //Copy from buffers
    for(int i = 0; i<startTrackerLen; i++){
        rtn->startInterruptTracker.push_back(startInterruptTracker[i]);
        rtn->startTimingTracker.push_back(startTimingTracker[i]);
    }

    int ind = endTrackerInd; //Begin copying from 
    for(int i = 0; i<endTrackerLen; i++){
        rtn->endInterruptTracker.push_back(endInterruptTracker[ind]);
        rtn->endTimingTracker.push_back(endTimingTracker[ind]);

        if(ind >= (endTrackerLen-1)){
            ind = 0;
        }else{
            ind++;
        }
    }

    return (void*) rtn;

    // return nullptr;
}

#endif
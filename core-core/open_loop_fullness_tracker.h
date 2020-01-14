#ifndef _H_OPEN_LOOP_FULLNESS_TRACKER
#define _H_OPEN_LOOP_FULLNESS_TRACKER

#include <atomic>
#include "open_loop_helpers.h"
#include "open_loop_buffer.h"
#include "closed_loop_helpers.h"

//This benchmarks measures the fullness of the buffer at the start of a reader cycle.  This is slightly
//different from the closed loop test case which measures at the end of a writer cycle.  It is read
//earlier in the cycle so that, in the error case, the measurement had already been made. 


template<typename elementType, typename idType = std::atomic_int32_t, typename indexType = std::atomic_int32_t>
class OpenLoopFullnessTrackerBufferArgs : public OpenLoopBufferArgs<elementType, idType, indexType>{
public:
    int checkPeriod; //Number of cycles between checks
    //These buffers are used to record the fullness values
    int32_t* startTracker;
    int startTrackerLen;
    int32_t* endTracker;
    int endTrackerLen;

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

    //The fullness tracking is only reported in the CSV file
    // std::string getTrialResultsHeader() override;
    // std::string getTrialResults() override;
    std::string getTrialCSVHeader() override;
    std::string getTrialCSVData() override;

    FifolessBufferFullnessTrackerEndCondition();
};

template<typename elementType, typename atomicIdType, typename atomicIndexType>
size_t openLoopFullnessTrackerAllocate(std::vector<elementType*> &shared_array_locs, std::vector<atomicIndexType*> &shared_write_id_locs, std::vector<atomicIndexType*> &shared_read_id_locs, std::vector<std::atomic_flag*> &ready_flags, std::vector<std::atomic_flag*> &start_flags, std::atomic_flag* &stop_flag, std::vector<int32_t*> &startTracker, std::vector<int32_t*> &endTracker, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, std::vector<int> cpus, int alignment, bool circular, bool include_dummy_flags, int startTrackerLen, int endTrackerLen){
    //Run the standard open loop allocator
    size_t maxBufferSize = openLoopAllocate<elementType, atomicIdType, atomicIndexType>(shared_array_locs, shared_write_id_locs, shared_read_id_locs, ready_flags, start_flags, stop_flag, array_lengths, block_lengths, cpus, alignment, circular, include_dummy_flags);

    //Allocate the tracker buffers
    int num_buffers = circular ? cpus.size() : cpus.size()-1;
    //Initialize
    for(int buffer = 0; buffer<num_buffers; buffer++){
        size_t amountToAllocStartTracker = sizeof(int32_t)*startTrackerLen;
        if(amountToAllocStartTracker % CACHE_LINE_SIZE != 0){
            amountToAllocStartTracker += (CACHE_LINE_SIZE - (amountToAllocStartTracker % CACHE_LINE_SIZE));
        }
        int32_t *startTrackerInst = (int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocStartTracker, cpus[buffer]);
        startTracker.push_back(startTrackerInst);

        size_t amountToAllocEndTracker = sizeof(int32_t)*startTrackerLen;
        if(amountToAllocEndTracker % CACHE_LINE_SIZE != 0){
            amountToAllocEndTracker += (CACHE_LINE_SIZE - (amountToAllocEndTracker % CACHE_LINE_SIZE));
        }
        int32_t *endTrackerInst = (int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocEndTracker, cpus[buffer]);
        endTracker.push_back(endTrackerInst);
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
    int startTrackerLen = args->startTrackerLen;
    int32_t* endTracker = args->endTracker;
    int endTrackerLen = args->endTrackerLen;

    for(int i = 0; i<startTrackerLen; i++){
        startTracker[i] = -1;
    }

    for(int i = 0; i<endTrackerLen; i++){
        endTracker[i] = -1;
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

    //Buffers
    int checkPeriod = args->checkPeriod;
    int32_t* startTracker = args->startTracker;
    int startTrackerLen = args->startTrackerLen;
    int32_t* endTracker = args->endTracker;
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
        //---- Check fullness ----
        if(checkCounter>=checkPeriod){
            checkCounter = 0;
            //Load Write Ptr
            indexLocalType writeOffset = std::atomic_load_explicit(write_offset_ptr, std::memory_order_acquire);

            //Compute the fullness
            indexLocalType numEntries = getNumItemsInBuffer(readOffset, writeOffset, array_length);

            if(startTrackerInd<startTrackerLen){
                startTracker[startTrackerInd] = numEntries;
                startTrackerInd++;
            }

            endTracker[endTrackerInd] = numEntries;
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
    }

    int ind = endTrackerInd; //Begin copying from 
    for(int i = 0; i<endTrackerLen; i++){
        rtn->endTracker.push_back(endTracker[ind]);

        if(ind >= (endTrackerLen-1)){
            ind = 0;
        }else{
            ind++;
        }
    }

    return (void*) rtn;
}

#endif
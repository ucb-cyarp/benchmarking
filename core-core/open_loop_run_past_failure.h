#ifndef _OPEN_LOOP_RUN_PAST_FAILURE_H
#define _OPEN_LOOP_RUN_PAST_FAILURE_H

#include <atomic>
#include "open_loop_helpers.h"

//This benchmark is designed to look for temporal patterns in underflow/overflows.
//Note, it is possible for the dithering around the underflow/overflow condition to occur.  This happens when operating mode settles around half empty or half full.

//Because the correct balance is not known a-priori, it is suggested that the iteration of the first failure be used as an indication of balancing

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

template<typename elementType, typename idType = std::atomic_int32_t, typename indexType = std::atomic_int32_t>
class OpenLoopPastFailureBufferArgs : public OpenLoopBufferArgs<elementType, idType, indexType>{
public:
    int64_t* underOverflowIterations; //Array used for tracking the iterations when underflows or overflows occured
    int16_t* underOverflowAmounts; //For each underOverflowIteration, lists how many underflows/overflows occured.  The sign of the amount indicates if it was an underflow or overflow
    bool* underOverflowPartial; //For each underOverflowIteration, indicates if both IDs were incorrect (true) or only a single one (false)
    int maxUnderOverFlowsRecording; //The number of entries in the above arrays

    //Not overiding title and results functions
};

class FifolessBufferPastFailureEndCondition : public BenchmarkSpecificResult{
public:
    std::vector<int64_t> underOverflowIterations;
    std::vector<int16_t> underOverflowAmounts;
    std::vector<bool> underOverflowPartial; 
    bool wasErrorSrc;
    bool errored; 
    int64_t final_transaction;

    std::string getTrialResultsHeader() override;
    std::string getTrialResults() override;
    std::string getTrialCSVHeader() override;
    std::string getTrialCSVData() override;

    FifolessBufferPastFailureEndCondition();
};

template<typename elementType, typename idType = std::atomic_int32_t, typename indexType = std::atomic_int32_t>
void* open_loop_buffer_past_failure_reset(void* arg){
    OpenLoopPastFailureBufferArgs<elementType, idType, indexType> *args = (OpenLoopPastFailureBufferArgs<elementType, idType, indexType>*) arg;
    int64_t* underOverflowIterations = args->underOverflowIterations;
    int16_t* underOverflowAmounts = args->underOverflowAmounts;
    int maxUnderOverFlowsRecording = args->maxUnderOverFlowsRecording;

    for(int i = 0; i<maxUnderOverFlowsRecording; i++){
        underOverflowIterations[i] = -1;
        underOverflowAmounts[i] = 0;
    }

    //This does the base reset for the open loop buffer test
    open_loop_buffer_reset_internal(args);

    return nullptr;
}

template<typename elementType, typename atomicIdType, typename atomicIndexType>
size_t openLoopRunPastFailureAllocate(std::vector<elementType*> &shared_array_locs, std::vector<atomicIndexType*> &shared_write_id_locs, std::vector<atomicIndexType*> &shared_read_id_locs, std::vector<std::atomic_flag*> &ready_flags, std::vector<std::atomic_flag*> &start_flags, std::atomic_flag* &stop_flag, std::vector<int64_t*> &underOverflowIterations, std::vector<int16_t*> &underOverflowAmounts, std::vector<bool*> &underOverflowPartial, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, std::vector<int> cpus, int alignment, bool circular, bool include_dummy_flags, int maxUnderOverFlowsRecording){
    size_t rtnVal = openLoopAllocate<elementType, atomicIdType, atomicIndexType>(shared_array_locs, shared_write_id_locs, shared_read_id_locs, ready_flags, start_flags, stop_flag, array_lengths, block_lengths, cpus, alignment, circular, include_dummy_flags);
    
    for(int cpuInd = 1; cpuInd<cpus.size(); cpuInd++){
        {
            size_t amountToAlloc = maxUnderOverFlowsRecording*sizeof(int64_t);
            if(amountToAlloc % CACHE_LINE_SIZE != 0){
                amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
            }
            int64_t *underOverflowIterations_loc = (int64_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpus[cpuInd]);
            underOverflowIterations.push_back(underOverflowIterations_loc);
        }

        {
            size_t amountToAlloc = maxUnderOverFlowsRecording*sizeof(int16_t);
            if(amountToAlloc % CACHE_LINE_SIZE != 0){
                amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
            }
            int16_t *underOverflowAmounts_loc = (int16_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpus[cpuInd]);
            underOverflowAmounts.push_back(underOverflowAmounts_loc);
        }

        {
            size_t amountToAlloc = maxUnderOverFlowsRecording*sizeof(bool);
            if(amountToAlloc % CACHE_LINE_SIZE != 0){
                amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
            }
            bool *underOverflowPartial_loc = (bool*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpus[cpuInd]);
            underOverflowPartial.push_back(underOverflowPartial_loc);
        }
    }

    return rtnVal;
}

//The server (writer) can remain the same since it does not check block IDs and is only informed of the failure case by the client (reader)
//The reader will simply not send the failure signal until either the maximum number of underflow/overflow conditions are recorded or the iteration limit is reached.

template<typename elementType, typename idType = std::atomic_int32_t, typename indexType = std::atomic_int32_t, typename idLocalType = int32_t, typename indexLocalType = int32_t, int idMax = INT32_MAX>
void* open_loop_buffer_past_failure_client(void* arg){
    OpenLoopPastFailureBufferArgs<elementType, idType, indexType>* args = (OpenLoopPastFailureBufferArgs<elementType, idType, indexType>*) arg;
    indexType *read_offset_ptr = args->read_offset_ptr;
    void *array = args->array;
    int array_length = args->array_length;
    int64_t max_block_transfers = args->max_block_transfers;
    std::atomic_flag *start_flag = args->start_flag;
    std::atomic_flag *stop_flag = args->stop_flag;
    std::atomic_flag *ready_flag = args->ready_flag;
    int blockSize = args->blockSize;

    int core = args->core_client;

    float balancing_nops = args->balancing_nops;
    float numNops = balancing_nops > 0 ? balancing_nops : 0;
    numNops += args->initialNOPs;
    float nops_current = 0; //Is used to carry over residual between nop cycles

    elementType expectedSampleVals = 0;
    idLocalType readBlockInd = 0;

    idLocalType newBlockIDStart = -1;
    idLocalType newBlockIDEnd = -1;

    idLocalType expectedBlockID = -1;

    elementType localBuffer[blockSize];

    int64_t* underOverflowIterations = args->underOverflowIterations;
    int16_t* underOverflowAmounts = args->underOverflowAmounts;
    bool* underOverflowPartial = args->underOverflowPartial;
    int maxUnderOverFlowsRecording = args->maxUnderOverFlowsRecording;

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

    int underOverflowInd = 0;

    //==== Singal start ====
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

        //TODO: For now, this logic is limited to block IDs that do not overflow (~4e9 transactions)
        expectedBlockID = readBlockInd == UINT32_MAX ? 0 : readBlockInd+1;
        if(expectedBlockID != newBlockIDStart || expectedBlockID != newBlockIDEnd){
            underOverflowIterations[underOverflowInd] = transfer;

            

            if(newBlockIDStart == newBlockIDEnd){
                underOverflowPartial[underOverflowInd] = false;

                int32_t distStart = newBlockIDStart - expectedBlockID;
                int32_t numberUnderOverflows = distStart/(array_length+1); //Trunkate
                underOverflowAmounts[underOverflowInd] = numberUnderOverflows;

                //Set the new read ind to either the start or end block ID
                readBlockInd = newBlockIDStart;
            }else{
                underOverflowPartial[underOverflowInd] = true;
                //Set the new read ind to the failed

                int32_t distStart = newBlockIDStart - expectedBlockID; //If positive, ID was greater than what was expected (overflow)
                int32_t distEnd = newBlockIDEnd - expectedBlockID;

                if((distStart < 0 && distEnd > 0) || (distStart > 0 && distEnd < 0)){
                    //This case does not make sense since the it both indicates and under and overflow
                    //We will indicate this with an indicated over/underflow amount of 0
                    underOverflowAmounts[underOverflowInd] = 0;

                    //Will pick the start as the new index
                    readBlockInd = newBlockIDStart;
                }else if(distStart < 0 && distEnd < 0){
                    //Pick the more neagive distance as the final
                    int32_t dist;

                    if(distStart<distEnd){
                        dist = distStart;
                        readBlockInd = newBlockIDStart;
                    }else{
                        dist = distEnd;
                        readBlockInd = newBlockIDEnd;
                    }

                    int32_t numberUnderOverflows = dist/(array_length+1); //Trunkate
                    underOverflowAmounts[underOverflowInd] = numberUnderOverflows;
                }else{
                    //Pick the more positive distance as the final
                    int32_t dist;

                    if(distStart>distEnd){
                        dist = distStart;
                        readBlockInd = newBlockIDStart;
                    }else{
                        dist = distEnd;
                        readBlockInd = newBlockIDEnd;
                    }

                    int32_t numberUnderOverflows = dist/(array_length+1); //Trunkate
                    underOverflowAmounts[underOverflowInd] = numberUnderOverflows;
                }

            }

            failureDetected = true;

            underOverflowInd++;

            if(underOverflowInd >= maxUnderOverFlowsRecording){
                //Will signal failure outside of loop
                break;
            }
        }else{

            readBlockInd = expectedBlockID;
        }

        //Not actually checking elements since we are allowing failures to occure
        for(int sample = 0; sample<blockSize; sample++){
            asm volatile("" //Force touch elements
            : 
            : "r" (localBuffer[sample]), "r" (expectedSampleVals)
            :);
        }

        expectedSampleVals = (expectedSampleVals+1)%2;

        //Perform NOPs
        nops_current += numNops;//nop_current contains the resitual from the last round
        int32_t nops_current_trunk = std::floor(nops_current); //Truncate to a fixed number of NOPs
        for(int32_t nop = 0; nop<nops_current_trunk; nop++){
            asm volatile ("nop" : : :);
        }
        nops_current -= nops_current_trunk;//Get the residual for the next round

    }

    std::atomic_thread_fence(std::memory_order_acquire);
    std::atomic_flag_clear_explicit(stop_flag, std::memory_order_release);

    FifolessBufferPastFailureEndCondition *rtn = new FifolessBufferPastFailureEndCondition;
    rtn->wasErrorSrc = true;
    rtn->errored = failureDetected;
    rtn->resultGranularity = HW_Granularity::CORE;
    rtn->granularityIndex = core;
    rtn->final_transaction = transfer;

    for(int i = 0; i<underOverflowInd; i++){
        rtn->underOverflowIterations.push_back(underOverflowIterations[i]);
        rtn->underOverflowAmounts.push_back(underOverflowAmounts[i]);
        rtn->underOverflowPartial.push_back(underOverflowPartial[i]);
    }

    return (void*) rtn;
}

#endif
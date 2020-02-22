#ifndef _CLOSED_LOOP_PI_BUFFER_H
#define _CLOSED_LOOP_PI_BUFFER_H

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
         typename nopsClient = std::atomic<float>,
         typename nopsClientLocal = float>
struct ClosedLoopPIBufferArgs : public ClosedLoopBufferArgs<elementType, idType, indexType, nopsClient>{
    nopsClientLocal control_gain_p; //The gain of the control system p
    nopsClientLocal control_gain_i; //The gain of the control system i

    void printStandaloneTitle(bool report_standalone, std::string title) override; //Prints the standalone title block if standalone results are requested
protected:
    virtual void printExportNonStandaloneResults(Results &result, bool report_standalone, std::string resultPrintFormatStr, FILE* file, std::ofstream* raw_file) override;
};

template<typename elementType, 
         typename idType, 
         typename indexType, 
         typename nopsClient,
         typename nopsClientLocal>
void ClosedLoopPIBufferArgs<elementType, idType, indexType, nopsClient, nopsClientLocal>::printStandaloneTitle(bool report_standalone, std::string title){
    //Print the title if a standalone report
    #if PRINT_TITLE == 1
        if(report_standalone)
        {
            printf("\n");
            printTitle(title);
            //Need to use this-> because of the class templating
            printf("Array Length (Blocks): %d, Block Length (int32_t Elements): %d, Server Control Period (Iterations): %d, Client Control Period (Iterations): %d, Control Gain P (NOPs): %f, Control Gain I (NOPs): %f, Initial NOPs: %d\n", this->array_length, this->blockSize, this->control_check_period, this->control_client_check_period, this->control_gain_p, this->control_gain_i, this->initialNops);
            fflush(stdout);
        }
    #endif
}

template<typename elementType, 
         typename idType, 
         typename indexType, 
         typename nopsClient,
         typename nopsClientLocal>
void ClosedLoopPIBufferArgs<elementType, idType, indexType, nopsClient, nopsClientLocal>::printExportNonStandaloneResults(Results &result, bool report_standalone, std::string resultPrintFormatStr, FILE* file, std::ofstream* raw_file){
    if(!report_standalone){
        //Print the complete results
            double avg_duration_ms = result.avg_duration();
            double stddev_duration_ms = result.stddev_duration();

            #if PRINT_STATS == 1
            //Need to use this-> because of the class templating
            printf(resultPrintFormatStr.c_str(), this->array_length, this->blockSize, this->control_check_period, this->control_client_check_period, this->control_gain_p, this->control_gain_i, this->initialNops, avg_duration_ms, stddev_duration_ms);
            #endif

            #if WRITE_CSV == 1
            //write the general results to the summary csv file
            //Need to use this-> because of the class templating
            fprintf(file, "%d,%d,%d,%d,%f,%f,%d,%f,%f\n", this->array_length, this->blockSize, this->control_check_period, this->control_client_check_period, this->control_gain_p, this->control_gain_i, this->initialNops, avg_duration_ms, stddev_duration_ms);
            //write the general and benchmark specific results to the raw file
            //Need to use this-> because of the class templating
            result.write_durations_and_benchmark_specific_results(*raw_file, {"", "", "", "", "", "", ""}, {std::to_string(this->array_length), std::to_string(this->blockSize), std::to_string(this->control_check_period), std::to_string(this->control_client_check_period), std::to_string(this->control_gain_p), std::to_string(this->control_gain_i), std::to_string(this->initialNops)}, false);
            #endif
    }
}

//Note: The server and client threads are different instruction streams.  One writes while the other reads and needs to check the block ids (and the block data).
//The writer needs to periodically check the error condition flag provided by the reader.  This can result in the two functions having different
//execution times.  To compensate, nop loops are placed in each thread

//This thread is the writer.  It will recieve a start signal from the client when it is ready (the client is the primary thread)
//The server also acts as the controller in this case.  It is capable of slowing itself down and can command the reciever to slow down if
//nessasary via a shared ptr
template<typename elementType, 
         typename idType = std::atomic_int32_t, 
         typename indexType = std::atomic_int32_t, 
         typename idLocalType = int32_t, 
         typename indexLocalType = int32_t, 
         typename nopsClientType = std::atomic<float>,
         typename nopsClientLocalType = float, 
         int idMax = INT32_MAX>
void* closed_loop_buffer_pi_rate_control_server(void* arg){
    ClosedLoopPIBufferArgs<elementType, idType, indexType, nopsClientType>* args = (ClosedLoopPIBufferArgs<elementType, idType, indexType, nopsClientType>*) arg;

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

    FILE* sirFile = args->writerSirFile;

    nopsClientType *clientNops = args->clientNops;
    int32_t control_check_period = args->control_check_period;

    nopsClientLocalType base_rate = 1/args->initialNops; //There is an approximation as there are more than the initial NOPS in each itteration (the actual reading/writing)
    nopsClientLocalType nops_server = args->initialNops;
    nopsClientLocalType nops_client_local = std::atomic_load_explicit(clientNops, std::memory_order_acquire);
    nopsClientLocalType nops_current = 0; //Is used to carry over residual between nop cycles

    nopsClientLocalType control_gain_p = args->control_gain_p;
    nopsClientLocalType control_gain_i = args->control_gain_i;

    nopsClientLocalType control_integral = 0;

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

    //Disable Interrupts For This Trial (Before Signalling Ready)
    #if CLOSED_LOOP_DISABLE_INTERRUPTS == 1
        int status = ioctl(fileno(sirFile), SIR_IOCTL_DISABLE_INTERRUPT, NULL);
        if(status < 0){
            std::cerr << "Problem accessing /dev/sir0 to Disable Interrupts" << std::endl;
            exit(1);
        }
    #endif

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

            nopsClientLocalType error = numEntries - halfFilledPoint;
            control_integral += error;

            //The correction is from the perspective of the server.
            //If the error is positive, then the array is more full then it should be.  The server should slow down (lower rate)
            //Alternativly, the client should be spead up
            nopsClientLocalType correction = -error*control_gain_p - control_integral*control_gain_i;
            if(correction<0){
                slowDownCount++;
            }else if(correction > 0){
                speedUpCount++;
            }

            nopsClientLocalType nops_client_local_prev = nops_client_local;
            //Try to resolve this at the server alone if possible
            nopsClientLocalType correctedServerRate = base_rate+correction;
            if(correctedServerRate<1){
                //Requires the server to have only a fractional NOP
                //Speed up the client as well
                nopsClientLocalType correctedClientRate = base_rate+(1-correctedServerRate);
                correctedServerRate = 1;
                nops_client_local = 1/correctedClientRate;
            }else{
                //Just correct in the server
                nops_client_local = 1/base_rate;
            }
            nops_server = 1/correctedServerRate;

            if(nops_client_local_prev!=nops_client_local){
                std::atomic_store_explicit(clientNops, nops_client_local, std::memory_order_release);
            }
        }

        //Perform NOPs
        nops_current += nops_server;//nop_current contains the resitual from the last round
        int32_t nops_current_trunk = std::floor(nops_current); //Truncate to a fixed number of NOPs
        for(int32_t nop = 0; nop<nops_current_trunk; nop++){
            asm volatile ("nop" : : :);
        }
        nops_current -= nops_current_trunk;//Get the residual for the next round
    }

    //Re-enable interrupts before processing results (which dynamically allocates memory which can result in a system call)
    #if CLOSED_LOOP_DISABLE_INTERRUPTS == 1
        status = ioctl(fileno(sirFile), SIR_IOCTL_RESTORE_INTERRUPT, NULL);
        if(status < 0){
            std::cerr << "Problem accessing /dev/sir0 to Restore Interrupts" << std::endl;
            exit(1);
        }
    #endif

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

template<typename elementType, 
         typename idType = std::atomic_int32_t, 
         typename indexType = std::atomic_int32_t, 
         typename idLocalType = int32_t, 
         typename indexLocalType = int32_t, 
         typename nopsClientType = std::atomic<float>,
         typename nopsClientLocalType = float, 
         int idMax = INT32_MAX>
void* closed_loop_buffer_pi_period_control_server(void* arg){
    ClosedLoopPIBufferArgs<elementType, idType, indexType, nopsClientType>* args = (ClosedLoopBangBufferArgs<elementType, idType, indexType, nopsClientType>*) arg;

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

    FILE* sirFile = args->writerSirFile;

    nopsClientType *clientNops = args->clientNops;
    int32_t control_check_period = args->control_check_period;
    int32_t control_gain = args->control_gain;

    nopsClientLocalType base_nops = args->initialNops;
    nopsClientLocalType nops_server = args->initialNops;
    nopsClientLocalType nops_client_local = std::atomic_load_explicit(clientNops, std::memory_order_acquire);
    nopsClientLocalType nops_current = 0; //Is used to carry over residual between nop cycles

    nopsClientLocalType control_gain_p = args->control_gain_p;
    nopsClientLocalType control_gain_i = args->control_gain_i;

    nopsClientLocalType control_integral = 0;

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

    //Disable Interrupts For This Trial (Before Signalling Ready)
    #if CLOSED_LOOP_DISABLE_INTERRUPTS == 1
        int status = ioctl(fileno(sirFile), SIR_IOCTL_DISABLE_INTERRUPT, NULL);
        if(status < 0){
            std::cerr << "Problem accessing /dev/sir0 to Disable Interrupts" << std::endl;
            exit(1);
        }
    #endif

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

            nopsClientLocalType error = numEntries - halfFilledPoint;
            control_integral += error;

            //The correction is from the perspective of the server.
            //If the error is positive, then the array is more full then it should be.  The server should slow down (higher period)
            //Alternativly, the client should be spead up
            nopsClientLocalType correction = error*control_gain_p + control_integral*control_gain_i;
            if(correction>0){
                slowDownCount++;
            }else if(correction < 0){
                speedUpCount++;
            }

            nopsClientLocalType nops_client_local_prev = nops_client_local;

            nops_server = base_nops+correction;
            if(nops_server<0){
                //Need to shift some of the correction to the client (tried to speed up the server too much, need to slow down the client)
                nops_client_local = base_nops - nops_server;
                nops_server = 0;
            }else{
                nops_client_local = base_nops;
            }

            if(nops_client_local_prev != nops_client_local){
                std::atomic_store_explicit(clientNops, nops_client_local, std::memory_order_release);
            }
        }

        //Perform NOPs
        nops_current += nops_server;//nop_current contains the resitual from the last round
        int32_t nops_current_trunk = std::floor(nops_current); //Truncate to a fixed number of NOPs
        for(int32_t nop = 0; nop<nops_current_trunk; nop++){
            asm volatile ("nop" : : :);
        }
        nops_current -= nops_current_trunk;//Get the residual for the next round
    }

    //Re-enable interrupts before processing results (which dynamically allocates memory which can result in a system call)
    #if CLOSED_LOOP_DISABLE_INTERRUPTS == 1
        status = ioctl(fileno(sirFile), SIR_IOCTL_RESTORE_INTERRUPT, NULL);
        if(status < 0){
            std::cerr << "Problem accessing /dev/sir0 to Restore Interrupts" << std::endl;
            exit(1);
        }
    #endif

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

template<typename elementType, 
         typename idType = std::atomic_int32_t, 
         typename indexType = std::atomic_int32_t, 
         typename idLocalType = int32_t, 
         typename indexLocalType = int32_t, 
         typename nopsClientType = std::atomic_int32_t, 
         typename nopsClientLocalType = int32_t, 
         int idMax = INT32_MAX>
void* closed_loop_buffer_pi_client(void* arg){
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

    FILE* sirFile = args->readerSirFile;

    int32_t control_client_check_period = args->control_client_check_period;
    nopsClientType *clientNops = args->clientNops;
    nopsClientLocalType nops_current = 0; //Is used to carry over residual between nop cycles
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
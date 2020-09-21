#ifndef _CLOSED_LOOP_BANG_BUFFER_H
#define _CLOSED_LOOP_BANG_BUFFER_H

//!!! IMPORTANT TODO: Must be Refactored To use Floating Point NOPs

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
         typename idType, 
         typename indexType, 
         typename nopsClient,
         typename nopsLocal>
class ClosedLoopBangBufferArgs : public ClosedLoopBufferArgs<elementType, idType, indexType, nopsClient, nopsLocal>{
public:
    nopsLocal control_gain; //The gain of the control system

    void printStandaloneTitle(bool report_standalone, std::string title) override; //Prints the standalone title block if standalone results are requested
protected:
    virtual void printExportNonStandaloneResults(Results &result, bool report_standalone, std::string resultPrintFormatStr, FILE* file, std::ofstream* raw_file) override;
};

template<typename elementType, 
         typename idType, 
         typename indexType, 
         typename nopsClient,
         typename nopsLocal>
void ClosedLoopBangBufferArgs<elementType, idType, indexType, nopsClient, nopsLocal>::printStandaloneTitle(bool report_standalone, std::string title){
    //Print the title if a standalone report
    #if PRINT_TITLE == 1
        if(report_standalone)
        {
            printf("\n");
            printTitle(title);
            //Need to use this-> because of the class templating
            printf("Array Length (Blocks): %d, Block Length (int32_t Elements): %d, Server Control Period (Iterations): %d, Client Control Period (Iterations): %d, Control Gain (NOPs): %f, Initial NOPs: %f\n", this->array_length, this->blockSize, this->control_check_period, this->control_client_check_period, this->control_gain, this->initialNops);
            fflush(stdout);
        }
    #endif
}

template<typename elementType, 
         typename idType, 
         typename indexType, 
         typename nopsClient,
         typename nopsLocal>
void ClosedLoopBangBufferArgs<elementType, idType, indexType, nopsClient, nopsLocal>::printExportNonStandaloneResults(Results &result, bool report_standalone, std::string resultPrintFormatStr, FILE* file, std::ofstream* raw_file){
    if(!report_standalone){
        //Print the complete results
            double avg_duration_ms = result.avg_duration();
            double stddev_duration_ms = result.stddev_duration();

            #if PRINT_STATS == 1
            //Need to use this-> because of the class templating
            printf(resultPrintFormatStr.c_str(), this->array_length, this->blockSize, this->control_check_period, this->control_client_check_period, this->control_gain, this->initialNops, avg_duration_ms, stddev_duration_ms);
            #endif

            #if WRITE_CSV == 1
            //write the general results to the summary csv file
            //Need to use this-> because of the class templating
            fprintf(file, "%d,%d,%d,%d,%f,%f,%f,%f\n", this->array_length, this->blockSize, this->control_check_period, this->control_client_check_period, this->control_gain, this->initialNops, avg_duration_ms, stddev_duration_ms);
            //write the general and benchmark specific results to the raw file
            //Need to use this-> because of the class templating
            result.write_durations_and_benchmark_specific_results(*raw_file, {"", "", "", "", "", ""}, {std::to_string(this->array_length), std::to_string(this->blockSize), std::to_string(this->control_check_period), std::to_string(this->control_client_check_period), std::to_string(this->control_gain), std::to_string(this->initialNops)}, false);
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
         typename idType, 
         typename indexType, 
         typename idLocalType, 
         typename indexLocalType, 
         typename nopsClientType,
         typename nopsClientLocalType, 
         int idMax>
void* closed_loop_buffer_bang_control_server(void* arg){
    ClosedLoopBangBufferArgs<elementType, idType, indexType, nopsClientType, nopsClientLocalType>* args = (ClosedLoopBangBufferArgs<elementType, idType, indexType, nopsClientType, nopsClientLocalType>*) arg;

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

    nopsClientLocalType control_gain = args->control_gain;
    nopsClientLocalType nops_server = args->initialNops;
    nopsClientLocalType nops_client_local = std::atomic_load_explicit(clientNops, std::memory_order_acquire);
    nopsClientLocalType nops_current = 0; //Is used to carry over residual between nop cycles

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

            //Check if the number of entries is above or below the half occupancy point
            if(numEntries>halfFilledPoint){
                //Above the set point, speed up the client or slow down the server
                slowDownCount++;

                //First check if the client can be speed up
                if(nops_client_local>0){
                    nops_client_local -= control_gain; 
                    if(nops_client_local<0){ //If the number of NOPS went negative, shift the difference to the server (slow it down)
                        nops_server-=nops_client_local;
                        nops_client_local=0;
                    }
                }else{
                    //Slow down the server
                    nops_server += control_gain;
                }
            }else if(numEntries<halfFilledPoint){
                //Below the set point, speed up the server or slow down the client
                speedUpCount++;

                //First, check if the server can be sped up
                if(nops_server > 0){
                    nops_server -= control_gain;
                    if(nops_server<0){ //If the number of NOPS went negative, shift the difference to the client (slow it down)
                        nops_client_local-=nops_server;
                        nops_server=0;
                    }
                }else{
                    nops_client_local += control_gain;
                }
            }
            //Write to the client in any case so that the amount of time spent per control itteration (due to the cache miss) is consistent
            //   ie. should not be different if the server is effected vs. the client.  This would potetnailly create its own imballance
            std::atomic_store_explicit(clientNops, nops_client_local, std::memory_order_release);
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

#endif
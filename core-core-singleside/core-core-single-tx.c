#ifndef _GNU_SOURCE
    #define _GNU_SOURCE //For clock_gettime
#endif

#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "core-core-single-tx.h"
#include "core-core-single-threadhelpers.h"
#include "core-core-singleside-commonDefines.h"

void* core_core_single_tx_thread(void* args){
    core_core_single_args_t *argsCast = (core_core_single_args_t*) args;

    //=== Initialize Locals (not timed) ====
    //Set local versions of tx and rx flags (Set local rx flag to allow code to run immeidatly)
    int8_t txFlagLocal = argsCast->initTxFlag;
    int8_t rxFlagLocal = argsCast->initRxFlag;

    //local Start & Stop time arrays
    timespec_t *startTimes = argsCast->startTimes;
    timespec_t *endTimes = argsCast->endTimes;

    //Pre-assemble local write buffer
    int blockSizeBytes = argsCast->blockSizeBytes;
    int blockSizeElements = blockSizeBytes/sizeof(TYPE_TO_TRANSFER);

    #ifdef MALLOC_LOCAL_BUFFERS
        TYPE_TO_TRANSFER *localBuffer = argsCast->localBuffer;
    #else
        TYPE_TO_TRANSFER localBuffer[blockSizeElements];
    #endif

    for(int i = 0; i<blockSizeElements; i++){
        localBuffer[i] = 0;
    }

    //Get Remaining params
    _Atomic int8_t *txFlagRemote = argsCast->txFlag;
    _Atomic int8_t *rxFlagRemote = argsCast->rxFlag;
    TYPE_TO_TRANSFER *sharedBuffer = argsCast->sharedBuffer;

    int trials = argsCast->trialsIncludingDiscards;
    //The first n timed entries will be discarded in the reporting thread
    for(int i = 0; i<trials; i++){
        //Wait for rxFlag (to be different from local rxFlag copy, replace local rx flag)
        bool rxReady = false;
        do{
            int8_t rxFlagRead = atomic_load_explicit(rxFlagRemote, memory_order_acquire);
            rxReady = rxFlagRead != rxFlagLocal;
            rxFlagLocal = rxFlagRead;
        }while(!rxReady);

        //Record Start Time
        asm volatile ("" ::: "memory"); //Stop Re-ordering of timer
        clock_gettime(CLOCK_MONOTONIC, startTimes+i);
        asm volatile ("" ::: "memory"); //Stop Re-ordering of timer

        //Copy pre-assembled buffer
        memcpy(sharedBuffer, localBuffer, blockSizeBytes);
        
        //Toggle Tx Flag
        txFlagLocal = !txFlagLocal;
        atomic_store_explicit(txFlagRemote, txFlagLocal, memory_order_release);

        //Record Stop Time (in local array)
        //Record Start Time
        asm volatile ("" ::: "memory"); //Stop Re-ordering of timer
        clock_gettime(CLOCK_MONOTONIC, endTimes+i);
        asm volatile ("" ::: "memory"); //Stop Re-ordering of timer
    }
}

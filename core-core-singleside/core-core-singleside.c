#ifndef _GNU_SOURCE
    #define _GNU_SOURCE //For clock_gettime
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdint.h>
#include <assert.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <sched.h>

#include "core-core-singleside-commonDefines.h"
#include "vitisNumaAllocHelpers.h"
#include "core-core-single-threadhelpers.h"
#include "telemetry_helpers.h"
#include "core-core-single-tx.h"
#include "core-core-single-rx.h"

int main(int argc, char *argv[])
{
    //*** Check for Incompatibile compile time parameters ***
    static_assert(BLOCK_SIZE_BYTES_START%sizeof(TYPE_TO_TRANSFER)==0, "Provided transfer type is incompatible with starting block size");
    static_assert(BLOCK_SIZE_BYTES_STEP%sizeof(TYPE_TO_TRANSFER)==0, "Provided transfer type is incompatible with block size sweep step size");
    static_assert(BLOCK_SIZE_BYTES_STEPS>=0, "Should have at least one step in the block size sweep");

    //*** Parse Arguments ***
    if(argc != 3)
    {
        printf("core-core core-a core-b\n"
               "    core-tx: Processor ID for the sending processor core\n"
               "    core-rx: Processor ID for the recieving processor core\n");
        exit(1);
    }

    char* cpu_tx_str = argv[1];
    int cpu_tx = atoi(cpu_tx_str);

    char* cpu_rx_str = argv[2];
    int cpu_rx = atoi(cpu_rx_str);

    #ifdef PRINT_TITLE
        printf("Core-Core Single Side Communication Test: From CPU%d to CPU%d\n", cpu_tx, cpu_rx);
    #endif

    //=== Allocate Buffer & Flags ===
    int maxBlockSizeBytes = BLOCK_SIZE_BYTES_START+BLOCK_SIZE_BYTES_STEP*(BLOCK_SIZE_BYTES_STEPS-1);

    _Atomic int8_t *txFlag = vitis_aligned_alloc_core(CACHE_LINE_SIZE, sizeof(_Atomic int8_t), cpu_tx);
    _Atomic int8_t *rxFlag = vitis_aligned_alloc_core(CACHE_LINE_SIZE, sizeof(_Atomic int8_t), cpu_rx);
    //Allocate for largest block size
    TYPE_TO_TRANSFER* sharedBuffer = vitis_aligned_alloc_core(CACHE_LINE_SIZE, maxBlockSizeBytes, cpu_tx);

    //=== Allocate Timer Arrays ===
    int trialsToAlloc = TRIALS+DISCARD_TRIALS+CALIBRATE_TRIALS;
    timespec_t *txStartTimes = vitis_aligned_alloc_core(CACHE_LINE_SIZE, sizeof(timespec_t)*trialsToAlloc, cpu_tx);
    timespec_t *txEndTimes = vitis_aligned_alloc_core(CACHE_LINE_SIZE, sizeof(timespec_t)*trialsToAlloc, cpu_tx);

    timespec_t *rxStartTimes = vitis_aligned_alloc_core(CACHE_LINE_SIZE, sizeof(timespec_t)*trialsToAlloc, cpu_rx);
    timespec_t *rxEndTimes = vitis_aligned_alloc_core(CACHE_LINE_SIZE, sizeof(timespec_t)*trialsToAlloc, cpu_rx);

    //+++ Allocate Local Buffers if Specified by Define +++
    #ifdef MALLOC_LOCAL_BUFFERS
        //Allocate for largest block size
        TYPE_TO_TRANSFER* txLocalBuffer = vitis_aligned_alloc_core(CACHE_LINE_SIZE, maxBlockSizeBytes, cpu_tx);
        TYPE_TO_TRANSFER* rxLocalBuffer = vitis_aligned_alloc_core(CACHE_LINE_SIZE, maxBlockSizeBytes, cpu_rx);
    #else
        TYPE_TO_TRANSFER* txLocalBuffer = NULL;
        TYPE_TO_TRANSFER* rxLocalBuffer = NULL;
    #endif

    //=== Init Atomics ===
    init_atomic(txFlag, 0, cpu_tx);
    init_atomic(rxFlag, 0, cpu_rx);

    //=== Run Test (for each block size) ===
    double txDurations[BLOCK_SIZE_BYTES_STEPS][TRIALS];
    double rxDurations[BLOCK_SIZE_BYTES_STEPS][TRIALS];

    double txCal[BLOCK_SIZE_BYTES_STEPS][CALIBRATE_TRIALS];
    double rxCal[BLOCK_SIZE_BYTES_STEPS][CALIBRATE_TRIALS];

    core_core_single_args_t *txArgs = vitis_aligned_alloc_core(CACHE_LINE_SIZE, sizeof(core_core_single_args_t), cpu_tx);
    core_core_single_args_t *rxArgs = vitis_aligned_alloc_core(CACHE_LINE_SIZE, sizeof(core_core_single_args_t), cpu_rx);

    for(int i = 0; i<BLOCK_SIZE_BYTES_STEPS; i++){
        int blockSizeBytes = BLOCK_SIZE_BYTES_START+BLOCK_SIZE_BYTES_STEP*i;

        //+++ Reset State +++
        reset_atomic(txFlag, 0, cpu_tx);
        reset_atomic(rxFlag, 0, cpu_rx);
    
        //+++ Run Threads +++
        //--- Thread Params ---
        txArgs->blockSizeBytes = blockSizeBytes;
        rxArgs->blockSizeBytes = blockSizeBytes;
        txArgs->txFlag = txFlag;
        rxArgs->txFlag = txFlag;
        txArgs->rxFlag = rxFlag;
        rxArgs->rxFlag = rxFlag;
        txArgs->sharedBuffer = sharedBuffer;
        rxArgs->sharedBuffer = sharedBuffer;
        txArgs->trialsIncludingDiscards = TRIALS+DISCARD_TRIALS;
        rxArgs->trialsIncludingDiscards = TRIALS+DISCARD_TRIALS;
        txArgs->trialsCalibrate = CALIBRATE_TRIALS;
        rxArgs->trialsCalibrate = CALIBRATE_TRIALS;

        //Tx Params
        txArgs->localBuffer = txLocalBuffer;
        txArgs->startTimes = txStartTimes;
        txArgs->endTimes = txEndTimes;
        txArgs->initTxFlag = 0;
        txArgs->initRxFlag = 1; //This allows the Tx to start things off (the shared value is actually initialized to 0)

        //Rx Params
        rxArgs->localBuffer = rxLocalBuffer;
        rxArgs->startTimes = rxStartTimes;
        rxArgs->endTimes = rxEndTimes;
        rxArgs->initTxFlag = 0;
        rxArgs->initRxFlag = 0;

        //---Create Threads ---
        pthread_t thread_tx, thread_rx;

        //Create attributes
        pthread_attr_t attr_tx, attr_rx;
        int status = pthread_attr_init(&attr_tx);
        if (status != 0)
        {
            printf("Could not create pthread attributes ... exiting");
            exit(1);
        }
        status = pthread_attr_init(&attr_rx);
        if (status != 0)
        {
            printf("Could not create pthread attributes ... exiting");
            exit(1);
        }

        //Set CPU Affinity
        cpu_set_t cpuset_tx, cpuset_rx;
        CPU_ZERO(&cpuset_tx);                                                         //Clear cpuset
        CPU_ZERO(&cpuset_rx);   
        CPU_SET(cpu_tx, &cpuset_tx);                                                  //Add CPU to cpuset
        CPU_SET(cpu_rx, &cpuset_rx);                                                  //Add CPU to cpuset
        status = pthread_attr_setaffinity_np(&attr_tx, sizeof(cpu_set_t), &cpuset_tx); //Set thread CPU affinity
        if (status != 0)
        {
            printf("Could not set thread core affinity ... exiting");
            exit(1);
        }
        status = pthread_attr_setaffinity_np(&attr_rx, sizeof(cpu_set_t), &cpuset_rx); //Set thread CPU affinity
        if (status != 0)
        {
            printf("Could not set thread core affinity ... exiting");
            exit(1);
        }

        //Set SCHED_FIFO
        #ifdef USE_SCHED_FIFO
            status=  pthread_attr_setinheritsched(&attr_tx, PTHREAD_EXPLICIT_SCHED);
            if(status != 0)
            {
                printf("Could not set pthread explicit schedule attribute ... exiting\n");
                exit(1);
            }
            status=  pthread_attr_setinheritsched(&attr_rx, PTHREAD_EXPLICIT_SCHED);
            if(status != 0)
            {
                printf("Could not set pthread explicit schedule attribute ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setschedpolicy(&attr_tx, SCHED_FIFO);
            if(status != 0)
            {
                printf("Could not set pthread schedule policy to SCHED_FIFO ... exiting\n");
                exit(1);
            }
            status=  pthread_attr_setschedpolicy(&attr_rx, SCHED_FIFO);
            if(status != 0)
            {
                printf("Could not set pthread schedule policy to SCHED_FIFO ... exiting\n");
                exit(1);
            }

            struct sched_param threadParams_tx, threadParams_rx;
            threadParams_tx.sched_priority = sched_get_priority_max(SCHED_FIFO);
            threadParams_rx.sched_priority = sched_get_priority_max(SCHED_FIFO);

            status=  pthread_attr_setschedparam(&attr_tx, &threadParams_tx);
            if(status != 0)
            {
                printf("Could not set pthread schedule parameter ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setschedparam(&attr_rx, &threadParams_rx);
            if(status != 0)
            {
                printf("Could not set pthread schedule parameter ... exiting\n");
                exit(1);
            }
        #endif

        //Start Threads
        status = pthread_create(&thread_rx, &attr_rx, core_core_single_rx_thread, rxArgs);
        if (status != 0)
        {
            printf("Could not create a thread ... exiting");
            errno = status;
            perror(NULL);
            exit(1);
        }
        status = pthread_create(&thread_tx, &attr_tx, core_core_single_tx_thread, txArgs);
        if (status != 0)
        {
            printf("Could not create a thread ... exiting");
            errno = status;
            perror(NULL);
            exit(1);
        }

        //+++ Wait for threads to finish +++
        void *resTx, *resRx;

        status = pthread_join(thread_tx, &resTx);
        if (status != 0)
        {
            printf("Could not join a thread ... exiting");
            errno = status;
            perror(NULL);
            exit(1);
        }

        status = pthread_join(thread_rx, &resRx);
        if (status != 0)
        {
            printf("Could not join a thread ... exiting");
            errno = status;
            perror(NULL);
            exit(1);
        }
        
        //+++ Compute times from returned timer arrays (discard early entries) +++
        for(int trialInd = 0; trialInd < CALIBRATE_TRIALS; trialInd++){
            txCal[i][trialInd] = difftimespec(txEndTimes+trialInd, txStartTimes+trialInd);
            rxCal[i][trialInd] = difftimespec(rxEndTimes+trialInd, rxStartTimes+trialInd);
        }

        for(int trialInd = 0; trialInd < TRIALS; trialInd++){
            txDurations[i][trialInd] = difftimespec(txEndTimes+trialInd+DISCARD_TRIALS+CALIBRATE_TRIALS, txStartTimes+trialInd+DISCARD_TRIALS+CALIBRATE_TRIALS);
            rxDurations[i][trialInd] = difftimespec(rxEndTimes+trialInd+DISCARD_TRIALS+CALIBRATE_TRIALS, rxStartTimes+trialInd+DISCARD_TRIALS+CALIBRATE_TRIALS);
        }
    }

    #ifdef WRITE_RESULTS
        //=== Dump Params ===
        FILE* paramsFile = fopen("core-core-single-side-params.csv", "w");
        fprintf(paramsFile, "cpu_tx,%d\n", cpu_tx);
        fprintf(paramsFile, "cpu_rx,%d\n", cpu_rx);
        fprintf(paramsFile, "TRIALS,%d\n", TRIALS);
        fprintf(paramsFile, "DISCARD_TRIALS,%d\n", DISCARD_TRIALS);
        fprintf(paramsFile, "TYPE_TO_TRANSFER_sizebytes,%lu\n", sizeof(TYPE_TO_TRANSFER));
        fprintf(paramsFile, "BLOCK_SIZE_BYTES_START,%d\n", BLOCK_SIZE_BYTES_START);
        fprintf(paramsFile, "BLOCK_SIZE_BYTES_STEP,%d\n", BLOCK_SIZE_BYTES_STEP);
        fprintf(paramsFile, "BLOCK_SIZE_BYTES_STEPS,%d\n", BLOCK_SIZE_BYTES_STEPS);

        fflush(paramsFile);
        fclose(paramsFile);

        //=== Dump to CSV ===
        FILE* calibrateFile = fopen("core-core-single-side-calibrate.csv", "w");
        FILE* csvFile = fopen("core-core-single-side-report.csv", "w");
        //Print header
        fprintf(calibrateFile, "BlockSize_bytes,TimeTx_ns,TimeRx_ns\n");
        fprintf(csvFile, "BlockSize_bytes,TimeToWrite_ns,TimeToRead_ns\n");
    #endif

    #ifdef PRINT_RESULTS
        printf("Block Size (Bytes) | Time Cal Tx (ns) | Time Cal Rx (ns)\n");
    #endif

    //Report calibration for trial in ns
    for(int i = 0; i<BLOCK_SIZE_BYTES_STEPS; i++){
        int blockSizeBytes = BLOCK_SIZE_BYTES_START+BLOCK_SIZE_BYTES_STEP*i;
        for(int j = 0; j<CALIBRATE_TRIALS; j++){
            #ifdef WRITE_RESULTS
                fprintf(calibrateFile, "%d,%f,%f\n", blockSizeBytes, txCal[i][j]*1.0e9, rxCal[i][j]*1.0e9);
            #endif
            #ifdef PRINT_RESULTS
                printf("%19d|%18f|%17f\n", blockSizeBytes, txCal[i][j]*1.0e9, rxCal[i][j]*1.0e9);
            #endif
        }
    }

    #ifdef PRINT_RESULTS
        printf("Block Size (Bytes) | Time to Write (ns) | Time to Read (ns)\n");
    #endif

    //Report times for trial in ns
    for(int i = 0; i<BLOCK_SIZE_BYTES_STEPS; i++){
        int blockSizeBytes = BLOCK_SIZE_BYTES_START+BLOCK_SIZE_BYTES_STEP*i;
        for(int j = 0; j<TRIALS; j++){
            #ifdef WRITE_RESULTS
                fprintf(csvFile, "%d,%f,%f\n", blockSizeBytes, txDurations[i][j]*1.0e9, rxDurations[i][j]*1.0e9);
            #endif
            #ifdef PRINT_RESULTS
                printf("%19d|%20f|%18f\n", blockSizeBytes, txDurations[i][j]*1.0e9, rxDurations[i][j]*1.0e9);
            #endif
        }
    }

    #ifdef WRITE_RESULTS
        fflush(csvFile);
        fclose(csvFile);
        fflush(calibrateFile);
        fclose(calibrateFile);
    #endif

    //=== Cleanup
    free(txFlag);
    free(rxFlag);
    free(sharedBuffer);

    free(txStartTimes);
    free(txEndTimes);

    free(rxStartTimes);
    free(rxEndTimes);

    #ifdef MALLOC_LOCAL_BUFFERS
        free(txLocalBuffer);
        free(rxLocalBuffer);
    #endif

    free(txArgs);
    free(rxArgs);
}
#ifndef _CORE_CORE_SINGLE_THREADHELPERS_H_
#define _CORE_CORE_SINGLE_THREADHELPERS_H_

#include <stdatomic.h>
#include <stdint.h>

#include "telemetry_helpers.h"
#include "core-core-singleside-commonDefines.h"

typedef struct {
    _Atomic int8_t *toInit;
    int8_t initVal;
} init_atomic_args_t;

void init_atomic(_Atomic int8_t *toInit, int8_t initVal, int core);

void reset_atomic(_Atomic int8_t *toInit, int8_t initVal, int core);

typedef struct {
    int blockSizeBytes;
    _Atomic int8_t *txFlag;
    _Atomic int8_t *rxFlag;
    TYPE_TO_TRANSFER* sharedBuffer;
    TYPE_TO_TRANSFER* localBuffer; //May be NULL if allocation is allowed to be automatic in thread
    timespec_t *startTimes;
    timespec_t *endTimes;
    //What the initial flags are "expected" to be.  May be different from what they are actually initially set to to allow the benchmark to start
    int8_t initTxFlag;
    int8_t initRxFlag;
    int trialsIncludingDiscards;
} core_core_single_args_t;

#endif
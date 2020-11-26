#ifndef _CORE_CORE_SINGLESIDE_COMMONDEFINES_H_
#define _CORE_CORE_SINGLESIDE_COMMONDEFINES_H_

#define CACHE_LINE_SIZE (64)

#define TRIALS (100)

#define CALIBRATE_TRIALS (100)

#define DISCARD_TRIALS (4)

#define TYPE_TO_TRANSFER double

#define BLOCK_SIZE_BYTES_START (8)
#define BLOCK_SIZE_BYTES_STEP (8)
#define BLOCK_SIZE_BYTES_STEPS (256) //This is the number of steps to evaluate, should be >=1

#define PRINT_TITLE
#define PRINT_RESULTS
#define WRITE_RESULTS
#define MALLOC_LOCAL_BUFFERS
#define USE_SCHED_FIFO

#endif
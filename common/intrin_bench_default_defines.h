#ifndef _H_INTRIN_BENCH_DEFAULT_DEFINES
    #define _H_INTRIN_BENCH_DEFAULT_DEFINES

    //There is a bug with older GCC versions not knowing about the ymm registers and thus erroring out
    //when ymm registers appear in the clobber list
    //This is a workaround described by https://stackoverflow.com/questions/14034048/xcode-linker-error-file-too-small-for-architecture-x86-64/29952569
    //which specifies xmm (lower half of ymm) for older versions of gcc
    #if (__GNUC__ == 4 && __GNUC_MINOR__ < 9) || (__GNUC__ < 4)
    #define MMREG(n) "xmm"#n
    #else
    #define MMREG(n) "ymm"#n
    #endif

    //Get the cache line from cat /sys/devices/system/cpu/cpu0/cache/index3/coherency_line_size or another cache index
    //TODO: automate
    #ifndef CACHE_LINE_SIZE
        #define CACHE_LINE_SIZE 64
    #endif

    //Set default options
    #ifndef STIM_LEN
	//#define STIM_LEN 200
	//#define STIM_LEN 2000
	//#define STIM_LEN 8000
	//#define STIM_LEN 20000
        //#define STIM_LEN 200000
        //#define STIM_LEN 2000000
        //#define STIM_LEN 200000000
        #define STIM_LEN 2000000000
        //#define STIM_LEN 100000
    #endif

    #ifndef TRIALS
        //#define TRIALS 20
        //#define TRIALS 100
        #define TRIALS 1
    #endif

    #ifndef MAX_DISCARD
        #define MAX_DISCARD 40
    #endif

    #ifndef PRINT_TITLE
        #define PRINT_TITLE 1
    #endif

    #ifndef PRINT_HEADER
        #define PRINT_HEADER 1
    #endif

    #ifndef PRINT_TRIALS_DONE
        #define PRINT_TRIALS_DONE 0
    #endif

    #ifndef PRINT_TRIALS
        #define PRINT_TRIALS 0
    #endif

    #ifndef PRINT_STATS
        #define PRINT_STATS 0
    #endif

    #ifndef PRINT_FREQ_CHANGE_EVENT
        #define PRINT_FREQ_CHANGE_EVENT 0
    #endif

    #ifndef USE_PCM
        #define USE_PCM 0
    #endif

    #ifndef USE_AMDuPROF
        #define USE_AMDuPROF 0
    #endif

    #ifndef USE_SCHED_FIFO 
        #define USE_SCHED_FIFO 1 //USE Real Time Scheduling SCHED_FIFO
    #endif
#endif

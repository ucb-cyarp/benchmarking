#ifndef _H_INTRIN_BENCH_DEFAULT_DEFINES
    #define _H_INTRIN_BENCH_DEFAULT_DEFINES

    #include <chrono>
    #include <ctime>
    #include <cstdio>
    #include <cmath>
    #include <fstream>
    #include <iostream>
    #include <cstdint>

    //Include Intrinsics
    #include <x86intrin.h>
    #include <immintrin.h>
    #include <mm_malloc.h>
    // #include <mmintrin.h>
    // #include <emmintrin.h>
    // #include <xmmintrin.h>

    //Set default options
    #ifndef STIM_LEN
        #define STIM_LEN 32000000
    #endif

    #ifndef TRIALS
        #define TRIALS 20
    #endif

    #ifndef MAX_DISCARD
        #define MAX_DISCARD 20
    #endif

    #ifndef PRINT_TITLE
        #define PRINT_TITLE 1
    #endif

    #ifndef PRINT_HEADER
        #define PRINT_HEADER 1
    #endif

    #ifndef PRINT_TRIALS
        #define PRINT_TRIALS 0
    #endif

    #ifndef PRINT_STATS
        #define PRINT_STATS 1
    #endif

    #ifndef PRINT_FREQ_CHANGE_EVENT
        #define PRINT_FREQ_CHANGE_EVENT 0
    #endif

    #ifndef USE_PCM
        #define USE_PCM 1
    #endif
#endif
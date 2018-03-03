#ifndef _H_LOAD
    #define _H_LOAD
    #include "intrin_bench_default_defines.h"

    #ifdef __AVX__
        //==========_mm256_load_si256 int8==========
        void test_mm256_load_si256_int8();
        //==========_mm256_load_si256 int16==========
        void test_mm256_load_si256_int16();
        //==========_mm256_load_si256 int32==========
        void test_mm256_load_si256_int32();
        //==========_mm256_load_ps==========
        void test_mm256_load_ps();
        //==========_mm256_load_pd==========
        void test_mm256_load_pd();
    #endif

#endif
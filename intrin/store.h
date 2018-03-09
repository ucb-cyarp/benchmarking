#ifndef _H_STORE
    #define _H_STORE
    #include "intrin_bench_default_defines.h"

    #ifdef __AVX__
        //==========_mm256_store_si256 int8==========
        void test_mm256_store_si256_int8();
        //==========_mm256_store_si256 int16==========
        void test_mm256_store_si256_int16();
        //==========_mm256_store_si256 int32==========
        void test_mm256_store_si256_int32();
        //==========_mm256_store_ps==========
        void test_mm256_store_ps();
        //==========_mm256_store_pd==========
        void test_mm256_store_pd();
    #endif

#endif
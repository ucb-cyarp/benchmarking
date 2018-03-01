#ifndef _H_LOAD_STORE
    #define _H_LOAD_STORE
    #include "intrin_bench_default_defines.h"

    #ifdef __AVX__
        //==========_mm256_load_si256 | _mm256_store_si256==========
        void test_mm256_load_si256__mm256_store_si256();
        //==========_mm256_load_ps | _mm256_store_ps==========
        void test_mm256_load_ps__mm256_store_ps();
        //==========_mm256_load_pd | _mm256_store_pd==========
        void test_mm256_load_pd__mm256_store_pd();
    #endif

#endif
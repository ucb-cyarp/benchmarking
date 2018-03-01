#ifndef _H_LOAD_ADD_STORE_UNROLL2
    #define _H_LOAD_ADD_STORE_UNROLL2
    #include "intrin_bench_default_defines.h"

    #ifdef __AVX2__
        //==========_mm256_add_epi8==========
        void test_u2_mm256_add_epi8();
        //==========_mm256_add_epi16==========
        void test_u2_mm256_add_epi16();
        //==========_mm256_add_epi32==========
        void test_u2_mm256_add_epi32();
    #endif

    #ifdef __AVX__
        //==========_mm256_add_ps==========
        void test_u2_mm256_add_ps();
        //==========_mm256_add_pd==========
        void test_u2_mm256_add_pd();
    #endif

#endif

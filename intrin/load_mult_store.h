#ifndef _H_LOAD_MULT_STORE
    #define _H_LOAD_MULT_STORE
    
    #include "intrin_bench_default_defines.h"

    #ifdef __AVX2__
        //==========_mm256_mullo_epi16==========
        void test_mm256_mullo_epi16();
        //==========_mm256_mullo_epi32==========
        void test_mm256_mullo_epi32();
    #endif

    #ifdef __AVX__
        //==========_mm256_mul_ps==========
        void test_mm256_mul_ps();
        //==========_mm256_mul_pd==========
        void test_mm256_mul_pd();
    #endif
    
#endif

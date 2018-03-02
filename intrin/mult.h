#ifndef _H_MULT
    #define _H_MULT
    
    #include "intrin_bench_default_defines.h"

    #ifdef __AVX2__
        //==========_mm256_mullo_epi16==========
        void test_only_mm256_mullo_epi16();
        //==========_mm256_mullo_epi32==========
        void test_only_mm256_mullo_epi32();
    #endif

    #ifdef __AVX__
        //==========_mm256_mul_ps==========
        void test_only_mm256_mul_ps();
        //==========_mm256_mul_pd==========
        void test_only_mm256_mul_pd();
    #endif
    
#endif

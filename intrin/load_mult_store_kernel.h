#ifndef _H_LOAD_MULT_STORE_KERNEL
    #define _H_LOAD_MULT_STORE_KERNEL

    #include "intrin_bench_default_defines_and_imports_cpp.h"

    #ifdef __AVX2__
        //==========_mm256_mullo_epi16==========
        void kernel_mm256_mullo_epi16( __m256i* a, __m256i* b, __m256i* c);
        //==========_mm256_mullo_epi32==========
        void kernel_mm256_mullo_epi32( __m256i* a, __m256i* b, __m256i* c);
    #endif

    #ifdef __AVX__
        //==========_mm256_mul_ps==========
        void kernel_mm256_mul_ps( __m256* a, __m256* b, __m256* c);
        //==========_mm256_mul_pd==========
        void kernel_mm256_mul_pd( __m256d* a, __m256d* b, __m256d* c);
    #endif

#endif

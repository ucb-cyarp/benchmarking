#ifndef _H_LOAD_DIV_STORE_KERNEL
    #define _H_LOAD_DIV_STORE_KERNEL

    #include "intrin_bench_default_defines.h"

    #ifdef __AVX__
        //==========_mm256_div_ps==========
        void kernel_mm256_div_ps( __m256* a, __m256* b, __m256* c);
        //==========_mm256_div_pd==========
        void kernel_mm256_div_pd( __m256d* a, __m256d* b, __m256d* c);
    #endif

#endif

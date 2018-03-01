#ifndef _H_STORE_KERNEL
    #define _H_STORE_KERNEL
    #include "intrin_bench_default_defines.h"

    #ifdef __AVX__
        //==========_mm256_store_si256==========
        void kernel_mm256_store_si256( __m256i* a);
        //==========_mm256_store_ps==========
        void kernel_mm256_store_ps( __m256* a);
        //==========_mm256_store_pd==========
        void kernel_mm256_store_pd( __m256d* a);
    #endif

#endif

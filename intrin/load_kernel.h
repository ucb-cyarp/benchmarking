#ifndef _H_LOAD_KERNEL
    #define _H_LOAD_KERNEL
    #include "intrin_bench_default_defines.h"

    #ifdef __AVX__
        //==========_mm256_load_si256==========
        void kernel_mm256_load_si256( __m256i* a);
        //==========_mm256_load_ps==========
        void kernel_mm256_load_ps( __m256* a);
        //==========_mm256_load_pd==========
        void kernel_mm256_load_pd( __m256d* a);
    #endif

#endif

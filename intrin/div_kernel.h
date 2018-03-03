#ifndef _H_DIV_KERNEL
    #define _H_DIV_KERNEL

    #include "intrin_bench_default_defines.h"

    #ifdef __AVX__
        //==========_mm256_div_ps==========
        void kernel_only_mm256_div_ps();
        //==========_mm256_div_pd==========
        void kernel_only_mm256_div_pd();
    #endif

#endif

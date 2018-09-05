#ifndef _H_DIV_KERNEL_ASM_UNROLL2
    #define _H_DIV_KERNEL_ASM_UNROLL2

    #include "intrin_bench_default_defines.h"

    #ifdef __AVX__
        //==========_mm256_div_ps==========
        void kernel_only_asm_mm256_div_ps_unroll2();
        //==========_mm256_div_pd==========
        void kernel_only_asm_mm256_div_pd_unroll2();
    #endif

#endif

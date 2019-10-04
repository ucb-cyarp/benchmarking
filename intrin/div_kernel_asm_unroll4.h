#ifndef _H_DIV_KERNEL_ASM_UNROLL4
    #define _H_DIV_KERNEL_ASM_UNROLL4

    #include "intrin_bench_default_defines_and_imports_cpp.h"

    #ifdef __AVX__
        //==========_mm256_div_ps==========
        void kernel_only_asm_mm256_div_ps_unroll4();
        //==========_mm256_div_pd==========
        void kernel_only_asm_mm256_div_pd_unroll4();
    #endif

#endif

#ifndef _H_DIV_KERNEL_ASM
    #define _H_DIV_KERNEL_ASM

    #include "intrin_bench_default_defines_and_imports_cpp.h"

    #ifdef __AVX__
        //==========_mm256_div_ps==========
        void kernel_only_asm_mm256_div_ps();
        //==========_mm256_div_pd==========
        void kernel_only_asm_mm256_div_pd();
    #endif

#endif

#ifndef _H_FMA_KERNEL_ASM_UNROLL8
    #define _H_FMA_KERNEL_ASM_UNROLL8

    #include "intrin_bench_default_defines_and_imports_cpp.h"

    #ifdef __FMA__
        //==========_mm256_fmadd_ps==========
        void kernel_only_asm_mm256_fmadd_ps_unroll8();
        //==========_mm256_fmadd_pd==========
        void kernel_only_asm_mm256_fmadd_pd_unroll8();
    #endif

#endif

#ifndef _H_MULT_KERNEL_ASM_UNROLL120
    #define _H_MULT_KERNEL_ASM_UNROLL120

    #include "intrin_bench_default_defines.h"

    #ifdef __AVX2__
        //==========_mm256_mullo_epi16==========
        void kernel_only_asm_mm256_mullo_epi16_unroll120();
        //==========_mm256_mullo_epi32==========
        void kernel_only_asm_mm256_mullo_epi32_unroll120();
    #endif

    #ifdef __AVX__
        //==========_mm256_mul_ps==========
        void kernel_only_asm_mm256_mul_ps_unroll120();
        //==========_mm256_mul_pd==========
        void kernel_only_asm_mm256_mul_pd_unroll120();
    #endif

#endif

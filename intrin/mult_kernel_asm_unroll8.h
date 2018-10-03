#ifndef _H_MULT_KERNEL_ASM_UNROLL8
    #define _H_MULT_KERNEL_ASM_UNROLL8

    #include "intrin_bench_default_defines.h"

    #ifdef __AVX2__
        //==========_mm256_mullo_epi16==========
        void kernel_only_asm_mm256_mullo_epi16_unroll8();
        //==========_mm256_mullo_epi32==========
        void kernel_only_asm_mm256_mullo_epi32_unroll8();
    #endif

    #ifdef __AVX__
        //==========_mm256_mul_ps==========
        void kernel_only_asm_mm256_mul_ps_unroll8();
        //==========_mm256_mul_pd==========
        void kernel_only_asm_mm256_mul_pd_unroll8();
    #endif

#endif

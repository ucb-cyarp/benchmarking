#ifndef _H_MULT_KERNEL_ASM
    #define _H_MULT_KERNEL_ASM

    #include "intrin_bench_default_defines.h"

    #ifdef __AVX2__
        //==========_mm256_mullo_epi16==========
        void kernel_only_asm_mm256_mullo_epi16();
        //==========_mm256_mullo_epi32==========
        void kernel_only_asm_mm256_mullo_epi32();
    #endif

    #ifdef __AVX__
        //==========_mm256_mul_ps==========
        void kernel_only_asm_mm256_mul_ps();
        //==========_mm256_mul_pd==========
        void kernel_only_asm_mm256_mul_pd();
    #endif

#endif

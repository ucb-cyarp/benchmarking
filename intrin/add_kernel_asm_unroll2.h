#ifndef _H_ADD_KERNEL_ASM_UNROLL2
    #define _H_ADD_KERNEL_ASM_UNROLL2

    #include "intrin_bench_default_defines.h"

    #ifdef __AVX2__
        //==========_mm256_add_epi8==========
        void kernel_only_asm_mm256_add_epi8_unroll2();
        //==========_mm256_add_epi16==========
        void kernel_only_asm_mm256_add_epi16_unroll2();
        //==========_mm256_add_epi32==========
        void kernel_only_asm_mm256_add_epi32_unroll2();
        //==========_mm256_add_epi64==========
        void kernel_only_asm_mm256_add_epi64_unroll2();
    #endif

    #ifdef __AVX__
        //==========_mm256_add_ps==========
        void kernel_only_asm_mm256_add_ps_unroll2();
        //==========_mm256_add_pd==========
        void kernel_only_asm_mm256_add_pd_unroll2();
    #endif
    
#endif

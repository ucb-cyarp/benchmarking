#ifndef _H_ADD_KERNEL_ASM_UNROLL4
    #define _H_ADD_KERNEL_ASM_UNROLL4

    #include "intrin_bench_default_defines.h"

    #ifdef __AVX2__
        //==========_mm256_add_epi8==========
        void kernel_only_asm_mm256_add_epi8_unroll4();
        //==========_mm256_add_epi16==========
        void kernel_only_asm_mm256_add_epi16_unroll4();
        //==========_mm256_add_epi32==========
        void kernel_only_asm_mm256_add_epi32_unroll4();
        //==========_mm256_add_epi64==========
        void kernel_only_asm_mm256_add_epi64_unroll4();
    #endif

    #ifdef __AVX__
        //==========_mm256_add_ps==========
        void kernel_only_asm_mm256_add_ps_unroll4();
        //==========_mm256_add_pd==========
        void kernel_only_asm_mm256_add_pd_unroll4();
    #endif
    
#endif

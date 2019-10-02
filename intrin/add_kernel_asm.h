#ifndef _H_ADD_KERNEL_ASM
    #define _H_ADD_KERNEL_ASM

    #include "intrin_bench_default_defines_and_imports_cpp.h"

    #ifdef __AVX2__
        //==========_mm256_add_epi8==========
        void kernel_only_asm_mm256_add_epi8();
        //==========_mm256_add_epi16==========
        void kernel_only_asm_mm256_add_epi16();
        //==========_mm256_add_epi32==========
        void kernel_only_asm_mm256_add_epi32();
        //==========_mm256_add_epi64==========
        void kernel_only_asm_mm256_add_epi64();
    #endif

    #ifdef __AVX__
        //==========_mm256_add_ps==========
        void kernel_only_asm_mm256_add_ps();
        //==========_mm256_add_pd==========
        void kernel_only_asm_mm256_add_pd();
    #endif
    
#endif

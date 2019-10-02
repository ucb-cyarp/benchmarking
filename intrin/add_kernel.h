#ifndef _H_ADD_KERNEL
    #define _H_ADD_KERNEL

    #include "intrin_bench_default_defines_and_imports_cpp.h"

    #ifdef __AVX2__
        //==========_mm256_add_epi8==========
        void kernel_only_mm256_add_epi8();
        //==========_mm256_add_epi16==========
        void kernel_only_mm256_add_epi16();
        //==========_mm256_add_epi32==========
        void kernel_only_mm256_add_epi32();
    #endif

    #ifdef __AVX__
        //==========_mm256_add_ps==========
        void kernel_only_mm256_add_ps();
        //==========_mm256_add_pd==========
        void kernel_only_mm256_add_pd();
    #endif
    
#endif

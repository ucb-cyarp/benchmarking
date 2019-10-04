#ifndef _H_LOAD_FMA_STORE_KERNEL
    #define _H_LOAD_FMA_STORE_KERNEL

    #include "intrin_bench_default_defines_and_imports_cpp.h"

    #ifdef __FMA__
        //==========_mm256_fmadd_ps==========
        void kernel_mm256_fmadd_ps( __m256* a, __m256* b, __m256* c, __m256* d);
        //==========_mm256_fmadd_pd==========
        void kernel_mm256_fmadd_pd( __m256d* a, __m256d* b, __m256d* c, __m256d* d);
    #endif

#endif

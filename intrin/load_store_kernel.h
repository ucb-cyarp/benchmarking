#ifndef _H_LOAD_STORE_KERNEL
    #define _H_LOAD_STORE_KERNEL
    #include "intrin_bench_default_defines.h"

    #ifdef __AVX__
        //==========_mm256_load_si256 | _mm256_store_si256==========
        void kernel_mm256_load_si256__mm256_store_si256( __m256i* a, __m256i* b);
        //==========_mm256_load_ps | _mm256_store_ps==========
        void kernel_mm256_load_ps__mm256_store_ps( __m256* a, __m256* b);
        //==========_mm256_load_pd | _mm256_store_pd==========
        void kernel_mm256_load_pd__mm256_store_pd( __m256d* a, __m256d* b);
    #endif
    
#endif

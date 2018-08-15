#ifndef _H_LOAD_ADD_STORE_UNROLL2_KERNEL
    #define _H_LOAD_ADD_STORE_UNROLL2_KERNEL
    #include "intrin_bench_default_defines.h"

    #ifdef __AVX2__
        //==========_mm256_add_epi8==========
        void kernel_u2_mm256_add_epi8( __m256i* a, __m256i* b, __m256i* c);
        //==========_mm256_add_epi16==========
        void kernel_u2_mm256_add_epi16( __m256i* a, __m256i* b, __m256i* c);
        //==========_mm256_add_epi32==========
        void kernel_u2_mm256_add_epi32( __m256i* a, __m256i* b, __m256i* c);
        //==========_mm256_add_epi64==========
        void kernel_u2_mm256_add_epi64( __m256i* a, __m256i* b, __m256i* c);
    #endif

    #ifdef __AVX__
        //==========_mm256_ad_u2d_ps==========
        void kernel_u2_mm256_add_ps( __m256* a, __m256* b, __m256* c);
        //==========_mm256_add_pd==========
        void kernel_u2_mm256_add_pd( __m256d* a, __m256d* b, __m256d* c);
    #endif

#endif

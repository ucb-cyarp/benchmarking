#ifndef _H_STORE_KERNEL_ASM
    #define _H_STORE_KERNEL_ASM
    #include "intrin_bench_default_defines.h"

    #ifdef __AVX__
        //==========_mm256_store_si256 int8==========
        void kernel_asm_mm256_store_si256_int8( __m256i* a);
        //==========_mm256_store_si256 int16==========
        void kernel_asm_mm256_store_si256_int16( __m256i* a);
        //==========_mm256_store_si256 int32==========
        void kernel_asm_mm256_store_si256_int32( __m256i* a);
        //==========_mm256_store_si256 int64==========
        void kernel_asm_mm256_store_si256_int64( __m256i* a);
        //==========_mm256_store_ps==========
        void kernel_asm_mm256_store_ps( __m256* a);
        //==========_mm256_store_pd==========
        void kernel_asm_mm256_store_pd( __m256d* a);
    #endif

#endif

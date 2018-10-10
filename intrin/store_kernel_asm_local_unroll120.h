#ifndef _H_STORE_KERNEL_ASM_LOCAL_UNROLL120
    #define _H_STORE_KERNEL_ASM_LOCAL_UNROLL120
    #include "intrin_bench_default_defines.h"

    #ifdef __AVX__
        //==========_mm256_store_si256 int8==========
        void kernel_asm_mm256_store_si256_int8_local_unroll120( __m256i* a);
        //==========_mm256_store_si256 int16==========
        void kernel_asm_mm256_store_si256_int16_local_unroll120( __m256i* a);
        //==========_mm256_store_si256 int32==========
        void kernel_asm_mm256_store_si256_int32_local_unroll120( __m256i* a);
        //==========_mm256_store_si256 int64==========
        void kernel_asm_mm256_store_si256_int64_local_unroll120( __m256i* a);
        //==========_mm256_store_ps==========
        void kernel_asm_mm256_store_ps_local_unroll120( __m256* a);
        //==========_mm256_store_pd==========
        void kernel_asm_mm256_store_pd_local_unroll120( __m256d* a);
    #endif

#endif

#ifndef _H_STORE_KERNEL_ASM_LOCAL_UNROLL2
    #define _H_STORE_KERNEL_ASM_LOCAL_UNROLL2
    #include "intrin_bench_default_defines_and_imports_cpp.h"

    #ifdef __AVX__
        //==========_mm256_store_si256 int8==========
        void kernel_asm_mm256_store_si256_int8_local_unroll2( __m256i* a);
        //==========_mm256_store_si256 int16==========
        void kernel_asm_mm256_store_si256_int16_local_unroll2( __m256i* a);
        //==========_mm256_store_si256 int32==========
        void kernel_asm_mm256_store_si256_int32_local_unroll2( __m256i* a);
        //==========_mm256_store_si256 int64==========
        void kernel_asm_mm256_store_si256_int64_local_unroll2( __m256i* a);
        //==========_mm256_store_ps==========
        void kernel_asm_mm256_store_ps_local_unroll2( __m256* a);
        //==========_mm256_store_pd==========
        void kernel_asm_mm256_store_pd_local_unroll2( __m256d* a);
    #endif

#endif

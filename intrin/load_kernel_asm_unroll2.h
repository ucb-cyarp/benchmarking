#ifndef _H_LOAD_KERNEL_ASM_UNROLL2
    #define _H_LOAD_KERNEL_ASM_UNROLL2
    #include "intrin_bench_default_defines_and_imports_cpp.h"

    #ifdef __AVX__
        //==========_mm256_load_si256 int8==========
        void kernel_asm_mm256_load_si256_int8_unroll2( __m256i* a);
        //==========_mm256_load_si256 int16==========
        void kernel_asm_mm256_load_si256_int16_unroll2( __m256i* a);
        //==========_mm256_load_si256 int32==========
        void kernel_asm_mm256_load_si256_int32_unroll2( __m256i* a);
        //==========_mm256_load_si256 int64==========
        void kernel_asm_mm256_load_si256_int64_unroll2( __m256i* a);
        //==========_mm256_load_ps==========
        void kernel_asm_mm256_load_ps_unroll2( __m256* a);
        //==========_mm256_load_pd==========
        void kernel_asm_mm256_load_pd_unroll2( __m256d* a);
    #endif

#endif

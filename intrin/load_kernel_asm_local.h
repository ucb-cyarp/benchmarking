#ifndef _H_LOAD_KERNEL_ASM_LOCAL
    #define _H_LOAD_KERNEL_ASM_LOCAL
    #include "intrin_bench_default_defines_and_imports_cpp.h"

    #ifdef __AVX__
        //==========_mm256_load_si256 int8==========
        void kernel_asm_mm256_load_si256_int8_local( __m256i* a);
        //==========_mm256_load_si256 int16==========
        void kernel_asm_mm256_load_si256_int16_local( __m256i* a);
        //==========_mm256_load_si256 int32==========
        void kernel_asm_mm256_load_si256_int32_local( __m256i* a);
        //==========_mm256_load_si256 int64==========
        void kernel_asm_mm256_load_si256_int64_local( __m256i* a);
        //==========_mm256_load_ps==========
        void kernel_asm_mm256_load_ps_local( __m256* a);
        //==========_mm256_load_pd==========
        void kernel_asm_mm256_load_pd_local( __m256d* a);
    #endif

#endif

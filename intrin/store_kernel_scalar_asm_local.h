#ifndef _H_STORE_KERNEL_SCALAR_ASM_LOCAL
    #define _H_STORE_KERNEL_SCALAR_ASM_LOCAL
    #include "intrin_bench_default_defines_and_imports_cpp.h"

    //==========store int8==========
    void kernel_asm_store_int8_local( __m256i* a);
    //==========store int16==========
    void kernel_asm_store_int16_local( __m256i* a);
    //==========store int32==========
    void kernel_asm_store_int32_local( __m256i* a);
    //==========store int64==========
    void kernel_asm_store_int64_local( __m256i* a);

    #ifdef __SSE2__
        //==========store float==========
        void kernel_asm_store_ss_local( __m256* a);
        //==========store double==========
        void kernel_asm_store_sd_local( __m256d* a);
    #endif

#endif

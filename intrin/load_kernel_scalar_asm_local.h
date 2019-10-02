#ifndef _H_LOAD_KERNEL_SCALAR_ASM_LOCAL
    #define _H_LOAD_KERNEL_SCALAR_ASM_LOCAL
    #include "intrin_bench_default_defines_and_imports_cpp.h"d_imports_cpp.h"

    //==========load int8==========
    void kernel_asm_load_int8_local( __m256i* a);
    //==========load int16==========
    void kernel_asm_load_int16_local( __m256i* a);
    //==========load int32==========
    void kernel_asm_load_int32_local( __m256i* a);
    //==========load int64==========
    void kernel_asm_load_int64_local( __m256i* a);

    #ifdef __SSE2__
        //==========load float==========
        void kernel_asm_load_ss_local( __m256* a);
        //==========load double==========
        void kernel_asm_load_sd_local( __m256d* a);
    #endif

#endif

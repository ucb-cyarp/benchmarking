#ifndef _H_LOAD_KERNEL_SCALAR_ASM
    #define _H_LOAD_KERNEL_SCALAR_ASM
    #include "intrin_bench_default_defines_and_imports_cpp.h"

    //==========load int8==========
    void kernel_asm_load_int8( __m256i* a);
    //==========load int16==========
    void kernel_asm_load_int16( __m256i* a);
    //==========load int32==========
    void kernel_asm_load_int32( __m256i* a);
    //==========load int64==========
    void kernel_asm_load_int64( __m256i* a);

    #ifdef __SSE2__
        //==========load float==========
        void kernel_asm_load_ss( __m256* a);
        //==========load double==========
        void kernel_asm_load_sd( __m256d* a);
    #endif

#endif

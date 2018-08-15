#ifndef _H_STORE_KERNEL_SCALAR_ASM
    #define _H_STORE_KERNEL_SCALAR_ASM
    #include "intrin_bench_default_defines.h"

    //==========store int8==========
    void kernel_asm_store_int8( __m256i* a);
    //==========store int16==========
    void kernel_asm_store_int16( __m256i* a);
    //==========store int32==========
    void kernel_asm_store_int32( __m256i* a);
    //==========store int64==========
    void kernel_asm_store_int64( __m256i* a);

    #ifdef __SSE2__
        //==========store float==========
        void kernel_asm_store_ss( __m256* a);
        //==========store double==========
        void kernel_asm_store_sd( __m256d* a);
    #endif

#endif

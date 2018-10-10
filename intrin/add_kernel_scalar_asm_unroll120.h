#ifndef _H_ADD_KERNEL_SCALAR_ASM_UNROLL120
    #define _H_ADD_KERNEL_SCALAR_ASM_UNROLL120

    #include "intrin_bench_default_defines.h"

        //==========add 8 bit==========
        void kernel_only_asm_add_i8_unroll120();
        //==========add 16 bit==========
        void kernel_only_asm_add_i16_unroll120();
        //==========add 32 bit==========
        void kernel_only_asm_add_i32_unroll120();
        //==========add 64 bit==========
        void kernel_only_asm_add_i64_unroll120();
        //==========add floating point==========
        void kernel_only_asm_add_fp_unroll120();
        //==========add single with SSE==========
        void kernel_only_asm_add_sp_unroll120();
        //==========add double with SSE==========
        void kernel_only_asm_add_dp_unroll120();
    
#endif

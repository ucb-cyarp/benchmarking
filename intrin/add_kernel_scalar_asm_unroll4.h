#ifndef _H_ADD_KERNEL_SCALAR_ASM_UNROLL4
    #define _H_ADD_KERNEL_SCALAR_ASM_UNROLL4

    #include "intrin_bench_default_defines_and_imports_cpp.h"

        //==========add 8 bit==========
        void kernel_only_asm_add_i8_unroll4();
        //==========add 16 bit==========
        void kernel_only_asm_add_i16_unroll4();
        //==========add 32 bit==========
        void kernel_only_asm_add_i32_unroll4();
        //==========add 64 bit==========
        void kernel_only_asm_add_i64_unroll4();
        //==========add floating point==========
        void kernel_only_asm_add_fp_unroll4();
        //==========add single with SSE==========
        void kernel_only_asm_add_sp_unroll4();
        //==========add double with SSE==========
        void kernel_only_asm_add_dp_unroll4();
    
#endif

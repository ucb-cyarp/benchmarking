#ifndef _H_DIV_KERNEL_SCALAR_ASM_UNROLL120
    #define _H_DIV_KERNEL_SCALAR_ASM_UNROLL120

    #include "intrin_bench_default_defines_and_imports_cpp.h"

        //==========div 8 bit==========
        void kernel_only_asm_div_i8_unroll120();
        void kernel_only_asm_div_i8_unroll120_regRename();
        //==========div 16 bit==========
        void kernel_only_asm_div_i16_unroll120();
        void kernel_only_asm_div_i16_unroll120_regRename();
        //==========div 32 bit==========
        void kernel_only_asm_div_i32_unroll120();
        void kernel_only_asm_div_i32_unroll120_regRename();
        //==========div 64 bit==========
        void kernel_only_asm_div_i64_unroll120();
        void kernel_only_asm_div_i64_unroll120_regRename();
        //==========div floating point==========
        void kernel_only_asm_div_fp_unroll120();
        //==========div single with SSE==========
        void kernel_only_asm_div_sp_unroll120();
        //==========div double with SSE==========
        void kernel_only_asm_div_dp_unroll120();
    
#endif

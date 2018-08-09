#ifndef _H_DIV_KERNEL_SCALAR_ASM_UNROLL2
    #define _H_DIV_KERNEL_SCALAR_ASM_UNROLL2

    #include "intrin_bench_default_defines.h"

        //==========div 8 bit==========
        void kernel_only_asm_div_i8_unroll2();
        void kernel_only_asm_div_i8_unroll2_regRename();
        //==========div 16 bit==========
        void kernel_only_asm_div_i16_unroll2();
        void kernel_only_asm_div_i16_unroll2_regRename();
        //==========div 32 bit==========
        void kernel_only_asm_div_i32_unroll2();
        void kernel_only_asm_div_i32_unroll2_regRename();
        //==========div 64 bit==========
        void kernel_only_asm_div_i64_unroll2();
        void kernel_only_asm_div_i64_unroll2_regRename();
        //==========div floating point==========
        void kernel_only_asm_div_fp_unroll2();
        //==========div single with SSE==========
        void kernel_only_asm_div_sp_unroll2();
        //==========div double with SSE==========
        void kernel_only_asm_div_dp_unroll2();
    
#endif

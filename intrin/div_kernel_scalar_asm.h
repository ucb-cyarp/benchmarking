#ifndef _H_DIV_KERNEL_SCALAR_ASM
    #define _H_DIV_KERNEL_SCALAR_ASM

    #include "intrin_bench_default_defines_and_imports_cpp.h"

        //==========div 8 bit==========
        void kernel_only_asm_div_i8();
        void kernel_only_asm_div_i8_regRename();
        //==========div 16 bit==========
        void kernel_only_asm_div_i16();
        void kernel_only_asm_div_i16_regRename();
        //==========div 32 bit==========
        void kernel_only_asm_div_i32();
        void kernel_only_asm_div_i32_regRename();
        //==========div 64 bit==========
        void kernel_only_asm_div_i64();
        void kernel_only_asm_div_i64_regRename();
        //==========div floating point==========
        void kernel_only_asm_div_fp();
        //==========div single with SSE==========
        void kernel_only_asm_div_sp();
        //==========div double with SSE==========
        void kernel_only_asm_div_dp();
    
#endif

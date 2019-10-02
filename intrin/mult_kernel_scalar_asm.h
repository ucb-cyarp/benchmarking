#ifndef _H_MULT_KERNEL_SCALAR_ASM
    #define _H_MULT_KERNEL_SCALAR_ASM

    #include "intrin_bench_default_defines_and_imports_cpp.h"

        //==========mult 8 bit==========
        void kernel_only_asm_mult_i8();
        void kernel_only_asm_mult_i8_regRename();
        //==========mult 16 bit==========
        void kernel_only_asm_mult_i16();
        //==========mult 32 bit==========
        void kernel_only_asm_mult_i32();
        //==========mult 64 bit==========
        void kernel_only_asm_mult_i64();
        //==========mult floating point==========
        void kernel_only_asm_mult_fp();
        //==========mult single with SSE==========
        void kernel_only_asm_mult_sp();
        //==========mult double with SSE==========
        void kernel_only_asm_mult_dp();
    
#endif

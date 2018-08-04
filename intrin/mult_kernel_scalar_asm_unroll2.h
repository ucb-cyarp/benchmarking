#ifndef _H_MULT_KERNEL_SCALAR_ASM_UNROLL2
    #define _H_MULT_KERNEL_SCALAR_ASM_UNROLL2

    #include "intrin_bench_default_defines.h"

        //==========mult 8 bit==========
        void kernel_only_asm_mult_i8_unroll2();
        //==========mult 16 bit==========
        void kernel_only_asm_mult_i16_unroll2();
        //==========mult 32 bit==========
        void kernel_only_asm_mult_i32_unroll2();
        //==========mult 64 bit==========
        void kernel_only_asm_mult_i64_unroll2();
        //==========mult floating point==========
        void kernel_only_asm_mult_fp_unroll2();
        //==========mult single with SSE==========
        void kernel_only_asm_mult_sp_unroll2();
        //==========mult double with SSE==========
        void kernel_only_asm_mult_dp_unroll2();
    
#endif

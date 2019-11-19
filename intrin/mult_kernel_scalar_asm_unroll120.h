#ifndef _H_MULT_KERNEL_SCALAR_ASM_UNROLL120
    #define _H_MULT_KERNEL_SCALAR_ASM_UNROLL120

    #include "intrin_bench_default_defines_and_imports_cpp.h"

        //==========mult 8 bit==========
        void kernel_only_asm_mult_i8_unroll120();
        //==========mult 16 bit==========
        void kernel_only_asm_mult_i16_unroll120();
        //==========mult 32 bit==========
        void kernel_only_asm_mult_i32_unroll120();
        //==========mult 64 bit==========
        void kernel_only_asm_mult_i64_unroll120();
        //==========mult floating point==========
        void kernel_only_asm_mult_fp_unroll120();
        //==========mult single with SSE==========
        void kernel_only_asm_mult_sp_unroll120();
        //==========mult double with SSE==========
        void kernel_only_asm_mult_dp_unroll120();
    
#endif

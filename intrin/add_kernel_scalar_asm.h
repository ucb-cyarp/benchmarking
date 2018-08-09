#ifndef _H_ADD_KERNEL_SCALAR_ASM
    #define _H_ADD_KERNEL_SCALAR_ASM

    #include "intrin_bench_default_defines.h"

        //==========add 8 bit==========
        void kernel_only_asm_add_i8();
        //==========add 16 bit==========
        void kernel_only_asm_add_i16();
        //==========add 32 bit==========
        void kernel_only_asm_add_i32();
        //==========add 64 bit==========
        void kernel_only_asm_add_i64();
        //==========add floating point==========
        void kernel_only_asm_add_fp();
        //==========add single with SSE==========
        void kernel_only_asm_add_sp();
        //==========add double with SSE==========
        void kernel_only_asm_add_dp();
    
#endif

#include "intrin_bench_default_defines.h"

//See https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/x64-architecture
//https://docs.oracle.com/cd/E18752_01/html/817-5477/eoizy.html
//http://flint.cs.yale.edu/cs421/papers/x86-asm/asm.html

//==========8 bit mult==========
void kernel_only_asm_mult_i8_unroll2()
{
    for(int i = 0; i<STIM_LEN/2; i++)
    {
        //TODO: Verify

        //The byte multiply does not have a 2 arg version
        //One arg is the al register and it writes back into the ax register: ax <- ar * reg8
        //NOTE: tests showed that this had half the throughput of the other integer multiplies.  Suggests dependence stopped multiple dispatch.
        asm volatile(
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            :
            :
            : "ax", "bl", "dl"
        );
    }
}

void kernel_only_asm_mult_i8_unroll2_regRename()
{
    for(int i = 0; i<STIM_LEN/2; i++)
    {
        //This version attempts to rename registers to avoid conflicts on dual dispatch.  Dependence is both WAW and RAW.  Need to move result into different reg to resolve WAW.  Need to move operand into AL to resolve RAW.
        asm volatile(
            "movb %%cl, %%al\n\t"
            "imulb  %%bl\n\t"
            "movw %%ax, %%si\n\t"
            "movb %%cl, %%al\n\t"
            "imulb  %%dl\n\t"
            "movw %%ax, %%di\n\t"
            :
            :
            : "ax", "bl", "cl", "dl", "si", "di"
        );
    }
}

//==========16 bit mult==========
void kernel_only_asm_mult_i16_unroll2()
{
    for(int i = 0; i<STIM_LEN/2; i++)
    {
        //Doing 2 adds to hopefully avoid an issue with dependence.  Intel ASM is reg1=reg1+reg2.  It is not a three register instruction like it is with the vector unit
        //TODO: Verify
        asm volatile(
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            :
            :
            : "ax", "bx", "cx", "dx"
        );
    }
}

//==========32 bit mult==========
void kernel_only_asm_mult_i32_unroll2()
{ 
    for(int i = 0; i<STIM_LEN/2; i++)
    {
        //Doing 2 adds to hopefully avoid an issue with dependence.  Intel ASM is reg1=reg1+reg2.  It is not a three register instruction like it is with the vector unit
        //TODO: Verify
        asm volatile(
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            :
            :
            : "eax", "ebx", "ecx", "edx"
        );
    }
}

//==========64 bit mult==========
void kernel_only_asm_mult_i64_unroll2()
{
    for(int i = 0; i<STIM_LEN/2; i++)
    {
        //Doing 2 adds to hopefully avoid an issue with dependence.  Intel ASM is reg1=reg1+reg2.  It is not a three register instruction like it is with the vector unit
        //TODO: Verify
        asm volatile(
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            :
            :
            : "rax", "rbx", "rcx", "rdx"
        );
    }
}

//==========mult floating point==========
void kernel_only_asm_mult_fp_unroll2()
{
    for(int i = 0; i<STIM_LEN/2; i++)
    {
        //Need to load onto stack, compute, and pop
        //Probably should be optomized further
        //Based on section 8.3.5 of the Intel® 64 and IA-32 Architectures Software Developer’s Manual
        //(https://software.intel.com/sites/default/files/managed/39/c5/325462-sdm-vol-1-2abcd-3abcd.pdf),
        //values are propomoted on load to extended double (long double - 80 bits)
        //The control word which handles the rounding mode is discussed in section 8.1.5.
        //Apparently, this also sets the precision to either extended double, double, or single precision
        //the default is supposed to be extended double but it can be changed.
        //TODO: Investigate how to change the Control Word

        //TODO: This may be a bit of a red harring as x87 is "obsolte".  It looks like GNU will use 
        //SSE registers (and therefore SSE instructions) instead of x87.  In fact, it looks like GCC
        //uses x87 with the long double mode if on legacy systems and does not actually set the precision
        //registers due to the overheads.  This can result in non IEEE complient results.  Using memory
        //operands helps but it produced a "double rounding" problem.
        //https://stackoverflow.com/questions/50308366/storing-the-x87-fpu-control-word
        //https://gcc.gnu.org/wiki/FloatingPointMath#x86note
        
        //TODO: Verify
        asm volatile(
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fmulp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fmulp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            :
            :
            :"st(3)", "st(2)", "st(1)", "st"
        );
    }
}

//==========mult single with SSE==========
void kernel_only_asm_mult_sp_unroll2()
{
    for(int i = 0; i<STIM_LEN/2; i++)
    {
        //This is using the SSE vector unit to multiply a single number (avoiding x87).
        //It was suggested in the GCC manual that the SSE instructions are used instead of x07
        
        //TODO: Verify
        asm volatile(
            "movd %%eax, %%xmm0 \n\t"
            "movd %%ebx, %%xmm1 \n\t"
            "mulss %%xmm1, %%xmm0 \n\t"
            "movd %%xmm0, %%ecx \n\t"
            "movd %%edx, %%xmm2 \n\t"
            "movd %%esi, %%xmm3 \n\t"
            "mulss %%xmm3, %%xmm2 \n\t"
            "movd %%xmm2, %%edi \n\t"
            :
            :
            :"eax", "ebx", "ecx", "edx", "esi", "edi", "xmm0", "xmm1", "xmm2", "xmm3"
        );
    }
}

//==========mult double with SSE==========
void kernel_only_asm_mult_dp_unroll2()
{
    for(int i = 0; i<STIM_LEN/2; i++)
    {
        //This is using the SSE vector unit to multiply a single number (avoiding x87).
        //It was suggested in the GCC manual that the SSE instructions are used instead of x07
        
        //TODO: Verify
        asm volatile(
            "movq %%rax, %%xmm0 \n\t"
            "movq %%rbx, %%xmm1 \n\t"
            "mulsd %%xmm1, %%xmm0 \n\t"
            "movq %%xmm0, %%rcx \n\t"
            "movd %%rdx, %%xmm2 \n\t"
            "movd %%rsi, %%xmm3 \n\t"
            "mulsd %%xmm3, %%xmm2 \n\t"
            "movd %%xmm2, %%rdi \n\t"
            :
            :
            :"rax", "rbx", "rcx", "rdx", "rsi", "rdi", "xmm0", "xmm1", "xmm2", "xmm3"
        );
    }
}

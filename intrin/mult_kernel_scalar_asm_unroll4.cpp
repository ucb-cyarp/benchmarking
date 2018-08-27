#include "intrin_bench_default_defines.h"

//See https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/x64-architecture
//https://docs.oracle.com/cd/E18752_01/html/817-5477/eoizy.html
//http://flint.cs.yale.edu/cs421/papers/x86-asm/asm.html

//==========8 bit mult==========
void kernel_only_asm_mult_i8_unroll4()
{
    for(int i = 0; i<STIM_LEN/4; i++)
    {
        //TODO: Verify

        //The byte multiply does not have a 2 arg version
        //One arg is the al register and it writes back into the ax register: ax <- ar * reg8
        //NOTE: tests showed that this had half the throughput of the other integer multiplies.  Suggests dependence stopped multiple dispatch.
        asm volatile(
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            :
            :
            : "ax", "bl", "dl", "bh", "dh"
        );
    }
}

//==========16 bit mult==========
void kernel_only_asm_mult_i16_unroll4()
{
    for(int i = 0; i<STIM_LEN/4; i++)
    {
        //Doing 2 adds to hopefully avoid an issue with dependence.  Intel ASM is reg1=reg1+reg2.  It is not a three register instruction like it is with the vector unit
        //TODO: Verify
        asm volatile(
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            :
            :
            : "ax", "bx", "cx", "dx", "si", "di", "r8w", "r9w"
        );
    }
}

//==========32 bit mult==========
void kernel_only_asm_mult_i32_unroll4()
{ 
    for(int i = 0; i<STIM_LEN/4; i++)
    {
        //Doing 2 adds to hopefully avoid an issue with dependence.  Intel ASM is reg1=reg1+reg2.  It is not a three register instruction like it is with the vector unit
        //TODO: Verify
        asm volatile(
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            :
            :
            : "eax", "ebx", "ecx", "edx", "esi", "edi", "r8d", "r9d"
        );
    }
}

//==========64 bit mult==========
void kernel_only_asm_mult_i64_unroll4()
{
    for(int i = 0; i<STIM_LEN/4; i++)
    {
        //Doing 2 adds to hopefully avoid an issue with dependence.  Intel ASM is reg1=reg1+reg2.  It is not a three register instruction like it is with the vector unit
        //TODO: Verify
        asm volatile(
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            :
            :
            : "rax", "rbx", "rcx", "rdx", "rsi", "rdi", "r8", "r9"
        );
    }
}

//==========mult floating point==========
void kernel_only_asm_mult_fp_unroll4()
{
    for(int i = 0; i<STIM_LEN/4; i++)
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
void kernel_only_asm_mult_sp_unroll4()
{
    float a = 0.5;
    float b = 0.25;
    float c;

    float d = 0.5;
    float e = 0.25;
    float f;

    float g = 0.5;
    float h = 0.25;
    float j;

    float k = 0.5;
    float l = 0.25;
    float m;

    for(int i = 0; i<STIM_LEN/4; i++)
    {
        //This is using the SSE vector unit to multiply a single number (avoiding x87).
        //It was suggested in the GCC manual that the SSE instructions are used instead of x07
        
        //TODO: Verify
        asm volatile(
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            :"=x"(c), "=x"(f), "=x"(j), "=x"(m)
            :"x"(a), "x"(b), "x"(d), "x"(e), "x"(g), "x"(h), "x"(k), "x"(l)
            :
        );
    }
}

//==========mult double with SSE==========
void kernel_only_asm_mult_dp_unroll4()
{
    double a = 0.5;
    double b = 0.25;
    double c;

    double d = 0.5;
    double e = 0.25;
    double f;

    double g = 0.5;
    double h = 0.25;
    double j;

    double k = 0.5;
    double l = 0.25;
    double m;

    for(int i = 0; i<STIM_LEN/4; i++)
    {
        //This is using the SSE vector unit to multiply a single number (avoiding x87).
        //It was suggested in the GCC manual that the SSE instructions are used instead of x07
        
        //TODO: Verify
        asm volatile(
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            :"=x"(c), "=x"(f), "=x"(j), "=x"(m)
            :"x"(a), "x"(b), "x"(d), "x"(e), "x"(g), "x"(h), "x"(k), "x"(l)
            :
        );
    }
}

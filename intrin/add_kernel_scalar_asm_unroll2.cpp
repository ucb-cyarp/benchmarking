#include "intrin_bench_default_defines.h"

//See https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/x64-architecture
//https://docs.oracle.com/cd/E18752_01/html/817-5477/eoizy.html
//http://flint.cs.yale.edu/cs421/papers/x86-asm/asm.html

//==========8 bit add==========
void kernel_only_asm_add_i8_unroll2()
{
    for(int i = 0; i<STIM_LEN/2; i++)
    {
        //Doing 2 adds to hopefully avoid an issue with dependence.  Intel ASM is reg1=reg1+reg2.  It is not a three register instruction like it is with the vector unit
        //TODO: Verify
        asm volatile(
            "add  %%al, %%bl\n\t"
            "add  %%cl, %%dl\n\t"
            :
            :
            : "al", "bl", "cl", "dl"
        );
    }
}

//==========16 bit add==========
void kernel_only_asm_add_i16_unroll2()
{
    for(int i = 0; i<STIM_LEN/2; i++)
    {
        //Doing 2 adds to hopefully avoid an issue with dependence.  Intel ASM is reg1=reg1+reg2.  It is not a three register instruction like it is with the vector unit
        //TODO: Verify
        asm volatile(
            "add  %%ax, %%bx\n\t"
            "add  %%cx, %%dx\n\t"
            :
            :
            : "ax", "bx", "cx", "dx"
        );
    }
}

//==========32 bit add==========
void kernel_only_asm_add_i32_unroll2()
{ 
    for(int i = 0; i<STIM_LEN/2; i++)
    {
        //Doing 2 adds to hopefully avoid an issue with dependence.  Intel ASM is reg1=reg1+reg2.  It is not a three register instruction like it is with the vector unit
        //TODO: Verify
        asm volatile(
            "add  %%eax, %%ebx\n\t"
            "add  %%ecx, %%edx\n\t"
            :
            :
            : "eax", "ebx", "ecx", "edx"
        );
    }
}

//==========64 bit add==========
void kernel_only_asm_add_i64_unroll2()
{
    for(int i = 0; i<STIM_LEN/2; i++)
    {
        //Doing 2 adds to hopefully avoid an issue with dependence.  Intel ASM is reg1=reg1+reg2.  It is not a three register instruction like it is with the vector unit
        //TODO: Verify
        asm volatile(
            "add  %%rax, %%rbx\n\t"
            "add  %%rcx, %%rdx\n\t"
            :
            :
            : "rax", "rbx", "rcx", "rdx"
        );
    }
}

//==========add floating point==========
void kernel_only_asm_add_fp_unroll2()
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
            "faddp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "faddp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            :
            :
            :"st(3)", "st(2)", "st(1)", "st"
        );
    }
}

//==========add single with SSE==========
void kernel_only_asm_add_sp_unroll2()
{
    float a = 0.5;
    float b = 0.25;
    float c;

    float d = 0.5;
    float e = 0.25;
    float f;

    for(int i = 0; i<STIM_LEN/2; i++)
    {
        //This is using the SSE vector unit to multiply a single number (avoiding x87).
        //It was suggested in the GCC manual that the SSE instructions are used instead of x07
        
        //TODO: Verify
        asm volatile(
            "vaddss %2, %3, %0 \n\t"
            "vaddss %4, %5, %1 \n\t"
            :"=x"(c), "=x"(f)
            :"x"(a), "x"(b), "x"(d), "x"(e)
            :
        );
    }
}

//==========add double with SSE==========
void kernel_only_asm_add_dp_unroll2()
{
    double a = 0.5;
    double b = 0.25;
    double c;

    double d = 0.5;
    double e = 0.25;
    double f;

    for(int i = 0; i<STIM_LEN/2; i++)
    {
        //This is using the SSE vector unit to multiply a single number (avoiding x87).
        //It was suggested in the GCC manual that the SSE instructions are used instead of x07
        
        //TODO: Verify
        asm volatile(
            "vaddsd %2, %3, %0 \n\t"
            "vaddsd %4, %5, %1 \n\t"
            :"=x"(c), "=x"(f)
            :"x"(a), "x"(b), "x"(d), "x"(e)
            :
        );
    }
}

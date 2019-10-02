#include "intrin_bench_default_defines_and_imports_cpp.h"

//See https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/x64-architecture
//https://docs.oracle.com/cd/E18752_01/html/817-5477/eoizy.html
//http://flint.cs.yale.edu/cs421/papers/x86-asm/asm.html

//==========8 bit add==========
void kernel_only_asm_add_i8_unroll6()
{
    for(int i = 0; i<STIM_LEN/6; i++)
    {
        //Doing 2 adds to hopefully avoid an issue with dependence.  Intel ASM is reg1=reg1+reg2.  It is not a three register instruction like it is with the vector unit
        //TODO: Verify
        asm volatile(
            "add  %%al, %%bl\n\t"
            "add  %%cl, %%dl\n\t"
            "add  %%r8b, %%r9b\n\t"
            "add  %%r10b, %%r11b\n\t"
            "add  %%r12b, %%r13b\n\t"
            "add  %%r14b, %%r15b\n\t"
            :
            :
            : "al", "bl", "cl", "dl", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"
        );
    }
}

//==========16 bit add==========
void kernel_only_asm_add_i16_unroll6()
{
    for(int i = 0; i<STIM_LEN/6; i++)
    {
        //Doing 2 adds to hopefully avoid an issue with dependence.  Intel ASM is reg1=reg1+reg2.  It is not a three register instruction like it is with the vector unit
        //TODO: Verify
        asm volatile(
            "add  %%ax, %%bx\n\t"
            "add  %%cx, %%dx\n\t"
            "add  %%r8w, %%r9w\n\t"
            "add  %%r10w, %%r11w\n\t"
            "add  %%r12w, %%r13w\n\t"
            "add  %%r14w, %%r15w\n\t"
            :
            :
            : "al", "bl", "cl", "dl", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"
        );
    }
}

//==========32 bit add==========
void kernel_only_asm_add_i32_unroll6()
{ 
    for(int i = 0; i<STIM_LEN/6; i++)
    {
        //Doing 2 adds to hopefully avoid an issue with dependence.  Intel ASM is reg1=reg1+reg2.  It is not a three register instruction like it is with the vector unit
        //TODO: Verify
        asm volatile(
            "add  %%eax, %%ebx\n\t"
            "add  %%ecx, %%edx\n\t"
            "add  %%r8d, %%r9d\n\t"
            "add  %%r10d, %%r11d\n\t"
            "add  %%r12d, %%r13d\n\t"
            "add  %%r14d, %%r15d\n\t"
            :
            :
            : "al", "bl", "cl", "dl", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"
        );
    }
}

//==========64 bit add==========
void kernel_only_asm_add_i64_unroll6()
{
    for(int i = 0; i<STIM_LEN/6; i++)
    {
        //Doing 2 adds to hopefully avoid an issue with dependence.  Intel ASM is reg1=reg1+reg2.  It is not a three register instruction like it is with the vector unit
        //TODO: Verify
        asm volatile(
            "add  %%rax, %%rbx\n\t"
            "add  %%rcx, %%rdx\n\t"
            "add  %%r8, %%r9\n\t"
            "add  %%r10, %%r11\n\t"
            "add  %%r12, %%r13\n\t"
            "add  %%r14, %%r15\n\t"
            :
            :
            : "al", "bl", "cl", "dl", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"
        );
    }
}

//==========add floating point==========
void kernel_only_asm_add_fp_unroll6()
{
    for(int i = 0; i<STIM_LEN/6; i++)
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
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "faddp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "faddp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "faddp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            :
            :
            :"st(3)", "st(2)", "st(1)", "st"
        );
    }
}

//==========add single with SSE==========
void kernel_only_asm_add_sp_unroll6()
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

    float n = 0.5;
    float o = 0.25;
    float p;

    float q = 0.5;
    float r = 0.25;
    float s;

    for(int i = 0; i<STIM_LEN/6; i++)
    {
        //This is using the SSE vector unit to multiply a single number (avoiding x87).
        //It was suggested in the GCC manual that the SSE instructions are used instead of x07
        
        //TODO: Verify
        asm volatile(
            "vaddss %[a], %[b], %[c] \n\t"
            "vaddss %[d], %[e], %[f] \n\t"
            "vaddss %[g], %[h], %[j] \n\t"
            "vaddss %[k], %[l], %[m] \n\t"
            "vaddss %[n], %[o], %[p] \n\t"
            "vaddss %[q], %[r], %[s] \n\t"
            :[c]"=x"(c), [f]"=x"(f), [j]"=x"(j), [m]"=x"(m), [p]"=x"(p), [s]"=x"(s)
            :[a]"x"(a), [b]"x"(b), [d]"x"(d), [e]"x"(e), [g]"x"(g), [h]"x"(h), [k]"x"(k), [l]"x"(l), [n]"x"(n), [o]"x"(o), [q]"x"(q), [r]"x"(r)
            :
        );
    }
}

//==========add double with SSE==========
void kernel_only_asm_add_dp_unroll6()
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

    double n = 0.5;
    double o = 0.25;
    double p;

    double q = 0.5;
    double r = 0.25;
    double s;

    for(int i = 0; i<STIM_LEN/6; i++)
    {
        //This is using the SSE vector unit to multiply a single number (avoiding x87).
        //It was suggested in the GCC manual that the SSE instructions are used instead of x07
        
        //TODO: Verify
        asm volatile(
            "vaddsd %[a], %[b], %[c] \n\t"
            "vaddsd %[d], %[e], %[f] \n\t"
            "vaddsd %[g], %[h], %[j] \n\t"
            "vaddsd %[k], %[l], %[m] \n\t"
            "vaddsd %[n], %[o], %[p] \n\t"
            "vaddsd %[q], %[r], %[s] \n\t"
            :[c]"=x"(c), [f]"=x"(f), [j]"=x"(j), [m]"=x"(m), [p]"=x"(p), [s]"=x"(s)
            :[a]"x"(a), [b]"x"(b), [d]"x"(d), [e]"x"(e), [g]"x"(g), [h]"x"(h), [k]"x"(k), [l]"x"(l), [n]"x"(n), [o]"x"(o), [q]"x"(q), [r]"x"(r)
            :
        );
    }
}

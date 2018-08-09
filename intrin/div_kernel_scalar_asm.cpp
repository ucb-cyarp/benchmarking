#include "intrin_bench_default_defines.h"

//See https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/x64-architecture
//https://docs.oracle.com/cd/E18752_01/html/817-5477/eoizy.html
//http://flint.cs.yale.edu/cs421/papers/x86-asm/asm.html

//==========8 bit div==========
void kernel_only_asm_div_i8()
{
    int8_t divBy = 1;    //Avoid div by 0 which causes div exception
    register int16_t initVal asm ("ax") = 1; //Even though we are doing an 8 bit divide 
                                             //The numerand is 16 bits
                                             //https://gcc.gnu.org/onlinedocs/gcc/Local-Register-Variables.html#Local-Register-Variables
    register int16_t clobberResult asm ("ax"); //Need to specify an output since inputs cannot occur in clobber list

    for(int i = 0; i<STIM_LEN; i++)
    {
        //The divide does not have a 2 arg version
        //(al <- quotient, ah <- remainder) <- ax/reg8

        asm volatile( 
            // "movw %[aInit], %%ax\n\t" //Need to reset since ah (remainder is set by div)
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            : "=r" (clobberResult)
            : [aInit] "r" (initVal), [aDivBy] "r" (divBy)
            : //al and ah are part of ax (cannot list in clibber since is an input - instead list as output)
        );
    }
}

void kernel_only_asm_div_i8_regRename()
{
    int8_t divBy = 1;    //Avoid div by 0 which causes div exception
    int16_t initVal = 1;

    for(int i = 0; i<STIM_LEN; i++)
    {
        //The divide does not have a 2 arg version
        //(al <- quotient, ah <- remainder) <- ax/reg8

        //This version attempts to rename registers to avoid conflicts on dual dispatch.  Dependence is both WAW and RAW.  Need to move result into different reg to resolve WAW.  Need to move operand to resolve RAW.
        asm volatile(
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            :
            : [aInit] "r" (initVal), [aDivBy] "r" (divBy)
            : "ax", "si"
        );
    }
}

//==========16 bit div==========
void kernel_only_asm_div_i16()
{
    int16_t divBy = 1;    //Avoid div by 0 which causes div exception
    register int16_t initVal0 asm ("ax") = 1; //https://gcc.gnu.org/onlinedocs/gcc/Local-Register-Variables.html#Local-Register-Variables
    register int16_t initVal1 asm ("dx") = 0; 
                                              
    register int16_t clobberResult0 asm ("ax"); //Need to specify an output since inputs cannot occur in clobber list
    register int16_t clobberResult1 asm ("dx");

    for(int i = 0; i<STIM_LEN; i++)
    {
        //(ax <- quotient, dx <- remainder) <- dx:ax/reg16 (the 1st arg is formed by treating dx as the upper sig bits and ax as the lower)

        asm volatile(
            "idivw  %[aDivBy]\n\t"
            : "=r" (clobberResult0), "=r" (clobberResult1)
            : [aInit0] "r" (initVal0), [aInit1] "r" (initVal1), [aDivBy] "r" (divBy)
            : 
        );
    }
}

void kernel_only_asm_div_i16_regRename()
{
    int16_t divBy = 1;    //Avoid div by 0 which causes div exception
    int16_t initVal0 = 1;
    int16_t initVal1 = 0;

    for(int i = 0; i<STIM_LEN; i++)
    {
        //(ax <- quotient, dx <- remainder) <- dx:ax/reg16 (the 1st arg is formed by treating dx as the upper sig bits and ax as the lower)

        //This version attempts to rename registers to avoid conflicts on dual dispatch.  Dependence is both WAW and RAW.  Need to move result into different reg to resolve WAW.  Need to move operand to resolve RAW.

        asm volatile(
            "movw %[aInit0], %%ax\n\t"
            "movw %[aInit1], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            :
            : [aInit0] "r" (initVal0), [aInit1] "r" (initVal1), [aDivBy] "r" (divBy)
            : "ax", "dx", "si", "di"
        );
    }
}

//==========32 bit div==========
void kernel_only_asm_div_i32()
{ 
    int32_t divBy = 1;    //Avoid div by 0 which causes div exception
    register int32_t initVal0 asm ("eax") = 1; //https://gcc.gnu.org/onlinedocs/gcc/Local-Register-Variables.html#Local-Register-Variables
    register int32_t initVal1 asm ("edx") = 0; 
                                              
    register int32_t clobberResult0 asm ("eax"); //Need to specify an output since inputs cannot occur in clobber list
    register int32_t clobberResult1 asm ("edx");

    for(int i = 0; i<STIM_LEN; i++)
    {
        //(eax <- quotient, edx <- remainder) <- edx:eax/reg16 (the 1st arg is formed by treating edx as the upper sig bits and eax as the lower)

        //Doing 2 div to hopefully avoid an issue with dependence.
        asm volatile(
            "idivl  %[aDivBy]\n\t"
            : "=r" (clobberResult0), "=r" (clobberResult1)
            : [aInit0] "r" (initVal0), [aInit1] "r" (initVal1), [aDivBy] "r" (divBy)
            : 
        );
    }
}

void kernel_only_asm_div_i32_regRename()
{ 
    int32_t divBy = 1;    //Avoid div by 0 which causes div exception
    int32_t initVal0 = 1;
    int32_t initVal1 = 0;

    for(int i = 0; i<STIM_LEN; i++)
    {
        //(eax <- quotient, edx <- remainder) <- edx:eax/reg16 (the 1st arg is formed by treating edx as the upper sig bits and eax as the lower)

        //This version attempts to rename registers to avoid conflicts on dual dispatch.  Dependence is both WAW and RAW.  Need to move result into different reg to resolve WAW.  Need to move operand to resolve RAW.

        asm volatile(
            "movl %[aInit0], %%eax\n\t"
            "movl %[aInit1], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            :
            : [aInit0] "r" (initVal0), [aInit1] "r" (initVal1), [aDivBy] "r" (divBy)
            : "eax", "edx", "esi", "edi"
        );
    }
}

//==========64 bit div==========
void kernel_only_asm_div_i64()
{
    int64_t divBy = 1;    //Avoid div by 0 which causes div exception
    register int64_t initVal0 asm ("rax") = 1; //https://gcc.gnu.org/onlinedocs/gcc/Local-Register-Variables.html#Local-Register-Variables
    register int64_t initVal1 asm ("rdx") = 0; 
                                              
    register int64_t clobberResult0 asm ("rax"); //Need to specify an output since inputs cannot occur in clobber list
    register int64_t clobberResult1 asm ("rdx");

    for(int i = 0; i<STIM_LEN; i++)
    {
        //Doing 2 adds to hopefully avoid an issue with dependence.  Intel ASM is reg1=reg1+reg2.  It is not a three register instruction like it is with the vector unit
        //TODO: Verify
        asm volatile(
            "idivq  %[aDivBy]\n\t"
            : "=r" (clobberResult0), "=r" (clobberResult1)
            : [aInit0] "r" (initVal0), [aInit1] "r" (initVal1), [aDivBy] "r" (divBy)
            : 
        );
    }
}

void kernel_only_asm_div_i64_regRename()
{
    int64_t divBy = 1;    //Avoid div by 0 which causes div exception
    int64_t initVal0 = 1;
    int64_t initVal1 = 0;

    for(int i = 0; i<STIM_LEN; i++)
    {
        //(rax <- quotient, rdx <- remainder) <- rdx:rax/reg16 (the 1st arg is formed by treating edx as the upper sig bits and eax as the lower)

        //This version attempts to rename registers to avoid conflicts on dual dispatch.  Dependence is both WAW and RAW.  Need to move result into different reg to resolve WAW.  Need to move operand to resolve RAW.    
        asm volatile(
            "movq %[aInit0], %%rax\n\t"
            "movq %[aInit1], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            :
            : [aInit0] "r" (initVal0), [aInit1] "r" (initVal1), [aDivBy] "r" (divBy)
            : "rax", "rdx", "rsi", "rdi"
        );
    }
}

//==========div floating point==========
void kernel_only_asm_div_fp()
{
    for(int i = 0; i<STIM_LEN; i++)
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
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            :
            :
            :"st(1)", "st"
        );
    }
}

//==========div single with SSE==========
void kernel_only_asm_div_sp()
{
    float divBy = 1.0;    //Avoid div by 0 which causes div exception
    float initVal = 1.0;

    for(int i = 0; i<STIM_LEN; i++)
    {
        //This is using the SSE vector unit to multiply a single number (avoiding x87).
        //It was suggested in the GCC manual that the SSE instructions are used instead of x07
        
        //TODO: Verify
        asm volatile(
            "movd %[aInit], %%xmm0 \n\t"
            "movd %[aDivBy], %%xmm1 \n\t"
            "divss %%xmm1, %%xmm0 \n\t"
            "movd %%xmm0, %%ecx \n\t"
            :
            : [aInit] "r" (initVal), [aDivBy] "r" (divBy)
            : "ecx", "xmm0", "xmm1"
        );
    }
}

//==========div double with SSE==========
void kernel_only_asm_div_dp()
{
    double divBy = 1.0;    //Avoid div by 0 which causes div exception
    double initVal = 1.0;

    for(int i = 0; i<STIM_LEN; i++)
    {
        //This is using the SSE vector unit to multiply a single number (avoiding x87).
        //It was suggested in the GCC manual that the SSE instructions are used instead of x07
        
        //TODO: Verify
        asm volatile(
            "movq %[aInit], %%xmm0 \n\t"
            "movq %[aDivBy], %%xmm1 \n\t"
            "divsd %%xmm1, %%xmm0 \n\t"
            "movq %%xmm0, %%rcx \n\t"
            :
            : [aInit] "r" (initVal), [aDivBy] "r" (divBy)
            : "rcx", "xmm0", "xmm1"
        );
    }
}

#include "intrin_bench_default_defines_and_imports_cpp.h"

//See https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/x64-architecture
//https://docs.oracle.com/cd/E18752_01/html/817-5477/eoizy.html
//http://flint.cs.yale.edu/cs421/papers/x86-asm/asm.html

//==========8 bit mult==========
void kernel_only_asm_mult_i8_unroll120()
{
    for(int i = 0; i<STIM_LEN/(4*30); i++)
    {
        //TODO: Verify

        //The byte multiply does not have a 2 arg version
        //One arg is the al register and it writes back into the ax register: ax <- ar * reg8
        //NOTE: tests showed that this had half the throughput of the other integer multiplies.  Suggests dependence stopped multiple dispatch.
        asm volatile(
            //1
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //2
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //3
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //4
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //5
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //6
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //7
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //8
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //9
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //10
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //11
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //12
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //13
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //14
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //15
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //16
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //17
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //18
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //19
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //20
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //21
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //22
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //23
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //24
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //25
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //26
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //27
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //28
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //29
            "imulb  %%bl\n\t"
            "imulb  %%dl\n\t"
            "imulb  %%bh\n\t"
            "imulb  %%dh\n\t"
            //30
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
void kernel_only_asm_mult_i16_unroll120()
{
    for(int i = 0; i<STIM_LEN/(4*30); i++)
    {
        //Doing 2 adds to hopefully avoid an issue with dependence.  Intel ASM is reg1=reg1+reg2.  It is not a three register instruction like it is with the vector unit
        //TODO: Verify
        asm volatile(
            //1
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //2
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //3
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //4
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //5
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //6
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //7
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //8
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //9
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //10
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //11
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //12
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //13
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //14
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //15
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //16
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //17
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //18
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //19
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //20
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //21
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //22
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //23
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //24
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //25
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //26
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //27
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //28
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //29
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"
            //30
            "imulw  %%ax, %%bx\n\t"
            "imulw  %%cx, %%dx\n\t"
            "imulw  %%si, %%di\n\t"
            "imulw  %%r8w, %%r9w\n\t"

            :
            :
            : "ax", "bx", "cx", "dx", "si", "di", "r8", "r9"
        );
    }
}

//==========32 bit mult==========
void kernel_only_asm_mult_i32_unroll120()
{ 
    for(int i = 0; i<STIM_LEN/(4*30); i++)
    {
        //Doing 2 adds to hopefully avoid an issue with dependence.  Intel ASM is reg1=reg1+reg2.  It is not a three register instruction like it is with the vector unit
        //TODO: Verify
        asm volatile(
            //1
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //2
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //3
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //4
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //5
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //6
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //7
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //8
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //9
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //10
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //11
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //12
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //13
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //14
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //15
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //16
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //17
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //18
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //19
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //20
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //21
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //22
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //23
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //24
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //25
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //26
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //27
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //28
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //29
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"
            //30
            "imull  %%eax, %%ebx\n\t"
            "imull  %%ecx, %%edx\n\t"
            "imull  %%esi, %%edi\n\t"
            "imull  %%r8d, %%r9d\n\t"

            :
            :
            : "eax", "ebx", "ecx", "edx", "esi", "edi", "r8", "r9"
        );
    }
}

//==========64 bit mult==========
void kernel_only_asm_mult_i64_unroll120()
{
    for(int i = 0; i<STIM_LEN/(4*30); i++)
    {
        //Doing 2 adds to hopefully avoid an issue with dependence.  Intel ASM is reg1=reg1+reg2.  It is not a three register instruction like it is with the vector unit
        //TODO: Verify
        asm volatile(
            //1
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //2
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //3
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //4
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //5
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //6
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //7
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //8
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //9
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //10
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //11
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //12
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //13
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //14
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //15
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //16
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //17
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //18
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //19
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //20
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //21
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //22
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //23
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //24
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //25
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //26
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //27
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //28
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //29
            "imulq  %%rax, %%rbx\n\t"
            "imulq  %%rcx, %%rdx\n\t"
            "imulq  %%rsi, %%rdi\n\t"
            "imulq  %%r8,  %%r9\n\t"
            //30
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
void kernel_only_asm_mult_fp_unroll120()
{
    for(int i = 0; i<STIM_LEN/(4*30); i++)
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
            //1
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
            //2
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
            //3
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
            //4
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
            //5
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
            //6
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
            //7
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
            //8
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
            //9
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
            //10
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
            //11
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
            //12
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
            //13
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
            //14
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
            //15
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
            //16
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
            //17
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
            //18
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
            //19
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
            //20
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
            //21
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
            //22
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
            //23
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
            //24
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
            //25
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
            //26
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
            //27
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
            //28
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
            //29
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
            //30
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
void kernel_only_asm_mult_sp_unroll120()
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

    for(int i = 0; i<STIM_LEN/(4*30); i++)
    {
        //This is using the SSE vector unit to multiply a single number (avoiding x87).
        //It was suggested in the GCC manual that the SSE instructions are used instead of x07
        
        //TODO: Verify
        asm volatile(
            //1
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //2
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //3
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //4
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //5
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //6
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //7
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //8
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //9
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //10
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //11
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //12
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //13
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //14
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //15
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //16
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //17
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //18
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //19
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //20
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //21
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //22
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //23
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //24
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //25
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //26
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //27
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //28
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //29
            "vmulss %4, %5, %0 \n\t"
            "vmulss %6, %7, %1 \n\t"
            "vmulss %8, %9, %2 \n\t"
            "vmulss %10, %11, %3 \n\t"
            //30
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
void kernel_only_asm_mult_dp_unroll120()
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

    for(int i = 0; i<STIM_LEN/(4*30); i++)
    {
        //This is using the SSE vector unit to multiply a single number (avoiding x87).
        //It was suggested in the GCC manual that the SSE instructions are used instead of x07
        
        //TODO: Verify
        asm volatile(
            //1
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //2
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //3
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //4
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //5
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //6
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //7
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //8
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //9
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //10
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //11
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //12
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //13
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //14
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //15
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //16
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //17
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //18
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //19
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //20
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //21
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //22
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //23
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //24
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //25
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //26
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //27
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //28
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //29
            "vmulsd %4, %5, %0 \n\t"
            "vmulsd %6, %7, %1 \n\t"
            "vmulsd %8, %9, %2 \n\t"
            "vmulsd %10, %11, %3 \n\t"
            //30
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

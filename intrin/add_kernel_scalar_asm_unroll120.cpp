#include "intrin_bench_default_defines_and_imports_cpp.h"

//See https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/x64-architecture
//https://docs.oracle.com/cd/E18752_01/html/817-5477/eoizy.html
//http://flint.cs.yale.edu/cs421/papers/x86-asm/asm.html

//==========8 bit add==========
void kernel_only_asm_add_i8_unroll120()
{
    for(int i = 0; i<STIM_LEN/120; i++)
    {
        //Doing 2 adds to hopefully avoid an issue with dependence.  Intel ASM is reg1=reg1+reg2.  It is not a three register instruction like it is with the vector unit
        //TODO: Verify
        asm volatile(
            //1
            "add  %%al, %%bl\n\t"
            "add  %%cl, %%dl\n\t"
            "add  %%r8b, %%r9b\n\t"
            "add  %%r10b, %%r11b\n\t"
            "add  %%r12b, %%r13b\n\t"
            "add  %%r14b, %%r15b\n\t"
            //2
            "add  %%al, %%bl\n\t"
            "add  %%cl, %%dl\n\t"
            "add  %%r8b, %%r9b\n\t"
            "add  %%r10b, %%r11b\n\t"
            "add  %%r12b, %%r13b\n\t"
            "add  %%r14b, %%r15b\n\t"
            //3
            "add  %%al, %%bl\n\t"
            "add  %%cl, %%dl\n\t"
            "add  %%r8b, %%r9b\n\t"
            "add  %%r10b, %%r11b\n\t"
            "add  %%r12b, %%r13b\n\t"
            "add  %%r14b, %%r15b\n\t"
            //4
            "add  %%al, %%bl\n\t"
            "add  %%cl, %%dl\n\t"
            "add  %%r8b, %%r9b\n\t"
            "add  %%r10b, %%r11b\n\t"
            "add  %%r12b, %%r13b\n\t"
            "add  %%r14b, %%r15b\n\t"
            //5
            "add  %%al, %%bl\n\t"
            "add  %%cl, %%dl\n\t"
            "add  %%r8b, %%r9b\n\t"
            "add  %%r10b, %%r11b\n\t"
            "add  %%r12b, %%r13b\n\t"
            "add  %%r14b, %%r15b\n\t"
            //6
            "add  %%al, %%bl\n\t"
            "add  %%cl, %%dl\n\t"
            "add  %%r8b, %%r9b\n\t"
            "add  %%r10b, %%r11b\n\t"
            "add  %%r12b, %%r13b\n\t"
            "add  %%r14b, %%r15b\n\t"
            //7
            "add  %%al, %%bl\n\t"
            "add  %%cl, %%dl\n\t"
            "add  %%r8b, %%r9b\n\t"
            "add  %%r10b, %%r11b\n\t"
            "add  %%r12b, %%r13b\n\t"
            "add  %%r14b, %%r15b\n\t"
            //8
            "add  %%al, %%bl\n\t"
            "add  %%cl, %%dl\n\t"
            "add  %%r8b, %%r9b\n\t"
            "add  %%r10b, %%r11b\n\t"
            "add  %%r12b, %%r13b\n\t"
            "add  %%r14b, %%r15b\n\t"
            //9
            "add  %%al, %%bl\n\t"
            "add  %%cl, %%dl\n\t"
            "add  %%r8b, %%r9b\n\t"
            "add  %%r10b, %%r11b\n\t"
            "add  %%r12b, %%r13b\n\t"
            "add  %%r14b, %%r15b\n\t"
            //10
            "add  %%al, %%bl\n\t"
            "add  %%cl, %%dl\n\t"
            "add  %%r8b, %%r9b\n\t"
            "add  %%r10b, %%r11b\n\t"
            "add  %%r12b, %%r13b\n\t"
            "add  %%r14b, %%r15b\n\t"
            //11
            "add  %%al, %%bl\n\t"
            "add  %%cl, %%dl\n\t"
            "add  %%r8b, %%r9b\n\t"
            "add  %%r10b, %%r11b\n\t"
            "add  %%r12b, %%r13b\n\t"
            "add  %%r14b, %%r15b\n\t"
            //12
            "add  %%al, %%bl\n\t"
            "add  %%cl, %%dl\n\t"
            "add  %%r8b, %%r9b\n\t"
            "add  %%r10b, %%r11b\n\t"
            "add  %%r12b, %%r13b\n\t"
            "add  %%r14b, %%r15b\n\t"
            //13
            "add  %%al, %%bl\n\t"
            "add  %%cl, %%dl\n\t"
            "add  %%r8b, %%r9b\n\t"
            "add  %%r10b, %%r11b\n\t"
            "add  %%r12b, %%r13b\n\t"
            "add  %%r14b, %%r15b\n\t"
            //14
            "add  %%al, %%bl\n\t"
            "add  %%cl, %%dl\n\t"
            "add  %%r8b, %%r9b\n\t"
            "add  %%r10b, %%r11b\n\t"
            "add  %%r12b, %%r13b\n\t"
            "add  %%r14b, %%r15b\n\t"
            //15
            "add  %%al, %%bl\n\t"
            "add  %%cl, %%dl\n\t"
            "add  %%r8b, %%r9b\n\t"
            "add  %%r10b, %%r11b\n\t"
            "add  %%r12b, %%r13b\n\t"
            "add  %%r14b, %%r15b\n\t"
            //16
            "add  %%al, %%bl\n\t"
            "add  %%cl, %%dl\n\t"
            "add  %%r8b, %%r9b\n\t"
            "add  %%r10b, %%r11b\n\t"
            "add  %%r12b, %%r13b\n\t"
            "add  %%r14b, %%r15b\n\t"
            //17
            "add  %%al, %%bl\n\t"
            "add  %%cl, %%dl\n\t"
            "add  %%r8b, %%r9b\n\t"
            "add  %%r10b, %%r11b\n\t"
            "add  %%r12b, %%r13b\n\t"
            "add  %%r14b, %%r15b\n\t"
            //18
            "add  %%al, %%bl\n\t"
            "add  %%cl, %%dl\n\t"
            "add  %%r8b, %%r9b\n\t"
            "add  %%r10b, %%r11b\n\t"
            "add  %%r12b, %%r13b\n\t"
            "add  %%r14b, %%r15b\n\t"
            //19
            "add  %%al, %%bl\n\t"
            "add  %%cl, %%dl\n\t"
            "add  %%r8b, %%r9b\n\t"
            "add  %%r10b, %%r11b\n\t"
            "add  %%r12b, %%r13b\n\t"
            "add  %%r14b, %%r15b\n\t"
            //20
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
void kernel_only_asm_add_i16_unroll120()
{
    for(int i = 0; i<STIM_LEN/120; i++)
    {
        //Doing 2 adds to hopefully avoid an issue with dependence.  Intel ASM is reg1=reg1+reg2.  It is not a three register instruction like it is with the vector unit
        //TODO: Verify
        asm volatile(
            //1
            "add  %%ax, %%bx\n\t"
            "add  %%cx, %%dx\n\t"
            "add  %%r8w, %%r9w\n\t"
            "add  %%r10w, %%r11w\n\t"
            "add  %%r12w, %%r13w\n\t"
            "add  %%r14w, %%r15w\n\t"
            //2
            "add  %%ax, %%bx\n\t"
            "add  %%cx, %%dx\n\t"
            "add  %%r8w, %%r9w\n\t"
            "add  %%r10w, %%r11w\n\t"
            "add  %%r12w, %%r13w\n\t"
            "add  %%r14w, %%r15w\n\t"
            //3
            "add  %%ax, %%bx\n\t"
            "add  %%cx, %%dx\n\t"
            "add  %%r8w, %%r9w\n\t"
            "add  %%r10w, %%r11w\n\t"
            "add  %%r12w, %%r13w\n\t"
            "add  %%r14w, %%r15w\n\t"
            //4
            "add  %%ax, %%bx\n\t"
            "add  %%cx, %%dx\n\t"
            "add  %%r8w, %%r9w\n\t"
            "add  %%r10w, %%r11w\n\t"
            "add  %%r12w, %%r13w\n\t"
            "add  %%r14w, %%r15w\n\t"
            //5
            "add  %%ax, %%bx\n\t"
            "add  %%cx, %%dx\n\t"
            "add  %%r8w, %%r9w\n\t"
            "add  %%r10w, %%r11w\n\t"
            "add  %%r12w, %%r13w\n\t"
            "add  %%r14w, %%r15w\n\t"
            //6
            "add  %%ax, %%bx\n\t"
            "add  %%cx, %%dx\n\t"
            "add  %%r8w, %%r9w\n\t"
            "add  %%r10w, %%r11w\n\t"
            "add  %%r12w, %%r13w\n\t"
            "add  %%r14w, %%r15w\n\t"
            //7
            "add  %%ax, %%bx\n\t"
            "add  %%cx, %%dx\n\t"
            "add  %%r8w, %%r9w\n\t"
            "add  %%r10w, %%r11w\n\t"
            "add  %%r12w, %%r13w\n\t"
            "add  %%r14w, %%r15w\n\t"
            //8
            "add  %%ax, %%bx\n\t"
            "add  %%cx, %%dx\n\t"
            "add  %%r8w, %%r9w\n\t"
            "add  %%r10w, %%r11w\n\t"
            "add  %%r12w, %%r13w\n\t"
            "add  %%r14w, %%r15w\n\t"
            //9
            "add  %%ax, %%bx\n\t"
            "add  %%cx, %%dx\n\t"
            "add  %%r8w, %%r9w\n\t"
            "add  %%r10w, %%r11w\n\t"
            "add  %%r12w, %%r13w\n\t"
            "add  %%r14w, %%r15w\n\t"
            //10
            "add  %%ax, %%bx\n\t"
            "add  %%cx, %%dx\n\t"
            "add  %%r8w, %%r9w\n\t"
            "add  %%r10w, %%r11w\n\t"
            "add  %%r12w, %%r13w\n\t"
            "add  %%r14w, %%r15w\n\t"
            //11
            "add  %%ax, %%bx\n\t"
            "add  %%cx, %%dx\n\t"
            "add  %%r8w, %%r9w\n\t"
            "add  %%r10w, %%r11w\n\t"
            "add  %%r12w, %%r13w\n\t"
            "add  %%r14w, %%r15w\n\t"
            //12
            "add  %%ax, %%bx\n\t"
            "add  %%cx, %%dx\n\t"
            "add  %%r8w, %%r9w\n\t"
            "add  %%r10w, %%r11w\n\t"
            "add  %%r12w, %%r13w\n\t"
            "add  %%r14w, %%r15w\n\t"
            //13
            "add  %%ax, %%bx\n\t"
            "add  %%cx, %%dx\n\t"
            "add  %%r8w, %%r9w\n\t"
            "add  %%r10w, %%r11w\n\t"
            "add  %%r12w, %%r13w\n\t"
            "add  %%r14w, %%r15w\n\t"
            //14
            "add  %%ax, %%bx\n\t"
            "add  %%cx, %%dx\n\t"
            "add  %%r8w, %%r9w\n\t"
            "add  %%r10w, %%r11w\n\t"
            "add  %%r12w, %%r13w\n\t"
            "add  %%r14w, %%r15w\n\t"
            //15
            "add  %%ax, %%bx\n\t"
            "add  %%cx, %%dx\n\t"
            "add  %%r8w, %%r9w\n\t"
            "add  %%r10w, %%r11w\n\t"
            "add  %%r12w, %%r13w\n\t"
            "add  %%r14w, %%r15w\n\t"
            //16
            "add  %%ax, %%bx\n\t"
            "add  %%cx, %%dx\n\t"
            "add  %%r8w, %%r9w\n\t"
            "add  %%r10w, %%r11w\n\t"
            "add  %%r12w, %%r13w\n\t"
            "add  %%r14w, %%r15w\n\t"
            //17
            "add  %%ax, %%bx\n\t"
            "add  %%cx, %%dx\n\t"
            "add  %%r8w, %%r9w\n\t"
            "add  %%r10w, %%r11w\n\t"
            "add  %%r12w, %%r13w\n\t"
            "add  %%r14w, %%r15w\n\t"
            //18
            "add  %%ax, %%bx\n\t"
            "add  %%cx, %%dx\n\t"
            "add  %%r8w, %%r9w\n\t"
            "add  %%r10w, %%r11w\n\t"
            "add  %%r12w, %%r13w\n\t"
            "add  %%r14w, %%r15w\n\t"
            //19
            "add  %%ax, %%bx\n\t"
            "add  %%cx, %%dx\n\t"
            "add  %%r8w, %%r9w\n\t"
            "add  %%r10w, %%r11w\n\t"
            "add  %%r12w, %%r13w\n\t"
            "add  %%r14w, %%r15w\n\t"
            //20
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
void kernel_only_asm_add_i32_unroll120()
{ 
    for(int i = 0; i<STIM_LEN/120; i++)
    {
        //Doing 2 adds to hopefully avoid an issue with dependence.  Intel ASM is reg1=reg1+reg2.  It is not a three register instruction like it is with the vector unit
        //TODO: Verify
        asm volatile(
            //1
            "add  %%eax, %%ebx\n\t"
            "add  %%ecx, %%edx\n\t"
            "add  %%r8d, %%r9d\n\t"
            "add  %%r10d, %%r11d\n\t"
            "add  %%r12d, %%r13d\n\t"
            "add  %%r14d, %%r15d\n\t"
            //2
            "add  %%eax, %%ebx\n\t"
            "add  %%ecx, %%edx\n\t"
            "add  %%r8d, %%r9d\n\t"
            "add  %%r10d, %%r11d\n\t"
            "add  %%r12d, %%r13d\n\t"
            "add  %%r14d, %%r15d\n\t"
            //3
            "add  %%eax, %%ebx\n\t"
            "add  %%ecx, %%edx\n\t"
            "add  %%r8d, %%r9d\n\t"
            "add  %%r10d, %%r11d\n\t"
            "add  %%r12d, %%r13d\n\t"
            "add  %%r14d, %%r15d\n\t"
            //4
            "add  %%eax, %%ebx\n\t"
            "add  %%ecx, %%edx\n\t"
            "add  %%r8d, %%r9d\n\t"
            "add  %%r10d, %%r11d\n\t"
            "add  %%r12d, %%r13d\n\t"
            "add  %%r14d, %%r15d\n\t"
            //5
            "add  %%eax, %%ebx\n\t"
            "add  %%ecx, %%edx\n\t"
            "add  %%r8d, %%r9d\n\t"
            "add  %%r10d, %%r11d\n\t"
            "add  %%r12d, %%r13d\n\t"
            "add  %%r14d, %%r15d\n\t"
            //6
            "add  %%eax, %%ebx\n\t"
            "add  %%ecx, %%edx\n\t"
            "add  %%r8d, %%r9d\n\t"
            "add  %%r10d, %%r11d\n\t"
            "add  %%r12d, %%r13d\n\t"
            "add  %%r14d, %%r15d\n\t"
            //7
            "add  %%eax, %%ebx\n\t"
            "add  %%ecx, %%edx\n\t"
            "add  %%r8d, %%r9d\n\t"
            "add  %%r10d, %%r11d\n\t"
            "add  %%r12d, %%r13d\n\t"
            "add  %%r14d, %%r15d\n\t"
            //8
            "add  %%eax, %%ebx\n\t"
            "add  %%ecx, %%edx\n\t"
            "add  %%r8d, %%r9d\n\t"
            "add  %%r10d, %%r11d\n\t"
            "add  %%r12d, %%r13d\n\t"
            "add  %%r14d, %%r15d\n\t"
            //9
            "add  %%eax, %%ebx\n\t"
            "add  %%ecx, %%edx\n\t"
            "add  %%r8d, %%r9d\n\t"
            "add  %%r10d, %%r11d\n\t"
            "add  %%r12d, %%r13d\n\t"
            "add  %%r14d, %%r15d\n\t"
            //10
            "add  %%eax, %%ebx\n\t"
            "add  %%ecx, %%edx\n\t"
            "add  %%r8d, %%r9d\n\t"
            "add  %%r10d, %%r11d\n\t"
            "add  %%r12d, %%r13d\n\t"
            "add  %%r14d, %%r15d\n\t"
            //11
            "add  %%eax, %%ebx\n\t"
            "add  %%ecx, %%edx\n\t"
            "add  %%r8d, %%r9d\n\t"
            "add  %%r10d, %%r11d\n\t"
            "add  %%r12d, %%r13d\n\t"
            "add  %%r14d, %%r15d\n\t"
            //12
            "add  %%eax, %%ebx\n\t"
            "add  %%ecx, %%edx\n\t"
            "add  %%r8d, %%r9d\n\t"
            "add  %%r10d, %%r11d\n\t"
            "add  %%r12d, %%r13d\n\t"
            "add  %%r14d, %%r15d\n\t"
            //13
            "add  %%eax, %%ebx\n\t"
            "add  %%ecx, %%edx\n\t"
            "add  %%r8d, %%r9d\n\t"
            "add  %%r10d, %%r11d\n\t"
            "add  %%r12d, %%r13d\n\t"
            "add  %%r14d, %%r15d\n\t"
            //14
            "add  %%eax, %%ebx\n\t"
            "add  %%ecx, %%edx\n\t"
            "add  %%r8d, %%r9d\n\t"
            "add  %%r10d, %%r11d\n\t"
            "add  %%r12d, %%r13d\n\t"
            "add  %%r14d, %%r15d\n\t"
            //15
            "add  %%eax, %%ebx\n\t"
            "add  %%ecx, %%edx\n\t"
            "add  %%r8d, %%r9d\n\t"
            "add  %%r10d, %%r11d\n\t"
            "add  %%r12d, %%r13d\n\t"
            "add  %%r14d, %%r15d\n\t"
            //16
            "add  %%eax, %%ebx\n\t"
            "add  %%ecx, %%edx\n\t"
            "add  %%r8d, %%r9d\n\t"
            "add  %%r10d, %%r11d\n\t"
            "add  %%r12d, %%r13d\n\t"
            "add  %%r14d, %%r15d\n\t"
            //17
            "add  %%eax, %%ebx\n\t"
            "add  %%ecx, %%edx\n\t"
            "add  %%r8d, %%r9d\n\t"
            "add  %%r10d, %%r11d\n\t"
            "add  %%r12d, %%r13d\n\t"
            "add  %%r14d, %%r15d\n\t"
            //18
            "add  %%eax, %%ebx\n\t"
            "add  %%ecx, %%edx\n\t"
            "add  %%r8d, %%r9d\n\t"
            "add  %%r10d, %%r11d\n\t"
            "add  %%r12d, %%r13d\n\t"
            "add  %%r14d, %%r15d\n\t"
            //19
            "add  %%eax, %%ebx\n\t"
            "add  %%ecx, %%edx\n\t"
            "add  %%r8d, %%r9d\n\t"
            "add  %%r10d, %%r11d\n\t"
            "add  %%r12d, %%r13d\n\t"
            "add  %%r14d, %%r15d\n\t"
            //20
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
void kernel_only_asm_add_i64_unroll120()
{
    for(int i = 0; i<STIM_LEN/120; i++)
    {
        //Doing 2 adds to hopefully avoid an issue with dependence.  Intel ASM is reg1=reg1+reg2.  It is not a three register instruction like it is with the vector unit
        //TODO: Verify
        asm volatile(
            //1
            "add  %%rax, %%rbx\n\t"
            "add  %%rcx, %%rdx\n\t"
            "add  %%r8, %%r9\n\t"
            "add  %%r10, %%r11\n\t"
            "add  %%r12, %%r13\n\t"
            "add  %%r14, %%r15\n\t"
            //2
            "add  %%rax, %%rbx\n\t"
            "add  %%rcx, %%rdx\n\t"
            "add  %%r8, %%r9\n\t"
            "add  %%r10, %%r11\n\t"
            "add  %%r12, %%r13\n\t"
            "add  %%r14, %%r15\n\t"
            //3
            "add  %%rax, %%rbx\n\t"
            "add  %%rcx, %%rdx\n\t"
            "add  %%r8, %%r9\n\t"
            "add  %%r10, %%r11\n\t"
            "add  %%r12, %%r13\n\t"
            "add  %%r14, %%r15\n\t"
            //4
            "add  %%rax, %%rbx\n\t"
            "add  %%rcx, %%rdx\n\t"
            "add  %%r8, %%r9\n\t"
            "add  %%r10, %%r11\n\t"
            "add  %%r12, %%r13\n\t"
            "add  %%r14, %%r15\n\t"
            //5
            "add  %%rax, %%rbx\n\t"
            "add  %%rcx, %%rdx\n\t"
            "add  %%r8, %%r9\n\t"
            "add  %%r10, %%r11\n\t"
            "add  %%r12, %%r13\n\t"
            "add  %%r14, %%r15\n\t"
            //6
            "add  %%rax, %%rbx\n\t"
            "add  %%rcx, %%rdx\n\t"
            "add  %%r8, %%r9\n\t"
            "add  %%r10, %%r11\n\t"
            "add  %%r12, %%r13\n\t"
            "add  %%r14, %%r15\n\t"
            //7
            "add  %%rax, %%rbx\n\t"
            "add  %%rcx, %%rdx\n\t"
            "add  %%r8, %%r9\n\t"
            "add  %%r10, %%r11\n\t"
            "add  %%r12, %%r13\n\t"
            "add  %%r14, %%r15\n\t"
            //8
            "add  %%rax, %%rbx\n\t"
            "add  %%rcx, %%rdx\n\t"
            "add  %%r8, %%r9\n\t"
            "add  %%r10, %%r11\n\t"
            "add  %%r12, %%r13\n\t"
            "add  %%r14, %%r15\n\t"
            //9
            "add  %%rax, %%rbx\n\t"
            "add  %%rcx, %%rdx\n\t"
            "add  %%r8, %%r9\n\t"
            "add  %%r10, %%r11\n\t"
            "add  %%r12, %%r13\n\t"
            "add  %%r14, %%r15\n\t"
            //10
            "add  %%rax, %%rbx\n\t"
            "add  %%rcx, %%rdx\n\t"
            "add  %%r8, %%r9\n\t"
            "add  %%r10, %%r11\n\t"
            "add  %%r12, %%r13\n\t"
            "add  %%r14, %%r15\n\t"
            //11
            "add  %%rax, %%rbx\n\t"
            "add  %%rcx, %%rdx\n\t"
            "add  %%r8, %%r9\n\t"
            "add  %%r10, %%r11\n\t"
            "add  %%r12, %%r13\n\t"
            "add  %%r14, %%r15\n\t"
            //12
            "add  %%rax, %%rbx\n\t"
            "add  %%rcx, %%rdx\n\t"
            "add  %%r8, %%r9\n\t"
            "add  %%r10, %%r11\n\t"
            "add  %%r12, %%r13\n\t"
            "add  %%r14, %%r15\n\t"
            //13
            "add  %%rax, %%rbx\n\t"
            "add  %%rcx, %%rdx\n\t"
            "add  %%r8, %%r9\n\t"
            "add  %%r10, %%r11\n\t"
            "add  %%r12, %%r13\n\t"
            "add  %%r14, %%r15\n\t"
            //14
            "add  %%rax, %%rbx\n\t"
            "add  %%rcx, %%rdx\n\t"
            "add  %%r8, %%r9\n\t"
            "add  %%r10, %%r11\n\t"
            "add  %%r12, %%r13\n\t"
            "add  %%r14, %%r15\n\t"
            //15
            "add  %%rax, %%rbx\n\t"
            "add  %%rcx, %%rdx\n\t"
            "add  %%r8, %%r9\n\t"
            "add  %%r10, %%r11\n\t"
            "add  %%r12, %%r13\n\t"
            "add  %%r14, %%r15\n\t"
            //16
            "add  %%rax, %%rbx\n\t"
            "add  %%rcx, %%rdx\n\t"
            "add  %%r8, %%r9\n\t"
            "add  %%r10, %%r11\n\t"
            "add  %%r12, %%r13\n\t"
            "add  %%r14, %%r15\n\t"
            //17
            "add  %%rax, %%rbx\n\t"
            "add  %%rcx, %%rdx\n\t"
            "add  %%r8, %%r9\n\t"
            "add  %%r10, %%r11\n\t"
            "add  %%r12, %%r13\n\t"
            "add  %%r14, %%r15\n\t"
            //18
            "add  %%rax, %%rbx\n\t"
            "add  %%rcx, %%rdx\n\t"
            "add  %%r8, %%r9\n\t"
            "add  %%r10, %%r11\n\t"
            "add  %%r12, %%r13\n\t"
            "add  %%r14, %%r15\n\t"
            //19
            "add  %%rax, %%rbx\n\t"
            "add  %%rcx, %%rdx\n\t"
            "add  %%r8, %%r9\n\t"
            "add  %%r10, %%r11\n\t"
            "add  %%r12, %%r13\n\t"
            "add  %%r14, %%r15\n\t"
            //20
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
void kernel_only_asm_add_fp_unroll120()
{
    for(int i = 0; i<STIM_LEN/120; i++)
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
            //2
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
            //3
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
            //4
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
            //5
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
            //6
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
            //7
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
            //8
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
            //9
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
            //10
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
            //11
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
            //12
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
            //13
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
            //14
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
            //15
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
            //16
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
            //17
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
            //18
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
            //19
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
            //20
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
void kernel_only_asm_add_sp_unroll120()
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

    for(int i = 0; i<STIM_LEN/120; i++)
    {
        //This is using the SSE vector unit to multiply a single number (avoiding x87).
        //It was suggested in the GCC manual that the SSE instructions are used instead of x07
        
        //TODO: Verify
        asm volatile(
            //1
            "vaddss %[a], %[b], %[c] \n\t"
            "vaddss %[d], %[e], %[f] \n\t"
            "vaddss %[g], %[h], %[j] \n\t"
            "vaddss %[k], %[l], %[m] \n\t"
            "vaddss %[n], %[o], %[p] \n\t"
            "vaddss %[q], %[r], %[s] \n\t"
            //2
            "vaddss %[a], %[b], %[c] \n\t"
            "vaddss %[d], %[e], %[f] \n\t"
            "vaddss %[g], %[h], %[j] \n\t"
            "vaddss %[k], %[l], %[m] \n\t"
            "vaddss %[n], %[o], %[p] \n\t"
            "vaddss %[q], %[r], %[s] \n\t"
            //3
            "vaddss %[a], %[b], %[c] \n\t"
            "vaddss %[d], %[e], %[f] \n\t"
            "vaddss %[g], %[h], %[j] \n\t"
            "vaddss %[k], %[l], %[m] \n\t"
            "vaddss %[n], %[o], %[p] \n\t"
            "vaddss %[q], %[r], %[s] \n\t"
            //4
            "vaddss %[a], %[b], %[c] \n\t"
            "vaddss %[d], %[e], %[f] \n\t"
            "vaddss %[g], %[h], %[j] \n\t"
            "vaddss %[k], %[l], %[m] \n\t"
            "vaddss %[n], %[o], %[p] \n\t"
            "vaddss %[q], %[r], %[s] \n\t"
            //5
            "vaddss %[a], %[b], %[c] \n\t"
            "vaddss %[d], %[e], %[f] \n\t"
            "vaddss %[g], %[h], %[j] \n\t"
            "vaddss %[k], %[l], %[m] \n\t"
            "vaddss %[n], %[o], %[p] \n\t"
            "vaddss %[q], %[r], %[s] \n\t"
            //6
            "vaddss %[a], %[b], %[c] \n\t"
            "vaddss %[d], %[e], %[f] \n\t"
            "vaddss %[g], %[h], %[j] \n\t"
            "vaddss %[k], %[l], %[m] \n\t"
            "vaddss %[n], %[o], %[p] \n\t"
            "vaddss %[q], %[r], %[s] \n\t"
            //7
            "vaddss %[a], %[b], %[c] \n\t"
            "vaddss %[d], %[e], %[f] \n\t"
            "vaddss %[g], %[h], %[j] \n\t"
            "vaddss %[k], %[l], %[m] \n\t"
            "vaddss %[n], %[o], %[p] \n\t"
            "vaddss %[q], %[r], %[s] \n\t"
            //8
            "vaddss %[a], %[b], %[c] \n\t"
            "vaddss %[d], %[e], %[f] \n\t"
            "vaddss %[g], %[h], %[j] \n\t"
            "vaddss %[k], %[l], %[m] \n\t"
            "vaddss %[n], %[o], %[p] \n\t"
            "vaddss %[q], %[r], %[s] \n\t"
            //9
            "vaddss %[a], %[b], %[c] \n\t"
            "vaddss %[d], %[e], %[f] \n\t"
            "vaddss %[g], %[h], %[j] \n\t"
            "vaddss %[k], %[l], %[m] \n\t"
            "vaddss %[n], %[o], %[p] \n\t"
            "vaddss %[q], %[r], %[s] \n\t"
            //10
            "vaddss %[a], %[b], %[c] \n\t"
            "vaddss %[d], %[e], %[f] \n\t"
            "vaddss %[g], %[h], %[j] \n\t"
            "vaddss %[k], %[l], %[m] \n\t"
            "vaddss %[n], %[o], %[p] \n\t"
            "vaddss %[q], %[r], %[s] \n\t"
            //11
            "vaddss %[a], %[b], %[c] \n\t"
            "vaddss %[d], %[e], %[f] \n\t"
            "vaddss %[g], %[h], %[j] \n\t"
            "vaddss %[k], %[l], %[m] \n\t"
            "vaddss %[n], %[o], %[p] \n\t"
            "vaddss %[q], %[r], %[s] \n\t"
            //12
            "vaddss %[a], %[b], %[c] \n\t"
            "vaddss %[d], %[e], %[f] \n\t"
            "vaddss %[g], %[h], %[j] \n\t"
            "vaddss %[k], %[l], %[m] \n\t"
            "vaddss %[n], %[o], %[p] \n\t"
            "vaddss %[q], %[r], %[s] \n\t"
            //13
            "vaddss %[a], %[b], %[c] \n\t"
            "vaddss %[d], %[e], %[f] \n\t"
            "vaddss %[g], %[h], %[j] \n\t"
            "vaddss %[k], %[l], %[m] \n\t"
            "vaddss %[n], %[o], %[p] \n\t"
            "vaddss %[q], %[r], %[s] \n\t"
            //14
            "vaddss %[a], %[b], %[c] \n\t"
            "vaddss %[d], %[e], %[f] \n\t"
            "vaddss %[g], %[h], %[j] \n\t"
            "vaddss %[k], %[l], %[m] \n\t"
            "vaddss %[n], %[o], %[p] \n\t"
            "vaddss %[q], %[r], %[s] \n\t"
            //15
            "vaddss %[a], %[b], %[c] \n\t"
            "vaddss %[d], %[e], %[f] \n\t"
            "vaddss %[g], %[h], %[j] \n\t"
            "vaddss %[k], %[l], %[m] \n\t"
            "vaddss %[n], %[o], %[p] \n\t"
            "vaddss %[q], %[r], %[s] \n\t"
            //16
            "vaddss %[a], %[b], %[c] \n\t"
            "vaddss %[d], %[e], %[f] \n\t"
            "vaddss %[g], %[h], %[j] \n\t"
            "vaddss %[k], %[l], %[m] \n\t"
            "vaddss %[n], %[o], %[p] \n\t"
            "vaddss %[q], %[r], %[s] \n\t"
            //17
            "vaddss %[a], %[b], %[c] \n\t"
            "vaddss %[d], %[e], %[f] \n\t"
            "vaddss %[g], %[h], %[j] \n\t"
            "vaddss %[k], %[l], %[m] \n\t"
            "vaddss %[n], %[o], %[p] \n\t"
            "vaddss %[q], %[r], %[s] \n\t"
            //18
            "vaddss %[a], %[b], %[c] \n\t"
            "vaddss %[d], %[e], %[f] \n\t"
            "vaddss %[g], %[h], %[j] \n\t"
            "vaddss %[k], %[l], %[m] \n\t"
            "vaddss %[n], %[o], %[p] \n\t"
            "vaddss %[q], %[r], %[s] \n\t"
            //19
            "vaddss %[a], %[b], %[c] \n\t"
            "vaddss %[d], %[e], %[f] \n\t"
            "vaddss %[g], %[h], %[j] \n\t"
            "vaddss %[k], %[l], %[m] \n\t"
            "vaddss %[n], %[o], %[p] \n\t"
            "vaddss %[q], %[r], %[s] \n\t"
            //20
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
void kernel_only_asm_add_dp_unroll120()
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

    for(int i = 0; i<STIM_LEN/120; i++)
    {
        //This is using the SSE vector unit to multiply a single number (avoiding x87).
        //It was suggested in the GCC manual that the SSE instructions are used instead of x07
        
        //TODO: Verify
        asm volatile(
            //1
            "vaddsd %[a], %[b], %[c] \n\t"
            "vaddsd %[d], %[e], %[f] \n\t"
            "vaddsd %[g], %[h], %[j] \n\t"
            "vaddsd %[k], %[l], %[m] \n\t"
            "vaddsd %[n], %[o], %[p] \n\t"
            "vaddsd %[q], %[r], %[s] \n\t"
            //2
            "vaddsd %[a], %[b], %[c] \n\t"
            "vaddsd %[d], %[e], %[f] \n\t"
            "vaddsd %[g], %[h], %[j] \n\t"
            "vaddsd %[k], %[l], %[m] \n\t"
            "vaddsd %[n], %[o], %[p] \n\t"
            "vaddsd %[q], %[r], %[s] \n\t"
            //3
            "vaddsd %[a], %[b], %[c] \n\t"
            "vaddsd %[d], %[e], %[f] \n\t"
            "vaddsd %[g], %[h], %[j] \n\t"
            "vaddsd %[k], %[l], %[m] \n\t"
            "vaddsd %[n], %[o], %[p] \n\t"
            "vaddsd %[q], %[r], %[s] \n\t"
            //4
            "vaddsd %[a], %[b], %[c] \n\t"
            "vaddsd %[d], %[e], %[f] \n\t"
            "vaddsd %[g], %[h], %[j] \n\t"
            "vaddsd %[k], %[l], %[m] \n\t"
            "vaddsd %[n], %[o], %[p] \n\t"
            "vaddsd %[q], %[r], %[s] \n\t"
            //5
            "vaddsd %[a], %[b], %[c] \n\t"
            "vaddsd %[d], %[e], %[f] \n\t"
            "vaddsd %[g], %[h], %[j] \n\t"
            "vaddsd %[k], %[l], %[m] \n\t"
            "vaddsd %[n], %[o], %[p] \n\t"
            "vaddsd %[q], %[r], %[s] \n\t"
            //6
            "vaddsd %[a], %[b], %[c] \n\t"
            "vaddsd %[d], %[e], %[f] \n\t"
            "vaddsd %[g], %[h], %[j] \n\t"
            "vaddsd %[k], %[l], %[m] \n\t"
            "vaddsd %[n], %[o], %[p] \n\t"
            "vaddsd %[q], %[r], %[s] \n\t"
            //7
            "vaddsd %[a], %[b], %[c] \n\t"
            "vaddsd %[d], %[e], %[f] \n\t"
            "vaddsd %[g], %[h], %[j] \n\t"
            "vaddsd %[k], %[l], %[m] \n\t"
            "vaddsd %[n], %[o], %[p] \n\t"
            "vaddsd %[q], %[r], %[s] \n\t"
            //8
            "vaddsd %[a], %[b], %[c] \n\t"
            "vaddsd %[d], %[e], %[f] \n\t"
            "vaddsd %[g], %[h], %[j] \n\t"
            "vaddsd %[k], %[l], %[m] \n\t"
            "vaddsd %[n], %[o], %[p] \n\t"
            "vaddsd %[q], %[r], %[s] \n\t"
            //9
            "vaddsd %[a], %[b], %[c] \n\t"
            "vaddsd %[d], %[e], %[f] \n\t"
            "vaddsd %[g], %[h], %[j] \n\t"
            "vaddsd %[k], %[l], %[m] \n\t"
            "vaddsd %[n], %[o], %[p] \n\t"
            "vaddsd %[q], %[r], %[s] \n\t"
            //10
            "vaddsd %[a], %[b], %[c] \n\t"
            "vaddsd %[d], %[e], %[f] \n\t"
            "vaddsd %[g], %[h], %[j] \n\t"
            "vaddsd %[k], %[l], %[m] \n\t"
            "vaddsd %[n], %[o], %[p] \n\t"
            "vaddsd %[q], %[r], %[s] \n\t"
            //11
            "vaddsd %[a], %[b], %[c] \n\t"
            "vaddsd %[d], %[e], %[f] \n\t"
            "vaddsd %[g], %[h], %[j] \n\t"
            "vaddsd %[k], %[l], %[m] \n\t"
            "vaddsd %[n], %[o], %[p] \n\t"
            "vaddsd %[q], %[r], %[s] \n\t"
            //12
            "vaddsd %[a], %[b], %[c] \n\t"
            "vaddsd %[d], %[e], %[f] \n\t"
            "vaddsd %[g], %[h], %[j] \n\t"
            "vaddsd %[k], %[l], %[m] \n\t"
            "vaddsd %[n], %[o], %[p] \n\t"
            "vaddsd %[q], %[r], %[s] \n\t"
            //13
            "vaddsd %[a], %[b], %[c] \n\t"
            "vaddsd %[d], %[e], %[f] \n\t"
            "vaddsd %[g], %[h], %[j] \n\t"
            "vaddsd %[k], %[l], %[m] \n\t"
            "vaddsd %[n], %[o], %[p] \n\t"
            "vaddsd %[q], %[r], %[s] \n\t"
            //14
            "vaddsd %[a], %[b], %[c] \n\t"
            "vaddsd %[d], %[e], %[f] \n\t"
            "vaddsd %[g], %[h], %[j] \n\t"
            "vaddsd %[k], %[l], %[m] \n\t"
            "vaddsd %[n], %[o], %[p] \n\t"
            "vaddsd %[q], %[r], %[s] \n\t"
            //15
            "vaddsd %[a], %[b], %[c] \n\t"
            "vaddsd %[d], %[e], %[f] \n\t"
            "vaddsd %[g], %[h], %[j] \n\t"
            "vaddsd %[k], %[l], %[m] \n\t"
            "vaddsd %[n], %[o], %[p] \n\t"
            "vaddsd %[q], %[r], %[s] \n\t"
            //16
            "vaddsd %[a], %[b], %[c] \n\t"
            "vaddsd %[d], %[e], %[f] \n\t"
            "vaddsd %[g], %[h], %[j] \n\t"
            "vaddsd %[k], %[l], %[m] \n\t"
            "vaddsd %[n], %[o], %[p] \n\t"
            "vaddsd %[q], %[r], %[s] \n\t"
            //17
            "vaddsd %[a], %[b], %[c] \n\t"
            "vaddsd %[d], %[e], %[f] \n\t"
            "vaddsd %[g], %[h], %[j] \n\t"
            "vaddsd %[k], %[l], %[m] \n\t"
            "vaddsd %[n], %[o], %[p] \n\t"
            "vaddsd %[q], %[r], %[s] \n\t"
            //18
            "vaddsd %[a], %[b], %[c] \n\t"
            "vaddsd %[d], %[e], %[f] \n\t"
            "vaddsd %[g], %[h], %[j] \n\t"
            "vaddsd %[k], %[l], %[m] \n\t"
            "vaddsd %[n], %[o], %[p] \n\t"
            "vaddsd %[q], %[r], %[s] \n\t"
            //19
            "vaddsd %[a], %[b], %[c] \n\t"
            "vaddsd %[d], %[e], %[f] \n\t"
            "vaddsd %[g], %[h], %[j] \n\t"
            "vaddsd %[k], %[l], %[m] \n\t"
            "vaddsd %[n], %[o], %[p] \n\t"
            "vaddsd %[q], %[r], %[s] \n\t"
            //20
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

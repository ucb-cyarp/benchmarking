#include "intrin_bench_default_defines.h"

//See https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/x64-architecture
//https://docs.oracle.com/cd/E18752_01/html/817-5477/eoizy.html
//http://flint.cs.yale.edu/cs421/papers/x86-asm/asm.html

//==========8 bit div==========
void kernel_only_asm_div_i8_unroll120()
{
    int8_t divBy = 1;    //Avoid div by 0 which causes div exception
    int8_t divBy2 = 1;    //Avoid div by 0 which causes div exception
    register int16_t initVal asm ("ax") = 1; //Even though we are doing an 8 bit divide 
                                             //The numerand is 16 bits
                                             //https://gcc.gnu.org/onlinedocs/gcc/Local-Register-Variables.html#Local-Register-Variables
    register int16_t clobberResult asm ("ax"); //Need to specify an output since inputs cannot occur in clobber list

    for(int i = 0; i<STIM_LEN/(2*60); i++)
    {
        //The divide does not have a 2 arg version
        //(al <- quotient, ah <- remainder) <- ax/reg8

        asm volatile( 
            //1
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //2
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //3
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //4
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //5
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //6
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //7
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //8
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //9
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //10
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //11
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //12
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //13
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //14
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //15
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //16
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //17
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //18
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //19
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //20
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //21
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //22
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //23
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //24
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //25
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //26
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //27
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //28
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //29
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //30
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //31
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //32
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //33
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //34
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //35
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //36
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //37
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //38
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //39
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //40
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //41
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //42
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //43
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //44
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //45
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //46
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //47
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //48
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //49
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //50
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //51
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //52
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //53
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //54
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //55
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //56
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //57
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //58
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //59
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            //60
            "idivb  %[aDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)
            "idivb  %[bDivBy]\n\t" //This case should cause no remainder and should in fact be the orig (div by 1)

            : "=r" (clobberResult)
            : [aInit] "r" (initVal), [aDivBy] "r" (divBy), [bDivBy] "r" (divBy2)
            : //al and ah are part of ax (cannot list in clibber since is an input - instead list as output)
        );
    }
}

void kernel_only_asm_div_i8_unroll120_regRename()
{
    int16_t initVal1 = 4;
    int16_t initVal2 = 6;

    int8_t divBy1 = 2;
    int8_t divBy2 = 3;

    for(int i = 0; i<STIM_LEN/(2*60); i++)
    {
        //The divide does not have a 2 arg version
        //(al <- quotient, ah <- remainder) <- ax/reg8

        //This version attempts to rename registers to avoid conflicts on dual dispatch.  Dependence is both WAW and RAW.  Need to move result into different reg to resolve WAW.  Need to move operand to resolve RAW.
        asm volatile(
            //1
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //2
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //3
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //4
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //5
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //6
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //7
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //8
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //9
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //10
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //11
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //12
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //13
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //14
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //15
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //16
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //17
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //18
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //19
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //20
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //21
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //22
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //23
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //24
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //25
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //26
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //27
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //28
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //29
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //30
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //31
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //32
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //33
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //34
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //35
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //36
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //37
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //38
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //39
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //40
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //41
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //42
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //43
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //44
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //45
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //46
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //47
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //48
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //49
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //50
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //51
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //52
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //53
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //54
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //55
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //56
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //57
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //58
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //59
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"
            //60
            "movw %[aInit], %%ax\n\t"
            "idivb  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %[bInit], %%ax\n\t"
            "idivb  %[bDivBy]\n\t"
            "movw %%ax, %%di\n\t"

            :
            : [aInit] "r" (initVal1), [aDivBy] "r" (divBy1), [bInit] "r" (initVal2), [bDivBy] "r" (divBy2)
            : "ax", "si", "di"
        );
    }
}

//==========16 bit div==========
void kernel_only_asm_div_i16_unroll120()
{
    int16_t divBy = 1;    //Avoid div by 0 which causes div exception
    int16_t divBy2 = 1;    //Avoid div by 0 which causes div exception
    register int16_t initVal0 asm ("ax") = 1; //https://gcc.gnu.org/onlinedocs/gcc/Local-Register-Variables.html#Local-Register-Variables
    register int16_t initVal1 asm ("dx") = 0; 
                                              
    register int16_t clobberResult0 asm ("ax"); //Need to specify an output since inputs cannot occur in clobber list
    register int16_t clobberResult1 asm ("dx");

    for(int i = 0; i<STIM_LEN/(2*60); i++)
    {
        //(ax <- quotient, dx <- remainder) <- dx:ax/reg16 (the 1st arg is formed by treating dx as the upper sig bits and ax as the lower)

        asm volatile(
            //1
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //2
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //3
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //4
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //5
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //6
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //7
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //8
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //9
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //10
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //11
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //12
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //13
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //14
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //15
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //16
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //17
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //18
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //19
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //20
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //21
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //22
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //23
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //24
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //25
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //26
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //27
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //28
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //29
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //30
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //31
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //32
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //33
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //34
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //35
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //36
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //37
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //38
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //39
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //40
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //41
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //42
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //43
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //44
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //45
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //46
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //47
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //48
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //49
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //50
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //51
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //52
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //53
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //54
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //55
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //56
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //57
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //58
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //59
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"
            //60
            "idivw  %[aDivBy]\n\t"
            "idivw  %[bDivBy]\n\t"

            : "=r" (clobberResult0), "=r" (clobberResult1)
            : [aInit0] "r" (initVal0), [aInit1] "r" (initVal1), [aDivBy] "r" (divBy), [bDivBy] "r" (divBy2)
            : 
        );
    }
}

void kernel_only_asm_div_i16_unroll120_regRename()
{
    int16_t initVal1 = 4;
    int16_t initVal2 = 6;

    int16_t initVal1_1 = 0;
    int16_t initVal2_1 = 0;

    int16_t divBy1 = 2;
    int16_t divBy2 = 3;

    for(int i = 0; i<STIM_LEN/(2*60); i++)
    {
        //(ax <- quotient, dx <- remainder) <- dx:ax/reg16 (the 1st arg is formed by treating dx as the upper sig bits and ax as the lower)

        //This version attempts to rename registers to avoid conflicts on dual dispatch.  Dependence is both WAW and RAW.  Need to move result into different reg to resolve WAW.  Need to move operand to resolve RAW.

        asm volatile(
            //1
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //2
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //3
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //4
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //5
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //6
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //7
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //8
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //9
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //10
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //11
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //12
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //13
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //14
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //15
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //16
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //17
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //18
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //19
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //20
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //21
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //22
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //23
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //24
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //25
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //26
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //27
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //28
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //29
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //30
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //31
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //32
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //33
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //34
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //35
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //36
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //37
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //38
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //39
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //40
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //41
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //42
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //43
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //44
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //45
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //46
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //47
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //48
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //49
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //50
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //51
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //52
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //53
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //54
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //55
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //56
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //57
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //58
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //59
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //60
            "movw %[aInit1], %%ax\n\t"
            "movw %[aInit2], %%dx\n\t"
            "idivw  %[aDivBy]\n\t"
            "movw %%ax, %%si\n\t"
            "movw %%dx, %%di\n\t"
            "movw %[bInit1], %%ax\n\t"
            "movw %[bInit2], %%dx\n\t"
            "idivw  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"

            :
            : [aInit1] "r" (initVal1), [aInit2] "r" (initVal1_1), [aDivBy] "r" (divBy1), [bInit1] "r" (initVal2), [bInit2] "r" (initVal2_1), [bDivBy] "r" (divBy2)
            : "rax", "rdx", "si", "di", "r8", "r9"
        );
    }
}

//==========32 bit div==========
void kernel_only_asm_div_i32_unroll120()
{ 
    int32_t divBy = 1;    //Avoid div by 0 which causes div exception
    int32_t divBy2 = 1;    //Avoid div by 0 which causes div exception
    register int32_t initVal0 asm ("eax") = 1; //https://gcc.gnu.org/onlinedocs/gcc/Local-Register-Variables.html#Local-Register-Variables
    register int32_t initVal1 asm ("edx") = 0; 
                                              
    register int32_t clobberResult0 asm ("eax"); //Need to specify an output since inputs cannot occur in clobber list
    register int32_t clobberResult1 asm ("edx");

    for(int i = 0; i<STIM_LEN/(2*60); i++)
    {
        //(eax <- quotient, edx <- remainder) <- edx:eax/reg16 (the 1st arg is formed by treating edx as the upper sig bits and eax as the lower)

        //Doing 2 div to hopefully avoid an issue with dependence.
        asm volatile(
            //1
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //2
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //3
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //4
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //5
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //6
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //7
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //8
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //9
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //10
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //11
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //12
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //13
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //14
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //15
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //16
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //17
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //18
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //19
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //20
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //21
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //22
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //23
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //24
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //25
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //26
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //27
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //28
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //29
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //30
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //31
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //32
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //33
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //34
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //35
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //36
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //37
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //38
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //39
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //40
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //41
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //42
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //43
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //44
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //45
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //46
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //47
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //48
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //49
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //50
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //51
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //52
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //53
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //54
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //55
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //56
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //57
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //58
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //59
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"
            //60
            "idivl  %[aDivBy]\n\t"
            "idivl  %[bDivBy]\n\t"

            : "=r" (clobberResult0), "=r" (clobberResult1)
            : [aInit0] "r" (initVal0), [aInit1] "r" (initVal1), [aDivBy] "r" (divBy), [bDivBy] "r" (divBy2)
            : 
        );
    }
}

void kernel_only_asm_div_i32_unroll120_regRename()
{ 
    int32_t initVal1 = 4;
    int32_t initVal2 = 6;

    int32_t initVal1_1 = 0;
    int32_t initVal2_1 = 0;

    int32_t divBy1 = 2;
    int32_t divBy2 = 3;

    for(int i = 0; i<STIM_LEN/(2*60); i++)
    {
        //(eax <- quotient, edx <- remainder) <- edx:eax/reg16 (the 1st arg is formed by treating edx as the upper sig bits and eax as the lower)

        //This version attempts to rename registers to avoid conflicts on dual dispatch.  Dependence is both WAW and RAW.  Need to move result into different reg to resolve WAW.  Need to move operand to resolve RAW.
        asm volatile(
            //1
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //2
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //3
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //4
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //5
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //6
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //7
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //8
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //9
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //10
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //11
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //12
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //13
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //14
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //15
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //16
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //17
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //18
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //19
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //20
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //21
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //22
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //23
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //24
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //25
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //26
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //27
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //28
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //29
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //30
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //31
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //32
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //33
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //34
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //35
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //36
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //37
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //38
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //39
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //40
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //41
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //42
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //43
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //44
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //45
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //46
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //47
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //48
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //49
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //50
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //51
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //52
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //53
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //54
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //55
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //56
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //57
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //58
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //59
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //60
            "movl %[aInit1], %%eax\n\t"
            "movl %[aInit2], %%edx\n\t"
            "idivl  %[aDivBy]\n\t"
            "movl %%eax, %%esi\n\t"
            "movl %%edx, %%edi\n\t"
            "movl %[bInit1], %%eax\n\t"
            "movl %[bInit2], %%edx\n\t"
            "idivl %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"

            :
            : [aInit1] "r" (initVal1), [aInit2] "r" (initVal1_1), [aDivBy] "r" (divBy1), [bInit1] "r" (initVal2), [bInit2] "r" (initVal2_1), [bDivBy] "r" (divBy2)
            : "rax", "rdx", "esi", "edi", "r8", "r9"
        );
    }
}

//==========64 bit div==========
void kernel_only_asm_div_i64_unroll120()
{
    int64_t divBy = 1;    //Avoid div by 0 which causes div exception
    int64_t divBy2 = 1;    //Avoid div by 0 which causes div exception
    register int64_t initVal0 asm ("rax") = 1; //https://gcc.gnu.org/onlinedocs/gcc/Local-Register-Variables.html#Local-Register-Variables
    register int64_t initVal1 asm ("rdx") = 0; 
                                              
    register int64_t clobberResult0 asm ("rax"); //Need to specify an output since inputs cannot occur in clobber list
    register int64_t clobberResult1 asm ("rdx");

    for(int i = 0; i<STIM_LEN/(2*60); i++)
    {
        //Doing 2 adds to hopefully avoid an issue with dependence.  Intel ASM is reg1=reg1+reg2.  It is not a three register instruction like it is with the vector unit
        //TODO: Verify
        asm volatile(
            //1
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //2
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //3
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //4
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //5
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //6
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //7
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //8
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //9
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //10
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //11
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //12
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //13
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //14
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //15
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //16
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //17
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //18
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //19
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //20
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //21
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //22
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //23
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //24
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //25
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //26
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //27
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //28
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //29
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //30
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //31
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //32
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //33
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //34
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //35
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //36
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //37
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //38
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //39
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //40
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //41
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //42
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //43
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //44
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //45
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //46
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //47
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //48
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //49
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //50
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //51
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //52
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //53
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //54
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //55
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //56
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //57
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //58
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //59
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"
            //60
            "idivq  %[aDivBy]\n\t"
            "idivq  %[bDivBy]\n\t"

            : "=r" (clobberResult0), "=r" (clobberResult1)
            : [aInit0] "r" (initVal0), [aInit1] "r" (initVal1), [aDivBy] "r" (divBy), [bDivBy] "r" (divBy2)
            : 
        );
    }
}

void kernel_only_asm_div_i64_unroll120_regRename()
{
    int64_t initVal1 = 4;
    int64_t initVal2 = 6;

    int64_t initVal1_1 = 0;
    int64_t initVal2_1 = 0;

    int64_t divBy1 = 2;
    int64_t divBy2 = 3;

    for(int i = 0; i<STIM_LEN/(2*60); i++)
    {
        //(rax <- quotient, rdx <- remainder) <- rdx:rax/reg16 (the 1st arg is formed by treating edx as the upper sig bits and eax as the lower)

        //This version attempts to rename registers to avoid conflicts on dual dispatch.  Dependence is both WAW and RAW.  Need to move result into different reg to resolve WAW.  Need to move operand to resolve RAW.
        asm volatile(
            //1
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //2
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //3
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //4
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //5
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //6
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //7
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //8
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //9
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //10
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //11
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //12
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //13
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //14
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //15
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //16
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //17
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //18
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //19
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //20
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //21
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //22
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //23
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //24
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //25
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //26
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //27
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //28
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //29
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //30
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //31
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //32
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //33
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //34
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //35
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //36
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //37
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //38
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //39
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //40
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //41
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //42
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //43
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //44
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //45
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //46
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //47
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //48
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //49
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //50
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //51
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //52
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //53
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //54
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //55
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //56
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //57
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //58
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //59
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"
            //60
            "movq %[aInit1], %%rax\n\t"
            "movq %[aInit2], %%rdx\n\t"
            "idivq  %[aDivBy]\n\t"
            "movq %%rax, %%rsi\n\t"
            "movq %%rdx, %%rdi\n\t"
            "movq %[bInit1], %%rax\n\t"
            "movq %[bInit2], %%rdx\n\t"
            "idivq  %[bDivBy]\n\t"
            "movq %%rax, %%r8\n\t"
            "movq %%rdx, %%r9\n\t"

            :
            : [aInit1] "r" (initVal1), [aInit2] "r" (initVal1_1), [aDivBy] "r" (divBy1), [bInit1] "r" (initVal2), [bInit2] "r" (initVal2_1), [bDivBy] "r" (divBy2)
            : "rax", "rdx", "rsi", "rdi", "r8", "r9"
        );
    }
}

//==========div floating point==========
void kernel_only_asm_div_fp_unroll120()
{
    for(int i = 0; i<STIM_LEN/(2*60); i++)
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
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //2
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //3
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //4
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //5
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //6
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //7
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //8
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //9
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //10
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //11
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //12
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //13
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //14
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //15
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //16
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //17
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //18
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //19
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //20
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //21
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //22
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //23
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //24
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //25
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //26
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //27
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //28
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //29
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //30
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //31
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //32
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //33
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //34
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //35
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //36
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //37
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //38
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //39
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //40
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //41
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //42
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //43
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //44
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //45
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //46
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //47
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //48
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //49
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //50
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //51
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //52
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //53
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //54
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //55
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //56
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //57
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //58
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //59
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            //60
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fld1 \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"
            "fdivp %%st(1), %%st \n\t"
            "fstp %%st(0) \n\t"

            :
            :
            :"st(3)", "st(2)", "st(1)", "st"
        );
    }
}

//==========div single with SSE==========
void kernel_only_asm_div_sp_unroll120()
{
    float divBy1 = 1.0;    //Avoid div by 0 which causes div exception
    float initVal1 = 3.0;

    float divBy2 = 2.0;    //Avoid div by 0 which causes div exception
    float initVal2 = 4.0;

    float result1;
    float result2;

    for(int i = 0; i<STIM_LEN/(2*60); i++)
    {
        //This is using the SSE vector unit to multiply a single number (avoiding x87).
        //It was suggested in the GCC manual that the SSE instructions are used instead of x07
        
        //TODO: Verify
        asm volatile(
            //1
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //2
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //3
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //4
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //5
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //6
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //7
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //8
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //9
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //10
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //11
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //12
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //13
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //14
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //15
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //16
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //17
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //18
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //19
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //20
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //21
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //22
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //23
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //24
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //25
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //26
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //27
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //28
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //29
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //30
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //31
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //32
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //33
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //34
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //35
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //36
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //37
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //38
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //39
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //40
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //41
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //42
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //43
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //44
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //45
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //46
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //47
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //48
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //49
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //50
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //51
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //52
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //53
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //54
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //55
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //56
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //57
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //58
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //59
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"
            //60
            "vdivss %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivss %[bInit], %[bDivBy], %[bResult] \n\t"

            : [aResult] "=x" (result1), [bResult] "=x" (result2)
            : [aInit] "x" (initVal1), [aDivBy] "x" (divBy1), [bInit] "x" (initVal2), [bDivBy] "x" (divBy2)
            : 
        );
    }
}

//==========div double with SSE==========
void kernel_only_asm_div_dp_unroll120()
{
    double divBy1 = 1.0;    //Avoid div by 0 which causes div exception
    double initVal1 = 3.0;

    double divBy2 = 2.0;    //Avoid div by 0 which causes div exception
    double initVal2 = 4.0;

    double result1;
    double result2;

    for(int i = 0; i<STIM_LEN/(2*60); i++)
    {
        //This is using the SSE vector unit to multiply a single number (avoiding x87).
        //It was suggested in the GCC manual that the SSE instructions are used instead of x07
        
        //TODO: Verify
        asm volatile(
            //1
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //2
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //3
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //4
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //5
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //6
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //7
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //8
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //9
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //10
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //11
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //12
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //13
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //14
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //15
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //16
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //17
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //18
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //19
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //20
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //21
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //22
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //23
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //24
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //25
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //26
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //27
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //28
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //29
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //30
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //31
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //32
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //33
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //34
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //35
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //36
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //37
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //38
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //39
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //40
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //41
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //42
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //43
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //44
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //45
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //46
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //47
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //48
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //49
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //50
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //51
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //52
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //53
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //54
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //55
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //56
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //57
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //58
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //59
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"
            //60
            "vdivsd %[aInit], %[aDivBy], %[aResult] \n\t"
            "vdivsd %[bInit], %[bDivBy], %[bResult] \n\t"

            : [aResult] "=x" (result1), [bResult] "=x" (result2)
            : [aInit] "x" (initVal1), [aDivBy] "x" (divBy1), [bInit] "x" (initVal2), [bDivBy] "x" (divBy2)
            : 
        );
    }
}

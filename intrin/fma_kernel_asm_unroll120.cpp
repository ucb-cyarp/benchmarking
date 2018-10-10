#include "intrin_bench_default_defines.h"

#ifdef __FMA__
    //==========_mm256_fmadd_ps==========
    void kernel_only_asm_mm256_fmadd_ps_unroll120()
    {
        for(int i = 0; i<STIM_LEN/(64*15); i++)
        {
            //NOTE: There are multiple vfmadd instructions.  Three varients change the meaning of the operands and which is replaced
            asm volatile(
                //1
                "vfmadd132ps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132ps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132ps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132ps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132ps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132ps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132ps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132ps  %%ymm14, %%ymm15, %%ymm14\n\t"
                //2
                "vfmadd132ps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132ps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132ps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132ps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132ps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132ps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132ps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132ps  %%ymm14, %%ymm15, %%ymm14\n\t"
                //3
                "vfmadd132ps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132ps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132ps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132ps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132ps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132ps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132ps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132ps  %%ymm14, %%ymm15, %%ymm14\n\t"
                //4
                "vfmadd132ps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132ps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132ps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132ps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132ps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132ps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132ps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132ps  %%ymm14, %%ymm15, %%ymm14\n\t"
                //5
                "vfmadd132ps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132ps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132ps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132ps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132ps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132ps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132ps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132ps  %%ymm14, %%ymm15, %%ymm14\n\t"
                //6
                "vfmadd132ps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132ps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132ps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132ps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132ps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132ps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132ps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132ps  %%ymm14, %%ymm15, %%ymm14\n\t"
                //7
                "vfmadd132ps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132ps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132ps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132ps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132ps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132ps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132ps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132ps  %%ymm14, %%ymm15, %%ymm14\n\t"
                //8
                "vfmadd132ps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132ps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132ps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132ps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132ps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132ps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132ps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132ps  %%ymm14, %%ymm15, %%ymm14\n\t"
                //9
                "vfmadd132ps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132ps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132ps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132ps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132ps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132ps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132ps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132ps  %%ymm14, %%ymm15, %%ymm14\n\t"
                //10
                "vfmadd132ps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132ps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132ps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132ps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132ps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132ps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132ps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132ps  %%ymm14, %%ymm15, %%ymm14\n\t"
                //11
                "vfmadd132ps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132ps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132ps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132ps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132ps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132ps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132ps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132ps  %%ymm14, %%ymm15, %%ymm14\n\t"
                //12
                "vfmadd132ps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132ps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132ps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132ps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132ps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132ps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132ps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132ps  %%ymm14, %%ymm15, %%ymm14\n\t"
                //13
                "vfmadd132ps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132ps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132ps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132ps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132ps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132ps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132ps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132ps  %%ymm14, %%ymm15, %%ymm14\n\t"
                //14
                "vfmadd132ps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132ps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132ps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132ps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132ps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132ps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132ps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132ps  %%ymm14, %%ymm15, %%ymm14\n\t"
                //15
                "vfmadd132ps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132ps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132ps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132ps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132ps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132ps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132ps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132ps  %%ymm14, %%ymm15, %%ymm14\n\t"

                :
                :
                : MMREG(15), MMREG(14), MMREG(13), MMREG(12), MMREG(11), MMREG(10), MMREG(9), MMREG(8), MMREG(7), MMREG(6), MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }

    //==========_mm256_fmadd_pd==========
    void kernel_only_asm_mm256_fmadd_pd_unroll120()
    {
        for(int i = 0; i<STIM_LEN/(32*15); i++)
        {
            //NOTE: There are multiple vfmadd instructions.  Three varients change the meaning of the operands and which is replaced
            asm volatile(
                //1
                "vfmadd132pd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132pd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132pd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132pd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132pd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132pd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132pd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132pd  %%ymm14, %%ymm15, %%ymm14\n\t"
                //2
                "vfmadd132pd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132pd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132pd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132pd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132pd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132pd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132pd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132pd  %%ymm14, %%ymm15, %%ymm14\n\t"
                //3
                "vfmadd132pd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132pd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132pd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132pd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132pd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132pd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132pd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132pd  %%ymm14, %%ymm15, %%ymm14\n\t"
                //4
                "vfmadd132pd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132pd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132pd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132pd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132pd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132pd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132pd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132pd  %%ymm14, %%ymm15, %%ymm14\n\t"
                //5
                "vfmadd132pd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132pd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132pd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132pd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132pd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132pd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132pd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132pd  %%ymm14, %%ymm15, %%ymm14\n\t"
                //6
                "vfmadd132pd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132pd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132pd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132pd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132pd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132pd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132pd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132pd  %%ymm14, %%ymm15, %%ymm14\n\t"
                //7
                "vfmadd132pd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132pd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132pd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132pd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132pd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132pd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132pd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132pd  %%ymm14, %%ymm15, %%ymm14\n\t"
                //8
                "vfmadd132pd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132pd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132pd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132pd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132pd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132pd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132pd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132pd  %%ymm14, %%ymm15, %%ymm14\n\t"
                //9
                "vfmadd132pd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132pd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132pd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132pd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132pd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132pd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132pd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132pd  %%ymm14, %%ymm15, %%ymm14\n\t"
                //10
                "vfmadd132pd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132pd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132pd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132pd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132pd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132pd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132pd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132pd  %%ymm14, %%ymm15, %%ymm14\n\t"
                //11
                "vfmadd132pd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132pd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132pd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132pd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132pd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132pd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132pd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132pd  %%ymm14, %%ymm15, %%ymm14\n\t"
                //12
                "vfmadd132pd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132pd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132pd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132pd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132pd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132pd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132pd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132pd  %%ymm14, %%ymm15, %%ymm14\n\t"
                //13
                "vfmadd132pd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132pd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132pd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132pd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132pd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132pd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132pd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132pd  %%ymm14, %%ymm15, %%ymm14\n\t"
                //14
                "vfmadd132pd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132pd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132pd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132pd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132pd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132pd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132pd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132pd  %%ymm14, %%ymm15, %%ymm14\n\t"
                //15
                "vfmadd132pd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vfmadd132pd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vfmadd132pd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vfmadd132pd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vfmadd132pd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vfmadd132pd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vfmadd132pd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vfmadd132pd  %%ymm14, %%ymm15, %%ymm14\n\t"

                :
                :
                : MMREG(15), MMREG(14), MMREG(13), MMREG(12), MMREG(11), MMREG(10), MMREG(9), MMREG(8), MMREG(7), MMREG(6), MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }
#endif

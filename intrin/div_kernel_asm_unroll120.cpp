#include "intrin_bench_default_defines_and_imports_cpp.h"

#ifdef __AVX__
    //==========_mm256_div_ps==========
    void kernel_only_asm_mm256_div_ps_unroll120()
    {
        for(int i = 0; i<STIM_LEN/(32*30); i++)
        {
            asm volatile(
                //1
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //2
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //3
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //4
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //5
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //6
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //7
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //8
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //9
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //10
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //11
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //12
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //13
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //14
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //15
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //16
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //17
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //18
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //19
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //20
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //21
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //22
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //23
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //24
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //25
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //26
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //27
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //28
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //29
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                //30
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivps  %%ymm6, %%ymm7, %%ymm6\n\t"
                :
                :
                : MMREG(7), MMREG(6), MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }

    //==========_mm256_div_pd==========
    void kernel_only_asm_mm256_div_pd_unroll120()
    {
        for(int i = 0; i<STIM_LEN/(16*30); i++)
        {
            asm volatile(
                //1
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //2
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //3
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //4
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //5
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //6
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //7
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //8
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //9
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //10
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //11
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //12
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //13
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //14
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //15
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //16
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //17
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //18
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //19
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //20
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //21
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //22
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //23
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //24
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //25
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //26
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //27
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //28
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //29
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                //30
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vdivpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vdivpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vdivpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                :
                :
                : MMREG(7), MMREG(6), MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }
#endif

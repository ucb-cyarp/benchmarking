#include "intrin_bench_default_defines_and_imports_cpp.h"

#ifdef __AVX__
    //==========_mm256_div_ps==========
    void kernel_only_asm_mm256_div_ps_unroll4()
    {
        for(int i = 0; i<STIM_LEN/32; i++)
        {
            asm volatile(
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
    void kernel_only_asm_mm256_div_pd_unroll4()
    {
        for(int i = 0; i<STIM_LEN/16; i++)
        {
            asm volatile(
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

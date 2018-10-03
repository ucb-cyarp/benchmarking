#include "intrin_bench_default_defines.h"

#ifdef __AVX__
    //==========_mm256_div_ps==========
    void kernel_only_asm_mm256_div_ps()
    {
        for(int i = 0; i<STIM_LEN/8; i++)
        {
            asm volatile(
                "vdivps  %%ymm0, %%ymm1, %%ymm0\n\t"
                :
                :
                : MMREG(1), MMREG(0)
            );
        }
    }

    //==========_mm256_div_pd==========
    void kernel_only_asm_mm256_div_pd()
    {
        for(int i = 0; i<STIM_LEN/4; i++)
        {
            asm volatile(
                "vdivpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                :
                :
                : MMREG(1), MMREG(0)
            );
        }
    }
#endif

#include "intrin_bench_default_defines.h"

#ifdef __AVX__
    //==========_mm256_div_ps==========
    void kernel_only_asm_mm256_div_ps()
    {
        for(int i = 0; i<STIM_LEN; i+=8)
        {
            asm volatile(
                "vdivps  %%ymm2, %%ymm0, %%ymm1\n\t"
                :
            );
        }
    }

    //==========_mm256_div_pd==========
    void kernel_only_asm_mm256_div_pd()
    {
        for(int i = 0; i<STIM_LEN; i+=4)
        {
            asm volatile(
                "vdivpd  %%ymm2, %%ymm0, %%ymm1\n\t"
                :
            );
        }
    }
#endif

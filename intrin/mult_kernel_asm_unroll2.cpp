#include "intrin_bench_default_defines.h"

#ifdef __AVX2__
    //==========_mm256_mullo_epi16==========
    void kernel_only_asm_mm256_mullo_epi16_unroll2()
    {
        for(int i = 0; i<STIM_LEN/32; i++)
        {
            asm volatile(
                "vpmullw  %%ymm2, %%ymm0, %%ymm1\n\t"
                "vpmullw  %%ymm5, %%ymm3, %%ymm4\n\t"
                :
                :
                : MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }

    //==========_mm256_mullo_epi32==========
    void kernel_only_asm_mm256_mullo_epi32_unroll2()
    {
        for(int i = 0; i<STIM_LEN/16; i++)
        {
            asm volatile(
                "vpmulld  %%ymm2, %%ymm0, %%ymm1\n\t"
                "vpmulld  %%ymm5, %%ymm3, %%ymm4\n\t"
                :
                :
                : MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }
#endif

#ifdef __AVX__
    //==========_mm256_mul_ps==========
    void kernel_only_asm_mm256_mul_ps_unroll2()
    {
        for(int i = 0; i<STIM_LEN/16; i++)
        {
            asm volatile(
                "vmulps  %%ymm2, %%ymm0, %%ymm1\n\t"
                "vmulps  %%ymm5, %%ymm3, %%ymm4\n\t"
                :
                :
                : MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }

    //==========_mm256_mul_pd==========
    void kernel_only_asm_mm256_mul_pd_unroll2()
    {
        for(int i = 0; i<STIM_LEN/8; i++)
        {
            asm volatile(
                "vmulpd  %%ymm2, %%ymm0, %%ymm1\n\t"
                "vmulpd  %%ymm5, %%ymm3, %%ymm4\n\t"
                :
                :
                : MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }
#endif

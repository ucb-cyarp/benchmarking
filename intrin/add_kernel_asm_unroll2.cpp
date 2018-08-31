#include "intrin_bench_default_defines.h"

#ifdef __AVX2__
    //==========_mm256_add_epi8==========
    void kernel_only_asm_mm256_add_epi8_unroll2()
    {
        for(int i = 0; i<STIM_LEN/64; i++)
        {
            asm volatile(
                "vpaddb  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpaddb  %%ymm2, %%ymm3, %%ymm2\n\t"
                :
                :
                : MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }

    //==========_mm256_add_epi16==========
    void kernel_only_asm_mm256_add_epi16_unroll2()
    {
        for(int i = 0; i<STIM_LEN/32; i++)
        {
            asm volatile(
                "vpaddw  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpaddw  %%ymm2, %%ymm3, %%ymm2\n\t"
                :
                :
                : MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }

    //==========_mm256_add_epi32==========
    void kernel_only_asm_mm256_add_epi32_unroll2()
    { 
        for(int i = 0; i<STIM_LEN/16; i++)
        {
            asm volatile(
                "vpaddd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpaddd  %%ymm2, %%ymm3, %%ymm2\n\t"
                :
                :
                : MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }

    //==========_mm256_add_epi64==========
    void kernel_only_asm_mm256_add_epi64_unroll2()
    { 
        for(int i = 0; i<STIM_LEN/8; i++)
        {
            asm volatile(
                "vpaddq  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpaddq  %%ymm2, %%ymm3, %%ymm2\n\t"
                :
                :
                : MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }
#endif

#ifdef __AVX__
    //==========_mm256_add_ps==========
    void kernel_only_asm_mm256_add_ps_unroll2()
    {
        for(int i = 0; i<STIM_LEN/16; i++)
        {
            asm volatile(
                "vaddps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vaddps  %%ymm2, %%ymm3, %%ymm2\n\t"
                :
                :
                : MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }

    //==========_mm256_add_pd==========
    void kernel_only_asm_mm256_add_pd_unroll2()
    {
        for(int i = 0; i<STIM_LEN/8; i++)
        {
            asm volatile(
                "vaddpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vaddpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                :
                :
                : MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }
#endif

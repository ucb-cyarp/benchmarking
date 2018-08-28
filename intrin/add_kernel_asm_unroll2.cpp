#include "intrin_bench_default_defines.h"

#ifdef __AVX2__
    //==========_mm256_add_epi8==========
    void kernel_only_asm_mm256_add_epi8_unroll2()
    {
        for(int i = 0; i<STIM_LEN/64; i++)
        {
            asm volatile(
                "vpaddb  %%ymm2, %%ymm0, %%ymm1\n\t"
                "vpaddb  %%ymm5, %%ymm3, %%ymm4\n\t"
                :
                :
                : MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }

    //==========_mm256_add_epi16==========
    void kernel_only_asm_mm256_add_epi16_unroll2()
    {
        for(int i = 0; i<STIM_LEN/32; i++)
        {
            asm volatile(
                "vpaddw  %%ymm2, %%ymm0, %%ymm1\n\t"
                "vpaddw  %%ymm5, %%ymm3, %%ymm4\n\t"
                :
                :
                :  MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }

    //==========_mm256_add_epi32==========
    void kernel_only_asm_mm256_add_epi32_unroll2()
    { 
        for(int i = 0; i<STIM_LEN/16; i++)
        {
            asm volatile(
                "vpaddd  %%ymm2, %%ymm0, %%ymm1\n\t"
                "vpaddd  %%ymm5, %%ymm3, %%ymm4\n\t"
                :
                :
                : MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }

    //==========_mm256_add_epi64==========
    void kernel_only_asm_mm256_add_epi64_unroll2()
    { 
        for(int i = 0; i<STIM_LEN/8; i++)
        {
            asm volatile(
                "vpaddq  %%ymm2, %%ymm0, %%ymm1\n\t"
                "vpaddq  %%ymm5, %%ymm3, %%ymm4\n\t"
                :
                :
                : MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
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
                "vaddps  %%ymm2, %%ymm0, %%ymm1\n\t"
                "vaddps  %%ymm5, %%ymm3, %%ymm4\n\t"
                :
                :
                : MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }

    //==========_mm256_add_pd==========
    void kernel_only_asm_mm256_add_pd_unroll2()
    {
        for(int i = 0; i<STIM_LEN/8; i++)
        {
            asm volatile(
                "vaddpd  %%ymm2, %%ymm0, %%ymm1\n\t"
                "vaddpd  %%ymm5, %%ymm3, %%ymm4\n\t"
                :
                :
                : MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }
#endif

#include "intrin_bench_default_defines.h"

#ifdef __AVX2__
    //==========_mm256_add_epi8==========
    void kernel_only_asm_mm256_add_epi8_unroll4()
    {
        for(int i = 0; i<STIM_LEN/128; i++)
        {
            asm volatile(
                "vpaddb  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpaddb  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpaddb  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpaddb  %%ymm6, %%ymm7, %%ymm6\n\t"
                :
                :
                : MMREG(7), MMREG(6), MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }

    //==========_mm256_add_epi16==========
    void kernel_only_asm_mm256_add_epi16_unroll4()
    {
        for(int i = 0; i<STIM_LEN/64; i++)
        {
            asm volatile(
                "vpaddw  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpaddw  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpaddw  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpaddw  %%ymm6, %%ymm7, %%ymm6\n\t"
                :
                :
                : MMREG(7), MMREG(6), MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }

    //==========_mm256_add_epi32==========
    void kernel_only_asm_mm256_add_epi32_unroll4()
    { 
        for(int i = 0; i<STIM_LEN/32; i++)
        {
            asm volatile(
                "vpaddd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpaddd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpaddd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpaddd  %%ymm6, %%ymm7, %%ymm6\n\t"
                :
                :
                : MMREG(7), MMREG(6), MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }

    //==========_mm256_add_epi64==========
    void kernel_only_asm_mm256_add_epi64_unroll4()
    { 
        for(int i = 0; i<STIM_LEN/16; i++)
        {
            asm volatile(
                "vpaddq  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpaddq  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpaddq  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpaddq  %%ymm6, %%ymm7, %%ymm6\n\t"
                :
                :
                : MMREG(7), MMREG(6), MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }
#endif

#ifdef __AVX__
    //==========_mm256_add_ps==========
    void kernel_only_asm_mm256_add_ps_unroll4()
    {
        for(int i = 0; i<STIM_LEN/32; i++)
        {
            asm volatile(
                "vaddps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vaddps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vaddps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vaddps  %%ymm6, %%ymm7, %%ymm6\n\t"
                :
                :
                : MMREG(7), MMREG(6), MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }

    //==========_mm256_add_pd==========
    void kernel_only_asm_mm256_add_pd_unroll4()
    {
        for(int i = 0; i<STIM_LEN/16; i++)
        {
            asm volatile(
                "vaddpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vaddpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vaddpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vaddpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                :
                :
                : MMREG(7), MMREG(6), MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }
#endif

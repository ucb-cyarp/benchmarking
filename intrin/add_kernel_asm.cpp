#include "intrin_bench_default_defines_and_imports_cpp.h"

#ifdef __AVX2__
    //==========_mm256_add_epi8==========
    void kernel_only_asm_mm256_add_epi8()
    {
        for(int i = 0; i<STIM_LEN/32; i++)
        {
            asm volatile(
                "vpaddb  %%ymm0, %%ymm1, %%ymm0\n\t"
                // "vpaddb  %%ymm5, %%ymm3, %%ymm4\n\t"
                // "vpaddb  %%ymm8, %%ymm6, %%ymm7\n\t"
                // "vpaddb %%ymm11, %%ymm9, %%ymm10\n\t"
                :
                :
                : MMREG(1), MMREG(0)
            );
        }
    }

    //==========_mm256_add_epi16==========
    void kernel_only_asm_mm256_add_epi16()
    {
        for(int i = 0; i<STIM_LEN/16; i++)
        {
            asm volatile(
                "vpaddw  %%ymm0, %%ymm1, %%ymm0\n\t"
                :
                :
                : MMREG(1), MMREG(0)
            );
        }
    }

    //==========_mm256_add_epi32==========
    void kernel_only_asm_mm256_add_epi32()
    { 
        for(int i = 0; i<STIM_LEN/8; i++)
        {
            asm volatile(
                "vpaddd  %%ymm0, %%ymm1, %%ymm0\n\t"
                :
                :
                : MMREG(1), MMREG(0)
            );
        }
    }

    //==========_mm256_add_epi64==========
    void kernel_only_asm_mm256_add_epi64()
    { 
        for(int i = 0; i<STIM_LEN/4; i++)
        {
            asm volatile(
                "vpaddq  %%ymm0, %%ymm1, %%ymm0\n\t"
                :
                :
                : MMREG(1), MMREG(0)
            );
        }
    }
#endif

#ifdef __AVX__
    //==========_mm256_add_ps==========
    void kernel_only_asm_mm256_add_ps()
    {
        for(int i = 0; i<STIM_LEN/8; i++)
        {
            asm volatile(
                "vaddps  %%ymm0, %%ymm1, %%ymm0\n\t"
                :
                :
                : MMREG(1), MMREG(0)
            );
        }
    }

    //==========_mm256_add_pd==========
    void kernel_only_asm_mm256_add_pd()
    {
        for(int i = 0; i<STIM_LEN/4; i++)
        {
            asm volatile(
                "vaddpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                :
                :
                : MMREG(1), MMREG(0)
            );
        }
    }
#endif

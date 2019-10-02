#include "intrin_bench_default_defines_and_imports_cpp.h"

#ifdef __AVX2__
    //==========_mm256_mullo_epi16==========
    void kernel_only_asm_mm256_mullo_epi16_unroll4()
    {
        for(int i = 0; i<STIM_LEN/64; i++)
        {
            asm volatile(
                "vpmullw  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmullw  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmullw  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmullw  %%ymm6, %%ymm7, %%ymm6\n\t"
                :
                :
                : MMREG(6), MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }

    //==========_mm256_mullo_epi32==========
    void kernel_only_asm_mm256_mullo_epi32_unroll4()
    {
        for(int i = 0; i<STIM_LEN/32; i++)
        {
            asm volatile(
                "vpmulld  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmulld  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmulld  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmulld  %%ymm6, %%ymm7, %%ymm6\n\t"
                :
                :
                : MMREG(6), MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }
#endif

#ifdef __AVX__
    //==========_mm256_mul_ps==========
    void kernel_only_asm_mm256_mul_ps_unroll4()
    {
        for(int i = 0; i<STIM_LEN/32; i++)
        {
            asm volatile(
                "vmulps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulps  %%ymm6, %%ymm7, %%ymm6\n\t"
                :
                :
                : MMREG(6), MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }

    //==========_mm256_mul_pd==========
    void kernel_only_asm_mm256_mul_pd_unroll4()
    {
        for(int i = 0; i<STIM_LEN/16; i++)
        {
            asm volatile(
                "vmulpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                :
                :
                : MMREG(6), MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }
#endif

#include "intrin_bench_default_defines.h"

#ifdef __AVX2__
    //==========_mm256_mullo_epi16==========
    void kernel_only_asm_mm256_mullo_epi16_unroll8()
    {
        for(int i = 0; i<STIM_LEN/128; i++)
        {
            asm volatile(
                "vpmullw  %%ymm2, %%ymm0, %%ymm1\n\t"
                "vpmullw  %%ymm5, %%ymm3, %%ymm4\n\t"
                "vpmullw  %%ymm8, %%ymm6, %%ymm7\n\t"
                "vpmullw  %%ymm11, %%ymm9, %%ymm10\n\t"
                "vpmullw  %%ymm14, %%ymm12, %%ymm13\n\t"
                "vpmullw  %%ymm2, %%ymm0, %%ymm1\n\t"
                "vpmullw  %%ymm5, %%ymm3, %%ymm4\n\t"
                "vpmullw  %%ymm8, %%ymm6, %%ymm7\n\t"
                :
                :
                : MMREG(14), MMREG(13), MMREG(12), MMREG(11), MMREG(10), MMREG(9), MMREG(8), MMREG(7), MMREG(6), MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }

    //==========_mm256_mullo_epi32==========
    void kernel_only_asm_mm256_mullo_epi32_unroll8()
    {
        for(int i = 0; i<STIM_LEN/64; i++)
        {
            asm volatile(
                "vpmulld  %%ymm2, %%ymm0, %%ymm1\n\t"
                "vpmulld  %%ymm5, %%ymm3, %%ymm4\n\t"
                "vpmulld  %%ymm8, %%ymm6, %%ymm7\n\t"
                "vpmulld  %%ymm11, %%ymm9, %%ymm10\n\t"
                "vpmulld  %%ymm14, %%ymm12, %%ymm13\n\t"
                "vpmulld  %%ymm2, %%ymm0, %%ymm1\n\t"
                "vpmulld  %%ymm5, %%ymm3, %%ymm4\n\t"
                "vpmulld  %%ymm8, %%ymm6, %%ymm7\n\t"
                :
                :
                : MMREG(14), MMREG(13), MMREG(12), MMREG(11), MMREG(10), MMREG(9), MMREG(8), MMREG(7), MMREG(6), MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }
#endif

#ifdef __AVX__
    //==========_mm256_mul_ps==========
    void kernel_only_asm_mm256_mul_ps_unroll8()
    {
        for(int i = 0; i<STIM_LEN/64; i++)
        {
            asm volatile(
                "vmulps  %%ymm2, %%ymm0, %%ymm1\n\t"
                "vmulps  %%ymm5, %%ymm3, %%ymm4\n\t"
                "vmulps  %%ymm8, %%ymm6, %%ymm7\n\t"
                "vmulps  %%ymm11, %%ymm9, %%ymm10\n\t"
                "vmulps  %%ymm14, %%ymm12, %%ymm13\n\t"
                "vmulps  %%ymm2, %%ymm0, %%ymm1\n\t"
                "vmulps  %%ymm5, %%ymm3, %%ymm4\n\t"
                "vmulps  %%ymm8, %%ymm6, %%ymm7\n\t"
                :
                :
                : MMREG(14), MMREG(13), MMREG(12), MMREG(11), MMREG(10), MMREG(9), MMREG(8), MMREG(7), MMREG(6), MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }

    //==========_mm256_mul_pd==========
    void kernel_only_asm_mm256_mul_pd_unroll8()
    {
        for(int i = 0; i<STIM_LEN/32; i++)
        {
            asm volatile(
                "vmulpd  %%ymm2, %%ymm0, %%ymm1\n\t"
                "vmulpd  %%ymm5, %%ymm3, %%ymm4\n\t"
                "vmulpd  %%ymm8, %%ymm6, %%ymm7\n\t"
                "vmulpd  %%ymm11, %%ymm9, %%ymm10\n\t"
                "vmulpd  %%ymm14, %%ymm12, %%ymm13\n\t"
                "vmulpd  %%ymm2, %%ymm0, %%ymm1\n\t"
                "vmulpd  %%ymm5, %%ymm3, %%ymm4\n\t"
                "vmulpd  %%ymm8, %%ymm6, %%ymm7\n\t"
                :
                :
                : MMREG(14), MMREG(13), MMREG(12), MMREG(11), MMREG(10), MMREG(9), MMREG(8), MMREG(7), MMREG(6), MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }
#endif

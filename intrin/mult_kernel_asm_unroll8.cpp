#include "intrin_bench_default_defines.h"

#ifdef __AVX2__
    //==========_mm256_mullo_epi16==========
    void kernel_only_asm_mm256_mullo_epi16_unroll8()
    {
        for(int i = 0; i<STIM_LEN/128; i++)
        {
            asm volatile(
                "vpmullw  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmullw  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmullw  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmullw  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmullw  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmullw  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmullw  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmullw  %%ymm14, %%ymm15, %%ymm14\n\t"
                :
                :
                : MMREG(15), MMREG(13), MMREG(12), MMREG(11), MMREG(10), MMREG(9), MMREG(8), MMREG(7), MMREG(6), MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)

            );
        }
    }

    //==========_mm256_mullo_epi32==========
    void kernel_only_asm_mm256_mullo_epi32_unroll8()
    {
        for(int i = 0; i<STIM_LEN/64; i++)
        {
            asm volatile(
                "vpmulld  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmulld  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmulld  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmulld  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmulld  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmulld  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmulld  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmulld  %%ymm14, %%ymm15, %%ymm14\n\t"
                :
                :
                : MMREG(15), MMREG(13), MMREG(12), MMREG(11), MMREG(10), MMREG(9), MMREG(8), MMREG(7), MMREG(6), MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
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
                "vmulps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulps  %%ymm14, %%ymm15, %%ymm14\n\t"
                :
                :
                : MMREG(15), MMREG(13), MMREG(12), MMREG(11), MMREG(10), MMREG(9), MMREG(8), MMREG(7), MMREG(6), MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }

    //==========_mm256_mul_pd==========
    void kernel_only_asm_mm256_mul_pd_unroll8()
    {
        for(int i = 0; i<STIM_LEN/32; i++)
        {
            asm volatile(
                "vmulpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulpd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulpd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulpd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulpd  %%ymm14, %%ymm15, %%ymm14\n\t"
                :
                :
                : MMREG(15), MMREG(13), MMREG(12), MMREG(11), MMREG(10), MMREG(9), MMREG(8), MMREG(7), MMREG(6), MMREG(5), MMREG(4), MMREG(3), MMREG(2), MMREG(1), MMREG(0)
            );
        }
    }
#endif

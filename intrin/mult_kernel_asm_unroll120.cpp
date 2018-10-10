#include "intrin_bench_default_defines.h"

#ifdef __AVX2__
    //==========_mm256_mullo_epi16==========
    void kernel_only_asm_mm256_mullo_epi16_unroll120()
    {
        for(int i = 0; i<STIM_LEN/(128*15); i++)
        {
            asm volatile(
                //1
                "vpmullw  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmullw  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmullw  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmullw  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmullw  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmullw  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmullw  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmullw  %%ymm14, %%ymm15, %%ymm14\n\t"
                //2
                "vpmullw  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmullw  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmullw  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmullw  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmullw  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmullw  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmullw  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmullw  %%ymm14, %%ymm15, %%ymm14\n\t"
                //3
                "vpmullw  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmullw  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmullw  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmullw  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmullw  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmullw  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmullw  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmullw  %%ymm14, %%ymm15, %%ymm14\n\t"
                //4
                "vpmullw  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmullw  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmullw  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmullw  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmullw  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmullw  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmullw  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmullw  %%ymm14, %%ymm15, %%ymm14\n\t"
                //5
                "vpmullw  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmullw  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmullw  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmullw  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmullw  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmullw  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmullw  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmullw  %%ymm14, %%ymm15, %%ymm14\n\t"
                //6
                "vpmullw  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmullw  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmullw  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmullw  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmullw  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmullw  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmullw  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmullw  %%ymm14, %%ymm15, %%ymm14\n\t"
                //7
                "vpmullw  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmullw  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmullw  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmullw  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmullw  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmullw  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmullw  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmullw  %%ymm14, %%ymm15, %%ymm14\n\t"
                //8
                "vpmullw  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmullw  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmullw  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmullw  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmullw  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmullw  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmullw  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmullw  %%ymm14, %%ymm15, %%ymm14\n\t"
                //9
                "vpmullw  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmullw  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmullw  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmullw  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmullw  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmullw  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmullw  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmullw  %%ymm14, %%ymm15, %%ymm14\n\t"
                //10
                "vpmullw  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmullw  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmullw  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmullw  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmullw  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmullw  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmullw  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmullw  %%ymm14, %%ymm15, %%ymm14\n\t"
                //11
                "vpmullw  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmullw  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmullw  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmullw  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmullw  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmullw  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmullw  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmullw  %%ymm14, %%ymm15, %%ymm14\n\t"
                //12
                "vpmullw  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmullw  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmullw  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmullw  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmullw  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmullw  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmullw  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmullw  %%ymm14, %%ymm15, %%ymm14\n\t"
                //13
                "vpmullw  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmullw  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmullw  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmullw  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmullw  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmullw  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmullw  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmullw  %%ymm14, %%ymm15, %%ymm14\n\t"
                //14
                "vpmullw  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmullw  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmullw  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmullw  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmullw  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmullw  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmullw  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmullw  %%ymm14, %%ymm15, %%ymm14\n\t"
                //15
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
    void kernel_only_asm_mm256_mullo_epi32_unroll120()
    {
        for(int i = 0; i<STIM_LEN/(64*15); i++)
        {
            asm volatile(
                //1
                "vpmulld  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmulld  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmulld  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmulld  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmulld  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmulld  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmulld  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmulld  %%ymm14, %%ymm15, %%ymm14\n\t"
                //2
                "vpmulld  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmulld  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmulld  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmulld  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmulld  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmulld  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmulld  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmulld  %%ymm14, %%ymm15, %%ymm14\n\t"
                //3
                "vpmulld  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmulld  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmulld  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmulld  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmulld  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmulld  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmulld  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmulld  %%ymm14, %%ymm15, %%ymm14\n\t"
                //4
                "vpmulld  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmulld  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmulld  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmulld  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmulld  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmulld  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmulld  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmulld  %%ymm14, %%ymm15, %%ymm14\n\t"
                //5
                "vpmulld  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmulld  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmulld  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmulld  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmulld  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmulld  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmulld  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmulld  %%ymm14, %%ymm15, %%ymm14\n\t"
                //6
                "vpmulld  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmulld  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmulld  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmulld  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmulld  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmulld  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmulld  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmulld  %%ymm14, %%ymm15, %%ymm14\n\t"
                //7
                "vpmulld  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmulld  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmulld  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmulld  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmulld  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmulld  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmulld  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmulld  %%ymm14, %%ymm15, %%ymm14\n\t"
                //8
                "vpmulld  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmulld  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmulld  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmulld  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmulld  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmulld  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmulld  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmulld  %%ymm14, %%ymm15, %%ymm14\n\t"
                //9
                "vpmulld  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmulld  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmulld  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmulld  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmulld  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmulld  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmulld  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmulld  %%ymm14, %%ymm15, %%ymm14\n\t"
                //10
                "vpmulld  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmulld  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmulld  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmulld  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmulld  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmulld  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmulld  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmulld  %%ymm14, %%ymm15, %%ymm14\n\t"
                //11
                "vpmulld  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmulld  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmulld  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmulld  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmulld  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmulld  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmulld  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmulld  %%ymm14, %%ymm15, %%ymm14\n\t"
                //12
                "vpmulld  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmulld  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmulld  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmulld  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmulld  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmulld  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmulld  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmulld  %%ymm14, %%ymm15, %%ymm14\n\t"
                //13
                "vpmulld  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmulld  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmulld  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmulld  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmulld  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmulld  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmulld  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmulld  %%ymm14, %%ymm15, %%ymm14\n\t"
                //14
                "vpmulld  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vpmulld  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vpmulld  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vpmulld  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vpmulld  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vpmulld  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vpmulld  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vpmulld  %%ymm14, %%ymm15, %%ymm14\n\t"
                //15
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
    void kernel_only_asm_mm256_mul_ps_unroll120()
    {
        for(int i = 0; i<STIM_LEN/(64*15); i++)
        {
            asm volatile(
                //1
                "vmulps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulps  %%ymm14, %%ymm15, %%ymm14\n\t"
                //2
                "vmulps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulps  %%ymm14, %%ymm15, %%ymm14\n\t"
                //3
                "vmulps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulps  %%ymm14, %%ymm15, %%ymm14\n\t"
                //4
                "vmulps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulps  %%ymm14, %%ymm15, %%ymm14\n\t"
                //5
                "vmulps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulps  %%ymm14, %%ymm15, %%ymm14\n\t"
                //6
                "vmulps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulps  %%ymm14, %%ymm15, %%ymm14\n\t"
                //7
                "vmulps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulps  %%ymm14, %%ymm15, %%ymm14\n\t"
                //8
                "vmulps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulps  %%ymm14, %%ymm15, %%ymm14\n\t"
                //9
                "vmulps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulps  %%ymm14, %%ymm15, %%ymm14\n\t"
                //10
                "vmulps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulps  %%ymm14, %%ymm15, %%ymm14\n\t"
                //11
                "vmulps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulps  %%ymm14, %%ymm15, %%ymm14\n\t"
                //12
                "vmulps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulps  %%ymm14, %%ymm15, %%ymm14\n\t"
                //13
                "vmulps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulps  %%ymm14, %%ymm15, %%ymm14\n\t"
                //14
                "vmulps  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulps  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulps  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulps  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulps  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulps  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulps  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulps  %%ymm14, %%ymm15, %%ymm14\n\t"
                //15
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
    void kernel_only_asm_mm256_mul_pd_unroll120()
    {
        for(int i = 0; i<STIM_LEN/(32*15); i++)
        {
            asm volatile(
                //1
                "vmulpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulpd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulpd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulpd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulpd  %%ymm14, %%ymm15, %%ymm14\n\t"
                //2
                "vmulpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulpd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulpd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulpd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulpd  %%ymm14, %%ymm15, %%ymm14\n\t"
                //3
                "vmulpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulpd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulpd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulpd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulpd  %%ymm14, %%ymm15, %%ymm14\n\t"
                //4
                "vmulpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulpd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulpd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulpd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulpd  %%ymm14, %%ymm15, %%ymm14\n\t"
                //5
                "vmulpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulpd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulpd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulpd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulpd  %%ymm14, %%ymm15, %%ymm14\n\t"
                //6
                "vmulpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulpd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulpd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulpd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulpd  %%ymm14, %%ymm15, %%ymm14\n\t"
                //7
                "vmulpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulpd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulpd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulpd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulpd  %%ymm14, %%ymm15, %%ymm14\n\t"
                //8
                "vmulpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulpd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulpd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulpd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulpd  %%ymm14, %%ymm15, %%ymm14\n\t"
                //9
                "vmulpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulpd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulpd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulpd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulpd  %%ymm14, %%ymm15, %%ymm14\n\t"
                //10
                "vmulpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulpd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulpd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulpd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulpd  %%ymm14, %%ymm15, %%ymm14\n\t"
                //11
                "vmulpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulpd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulpd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulpd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulpd  %%ymm14, %%ymm15, %%ymm14\n\t"
                //12
                "vmulpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulpd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulpd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulpd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulpd  %%ymm14, %%ymm15, %%ymm14\n\t"
                //13
                "vmulpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulpd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulpd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulpd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulpd  %%ymm14, %%ymm15, %%ymm14\n\t"
                //14
                "vmulpd  %%ymm0, %%ymm1, %%ymm0\n\t"
                "vmulpd  %%ymm2, %%ymm3, %%ymm2\n\t"
                "vmulpd  %%ymm4, %%ymm5, %%ymm4\n\t"
                "vmulpd  %%ymm6, %%ymm7, %%ymm6\n\t"
                "vmulpd  %%ymm8, %%ymm9, %%ymm8\n\t"
                "vmulpd  %%ymm10, %%ymm11, %%ymm10\n\t"
                "vmulpd  %%ymm12, %%ymm13, %%ymm12\n\t"
                "vmulpd  %%ymm14, %%ymm15, %%ymm14\n\t"
                //15
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

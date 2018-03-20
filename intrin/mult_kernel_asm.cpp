#include "intrin_bench_default_defines.h"

#ifdef __AVX2__
    //==========_mm256_mullo_epi16==========
    void kernel_only_asm_mm256_mullo_epi16()
    {
        for(int i = 0; i<STIM_LEN/16; i++)
        {
            asm volatile(
                "vpmullw  %%ymm2, %%ymm0, %%ymm1\n\t"
                :
            );
        }
    }

    //==========_mm256_mullo_epi32==========
    void kernel_only_asm_mm256_mullo_epi32()
    {
        for(int i = 0; i<STIM_LEN/8; i++)
        {
            asm volatile(
                "vpmulld  %%ymm2, %%ymm0, %%ymm1\n\t"
                :
            );
        }
    }
#endif

#ifdef __AVX__
    //==========_mm256_mul_ps==========
    void kernel_only_asm_mm256_mul_ps()
    {
        for(int i = 0; i<STIM_LEN; i+=8)
        {
            asm volatile(
                "vmulps  %%ymm2, %%ymm0, %%ymm1\n\t"
                :
            );
        }
    }

    //==========_mm256_mul_pd==========
    void kernel_only_asm_mm256_mul_pd()
    {
        for(int i = 0; i<STIM_LEN; i+=4)
        {
            asm volatile(
                "vmulpd  %%ymm2, %%ymm0, %%ymm1\n\t"
                :
            );
        }
    }
#endif

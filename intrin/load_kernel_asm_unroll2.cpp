#include "intrin_bench_default_defines.h"

#ifdef __AVX__
    //==========_mm256_load_si256 int8==========
    void kernel_asm_mm256_load_si256_int8_unroll2( __m256i* a)
    {
        for(int i = 0; i<STIM_LEN/32; i+=2)
        {
            __m256i* b = a+i;
            __m256i* c = b+1;

            asm volatile(
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                :
                : "m" (*b), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_load_si256 int16==========
    void kernel_asm_mm256_load_si256_int16_unroll2( __m256i* a)
    {
        for(int i = 0; i<STIM_LEN/16; i+=2)
        {
            __m256i* b = a+i;
            __m256i* c = b+1;

            asm volatile(
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                :
                : "m" (*b), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_load_si256 int32==========
    void kernel_asm_mm256_load_si256_int32_unroll2( __m256i* a)
    {
        for(int i = 0; i<STIM_LEN/8; i+=2)
        {
            __m256i* b = a+i;
            __m256i* c = b+1;

            asm volatile(
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                :
                : "m" (*b), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_load_ps==========
    void kernel_asm_mm256_load_ps_unroll2( __m256* a)
    {
        float* a_float = (float*) a;

        for(int i = 0; i<STIM_LEN; i+=16)
        {
            float* b = a_float+i;
            float* c = b+8;

            asm volatile(
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                :
                : "m" (*b), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_load_pd==========
    void kernel_asm_mm256_load_pd_unroll2( __m256d* a)
    {
        double* a_double = (double*) a;

        for(int i = 0; i<STIM_LEN; i+=8)
        {
            double* b = a_double+i;
            double* c = b+4;

            asm volatile(
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                :
                : "m" (*b), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }
#endif
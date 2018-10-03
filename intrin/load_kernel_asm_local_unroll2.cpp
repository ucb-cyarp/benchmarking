#include "intrin_bench_default_defines.h"

#ifdef __AVX__
    //==========_mm256_load_si256 int8==========
    void kernel_asm_mm256_load_si256_int8_local_unroll2( __m256i* a)
    {
        __m256i* c = a+1;
        for(int i = 0; i<STIM_LEN/32; i+=2)
        {
            asm volatile(
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                :
                : "m" (*a), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_load_si256 int16==========
    void kernel_asm_mm256_load_si256_int16_local_unroll2( __m256i* a)
    {
        __m256i* c = a+1;

        for(int i = 0; i<STIM_LEN/16; i+=2)
        {
            asm volatile(
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                :
                : "m" (*a), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_load_si256 int32==========
    void kernel_asm_mm256_load_si256_int32_local_unroll2( __m256i* a)
    {
        __m256i* c = a+1;

        for(int i = 0; i<STIM_LEN/8; i+=2)
        {
            asm volatile(
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                :
                : "m" (*a), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_load_si256 int64==========
    void kernel_asm_mm256_load_si256_int64_local_unroll2( __m256i* a)
    {
        __m256i* c = a+1;
        for(int i = 0; i<STIM_LEN/4; i+=2)
        {
            asm volatile(
                "vmovdqa  %0, %%ymm0\n\t"
                "vmovdqa  %1, %%ymm1\n\t"
                :
                : "m" (*a), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_load_ps==========
    void kernel_asm_mm256_load_ps_local_unroll2( __m256* a)
    {
        float* a_float = (float*) a;
        float* c = a_float+8;

        for(int i = 0; i<STIM_LEN; i+=16)
        {
            asm volatile(
                "vmovaps  %0, %%ymm0\n\t"
                "vmovaps  %1, %%ymm1\n\t"
                :
                : "m" (*a_float), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_load_pd==========
    void kernel_asm_mm256_load_pd_local_unroll2( __m256d* a)
    {
        double* a_double = (double*) a;
        double* c = a_double+4;

        for(int i = 0; i<STIM_LEN; i+=8)
        {
            asm volatile(
                "vmovapd   %0, %%ymm0\n\t"
                "vmovapd   %1, %%ymm1\n\t"
                :
                : "m" (*a_double), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }
#endif
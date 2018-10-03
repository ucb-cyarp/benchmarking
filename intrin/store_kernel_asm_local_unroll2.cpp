#include "intrin_bench_default_defines.h"

#ifdef __AVX__
    //==========_mm256_store_si256 int8==========
    void kernel_asm_mm256_store_si256_int8_local_unroll2( __m256i* a)
    {
        __m256i* c = a+1;
        for(int i = 0; i<STIM_LEN/32; i+=2)
        {
            asm volatile(
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                :
                : "m" (*a), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_store_si256 int16==========
    void kernel_asm_mm256_store_si256_int16_local_unroll2( __m256i* a)
    {
        // __m256i a_val = _mm256_set_epi32(0, 1, 2, 3, 4, 5, 6, 7);
        __m256i* c = a+1;
        for(int i = 0; i<STIM_LEN/16; i+=2)
        {
            asm volatile(
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                :
                : "m" (*a), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_store_si256 int32==========
    void kernel_asm_mm256_store_si256_int32_local_unroll2( __m256i* a)
    {
        // __m256i a_val = _mm256_set_epi32(0, 1, 2, 3, 4, 5, 6, 7);
        __m256i* c = a+1;
        for(int i = 0; i<STIM_LEN/8; i+=2)
        {
            asm volatile(
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                :
                : "m" (*a), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_store_si256 int64==========
    void kernel_asm_mm256_store_si256_int64_local_unroll2( __m256i* a)
    {
        // __m256i a_val = _mm256_set_epi32(0, 1, 2, 3);
        __m256i* c = a+1;
        for(int i = 0; i<STIM_LEN/4; i+=2)
        {
            asm volatile(
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                :
                : "m" (*a), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_store_ps==========
    void kernel_asm_mm256_store_ps_local_unroll2( __m256* a)
    {
        float* a_float = (float*) a;
        // __m256 a_val = _mm256_set_ps(0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0);
        float* c = a_float+8;
        for(int i = 0; i<STIM_LEN; i+=16)
        {
            asm volatile(
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                :
                : "m" (*a), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_store_pd==========
    void kernel_asm_mm256_store_pd_local_unroll2( __m256d* a)
    {
        double* a_double = (double*) a;
        // __m256d a_val = _mm256_set_pd(0.0, 1.0, 2.0, 3.0);
        double* c = a_double+4;

        for(int i = 0; i<STIM_LEN; i+=8)
        {
            asm volatile(
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                :
                : "m" (*a), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }
#endif
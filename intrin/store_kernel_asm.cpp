#include "intrin_bench_default_defines.h"

#ifdef __AVX__
    //==========_mm256_store_si256 int8==========
    void kernel_asm_mm256_store_si256_int8( __m256i* a)
    {
        for(int i = 0; i<STIM_LEN/32; i++)
        {
            __m256i* b = a+i;

            asm volatile(
                "vmovdqa  %%ymm0, %0\n\t"
                :
                : "m" (*b)
                : MMREG(0)
            );
        }
    }

    //==========_mm256_store_si256 int16==========
    void kernel_asm_mm256_store_si256_int16( __m256i* a)
    {
        // __m256i a_val = _mm256_set_epi32(0, 1, 2, 3, 4, 5, 6, 7);

        for(int i = 0; i<STIM_LEN/16; i++)
        {
            __m256i* b = a+i;

            asm volatile(
                "vmovdqa  %%ymm0, %0\n\t"
                :
                : "m" (*b)
                : MMREG(0)
            );
        }
    }

    //==========_mm256_store_si256 int32==========
    void kernel_asm_mm256_store_si256_int32( __m256i* a)
    {
        // __m256i a_val = _mm256_set_epi32(0, 1, 2, 3, 4, 5, 6, 7);

        for(int i = 0; i<STIM_LEN/8; i++)
        {
            __m256i* b = a+i;

            asm volatile(
                "vmovdqa  %%ymm0, %0\n\t"
                :
                : "m" (*b)
                : MMREG(0)
            );
        }
    }

    //==========_mm256_store_si256 int64==========
    void kernel_asm_mm256_store_si256_int64( __m256i* a)
    {
        // __m256i a_val = _mm256_set_epi64(0, 1, 2, 3);

        for(int i = 0; i<STIM_LEN/4; i++)
        {
            __m256i* b = a+i;

            asm volatile(
                "vmovdqa  %%ymm0, %0\n\t"
                :
                : "m" (*b)
                : MMREG(0)
            );
        }
    }

    //==========_mm256_store_ps==========
    void kernel_asm_mm256_store_ps( __m256* a)
    {
        float* a_float = (float*) a;

        // __m256 a_val = _mm256_set_ps(0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0);

        for(int i = 0; i<STIM_LEN; i+=8)
        {
            float* b = a_float+i;

            asm volatile(
                "vmovaps  %%ymm0, %0\n\t"
                :
                : "m" (*b)
                : MMREG(0)
            );
        }
    }

    //==========_mm256_store_pd==========
    void kernel_asm_mm256_store_pd( __m256d* a)
    {
        double* a_double = (double*) a;

        // __m256d a_val = _mm256_set_pd(0.0, 1.0, 2.0, 3.0);

        for(int i = 0; i<STIM_LEN; i+=4)
        {
            double* b = a_double+i;

            asm volatile(
                "vmovapd  %%ymm0, %0\n\t"
                :
                : "m" (*b)
                : MMREG(0)
            );
        }
    }
#endif
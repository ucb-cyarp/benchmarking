#include "intrin_bench_default_defines_and_imports_cpp.h"

#ifdef __AVX__
    //==========_mm256_store_si256 int8==========
    void kernel_asm_mm256_store_si256_int8_local( __m256i* a)
    {
        for(int i = 0; i<STIM_LEN/32; i++)
        {
            asm volatile(
                "vmovdqa  %%ymm0, %0\n\t"
                :
                : "m" (*a)
                : MMREG(0)
            );
        }
    }

    //==========_mm256_store_si256 int16==========
    void kernel_asm_mm256_store_si256_int16_local( __m256i* a)
    {
        // __m256i a_val = _mm256_set_epi32(0, 1, 2, 3, 4, 5, 6, 7);

        for(int i = 0; i<STIM_LEN/16; i++)
        {
            asm volatile(
                "vmovdqa  %%ymm0, %0\n\t"
                :
                : "m" (*a)
                : MMREG(0)
            );
        }
    }

    //==========_mm256_store_si256 int32==========
    void kernel_asm_mm256_store_si256_int32_local( __m256i* a)
    {
        // __m256i a_val = _mm256_set_epi32(0, 1, 2, 3, 4, 5, 6, 7);

        for(int i = 0; i<STIM_LEN/8; i++)
        {
            asm volatile(
                "vmovdqa  %%ymm0, %0\n\t"
                :
                : "m" (*a)
                : MMREG(0)
            );
        }
    }

    //==========_mm256_store_si256 int64==========
    void kernel_asm_mm256_store_si256_int64_local( __m256i* a)
    {
        // __m256i a_val = _mm256_set_epi64(0, 1, 2, 3);

        for(int i = 0; i<STIM_LEN/4; i++)
        {
            asm volatile(
                "vmovdqa  %%ymm0, %0\n\t"
                :
                : "m" (*a)
                : MMREG(0)
            );
        }
    }

    //==========_mm256_store_ps==========
    void kernel_asm_mm256_store_ps_local( __m256* a)
    {
        float* a_float = (float*) a;

        // __m256 a_val = _mm256_set_ps(0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0);

        for(int i = 0; i<STIM_LEN; i+=8)
        {
            asm volatile(
                "vmovaps  %%ymm0, %0\n\t"
                :
                : "m" (*a_float)
                : MMREG(0)
            );
        }
    }

    //==========_mm256_store_pd==========
    void kernel_asm_mm256_store_pd_local( __m256d* a)
    {
        double* a_double = (double*) a;

        // __m256d a_val = _mm256_set_pd(0.0, 1.0, 2.0, 3.0);

        for(int i = 0; i<STIM_LEN; i+=4)
        {
            asm volatile(
                "vmovapd  %%ymm0, %0\n\t"
                :
                : "m" (*a_double)
                : MMREG(0)
            );
        }
    }
#endif
#include "intrin_bench_default_defines_and_imports_cpp.h"

#ifdef __AVX__
    //==========_mm256_store_si256 int8==========
    void kernel_asm_mm256_store_si256_int8_unroll2( __m256i* a)
    {
        for(int i = 0; i<STIM_LEN/32; i+=2)
        {
            __m256i* b = a+i;
            __m256i* c = b+1;

            asm volatile(
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                :
                : "m" (*b), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_store_si256 int16==========
    void kernel_asm_mm256_store_si256_int16_unroll2( __m256i* a)
    {
        // __m256i a_val = _mm256_set_epi32(0, 1, 2, 3, 4, 5, 6, 7);

        for(int i = 0; i<STIM_LEN/16; i+=2)
        {
            __m256i* b = a+i;
            __m256i* c = b+1;

            asm volatile(
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                :
                : "m" (*b), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_store_si256 int32==========
    void kernel_asm_mm256_store_si256_int32_unroll2( __m256i* a)
    {
        // __m256i a_val = _mm256_set_epi32(0, 1, 2, 3, 4, 5, 6, 7);

        for(int i = 0; i<STIM_LEN/8; i+=2)
        {
            __m256i* b = a+i;
            __m256i* c = b+1;

            asm volatile(
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                :
                : "m" (*b), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_store_si256 int64==========
    void kernel_asm_mm256_store_si256_int64_unroll2( __m256i* a)
    {
        // __m256i a_val = _mm256_set_epi32(0, 1, 2, 3);

        for(int i = 0; i<STIM_LEN/4; i+=2)
        {
            __m256i* b = a+i;
            __m256i* c = b+1;

            asm volatile(
                "vmovdqa  %%ymm0, %0\n\t"
                "vmovdqa  %%ymm1, %1\n\t"
                :
                : "m" (*b), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_store_ps==========
    void kernel_asm_mm256_store_ps_unroll2( __m256* a)
    {
        float* a_float = (float*) a;

        // __m256 a_val = _mm256_set_ps(0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0);

        for(int i = 0; i<STIM_LEN; i+=16)
        {
            float* b = a_float+i;
            float* c = b+8;

            asm volatile(
                "vmovaps  %%ymm0, %0\n\t"
                "vmovaps  %%ymm1, %1\n\t"
                :
                : "m" (*b), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }

    //==========_mm256_store_pd==========
    void kernel_asm_mm256_store_pd_unroll2( __m256d* a)
    {
        double* a_double = (double*) a;

        // __m256d a_val = _mm256_set_pd(0.0, 1.0, 2.0, 3.0);

        for(int i = 0; i<STIM_LEN; i+=8)
        {
            double* b = a_double+i;
            double* c = b+4;

            asm volatile(
                "vmovapd  %%ymm0, %0\n\t"
                "vmovapd  %%ymm1, %1\n\t"
                :
                : "m" (*b), "m" (*c)
                : MMREG(0), MMREG(1)
            );
        }
    }
#endif
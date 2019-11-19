#include "intrin_bench_default_defines_and_imports_cpp.h"

#ifdef __AVX__
    //==========_mm256_load_si256 int8==========
    void kernel_mm256_load_si256_int8( __m256i* a)
    {
        for(int i = 0; i<STIM_LEN/32; i++)
        {
            __m256i a_val = _mm256_load_si256(a+i);
        }
    }

    //==========_mm256_load_si256 int16==========
    void kernel_mm256_load_si256_int16( __m256i* a)
    {
        for(int i = 0; i<STIM_LEN/16; i++)
        {
            __m256i a_val = _mm256_load_si256(a+i);
        }
    }

    //==========_mm256_load_si256 int32==========
    void kernel_mm256_load_si256_int32( __m256i* a)
    {
        for(int i = 0; i<STIM_LEN/8; i++)
        {
            __m256i a_val = _mm256_load_si256(a+i);
        }
    }

    //==========_mm256_load_ps==========
    void kernel_mm256_load_ps( __m256* a)
    {
        float* a_float = (float*) a;

        for(int i = 0; i<STIM_LEN; i+=8)
        {
            __m256 a_val = _mm256_load_ps(a_float+i);
        }
    }

    //==========_mm256_load_pd==========
    void kernel_mm256_load_pd( __m256d* a)
    {
        double* a_double = (double*) a;

        for(int i = 0; i<STIM_LEN; i+=4)
        {
            __m256d a_val = _mm256_load_pd(a_double+i);
        }
    }
#endif
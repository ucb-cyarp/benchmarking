#include "intrin_bench_default_defines_and_imports_cpp.h"d_imports_cpp.h"

#ifdef __AVX__
    //==========_mm256_load_si256 | _mm256_store_si256 int8==========
    void kernel_mm256_load_si256__mm256_store_si256_int8( __m256i* a, __m256i* b)
    {
        for(int i = 0; i<STIM_LEN/32; i++)
        {
            __m256i a_val = _mm256_load_si256(a+i);
            _mm256_store_si256(b+i, a_val);
        }
    }

    //==========_mm256_load_si256 | _mm256_store_si256 int16==========
    void kernel_mm256_load_si256__mm256_store_si256_int16( __m256i* a, __m256i* b)
    {
        for(int i = 0; i<STIM_LEN/16; i++)
        {
            __m256i a_val = _mm256_load_si256(a+i);
            _mm256_store_si256(b+i, a_val);
        }
    }

    //==========_mm256_load_si256 | _mm256_store_si256 int32==========
    void kernel_mm256_load_si256__mm256_store_si256_int32( __m256i* a, __m256i* b)
    {
        for(int i = 0; i<STIM_LEN/8; i++)
        {
            __m256i a_val = _mm256_load_si256(a+i);
            _mm256_store_si256(b+i, a_val);
        }
    }

    //==========_mm256_load_si256 | _mm256_store_si256 int64==========
    void kernel_mm256_load_si256__mm256_store_si256_int64( __m256i* a, __m256i* b)
    {
        for(int i = 0; i<STIM_LEN/4; i++)
        {
            __m256i a_val = _mm256_load_si256(a+i);
            _mm256_store_si256(b+i, a_val);
        }
    }

    //==========kernel_mm256_load_ps | _mm256_store_ps==========
    void kernel_mm256_load_ps__mm256_store_ps( __m256* a, __m256* b)
    {
        float* a_float = (float*) a;
        float* b_float = (float*) b;

        for(int i = 0; i<STIM_LEN; i+=8)
        {
            __m256 a_val = _mm256_load_ps(a_float+i);
            _mm256_store_ps(b_float+i, a_val);
        }
    }

    //==========_mm256_load_pd | _mm256_store_pd==========
    void kernel_mm256_load_pd__mm256_store_pd( __m256d* a, __m256d* b)
    {
        double* a_double = (double*) a;
        double* b_double = (double*) b;

        for(int i = 0; i<STIM_LEN; i+=4)
        {
            __m256d a_val = _mm256_load_pd(a_double+i);
            _mm256_store_pd(b_double+i, a_val);
        }
    }
#endif

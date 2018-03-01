#include "intrin_bench_default_defines.h"

#ifdef __AVX2__
    //==========_mm256_add_epi8==========
    void kernel_mm256_add_epi8( __m256i* a, __m256i* b, __m256i* c)
    {
        for(int i = 0; i<STIM_LEN/32; i++)
        {
            __m256i a_val = _mm256_load_si256(a+i);
            __m256i b_val = _mm256_load_si256(b+i);
            __m256i c_val = _mm256_add_epi8(a_val, b_val);
            _mm256_store_si256(c+i, c_val);
        }
    }

    //==========_mm256_add_epi16==========
    void kernel_mm256_add_epi16( __m256i* a, __m256i* b, __m256i* c)
    {
        for(int i = 0; i<STIM_LEN/16; i++)
        {
            __m256i a_val = _mm256_load_si256(a+i);
            __m256i b_val = _mm256_load_si256(b+i);
            __m256i c_val = _mm256_add_epi16(a_val, b_val);
            _mm256_store_si256(c+i, c_val);
        }
    }

    //==========_mm256_add_epi32==========
    void kernel_mm256_add_epi32( __m256i* a, __m256i* b, __m256i* c)
    {
        for(int i = 0; i<STIM_LEN/8; i++)
        {
            __m256i a_val = _mm256_load_si256(a+i);
            __m256i b_val = _mm256_load_si256(b+i);
            __m256i c_val = _mm256_add_epi32(a_val, b_val);
            _mm256_store_si256(c+i, c_val);
        }
    }
#endif

#ifdef __AVX__
    //==========_mm256_add_ps==========
    void kernel_mm256_add_ps( __m256* a, __m256* b, __m256* c)
    {
        float* a_float = (float*) a;
        float* b_float = (float*) b;
        float* c_float = (float*) c;

        for(int i = 0; i<STIM_LEN; i+=8)
        {
            __m256 a_val = _mm256_load_ps(a_float+i);
            __m256 b_val = _mm256_load_ps(b_float+i);
            __m256 c_val = _mm256_add_ps(a_val, b_val);
            _mm256_store_ps(c_float+i, c_val);
        }
    }

    //==========_mm256_add_pd==========
    void kernel_mm256_add_pd( __m256d* a, __m256d* b, __m256d* c)
    {
        double* a_double = (double*) a;
        double* b_double = (double*) b;
        double* c_double = (double*) c;

        for(int i = 0; i<STIM_LEN; i+=4)
        {
            __m256d a_val = _mm256_load_pd(a_double+i);
            __m256d b_val = _mm256_load_pd(b_double+i);
            __m256d c_val = _mm256_add_pd(a_val, b_val);
            _mm256_store_pd(c_double+i, c_val);
        }
    }
#endif

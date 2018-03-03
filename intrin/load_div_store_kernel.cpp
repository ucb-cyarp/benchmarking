#include "intrin_bench_default_defines.h"

#ifdef __AVX__
    //==========_mm256_div_ps==========
    void kernel_mm256_div_ps( __m256* a, __m256* b, __m256* c)
    {
        float* a_float = (float*) a;
        float* b_float = (float*) b;
        float* c_float = (float*) c;

        for(int i = 0; i<STIM_LEN; i+=8)
        {
            __m256 a_val = _mm256_load_ps(a_float+i);
            __m256 b_val = _mm256_load_ps(b_float+i);
            __m256 c_val = _mm256_div_ps(a_val, b_val);
            _mm256_store_ps(c_float+i, c_val);
        }
    }

    //==========_mm256_div_pd==========
    void kernel_mm256_div_pd( __m256d* a, __m256d* b, __m256d* c)
    {
        double* a_double = (double*) a;
        double* b_double = (double*) b;
        double* c_double = (double*) c;

        for(int i = 0; i<STIM_LEN; i+=4)
        {
            __m256d a_val = _mm256_load_pd(a_double+i);
            __m256d b_val = _mm256_load_pd(b_double+i);
            __m256d c_val = _mm256_div_pd(a_val, b_val);
            _mm256_store_pd(c_double+i, c_val);
        }
    }
#endif

#include "intrin_bench_default_defines_and_imports_cpp.h"

#ifdef __FMA__
    //==========_mm256_fmadd_ps==========
    void kernel_mm256_fmadd_ps( __m256* a, __m256* b, __m256* c, __m256* d)
    {
        float* a_float = (float*) a;
        float* b_float = (float*) b;
        float* c_float = (float*) c;
        float* d_float = (float*) d;

        for(int i = 0; i<STIM_LEN; i+=8)
        {
            __m256 a_val = _mm256_load_ps(a_float+i);
            __m256 b_val = _mm256_load_ps(b_float+i);
            __m256 c_val = _mm256_load_ps(c_float+i);
            __m256 d_val = _mm256_fmadd_ps(a_val, b_val, c_val);
            _mm256_store_ps(d_float+i, d_val);
        }
    }

    //==========_mm256_fmadd_pd==========
    void kernel_mm256_fmadd_pd( __m256d* a, __m256d* b, __m256d* c, __m256d* d)
    {
        double* a_double = (double*) a;
        double* b_double = (double*) b;
        double* c_double = (double*) c;
        double* d_double = (double*) d;

        for(int i = 0; i<STIM_LEN; i+=4)
        {
            __m256d a_val = _mm256_load_pd(a_double+i);
            __m256d b_val = _mm256_load_pd(b_double+i);
            __m256d c_val = _mm256_load_pd(c_double+i);
            __m256d d_val = _mm256_fmadd_pd(a_val, b_val, c_val);
            _mm256_store_pd(d_double+i, d_val);
        }
    }
#endif

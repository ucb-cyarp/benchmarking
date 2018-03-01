#include "intrin_bench_default_defines.h"

#ifdef __AVX__
    //==========_mm256_store_si256==========
    void kernel_mm256_store_si256( __m256i* a)
    {
        __m256i a_val = _mm256_set_epi32(0, 1, 2, 3, 4, 5, 6, 7);

        for(int i = 0; i<STIM_LEN/32; i++)
        {
            _mm256_store_si256(a+i, a_val);
        }
    }

    //==========_mm256_store_ps==========
    void kernel_mm256_store_ps( __m256* a)
    {
        float* a_float = (float*) a;

        __m256 a_val = _mm256_set_ps(0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0);

        for(int i = 0; i<STIM_LEN; i+=8)
        {
            _mm256_store_ps(a_float+i, a_val);
        }
    }

    //==========_mm256_store_pd==========
    void kernel_mm256_store_pd( __m256d* a)
    {
        double* a_double = (double*) a;

        __m256d a_val = _mm256_set_pd(0.0, 1.0, 2.0, 3.0);

        for(int i = 0; i<STIM_LEN; i+=4)
        {
            _mm256_store_pd(a_double+i, a_val);
        }
    }
#endif
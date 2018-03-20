#include "intrin_bench_default_defines.h"

#ifdef __FMA__
    //==========_mm256_fmadd_ps==========
    void kernel_only_mm256_fmadd_ps()
    {
        __m256 a_val = _mm256_set_ps(0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0);
        __m256 b_val = _mm256_set_ps(7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0);
        __m256 c_val = _mm256_set_ps(0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0);

        for(int i = 0; i<STIM_LEN; i+=8)
        {
            _mm256_fmadd_ps(a_val, b_val, c_val);
        }
    }

    //==========_mm256_fmadd_pd==========
    void kernel_only_mm256_fmadd_pd()
    {
        __m256d a_val = _mm256_set_pd(0.0, 1.0, 2.0, 3.0);
        __m256d b_val = _mm256_set_pd(3.0, 2.0, 1.0, 0.0);
        __m256d c_val = _mm256_set_pd(0.0, 1.0, 2.0, 3.0);

        for(int i = 0; i<STIM_LEN; i+=4)
        {
            _mm256_fmadd_pd(a_val, b_val, c_val);
        }
    }
#endif

#include "intrin_bench_default_defines_and_imports_cpp.h"

#ifdef __AVX__
    //==========_mm256_div_ps==========
    void kernel_only_mm256_div_ps()
    {
        __m256 a_val = _mm256_set_ps(0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0);
        __m256 b_val = _mm256_set_ps(7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0);

        for(int i = 0; i<STIM_LEN; i+=8)
        {
            __m256 c_val = _mm256_div_ps(a_val, b_val);
        }
    }

    //==========_mm256_div_pd==========
    void kernel_only_mm256_div_pd()
    {
        __m256d a_val = _mm256_set_pd(0.0, 1.0, 2.0, 3.0);
        __m256d b_val = _mm256_set_pd(3.0, 2.0, 1.0, 0.0);

        for(int i = 0; i<STIM_LEN; i+=4)
        {
            __m256d c_val = _mm256_div_pd(a_val, b_val);
        }
    }
#endif

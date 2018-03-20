#include "intrin_bench_default_defines.h"

#ifdef __AVX2__
    //==========_mm256_add_epi8==========
    void kernel_only_mm256_add_epi8()
    {
        __m256i a_val = _mm256_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31);
        __m256i b_val = _mm256_set_epi8(31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);

        for(int i = 0; i<STIM_LEN/32; i++)
        {
            _mm256_add_epi8(a_val, b_val);
        }
    }

    //==========_mm256_add_epi16==========
    void kernel_only_mm256_add_epi16()
    {
        __m256i a_val = _mm256_set_epi16(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
        __m256i b_val = _mm256_set_epi16(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);

        for(int i = 0; i<STIM_LEN/16; i++)
        {
            _mm256_add_epi16(a_val, b_val);
        }
    }

    //==========_mm256_add_epi32==========
    void kernel_only_mm256_add_epi32()
    {
        __m256i a_val = _mm256_set_epi32(0, 1, 2, 3, 4, 5, 6, 7);
        __m256i b_val = _mm256_set_epi32(7, 6, 5, 4, 3, 2, 1, 0);
        
        for(int i = 0; i<STIM_LEN/8; i++)
        {
            _mm256_add_epi32(a_val, b_val);
        }
    }
#endif

#ifdef __AVX__
    //==========_mm256_add_ps==========
    void kernel_only_mm256_add_ps()
    {
        __m256 a_val = _mm256_set_ps(0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0);
        __m256 b_val = _mm256_set_ps(7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0);

        for(int i = 0; i<STIM_LEN; i+=8)
        {
            _mm256_add_ps(a_val, b_val);
        }
    }

    //==========_mm256_add_pd==========
    void kernel_only_mm256_add_pd()
    {
        __m256d a_val = _mm256_set_pd(0.0, 1.0, 2.0, 3.0);
        __m256d b_val = _mm256_set_pd(3.0, 2.0, 1.0, 0.0);

        for(int i = 0; i<STIM_LEN; i+=4)
        {
            _mm256_add_pd(a_val, b_val);
        }
    }
#endif

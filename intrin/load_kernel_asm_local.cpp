#include "intrin_bench_default_defines_and_imports_cpp.h"

#ifdef __AVX__
    //==========_mm256_load_si256 int8 local==========
    void kernel_asm_mm256_load_si256_int8_local( __m256i* a)
    {
        for(int i = 0; i<STIM_LEN/32; i++)
        {

            asm volatile(
                "vmovdqa  %0, %%ymm0\n\t"
                :
                : "m" (*a)
                : MMREG(0)
            );
        }
    }

    //==========_mm256_load_si256 int16 local==========
    void kernel_asm_mm256_load_si256_int16_local( __m256i* a)
    {
        for(int i = 0; i<STIM_LEN/16; i++)
        {
            asm volatile(
                "vmovdqa  %0, %%ymm0\n\t"
                :
                : "m" (*a)
                : MMREG(0)
            );
        }
    }

    //==========_mm256_load_si256 int32 local==========
    void kernel_asm_mm256_load_si256_int32_local( __m256i* a)
    {
        for(int i = 0; i<STIM_LEN/8; i++)
        {
            asm volatile(
                "vmovdqa  %0, %%ymm0\n\t"
                :
                : "m" (*a)
                : MMREG(0)
            );
        }
    }

    //==========_mm256_load_si256 int64 local==========
    void kernel_asm_mm256_load_si256_int64_local( __m256i* a)
    {
        for(int i = 0; i<STIM_LEN/4; i++)
        {
            asm volatile(
                "vmovdqa  %0, %%ymm0\n\t"
                :
                : "m" (*a)
                : MMREG(0)
            );
        }
    }

    //==========_mm256_load_ps local==========
    void kernel_asm_mm256_load_ps_local( __m256* a)
    {
        float* a_float = (float*) a;

        for(int i = 0; i<STIM_LEN; i+=8)
        {
            asm volatile(
                "vmovaps  %0, %%ymm0\n\t"
                :
                : "m" (*a_float)
                : MMREG(0)
            );
        }
    }

    //==========_mm256_load_pd local==========
    void kernel_asm_mm256_load_pd_local( __m256d* a)
    {
        double* a_double = (double*) a;

        for(int i = 0; i<STIM_LEN; i+=4)
        {
            asm volatile(
                "vmovapd   %0, %%ymm0\n\t"
                :
                : "m" (*a_double)
                : MMREG(0)
            );
        }
    }
#endif
#include "intrin_bench_default_defines_and_imports_cpp.h"

    //==========load int8==========
    void kernel_asm_load_int8_local( __m256i* a)
    {
        int8_t* a_cast = (int8_t*) a;
        for(int i = 0; i<STIM_LEN; i++)
        {
            asm volatile(
                "movb  %0, %%al\n\t"
                :
                : "m" (*a_cast)
                : "al"
            );
        }
    }

    //==========load int16==========
    void kernel_asm_load_int16_local( __m256i* a)
    {
        int16_t* a_cast = (int16_t*) a;
        for(int i = 0; i<STIM_LEN; i++)
        {
            asm volatile(
                "movw  %0, %%ax\n\t"
                :
                : "m" (*a_cast)
                : "ax"
            );
        }
    }

    //==========load int32==========
    void kernel_asm_load_int32_local( __m256i* a)
    {
        int32_t* a_cast = (int32_t*) a;
        for(int i = 0; i<STIM_LEN; i++)
        {
            asm volatile(
                "movl  %0, %%eax\n\t"
                :
                : "m" (*a_cast)
                : "eax"
            );
        }
    }

    //==========load int64==========
    void kernel_asm_load_int64_local( __m256i* a)
    {
        int64_t* a_cast = (int64_t*) a;
        for(int i = 0; i<STIM_LEN; i++)
        {
            asm volatile(
                "movq  %0, %%rax\n\t"
                :
                : "m" (*a_cast)
                : "rax"
            );
        }
    }

    #ifdef __SSE2__
    //==========load float==========
    void kernel_asm_load_ss_local( __m256* a)
    {
        float* a_float = (float*) a;

        for(int i = 0; i<STIM_LEN; i++)
        {
            asm volatile(
                "vmovss  %0, %%xmm0\n\t"
                :
                : "m" (*a_float)
                : "xmm0"
            );
        }
    }

    //==========load double==========
    void kernel_asm_load_sd_local( __m256d* a)
    {
        double* a_double = (double*) a;

        for(int i = 0; i<STIM_LEN; i++)
        {
            asm volatile(
                "vmovsd   %0, %%xmm0\n\t"
                :
                : "m" (*a_double)
                : "xmm0"
            );
        }
    }
    #endif
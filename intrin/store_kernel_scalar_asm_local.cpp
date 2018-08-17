#include "intrin_bench_default_defines.h"

    //==========store int8==========
    void kernel_asm_store_int8_local( __m256i* a)
    {
        int8_t* a_cast = (int8_t*) a;
        for(int i = 0; i<STIM_LEN; i++)
        {
            asm volatile(
                "movb  %%al, %0\n\t"
                :
                : "m" (*a_cast)
                : "al"
            );
        }
    }

    //==========store int16==========
    void kernel_asm_store_int16_local( __m256i* a)
    {
        int16_t* a_cast = (int16_t*) a;
        for(int i = 0; i<STIM_LEN; i++)
        {
            asm volatile(
                "movw  %%ax, %0\n\t"
                :
                : "m" (*a_cast)
                : "ax"
            );
        }
    }

    //==========store int32==========
    void kernel_asm_store_int32_local( __m256i* a)
    {
        int32_t* a_cast = (int32_t*) a;
        for(int i = 0; i<STIM_LEN; i++)
        {
            asm volatile(
                "movl  %%eax, %0\n\t"
                :
                : "m" (*a_cast)
                : "eax"
            );
        }
    }

    //==========store int64==========
    void kernel_asm_store_int64_local( __m256i* a)
    {
        int64_t* a_cast = (int64_t*) a;
        for(int i = 0; i<STIM_LEN; i++)
        {
            asm volatile(
                "movq  %%rax, %0\n\t"
                :
                : "m" (*a_cast)
                : "rax"
            );
        }
    }

    #ifdef __SSE2__
    //==========store float==========
    void kernel_asm_store_ss_local( __m256* a)
    {
        float* a_float = (float*) a;

        for(int i = 0; i<STIM_LEN; i++)
        {
            asm volatile(
                "vmovss  %%xmm0, %0\n\t"
                :
                : "m" (*a_float)
                : "xmm0"
            );
        }
    }

    //==========store double==========
    void kernel_asm_store_sd_local( __m256d* a)
    {
        double* a_double = (double*) a;

        for(int i = 0; i<STIM_LEN; i++)
        {
            asm volatile(
                "vmovsd   %%xmm0, %0\n\t"
                :
                : "m" (*a_double)
                : "xmm0"
            );
        }
    }
    #endif
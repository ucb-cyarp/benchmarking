#include "intrin_bench_default_defines.h"

    //==========load int8==========
    void kernel_asm_load_int8( __m256i* a)
    {
        int8_t* a_cast = (int8_t*) a;
        for(int i = 0; i<STIM_LEN; i++)
        {
            int8_t* b = a_cast+i;

            asm volatile(
                "movb  %0, %%al\n\t"
                :
                : "m" (*b)
                : "al"
            );
        }
    }

    //==========load int16==========
    void kernel_asm_load_int16( __m256i* a)
    {
        int16_t* a_cast = (int16_t*) a;
        for(int i = 0; i<STIM_LEN; i++)
        {
            int16_t* b = a_cast+i;

            asm volatile(
                "movw  %0, %%ax\n\t"
                :
                : "m" (*b)
                : "ax"
            );
        }
    }

    //==========load int32==========
    void kernel_asm_load_int32( __m256i* a)
    {
        int32_t* a_cast = (int32_t*) a;
        for(int i = 0; i<STIM_LEN; i++)
        {
            int32_t* b = a_cast+i;

            asm volatile(
                "movl  %0, %%eax\n\t"
                :
                : "m" (*b)
                : "eax"
            );
        }
    }

    //==========load int64==========
    void kernel_asm_load_int64( __m256i* a)
    {
        int64_t* a_cast = (int64_t*) a;
        for(int i = 0; i<STIM_LEN; i++)
        {
            int64_t* b = a_cast+i;

            asm volatile(
                "movq  %0, %%rax\n\t"
                :
                : "m" (*b)
                : "rax"
            );
        }
    }

    #ifdef __SSE2__
    //==========load float==========
    void kernel_asm_load_ss( __m256* a)
    {
        float* a_float = (float*) a;

        for(int i = 0; i<STIM_LEN; i++)
        {
            float* b = a_float+i;

            asm volatile(
                "vmovss  %0, %%xmm0\n\t"
                :
                : "m" (*b)
                : "xmm0"
            );
        }
    }

    //==========load double==========
    void kernel_asm_load_sd( __m256d* a)
    {
        double* a_double = (double*) a;

        for(int i = 0; i<STIM_LEN; i++)
        {
            double* b = a_double+i;

            asm volatile(
                "vmovsd   %0, %%xmm0\n\t"
                :
                : "m" (*b)
                : "xmm0"
            );
        }
    }
    #endif
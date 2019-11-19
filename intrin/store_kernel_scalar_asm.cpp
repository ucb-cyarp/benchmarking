#include "intrin_bench_default_defines_and_imports_cpp.h"

    //==========store int8==========
    void kernel_asm_store_int8( __m256i* a)
    {
        int8_t* a_cast = (int8_t*) a;
        for(int i = 0; i<STIM_LEN; i++)
        {
            int8_t* b = a_cast+i;

            asm volatile(
                "movb  %%al, %0\n\t"
                :
                : "m" (*b)
                : "al"
            );
        }
    }

    //==========store int16==========
    void kernel_asm_store_int16( __m256i* a)
    {
        int16_t* a_cast = (int16_t*) a;
        for(int i = 0; i<STIM_LEN; i++)
        {
            int16_t* b = a_cast+i;

            asm volatile(
                "movw  %%ax, %0\n\t"
                :
                : "m" (*b)
                : "ax"
            );
        }
    }

    //==========store int32==========
    void kernel_asm_store_int32( __m256i* a)
    {
        int32_t* a_cast = (int32_t*) a;
        for(int i = 0; i<STIM_LEN; i++)
        {
            int32_t* b = a_cast+i;

            asm volatile(
                "movl  %%eax, %0\n\t"
                :
                : "m" (*b)
                : "eax"
            );
        }
    }

    //==========store int64==========
    void kernel_asm_store_int64( __m256i* a)
    {
        int64_t* a_cast = (int64_t*) a;
        for(int i = 0; i<STIM_LEN; i++)
        {
            int64_t* b = a_cast+i;

            asm volatile(
                "movq  %%rax, %0\n\t"
                :
                : "m" (*b)
                : "rax"
            );
        }
    }

    #ifdef __SSE2__
    //==========store float==========
    void kernel_asm_store_ss( __m256* a)
    {
        float* a_float = (float*) a;

        for(int i = 0; i<STIM_LEN; i++)
        {
            float* b = a_float+i;

            asm volatile(
                "vmovss  %%xmm0, %0\n\t"
                :
                : "m" (*b)
                : "xmm0"
            );
        }
    }

    //==========store double==========
    void kernel_asm_store_sd( __m256d* a)
    {
        double* a_double = (double*) a;

        for(int i = 0; i<STIM_LEN; i++)
        {
            double* b = a_double+i;

            asm volatile(
                "vmovsd   %%xmm0, %0\n\t"
                :
                : "m" (*b)
                : "xmm0"
            );
        }
    }
    #endif
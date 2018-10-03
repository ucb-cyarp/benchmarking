#include "intrin_bench_default_defines.h"

//==========_mm256_add_epi8==========
void kernel_nointrin_mm256_add_epi8( int8_t* a, int8_t* b, int8_t* c)
{
    for(int i = 0; i<STIM_LEN; i++)
    {
        int8_t a_val = a[i];
        int8_t b_val = b[i];
        int8_t c_val = a_val + b_val;
        c[i] = c_val;
    }
}

//==========_mm256_add_epi16==========
void kernel_nointrin_mm256_add_epi16( int16_t* a, int16_t* b, int16_t* c)
{
    for(int i = 0; i<STIM_LEN; i++)
    {
        int16_t a_val = a[i];
        int16_t b_val = b[i];
        int16_t c_val = a_val + b_val;
        c[i] = c_val;
    }
}

//==========_mm256_add_epi32==========
void kernel_nointrin_mm256_add_epi32( int32_t* a, int32_t* b, int32_t* c)
{
    for(int i = 0; i<STIM_LEN; i++)
    {
        int32_t a_val = a[i];
        int32_t b_val = b[i];
        int32_t c_val = a_val + b_val;
        c[i] = c_val;
    }
}

//==========_mm256_add_epi64==========
void kernel_nointrin_mm256_add_epi64( int64_t* a, int64_t* b, int64_t* c)
{
    for(int i = 0; i<STIM_LEN; i++)
    {
        int64_t a_val = a[i];
        int64_t b_val = b[i];
        int64_t c_val = a_val + b_val;
        c[i] = c_val;
    }
}

//==========_mm256_add_ps==========
void kernel_nointrin_mm256_add_ps( float* a, float* b, float* c)
{
    float* a_float = (float*) a;
    float* b_float = (float*) b;
    float* c_float = (float*) c;

    for(int i = 0; i<STIM_LEN; i+=8)
    {
        float a_val = a_float[i];
        float b_val = b_float[i];
        float c_val = a_val + b_val;
        c_float[i] = c_val;
    }
}

//==========_mm256_add_pd==========
void kernel_nointrin_mm256_add_pd( double* a, double* b, double* c)
{
    double* a_double = (double*) a;
    double* b_double = (double*) b;
    double* c_double = (double*) c;

    for(int i = 0; i<STIM_LEN; i+=4)
    {
        double a_val = a_double[i];
        double b_val = b_double[i];
        double c_val = a_val + b_val;
        c_double[i] = c_val;
    }
}

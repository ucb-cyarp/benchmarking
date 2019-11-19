#ifndef _H_LOAD_ADD_STORE_NOINTRIN_KERNEL
    #define _H_LOAD_ADD_STORE_NOINTRIN_KERNEL

    #include "intrin_bench_default_defines_and_imports_cpp.h"

    //==========_mm256_add_epi8==========
    void kernel_nointrin_mm256_add_epi8( int8_t* a, int8_t* b, int8_t* c);
    //==========_mm256_add_epi16==========
    void kernel_nointrin_mm256_add_epi16( int16_t* a, int16_t* b, int16_t* c);
    //==========_mm256_add_epi32==========
    void kernel_nointrin_mm256_add_epi32( int32_t* a, int32_t* b, int32_t* c);
    //==========_mm256_add_epi64==========
    void kernel_nointrin_mm256_add_epi64( int64_t* a, int64_t* b, int64_t* c);
    
    //==========_mm256_add_ps==========
    void kernel_nointrin_mm256_add_ps( float* a, float* b, float* c);
    //==========_mm256_add_pd==========
    void kernel_nointrin_mm256_add_pd( double* a, double* b, double* c);
    
#endif

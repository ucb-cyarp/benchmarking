#ifndef _H_LOAD_DIV_STORE
    #define _H_LOAD_DIV_STORE
    
    #include "intrin_bench_default_defines.h"

    #ifdef __AVX__
        //==========_mm256_div_ps==========
        void test_mm256_div_ps();
        //==========_mm256_div_pd==========
        void test_mm256_div_pd();
    #endif
    
#endif

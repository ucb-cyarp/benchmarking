#ifndef _H_DIV
    #define _H_DIV
    
    #include "intrin_bench_default_defines.h"

    #ifdef __AVX__
        //==========_mm256_div_ps==========
        void test_only_mm256_div_ps();
        //==========_mm256_div_pd==========
        void test_only_mm256_div_pd();
    #endif
    
#endif

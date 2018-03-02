#ifndef _H_FMA
    #define _H_FMA
    
    #include "intrin_bench_default_defines.h"

    #ifdef __FMA__
        //==========_mm256_fmadd_ps==========
        void test_only_mm256_fmadd_ps();
        //==========_mm256_fmadd_pd==========
        void test_only_mm256_fmadd_pd();
    #endif
    
#endif

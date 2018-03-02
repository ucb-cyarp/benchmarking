#ifndef _H_LOAD_FMA_STORE
    #define _H_LOAD_FMA_STORE
    
    #include "intrin_bench_default_defines.h"

    #ifdef __FMA__
        //==========_mm256_fmadd_ps==========
        void test_mm256_fmadd_ps();
        //==========_mm256_fmadd_pd==========
        void test_mm256_fmadd_pd();
    #endif
    
#endif

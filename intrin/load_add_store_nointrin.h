#ifndef _H_LOAD_ADD_STORE_NOINTRIN
    #define _H_LOAD_ADD_STORE_NOINTRIN

    #include "intrin_bench_default_defines.h"

    //==========_mm256_add_epi8==========
    void test_nointrin_mm256_add_epi8();
    //==========_mm256_add_epi16==========
    void test_nointrin_mm256_add_epi16();
    //==========_mm256_add_epi32==========
    void test_nointrin_mm256_add_epi32();

    //==========_mm256_add_ps==========
    void test_nointrin_mm256_add_ps();
    //==========_mm256_add_pd==========
    void test_nointrin_mm256_add_pd();

#endif

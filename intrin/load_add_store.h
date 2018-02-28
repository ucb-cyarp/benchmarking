#include "intrin_bench_default_defines.h"

//==========_mm256_add_epi8==========
void kernel_mm256_add_epi8( __m256i* a, __m256i* b, __m256i* c);
void test_mm256_add_epi8();

//==========_mm256_add_epi16==========
void kernel_mm256_add_epi16( __m256i* a, __m256i* b, __m256i* c);
void test_mm256_add_epi16();

//==========_mm256_add_epi32==========
void kernel_mm256_add_epi32( __m256i* a, __m256i* b, __m256i* c);
void test_mm256_add_epi32();

//==========_mm256_add_ps==========
void kernel_mm256_add_ps( __m256* a, __m256* b, __m256* c);
void test_mm256_add_ps();

//==========_mm256_add_pd==========
void kernel_mm256_add_pd( __m256d* a, __m256d* b, __m256d* c);
void test_mm256_add_pd();
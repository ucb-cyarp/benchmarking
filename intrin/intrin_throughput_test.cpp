/*
 * Benchmark for SSE/AVX/FMA Vector Unit
 * 
 * Test execution speeds for SSE/AVX/FMA for different
 * operations and datatypes.
 * 
 * Compiler Parameters (defined as macros)
 *     - STIM_LEN:     Stimulus Length
 *     - TRIALS:       Number of Trial Runs
 *     - PRINT_TITLE:  1 to Print Title Text
 *     - PRINT_HEADER: 1 to Print Test Header
 *     - PRINT_TRIALS: 1 to Print Each Trial
 *     - PRINT_STATS:  1 to Print Statistics
 * 
 * Arguments
 *     
 */

#include "intrin_bench_default_defines.h"

#include "load.h"
#include "load_store.h"

#include "load_add_store.h"
#include "load_mult_store.h"

#include "load_add_store_unroll2.h"

#include "load_add_store_nointrin.h"

void test_load()
{
    #ifdef __AVX__
        test_mm256_load_si256();
        test_mm256_load_ps();
        test_mm256_load_pd();
    #endif
}

void test_load_store()
{
    #ifdef __AVX__
        test_mm256_load_si256__mm256_store_si256();
        test_mm256_load_ps__mm256_store_ps();
        test_mm256_load_pd__mm256_store_pd();
    #endif
}

void test_add()
{
    #ifdef __AVX2__
        test_mm256_add_epi8();
        test_mm256_add_epi16();
        test_mm256_add_epi32();
    #endif

    #ifdef __AVX__
        test_mm256_add_ps();
        test_mm256_add_pd();
    #endif
}

void test_nointrin_add()
{
    test_nointrin_mm256_add_epi8();
    test_nointrin_mm256_add_epi16();
    test_nointrin_mm256_add_epi32();

    test_nointrin_mm256_add_ps();
    test_nointrin_mm256_add_pd();
}

void test_unroll2_add()
{
    #ifdef __AVX2__
        test_u2_mm256_add_epi8();
        test_u2_mm256_add_epi16();
        test_u2_mm256_add_epi32();
    #endif

    #ifdef __AVX__
        test_u2_mm256_add_ps();
        test_u2_mm256_add_pd();
    #endif
}

void test_mult()
{
    #ifdef __AVX2__
        test_mm256_mullo_epi16();
        test_mm256_mullo_epi32();
    #endif

    #ifdef __AVX__
        test_mm256_mul_ps();
        test_mm256_mul_pd();
    #endif
}

int main(int argc, char *argv[])
{
    #if PRINT_TITLE == 1
    printf("SSE/AVX/FMA Intrinsic Tester\n");
    printf("STIM_LEN: %d, TRIALS: %d\n", STIM_LEN, TRIALS);
    #endif

    test_load();
    test_load_store();

    test_add();
    test_mult();

    test_unroll2_add();

    test_nointrin_add();

    return 0;
}
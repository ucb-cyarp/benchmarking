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
#include "store.h"
#include "load_store.h"

#include "add.h"
#include "mult.h"
#include "fma.h"

#include "load_add_store.h"
#include "load_mult_store.h"
#include "load_div_store.h"
#include "load_fma_store.h"

#include "load_add_store_unroll2.h"

#include "load_add_store_nointrin.h"

void test_load()
{
    printf("########## Load Benchmarks ##########\n");
    #ifdef __AVX__
        test_mm256_load_si256();
        test_mm256_load_ps();
        test_mm256_load_pd();
    #endif
}

void test_store()
{
    printf("########## Store Benchmarks ##########\n");
    #ifdef __AVX__
        test_mm256_store_si256();
        test_mm256_store_ps();
        test_mm256_store_pd();
    #endif
}

void test_load_store()
{
    printf("########## Load/Store Benchmarks ##########\n");
    #ifdef __AVX__
        test_mm256_load_si256__mm256_store_si256();
        test_mm256_load_ps__mm256_store_ps();
        test_mm256_load_pd__mm256_store_pd();
    #endif
}

void test_only_add()
{
    printf("########## Add Benchmarks ##########\n");
    #ifdef __AVX2__
        test_only_mm256_add_epi8();
        test_only_mm256_add_epi16();
        test_only_mm256_add_epi32();
    #endif

    #ifdef __AVX__
        test_only_mm256_add_ps();
        test_only_mm256_add_pd();
    #endif
}

void test_only_mult()
{
    printf("########## Multiply Benchmarks ##########\n");
    #ifdef __AVX2__
        test_only_mm256_mullo_epi16();
        test_only_mm256_mullo_epi32();
    #endif

    #ifdef __AVX__
        test_only_mm256_mul_ps();
        test_only_mm256_mul_pd();
    #endif
}

void test_only_fma()
{
    printf("########## FMA Benchmarks ##########\n");
    #ifdef __FMA__
        test_only_mm256_fmadd_ps();
        test_only_mm256_fmadd_pd();
    #endif
}

void test_add()
{
    printf("########## Load/Add/Store Benchmarks ##########\n");
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
    printf("########## Load/Add/Store (No Intrinsics) Benchmarks ##########\n");
    test_nointrin_mm256_add_epi8();
    test_nointrin_mm256_add_epi16();
    test_nointrin_mm256_add_epi32();

    test_nointrin_mm256_add_ps();
    test_nointrin_mm256_add_pd();
}

void test_unroll2_add()
{
    printf("########## Load/Add/Store (Unrolled by 2) Benchmarks ##########\n");
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
    printf("########## Load/Multiply/Store Benchmarks ##########\n");
    #ifdef __AVX2__
        test_mm256_mullo_epi16();
        test_mm256_mullo_epi32();
    #endif

    #ifdef __AVX__
        test_mm256_mul_ps();
        test_mm256_mul_pd();
    #endif
}

void test_div()
{
    printf("########## Load/Divide/Store Benchmarks ##########\n");
    #ifdef __AVX__
        test_mm256_div_ps();
        test_mm256_div_pd();
    #endif
}

void test_fma()
{
    printf("########## Load/FMA/Store Benchmarks ##########\n");
    #ifdef __AVX__
        test_mm256_fmadd_ps();
        test_mm256_fmadd_pd();
    #endif
}

int main(int argc, char *argv[])
{
    #if PRINT_TITLE == 1
    printf("SSE/AVX/FMA Intrinsic Tester\n");
    printf("STIM_LEN: %d (Samples/Vector/Trial), TRIALS: %d\n", STIM_LEN, TRIALS);
    #endif

    test_load();
    printf("\n");
    test_store();
    printf("\n");
    test_load_store();
    printf("\n");

    test_only_add();
    printf("\n");
    test_only_mult();
    printf("\n");
    test_only_fma();
    printf("\n");

    test_add();
    printf("\n");
    test_mult();
    printf("\n");
    test_div();
    printf("\n");
    test_fma();
    printf("\n");

    test_unroll2_add();
    printf("\n");

    test_nointrin_add();

    return 0;
}
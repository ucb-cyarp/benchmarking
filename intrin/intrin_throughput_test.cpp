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

#include "kernel_runner.h"

//Legacy <REMOVE>
#include "load.h"
#include "store.h"
#include "load_store.h"

//Legacy <REMOVE>
#include "add.h"
#include "mult.h"
#include "div.h"
#include "fma.h"

//Kernels Load/Store
#include "load_kernel.h"
#include "store_kernel.h"
#include "load_store_kernel.h"

//Kernels Load-op-Store
#include "load_add_store_kernel.h"
#include "load_mult_store_kernel.h"
#include "load_div_store_kernel.h"
#include "load_fma_store_kernel.h"

//Kernels Op Only
#include "add_kernel.h"
#include "mult_kernel.h"
#include "div_kernel.h"
#include "fma_kernel.h"

//Kernels Special Case
#include "load_add_store_nointrin_kernel.h"
#include "load_add_store_unroll2_kernel.h"

//Legacy <REMOVE>
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
        test_mm256_load_si256_int8();
        load_store_one_arg_kernel<__m256i, int8_t>  (&kernel_mm256_load_si256_int8,  "[AVX] ===== Load 32 Signed Bytes [Store in Local Var] (_mm256_load_si256) =====");
        test_mm256_load_si256_int16();
        load_store_one_arg_kernel<__m256i, int16_t> (&kernel_mm256_load_si256_int16, "[AVX] ===== Load 16 Signed 16 Bit Integers [Store in Local Var] (_mm256_load_si256) =====");
        test_mm256_load_si256_int32();
        load_store_one_arg_kernel<__m256i, int32_t> (&kernel_mm256_load_si256_int32, "[AVX] ===== Load 8 Signed 32 Bit Integers [Store in Local Var] (_mm256_load_si256) =====");
        test_mm256_load_ps();
        load_store_one_arg_kernel<__m256,  float>   (&kernel_mm256_load_ps,          "[AVX] ===== Load 8 Packed 32 bit Signed Floating Point Numbers [Store in Local Var] (_mm256_load_ps) =====");
        test_mm256_load_pd();
        load_store_one_arg_kernel<__m256d, double>  (&kernel_mm256_load_pd,          "[AVX] ===== Load 4 Packed 64 bit Signed Floating Point Numbers [Store in Local Var] (_mm256_load_pd) =====");
    #endif
}

void test_store()
{
    printf("########## Store Benchmarks ##########\n");
    #ifdef __AVX__
        test_mm256_store_si256_int8();
        load_store_one_arg_kernel<__m256i, int8_t>   (&kernel_mm256_store_si256_int8,  "[AVX] ===== Store 32 Packed Signed Bytes (_mm256_store_si256) =====");
        test_mm256_store_si256_int16();
        load_store_one_arg_kernel<__m256i, int16_t>  (&kernel_mm256_store_si256_int16, "[AVX] ===== Store 16 Packed Signed 16 Bit Integers (_mm256_store_si256) =====");
        test_mm256_store_si256_int32();
        load_store_one_arg_kernel<__m256i, int32_t>  (&kernel_mm256_store_si256_int32, "[AVX] ===== Store 8 Packed Signed 32 Bit Integers (_mm256_store_si256) =====");
        test_mm256_store_ps();
        load_store_one_arg_kernel<__m256, float>     (&kernel_mm256_store_ps,          "[AVX] ===== Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_store_ps) =====");
        test_mm256_store_pd();
        load_store_one_arg_kernel<__m256d, double>   (&kernel_mm256_store_pd,          "[AVX] ===== Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_store_pd) =====");
    #endif
}

void test_load_store()
{
    printf("########## Load/Store Benchmarks ##########\n");
    #ifdef __AVX__
        test_mm256_load_si256__mm256_store_si256_int8();
        load_store_two_arg_kernel<__m256i, int8_t>  (&kernel_mm256_load_si256__mm256_store_si256_int8,  "[AVX] ===== Load/Store (Copy) 32 Packed Signed Bytes (_mm256_load_si256 | _mm256_store_si256) =====");
        test_mm256_load_si256__mm256_store_si256_int16();
        load_store_two_arg_kernel<__m256i, int16_t> (&kernel_mm256_load_si256__mm256_store_si256_int16, "[AVX] ===== Load/Store (Copy) 16 Packed Signed 16 Bit Integers (_mm256_load_si256 | _mm256_store_si256) =====");
        test_mm256_load_si256__mm256_store_si256_int32();
        load_store_two_arg_kernel<__m256i, int32_t> (&kernel_mm256_load_si256__mm256_store_si256_int32, "[AVX] ===== Load/Store (Copy) 8 Packed Signed 32 Bit Integers (_mm256_load_si256 | _mm256_store_si256) =====");
        test_mm256_load_ps__mm256_store_ps();
        load_store_two_arg_kernel<__m256,  float>   (&kernel_mm256_load_ps__mm256_store_ps,             "[AVX] ===== Load/Store (Copy) 8 Packed 32 bit Signed Floating Point Numbers (_mm256_load_ps | _mm256_store_ps) =====");
        test_mm256_load_pd__mm256_store_pd();
        load_store_two_arg_kernel<__m256d, double>  (&kernel_mm256_load_pd__mm256_store_pd,             "[AVX] ===== Load/Store (Copy) 4 Packed 64 bit Signed Floating Point Numbers (_mm256_load_pd | _mm256_store_pd) =====");
    #endif
}

void test_only_add()
{
    printf("########## Add Benchmarks ##########\n");
    #ifdef __AVX2__
        test_only_mm256_add_epi8();
        zero_arg_kernel(&kernel_only_mm256_add_epi8,  "[AVX2] ===== Add 32 Packed 8 bit Signed Integers [Store in Local Var] (_mm256_add_epi8) =====");
        test_only_mm256_add_epi16();
        zero_arg_kernel(&kernel_only_mm256_add_epi16, "[AVX2] ===== Add 16 Packed 16 bit Signed Integers [Store in Local Var] (_mm256_add_epi16) =====");
        test_only_mm256_add_epi32();
        zero_arg_kernel(&kernel_only_mm256_add_epi32, "[AVX2] ===== Add 8 Packed 32 bit Signed Integers [Store in Local Var] (_mm256_add_epi32) =====");
    #endif

    #ifdef __AVX__
        test_only_mm256_add_ps();
        zero_arg_kernel(&kernel_only_mm256_add_ps,    "[AVX] ===== Add 8 Packed 32 bit Signed Floating Point Numbers [Store in Local Var] (_mm256_add_ps) =====");
        test_only_mm256_add_pd();
        zero_arg_kernel(&kernel_only_mm256_add_pd,    "[AVX] ===== Add 4 Packed 64 bit Signed Floating Point Numbers [Store in Local Var] (_mm256_add_pd) =====");
    #endif
}

void test_only_mult()
{
    printf("########## Multiply Benchmarks ##########\n");
    #ifdef __AVX2__
        test_only_mm256_mullo_epi16();
        zero_arg_kernel(&kernel_only_mm256_mullo_epi16, "[AVX2] ===== Multiply 16 Packed 16 bit Signed Integers -> Produce 32 Bit Intermediates -> Store Lower 16 Bits of Intermediates [In Local Var] (_mm256_mullo_epi16) =====");
        test_only_mm256_mullo_epi32();
        zero_arg_kernel(&kernel_only_mm256_mullo_epi32, "[AVX2] ===== Multiply 8 Packed 32 bit Signed Integers -> Produce 64 Bit Intermediates -> Store Lower 32 Bits of Intermediates [In Local Var] (_mm256_mullo_epi32) =====");
    #endif

    #ifdef __AVX__
        test_only_mm256_mul_ps();
        zero_arg_kernel(&kernel_only_mm256_mul_ps, "[AVX] ===== Multiply 8 Packed 32 bit Signed Floating Point Numbers [Store In Local Var] (_mm256_mul_ps) =====");
        test_only_mm256_mul_pd();
        zero_arg_kernel(&kernel_only_mm256_mul_pd, "[AVX] ===== Multiply 4 Packed 64 bit Signed Floating Point Numbers [Store In Local Var] (_mm256_mul_pd) =====");
    #endif
}

void test_only_div()
{
    printf("########## Divide Benchmarks ##########\n");
    #ifdef __AVX__
        test_only_mm256_div_ps();
        zero_arg_kernel(&kernel_only_mm256_div_ps, "[AVX] ===== Divide 8 Packed 32 bit Signed Floating Point Numbers [Store In Local Var] (_mm256_div_ps) =====");
        test_only_mm256_div_pd();
        zero_arg_kernel(&kernel_only_mm256_div_pd, "[AVX] ===== Divide 4 Packed 64 bit Signed Floating Point Numbers [Store In Local Var] (_mm256_div_pd) =====");
    #endif
}

void test_only_fma()
{
    printf("########## FMA Benchmarks ##########\n");
    #ifdef __FMA__
        test_only_mm256_fmadd_ps();
        zero_arg_kernel(&kernel_only_mm256_fmadd_ps, "[FMA] ===== FMA 8 Packed 32 bit Signed Floating Point Numbers [Store in Local Var] (_mm256_fmadd_ps) =====");
        test_only_mm256_fmadd_pd();
        zero_arg_kernel(&kernel_only_mm256_fmadd_pd, "[FMA] ===== FMA 4 Packed 64 bit Signed Floating Point Numbers [Store in Local Var] (_mm256_fmadd_pd) =====");
    #endif
}

void test_add()
{
    printf("########## Load/Add/Store Benchmarks ##########\n");
    #ifdef __AVX2__
        test_mm256_add_epi8();
        load_store_three_arg_kernel<__m256i, int8_t>  (&kernel_mm256_add_epi8,  "[AVX2] ===== Load/Add/Store 32 Packed 8 bit Signed Integers (_mm256_add_epi8) =====");
        test_mm256_add_epi16();
        load_store_three_arg_kernel<__m256i, int16_t> (&kernel_mm256_add_epi16, "[AVX2] ===== Load/Add/Store 16 Packed 16 bit Signed Integers (_mm256_add_epi16) =====");
        test_mm256_add_epi32();
        load_store_three_arg_kernel<__m256i, int32_t> (&kernel_mm256_add_epi32, "[AVX2] ===== Load/Add/Store 8 Packed 32 bit Signed Integers (_mm256_add_epi32) =====");
    #endif

    #ifdef __AVX__
        test_mm256_add_ps();
        load_store_three_arg_kernel<__m256,  float>   (&kernel_mm256_add_ps,    "[AVX] ===== Load/Add/Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_add_ps) =====");
        test_mm256_add_pd();
        load_store_three_arg_kernel<__m256d, double>  (&kernel_mm256_add_pd,    "[AVX] ===== Load/Add/Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_add_pd) =====");
    #endif
}

void test_nointrin_add()
{
    printf("########## Load/Add/Store (No Intrinsics) Benchmarks ##########\n");
    test_nointrin_mm256_add_epi8();
    no_vec_three_arg_kernel<int8_t> (&kernel_nointrin_mm256_add_epi8,   "[No Intrin] ===== Load/Add/Store 32 Packed 8 bit Signed Integers (_mm256_add_epi8) =====");
    test_nointrin_mm256_add_epi16();
    no_vec_three_arg_kernel<int16_t> (&kernel_nointrin_mm256_add_epi16, "[No Intrin] ===== Load/Add/Store 16 Packed 16 bit Signed Integers (_mm256_add_epi16) =====");
    test_nointrin_mm256_add_epi32();
    no_vec_three_arg_kernel<int32_t> (&kernel_nointrin_mm256_add_epi32, "[No Intrin] ===== Load/Add/Store 8 Packed 32 bit Signed Integers (_mm256_add_epi32) =====");

    test_nointrin_mm256_add_ps();
    no_vec_three_arg_kernel<float>   (&kernel_nointrin_mm256_add_ps,    "[No Intrin] ===== Load/Add/Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_add_ps) =====");
    test_nointrin_mm256_add_pd();
    no_vec_three_arg_kernel<double>  (&kernel_nointrin_mm256_add_pd,    "[No Intrin] ===== Load/Add/Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_add_pd) =====");
}

void test_unroll2_add()
{
    printf("########## Load/Add/Store (Unrolled by 2) Benchmarks ##########\n");
    #ifdef __AVX2__
        test_u2_mm256_add_epi8();
        load_store_three_arg_kernel<__m256i, int8_t>  (&kernel_u2_mm256_add_epi8,  "[AVX2] ===== (Unroll 2) Load/Add/Store 32 Packed 8 bit Signed Integers (_mm256_add_epi8) =====");
        test_u2_mm256_add_epi16();
        load_store_three_arg_kernel<__m256i, int16_t> (&kernel_u2_mm256_add_epi16, "[AVX2] ===== (Unroll 2) Load/Add/Store 16 Packed 16 bit Signed Integers (_mm256_add_epi16) =====");
        test_u2_mm256_add_epi32();
        load_store_three_arg_kernel<__m256i, int32_t> (&kernel_u2_mm256_add_epi32, "[AVX2] ===== (Unroll 2) Load/Add/Store 8 Packed 32 bit Signed Integers (_mm256_add_epi32) =====");
    #endif

    #ifdef __AVX__
        test_u2_mm256_add_ps();
        load_store_three_arg_kernel<__m256,  float>   (&kernel_u2_mm256_add_ps,    "[AVX] ===== (Unroll 2) Load/Add/Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_add_ps) =====");
        test_u2_mm256_add_pd();
        load_store_three_arg_kernel<__m256d, double>  (&kernel_u2_mm256_add_pd,    "[AVX] ===== (Unroll 2) Load/Add/Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_add_pd) =====");
    #endif
}

void test_mult()
{
    printf("########## Load/Multiply/Store Benchmarks ##########\n");
    #ifdef __AVX2__
        test_mm256_mullo_epi16();
        load_store_three_arg_kernel<__m256i, int16_t>  (&kernel_mm256_mullo_epi16,  "[AVX2] ===== Load/Multiply 16 Packed 16 bit Signed Integers -> Produce 32 Bit Intermediates -> Store Lower 16 Bits of Intermediates (_mm256_mullo_epi16) =====");
        test_mm256_mullo_epi32();
        load_store_three_arg_kernel<__m256i, int32_t>  (&kernel_mm256_mullo_epi32,  "[AVX2] ===== Load/Multiply 8 Packed 32 bit Signed Integers -> Produce 64 Bit Intermediates -> Store Lower 32 Bits of Intermediates (_mm256_mullo_epi32) =====");
    #endif

    #ifdef __AVX__
        test_mm256_mul_ps();
        load_store_three_arg_kernel<__m256,  float>    (&kernel_mm256_mul_ps,       "[AVX] ===== Load/Multiply/Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_mul_ps) =====");
        test_mm256_mul_pd();
        load_store_three_arg_kernel<__m256d, double>   (&kernel_mm256_mul_pd,       "[AVX] ===== Load/Multiply/Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_mul_pd) =====");
    #endif
}

void test_div()
{
    printf("########## Load/Divide/Store Benchmarks ##########\n");
    #ifdef __AVX__
        test_mm256_div_ps();
        load_store_three_arg_kernel<__m256,  float>    (&kernel_mm256_div_ps,      "[AVX] ===== Load/Divide/Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_div_ps) =====");
        test_mm256_div_pd();
        load_store_three_arg_kernel<__m256d, double>   (&kernel_mm256_div_pd,      "[AVX] ===== Load/Divide/Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_div_pd) =====");
    #endif
}

void test_fma()
{
    printf("########## Load/FMA/Store Benchmarks ##########\n");
    #ifdef __FMA__
        test_mm256_fmadd_ps();
        load_store_four_arg_kernel<__m256,  float>    (&kernel_mm256_fmadd_ps,    "[FMA] ===== Load/FMA/Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_fmadd_ps) =====");
        test_mm256_fmadd_pd();
        load_store_four_arg_kernel<__m256d, double>   (&kernel_mm256_fmadd_pd,    "[FMA] ===== Load/FMA/Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_fmadd_pd) =====");
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
    test_only_div();
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
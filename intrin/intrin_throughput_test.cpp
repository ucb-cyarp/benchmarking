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

#include <pthread.h>
#include <stdio.h>
#include <map>
#include <unistd.h>

#include <chrono>
#include <ctime>

#include "intrin_bench_default_defines.h"

#include "cpucounters.h"

#include "kernel_runner.h"
#include "results.h"

//Kernels Load/Store
#include "load_kernel_asm.h"
#include "load_kernel_asm_unroll2.h"
#include "store_kernel_asm.h"
#include "store_kernel_asm_unroll2.h"
#include "load_store_kernel.h"

//Kernels Load-op-Store
#include "load_add_store_kernel.h"
#include "load_mult_store_kernel.h"
#include "load_div_store_kernel.h"
#include "load_fma_store_kernel.h"

//Kernels Op Only
#include "add_kernel_scalar_asm.h"
#include "add_kernel_scalar_asm_unroll2.h"
#include "add_kernel_asm.h"
#include "mult_kernel_scalar_asm.h"
#include "mult_kernel_scalar_asm_unroll2.h"
#include "mult_kernel_asm.h"
#include "div_kernel_asm.h"
#include "fma_kernel_asm.h"

//Kernels Special Case
#include "load_add_store_nointrin_kernel.h"
#include "load_add_store_unroll2_kernel.h"

void test_load(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Load Benchmarks ##########\n");
    #ifdef __AVX__
        Results* res_int8_t =  load_store_one_arg_kernel<__m256i, int8_t>  (pcm, &kernel_asm_mm256_load_si256_int8,  cpu_num, "[AVX] ===== Load 32 Signed Bytes (_mm256_load_si256) =====");
        type_result["int8_t"] = res_int8_t;
        Results* res_int16_t = load_store_one_arg_kernel<__m256i, int16_t> (pcm, &kernel_asm_mm256_load_si256_int16, cpu_num, "[AVX] ===== Load 16 Signed 16 Bit Integers(_mm256_load_si256) =====");
        type_result["int16_t"] = res_int16_t;
        Results* res_int32_t = load_store_one_arg_kernel<__m256i, int32_t> (pcm, &kernel_asm_mm256_load_si256_int32, cpu_num, "[AVX] ===== Load 8 Signed 32 Bit Integers (_mm256_load_si256) =====");
        type_result["int32_t"] = res_int32_t;
        Results* res_float =   load_store_one_arg_kernel<__m256,  float>   (pcm, &kernel_asm_mm256_load_ps,          cpu_num, "[AVX] ===== Load 8 Packed 32 bit Signed Floating Point Numbers (_mm256_load_ps) =====");
        type_result["float"] = res_float;
        Results* res_double =  load_store_one_arg_kernel<__m256d, double>  (pcm, &kernel_asm_mm256_load_pd,          cpu_num, "[AVX] ===== Load 4 Packed 64 bit Signed Floating Point Numbers (_mm256_load_pd) =====");
        type_result["double"] = res_double;
    #endif
}

void test_load_unroll2(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Load Benchmarks (Unroll 2) ##########\n");
    #ifdef __AVX__
        Results* res_int8_t =  load_store_one_arg_kernel<__m256i, int8_t>  (pcm, &kernel_asm_mm256_load_si256_int8_unroll2,  cpu_num, "[AVX] ===== Load 32 Signed Bytes (_mm256_load_si256) =====");
        type_result["int8_t"] = res_int8_t;
        Results* res_int16_t = load_store_one_arg_kernel<__m256i, int16_t> (pcm, &kernel_asm_mm256_load_si256_int16_unroll2, cpu_num, "[AVX] ===== Load 16 Signed 16 Bit Integers(_mm256_load_si256) =====");
        type_result["int16_t"] = res_int16_t;
        Results* res_int32_t = load_store_one_arg_kernel<__m256i, int32_t> (pcm, &kernel_asm_mm256_load_si256_int32_unroll2, cpu_num, "[AVX] ===== Load 8 Signed 32 Bit Integers (_mm256_load_si256) =====");
        type_result["int32_t"] = res_int32_t;
        Results* res_float =   load_store_one_arg_kernel<__m256,  float>   (pcm, &kernel_asm_mm256_load_ps_unroll2,          cpu_num, "[AVX] ===== Load 8 Packed 32 bit Signed Floating Point Numbers (_mm256_load_ps) =====");
        type_result["float"] = res_float;
        Results* res_double =  load_store_one_arg_kernel<__m256d, double>  (pcm, &kernel_asm_mm256_load_pd_unroll2,          cpu_num, "[AVX] ===== Load 4 Packed 64 bit Signed Floating Point Numbers (_mm256_load_pd) =====");
        type_result["double"] = res_double;
    #endif
}

void test_store(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Store Benchmarks ##########\n");
    #ifdef __AVX__
        Results* res_int8_t =  load_store_one_arg_kernel<__m256i, int8_t>   (pcm, &kernel_asm_mm256_store_si256_int8,  cpu_num, "[AVX] ===== Store 32 Packed Signed Bytes (_mm256_store_si256) =====");
        type_result["int8_t"] = res_int8_t;
        Results* res_int16_t = load_store_one_arg_kernel<__m256i, int16_t>  (pcm, &kernel_asm_mm256_store_si256_int16, cpu_num, "[AVX] ===== Store 16 Packed Signed 16 Bit Integers (_mm256_store_si256) =====");
        type_result["int16_t"] = res_int16_t;
        Results* res_int32_t = load_store_one_arg_kernel<__m256i, int32_t>  (pcm, &kernel_asm_mm256_store_si256_int32, cpu_num, "[AVX] ===== Store 8 Packed Signed 32 Bit Integers (_mm256_store_si256) =====");
        type_result["int32_t"] = res_int32_t;
        Results* res_float = load_store_one_arg_kernel<__m256, float>     (pcm, &kernel_asm_mm256_store_ps,          cpu_num, "[AVX] ===== Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_store_ps) =====");
        type_result["float"] = res_float;
        Results* res_double = load_store_one_arg_kernel<__m256d, double>   (pcm, &kernel_asm_mm256_store_pd,          cpu_num, "[AVX] ===== Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_store_pd) =====");
        type_result["double"] = res_double;
    #endif
}

void test_store_unroll2(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Store Benchmarks (Unroll 2) ##########\n");
    #ifdef __AVX__
        Results* res_int8_t =  load_store_one_arg_kernel<__m256i, int8_t>   (pcm, &kernel_asm_mm256_store_si256_int8_unroll2,  cpu_num, "[AVX] ===== Store 32 Packed Signed Bytes (_mm256_store_si256) =====");
        type_result["int8_t"] = res_int8_t;
        Results* res_int16_t = load_store_one_arg_kernel<__m256i, int16_t>  (pcm, &kernel_asm_mm256_store_si256_int16_unroll2, cpu_num, "[AVX] ===== Store 16 Packed Signed 16 Bit Integers (_mm256_store_si256) =====");
        type_result["int16_t"] = res_int16_t;
        Results* res_int32_t = load_store_one_arg_kernel<__m256i, int32_t>  (pcm, &kernel_asm_mm256_store_si256_int32_unroll2, cpu_num, "[AVX] ===== Store 8 Packed Signed 32 Bit Integers (_mm256_store_si256) =====");
        type_result["int32_t"] = res_int32_t;
        Results* res_float = load_store_one_arg_kernel<__m256, float>     (pcm, &kernel_asm_mm256_store_ps_unroll2,          cpu_num, "[AVX] ===== Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_store_ps) =====");
        type_result["float"] = res_float;
        Results* res_double = load_store_one_arg_kernel<__m256d, double>   (pcm, &kernel_asm_mm256_store_pd_unroll2,          cpu_num, "[AVX] ===== Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_store_pd) =====");
        type_result["double"] = res_double;
    #endif
}

void test_load_store(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Load/Store Benchmarks ##########\n");
    #ifdef __AVX__
        Results* res_int8_t =  load_store_two_arg_kernel<__m256i, int8_t>  (pcm, &kernel_mm256_load_si256__mm256_store_si256_int8,  cpu_num, "[AVX] ===== Load/Store (Copy) 32 Packed Signed Bytes (_mm256_load_si256 | _mm256_store_si256) =====");
        type_result["int8_t"] = res_int8_t;
        Results* res_int16_t = load_store_two_arg_kernel<__m256i, int16_t> (pcm, &kernel_mm256_load_si256__mm256_store_si256_int16, cpu_num, "[AVX] ===== Load/Store (Copy) 16 Packed Signed 16 Bit Integers (_mm256_load_si256 | _mm256_store_si256) =====");
        type_result["int16_t"] = res_int16_t;
        Results* res_int32_t = load_store_two_arg_kernel<__m256i, int32_t> (pcm, &kernel_mm256_load_si256__mm256_store_si256_int32, cpu_num, "[AVX] ===== Load/Store (Copy) 8 Packed Signed 32 Bit Integers (_mm256_load_si256 | _mm256_store_si256) =====");
        type_result["int32_t"] = res_int32_t;
        Results* res_float =   load_store_two_arg_kernel<__m256,  float>   (pcm, &kernel_mm256_load_ps__mm256_store_ps,             cpu_num, "[AVX] ===== Load/Store (Copy) 8 Packed 32 bit Signed Floating Point Numbers (_mm256_load_ps | _mm256_store_ps) =====");
        type_result["float"] = res_float;
        Results* res_double =  load_store_two_arg_kernel<__m256d, double>  (pcm, &kernel_mm256_load_pd__mm256_store_pd,             cpu_num, "[AVX] ===== Load/Store (Copy) 4 Packed 64 bit Signed Floating Point Numbers (_mm256_load_pd | _mm256_store_pd) =====");
        type_result["double"] = res_double;
    #endif
}

void test_only_add_scalar(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Scalar Add Benchmarks ##########\n");
    Results* res_int8_t_scalar =  zero_arg_kernel(pcm, &kernel_only_asm_add_i8,  cpu_num, "[x86] ===== Add 8 bit Signed Integers =====");
    type_result["int8_t"] = res_int8_t_scalar;
    Results* res_int16_t_scalar = zero_arg_kernel(pcm, &kernel_only_asm_add_i16, cpu_num, "[x86] ===== Add 16 bit Signed Integers =====");
    type_result["int16_t"] = res_int16_t_scalar;
    Results* res_int32_t_scalar = zero_arg_kernel(pcm, &kernel_only_asm_add_i32, cpu_num, "[x86] ===== Add 32 bit Signed Integers =====");
    type_result["int32_t"] = res_int32_t_scalar;
    Results* res_int64_t_scalar = zero_arg_kernel(pcm, &kernel_only_asm_add_i64, cpu_num, "[x86_64] ===== Add 64 bit Signed Integers =====");
    type_result["int64_t"] = res_int64_t_scalar;

    Results* res_single_scalar = zero_arg_kernel(pcm, &kernel_only_asm_add_sp, cpu_num, "[SSE2] ===== Add Float Point (single via SSE2) =====");
    type_result["float"] = res_single_scalar;
    Results* res_double_scalar = zero_arg_kernel(pcm, &kernel_only_asm_add_dp, cpu_num, "[SSE2] ===== Add Floating Point (double via SSE2) =====");
    type_result["double"] = res_double_scalar;

    Results* res_x87_scalar = zero_arg_kernel(pcm, &kernel_only_asm_add_fp, cpu_num, "[x87] ===== Add Floating Point (via x87) =====");
    type_result["x87 Floating Point"] = res_x87_scalar;
}

void test_only_add_scalar_unroll2(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Scalar Add Benchmarks (Unroll 2) ##########\n");
    Results* res_int8_t_scalar =  zero_arg_kernel(pcm, &kernel_only_asm_add_i8_unroll2,  cpu_num, "[x86] ===== Add 8 bit Signed Integers =====");
    type_result["int8_t"] = res_int8_t_scalar;
    Results* res_int16_t_scalar = zero_arg_kernel(pcm, &kernel_only_asm_add_i16_unroll2, cpu_num, "[x86] ===== Add 16 bit Signed Integers =====");
    type_result["int16_t"] = res_int16_t_scalar;
    Results* res_int32_t_scalar = zero_arg_kernel(pcm, &kernel_only_asm_add_i32_unroll2, cpu_num, "[x86] ===== Add 32 bit Signed Integers =====");
    type_result["int32_t"] = res_int32_t_scalar;
    Results* res_int64_t_scalar = zero_arg_kernel(pcm, &kernel_only_asm_add_i64_unroll2, cpu_num, "[x86_64] ===== Add 64 bit Signed Integers =====");
    type_result["int64_t"] = res_int64_t_scalar;

    Results* res_single_scalar = zero_arg_kernel(pcm, &kernel_only_asm_add_sp_unroll2, cpu_num, "[SSE2] ===== Add Float Point (single via SSE2) =====");
    type_result["float"] = res_single_scalar;
    Results* res_double_scalar = zero_arg_kernel(pcm, &kernel_only_asm_add_dp_unroll2, cpu_num, "[SSE2] ===== Add Floating Point (double via SSE2) =====");
    type_result["double"] = res_double_scalar;

    Results* res_x87_scalar = zero_arg_kernel(pcm, &kernel_only_asm_add_fp_unroll2, cpu_num, "[x87] ===== Add Floating Point (via x87) =====");
    type_result["x87 Floating Point"] = res_x87_scalar;
}

void test_only_add(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Add Benchmarks ##########\n");
    #ifdef __AVX2__
        Results* res_int8_t =  zero_arg_kernel(pcm, &kernel_only_asm_mm256_add_epi8,  cpu_num, "[AVX2] ===== Add 32 Packed 8 bit Signed Integers (_mm256_add_epi8) =====");
        type_result["int8_t"] = res_int8_t;
        Results* res_int16_t = zero_arg_kernel(pcm, &kernel_only_asm_mm256_add_epi16, cpu_num, "[AVX2] ===== Add 16 Packed 16 bit Signed Integers (_mm256_add_epi16) =====");
        type_result["int16_t"] = res_int16_t;
        Results* res_int32_t = zero_arg_kernel(pcm, &kernel_only_asm_mm256_add_epi32, cpu_num, "[AVX2] ===== Add 8 Packed 32 bit Signed Integers (_mm256_add_epi32) =====");
        type_result["int32_t"] = res_int32_t;
    #endif

    #ifdef __AVX__
        Results* res_float =   zero_arg_kernel(pcm, &kernel_only_asm_mm256_add_ps,    cpu_num, "[AVX] ===== Add 8 Packed 32 bit Signed Floating Point Numbers (_mm256_add_ps) =====");
        type_result["float"] = res_float;
        Results* res_double =  zero_arg_kernel(pcm, &kernel_only_asm_mm256_add_pd,    cpu_num, "[AVX] ===== Add 4 Packed 64 bit Signed Floating Point Numbers (_mm256_add_pd) =====");
        type_result["double"] = res_double;
    #endif
}

void test_only_mult_scalar(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Scalar Mult Benchmarks ##########\n");
    Results* res_int8_t_scalar =  zero_arg_kernel(pcm, &kernel_only_asm_mult_i8,  cpu_num, "[x86] ===== Mult 8 bit Signed Integers =====");
    type_result["int8_t"] = res_int8_t_scalar;
    Results* res_int16_t_scalar = zero_arg_kernel(pcm, &kernel_only_asm_mult_i16, cpu_num, "[x86] ===== Mult 16 bit Signed Integers =====");
    type_result["int16_t"] = res_int16_t_scalar;
    Results* res_int32_t_scalar = zero_arg_kernel(pcm, &kernel_only_asm_mult_i32, cpu_num, "[x86] ===== Mult 32 bit Signed Integers =====");
    type_result["int32_t"] = res_int32_t_scalar;
    Results* res_int64_t_scalar = zero_arg_kernel(pcm, &kernel_only_asm_mult_i64, cpu_num, "[x86_64] ===== Mult 64 bit Signed Integers =====");
    type_result["int64_t"] = res_int64_t_scalar;

    Results* res_single_scalar = zero_arg_kernel(pcm, &kernel_only_asm_mult_sp, cpu_num, "[SSE2] ===== Mult Float Point (single via SSE2) =====");
    type_result["float"] = res_single_scalar;
    Results* res_double_scalar = zero_arg_kernel(pcm, &kernel_only_asm_mult_dp, cpu_num, "[SSE2] ===== Mult Floating Point (double via SSE2) =====");
    type_result["double"] = res_double_scalar;

    Results* res_x87_scalar = zero_arg_kernel(pcm, &kernel_only_asm_mult_fp, cpu_num, "[x87] ===== Mult Floating Point (via x87) =====");
    type_result["x87 Floating Point"] = res_x87_scalar;
}

void test_only_mult_scalar_regRename(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Scalar Mult Benchmarks (Reg Rename) ##########\n");
    Results* res_int8_t_scalar =  zero_arg_kernel(pcm, &kernel_only_asm_mult_i8_regRename,  cpu_num, "[x86] ===== Mult 8 bit Signed Integers =====");
    type_result["int8_t"] = res_int8_t_scalar;
}

void test_only_mult_scalar_unroll2(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Scalar Mult Benchmarks (Unroll 2) ##########\n");
    Results* res_int8_t_scalar =  zero_arg_kernel(pcm, &kernel_only_asm_mult_i8_unroll2,  cpu_num, "[x86] ===== Mult 8 bit Signed Integers =====");
    type_result["int8_t"] = res_int8_t_scalar;
    Results* res_int16_t_scalar = zero_arg_kernel(pcm, &kernel_only_asm_mult_i16_unroll2, cpu_num, "[x86] ===== Mult 16 bit Signed Integers =====");
    type_result["int16_t"] = res_int16_t_scalar;
    Results* res_int32_t_scalar = zero_arg_kernel(pcm, &kernel_only_asm_mult_i32_unroll2, cpu_num, "[x86] ===== Mult 32 bit Signed Integers =====");
    type_result["int32_t"] = res_int32_t_scalar;
    Results* res_int64_t_scalar = zero_arg_kernel(pcm, &kernel_only_asm_mult_i64_unroll2, cpu_num, "[x86_64] ===== Mult 64 bit Signed Integers =====");
    type_result["int64_t"] = res_int64_t_scalar;

    Results* res_single_scalar = zero_arg_kernel(pcm, &kernel_only_asm_mult_sp_unroll2, cpu_num, "[SSE2] ===== Mult Float Point (single via SSE2) =====");
    type_result["float"] = res_single_scalar;
    Results* res_double_scalar = zero_arg_kernel(pcm, &kernel_only_asm_mult_dp_unroll2, cpu_num, "[SSE2] ===== Mult Floating Point (double via SSE2) =====");
    type_result["double"] = res_double_scalar;

    Results* res_x87_scalar = zero_arg_kernel(pcm, &kernel_only_asm_mult_fp_unroll2, cpu_num, "[x87] ===== Mult Floating Point (via x87) =====");
    type_result["x87 Floating Point"] = res_x87_scalar;
}

void test_only_mult_scalar_unroll2_regRename(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Scalar Mult Benchmarks (Unroll 2, Reg Rename) ##########\n");
    Results* res_int8_t_scalar =  zero_arg_kernel(pcm, &kernel_only_asm_mult_i8_unroll2_regRename,  cpu_num, "[x86] ===== Mult 8 bit Signed Integers =====");
    type_result["int8_t"] = res_int8_t_scalar;
}

void test_only_mult(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Multiply Benchmarks ##########\n");
    #ifdef __AVX2__
        Results* res_int16_t = zero_arg_kernel(pcm, &kernel_only_asm_mm256_mullo_epi16, cpu_num, "[AVX2] ===== Multiply 16 Packed 16 bit Signed Integers -> Produce 32 Bit Intermediates (_mm256_mullo_epi16) =====");
        type_result["int16_t"] = res_int16_t;
        Results* res_int32_t = zero_arg_kernel(pcm, &kernel_only_asm_mm256_mullo_epi32, cpu_num, "[AVX2] ===== Multiply 8 Packed 32 bit Signed Integers -> Produce 64 Bit Intermediates  (_mm256_mullo_epi32) =====");
        type_result["int32_t"] = res_int32_t;
    #endif

    #ifdef __AVX__
        Results* res_float   = zero_arg_kernel(pcm, &kernel_only_asm_mm256_mul_ps, cpu_num, "[AVX] ===== Multiply 8 Packed 32 bit Signed Floating Point Numbers (_mm256_mul_ps) =====");
        type_result["float"] = res_float;
        Results* res_double  = zero_arg_kernel(pcm, &kernel_only_asm_mm256_mul_pd, cpu_num, "[AVX] ===== Multiply 4 Packed 64 bit Signed Floating Point Numbers (_mm256_mul_pd) =====");
        type_result["double"] = res_double;
    #endif
}

void test_only_div(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Divide Benchmarks ##########\n");
    #ifdef __AVX__
        Results* res_float = zero_arg_kernel(pcm, &kernel_only_asm_mm256_div_ps, cpu_num, "[AVX] ===== Divide 8 Packed 32 bit Signed Floating Point Numbers (_mm256_div_ps) =====");
        type_result["float"] = res_float;
        Results* res_double = zero_arg_kernel(pcm, &kernel_only_asm_mm256_div_pd, cpu_num, "[AVX] ===== Divide 4 Packed 64 bit Signed Floating Point Numbers (_mm256_div_pd) =====");
        type_result["double"] = res_double;
    #endif
}

void test_only_fma(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## FMA Benchmarks ##########\n");
    #ifdef __FMA__
        Results* res_float   = zero_arg_kernel(pcm, &kernel_only_asm_mm256_fmadd_ps, cpu_num, "[FMA] ===== FMA 8 Packed 32 bit Signed Floating Point Numbers [Store in Local Var] (_mm256_fmadd_ps) =====");
        type_result["float"] = res_float;
        Results* res_double  = zero_arg_kernel(pcm, &kernel_only_asm_mm256_fmadd_pd, cpu_num, "[FMA] ===== FMA 4 Packed 64 bit Signed Floating Point Numbers [Store in Local Var] (_mm256_fmadd_pd) =====");
        type_result["double"] = res_double;
    #endif
}

void test_add(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Load/Add/Store Benchmarks ##########\n");
    #ifdef __AVX2__
        Results* res_int8_t =  load_store_three_arg_kernel<__m256i, int8_t>  (pcm, &kernel_mm256_add_epi8,  cpu_num, "[AVX2] ===== Load/Add/Store 32 Packed 8 bit Signed Integers (_mm256_add_epi8) =====");
        type_result["int8_t"] = res_int8_t;
        Results* res_int16_t = load_store_three_arg_kernel<__m256i, int16_t> (pcm, &kernel_mm256_add_epi16, cpu_num, "[AVX2] ===== Load/Add/Store 16 Packed 16 bit Signed Integers (_mm256_add_epi16) =====");
        type_result["int16_t"] = res_int16_t;
        Results* res_int32_t = load_store_three_arg_kernel<__m256i, int32_t> (pcm, &kernel_mm256_add_epi32, cpu_num, "[AVX2] ===== Load/Add/Store 8 Packed 32 bit Signed Integers (_mm256_add_epi32) =====");
        type_result["int32_t"] = res_int32_t;
    #endif

    #ifdef __AVX__
        Results* res_float =   load_store_three_arg_kernel<__m256,  float>   (pcm, &kernel_mm256_add_ps,    cpu_num, "[AVX] ===== Load/Add/Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_add_ps) =====");
        type_result["float"] = res_float;
        Results* res_double =  load_store_three_arg_kernel<__m256d, double>  (pcm, &kernel_mm256_add_pd,    cpu_num, "[AVX] ===== Load/Add/Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_add_pd) =====");
        type_result["double"] = res_double;
    #endif
}

void test_nointrin_add(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Load/Add/Store (No Intrinsics) Benchmarks ##########\n");
    Results* res_int8_t =  no_vec_three_arg_kernel<int8_t>  (pcm, &kernel_nointrin_mm256_add_epi8,  cpu_num, "[No Intrin] ===== Load/Add/Store 32 Packed 8 bit Signed Integers (_mm256_add_epi8) =====");
    type_result["int8_t"] = res_int8_t;
    Results* res_int16_t = no_vec_three_arg_kernel<int16_t> (pcm, &kernel_nointrin_mm256_add_epi16, cpu_num, "[No Intrin] ===== Load/Add/Store 16 Packed 16 bit Signed Integers (_mm256_add_epi16) =====");
    type_result["int16_t"] = res_int16_t;
    Results* res_int32_t = no_vec_three_arg_kernel<int32_t> (pcm, &kernel_nointrin_mm256_add_epi32, cpu_num, "[No Intrin] ===== Load/Add/Store 8 Packed 32 bit Signed Integers (_mm256_add_epi32) =====");
    type_result["int32_t"] = res_int32_t;

    Results* res_float =   no_vec_three_arg_kernel<float>   (pcm, &kernel_nointrin_mm256_add_ps,    cpu_num, "[No Intrin] ===== Load/Add/Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_add_ps) =====");
    type_result["float"] = res_float;
    Results* res_double =  no_vec_three_arg_kernel<double>  (pcm, &kernel_nointrin_mm256_add_pd,    cpu_num, "[No Intrin] ===== Load/Add/Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_add_pd) =====");
    type_result["double"] = res_double;
}

void test_unroll2_add(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Load/Add/Store (Unrolled by 2) Benchmarks ##########\n");
    #ifdef __AVX2__
        Results* res_int8_t =  load_store_three_arg_kernel<__m256i, int8_t>  (pcm, &kernel_u2_mm256_add_epi8,  cpu_num, "[AVX2] ===== (Unroll 2) Load/Add/Store 32 Packed 8 bit Signed Integers (_mm256_add_epi8) =====");
        type_result["int8_t"] = res_int8_t;
        Results* res_int16_t = load_store_three_arg_kernel<__m256i, int16_t> (pcm, &kernel_u2_mm256_add_epi16, cpu_num, "[AVX2] ===== (Unroll 2) Load/Add/Store 16 Packed 16 bit Signed Integers (_mm256_add_epi16) =====");
        type_result["int16_t"] = res_int16_t;
        Results* res_int32_t = load_store_three_arg_kernel<__m256i, int32_t> (pcm, &kernel_u2_mm256_add_epi32, cpu_num, "[AVX2] ===== (Unroll 2) Load/Add/Store 8 Packed 32 bit Signed Integers (_mm256_add_epi32) =====");
        type_result["int32_t"] = res_int32_t;
    #endif

    #ifdef __AVX__
        Results* res_float =   load_store_three_arg_kernel<__m256,  float>   (pcm, &kernel_u2_mm256_add_ps,    cpu_num, "[AVX] ===== (Unroll 2) Load/Add/Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_add_ps) =====");
        type_result["float"] = res_float;
        Results* res_double =  load_store_three_arg_kernel<__m256d, double>  (pcm, &kernel_u2_mm256_add_pd,    cpu_num, "[AVX] ===== (Unroll 2) Load/Add/Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_add_pd) =====");
        type_result["double"] = res_double;
    #endif
}

void test_mult(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Load/Multiply/Store Benchmarks ##########\n");
    #ifdef __AVX2__
        Results* res_int16_t = load_store_three_arg_kernel<__m256i, int16_t> (pcm, &kernel_mm256_mullo_epi16, cpu_num, "[AVX2] ===== Load/Multiply 16 Packed 16 bit Signed Integers -> Produce 32 Bit Intermediates -> Store Lower 16 Bits of Intermediates (_mm256_mullo_epi16) =====");
        type_result["int16_t"] = res_int16_t;
        Results* res_int32_t = load_store_three_arg_kernel<__m256i, int32_t> (pcm, &kernel_mm256_mullo_epi32, cpu_num, "[AVX2] ===== Load/Multiply 8 Packed 32 bit Signed Integers -> Produce 64 Bit Intermediates -> Store Lower 32 Bits of Intermediates (_mm256_mullo_epi32) =====");
        type_result["int32_t"] = res_int32_t;
    #endif

    #ifdef __AVX__
        Results* res_float =   load_store_three_arg_kernel<__m256,  float>   (pcm, &kernel_mm256_mul_ps,      cpu_num, "[AVX] ===== Load/Multiply/Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_mul_ps) =====");
        type_result["float"] = res_float;
        Results* res_double =  load_store_three_arg_kernel<__m256d, double>  (pcm, &kernel_mm256_mul_pd,      cpu_num, "[AVX] ===== Load/Multiply/Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_mul_pd) =====");
        type_result["double"] = res_double;
    #endif
}

void test_div(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Load/Divide/Store Benchmarks ##########\n");
    #ifdef __AVX__
        Results* res_float =  load_store_three_arg_kernel<__m256,  float>  (pcm, &kernel_mm256_div_ps, cpu_num, "[AVX] ===== Load/Divide/Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_div_ps) =====");
        type_result["float"] = res_float;
        Results* res_double = load_store_three_arg_kernel<__m256d, double> (pcm, &kernel_mm256_div_pd, cpu_num, "[AVX] ===== Load/Divide/Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_div_pd) =====");
        type_result["double"] = res_double;
    #endif
}

void test_fma(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Load/FMA/Store Benchmarks ##########\n");
    #ifdef __FMA__
        Results* res_float =  load_store_four_arg_kernel<__m256,  float>  (pcm, &kernel_mm256_fmadd_ps,    cpu_num, "[FMA] ===== Load/FMA/Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_fmadd_ps) =====");
        type_result["float"] = res_float;
        Results* res_double = load_store_four_arg_kernel<__m256d, double> (pcm, &kernel_mm256_fmadd_pd,    cpu_num, "[FMA] ===== Load/FMA/Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_fmadd_pd) =====");
        type_result["double"] = res_double;
    #endif
}

void* run_benchmarks(void* cpu_num)
{
    #if PRINT_TITLE == 1
    printf("SSE/AVX/FMA Intrinsic Tester\n");
    printf("STIM_LEN: %d (Samples/Vector/Trial), TRIALS: %d\n", STIM_LEN, TRIALS);
    #endif

#if USE_PCM == 1

    bool print_topology = false;
    #if PRINT_TITLE == 1
    print_topology = true;
    #endif

    #if PRINT_TITLE == 1
    printf("\n");
    printf("****** Platform Information Provided by PCM ******\n");
    #endif

    PCM* pcm = init_PCM(print_topology);

    #if PRINT_TITLE == 1
    printf("**************************************************\n");
    printf("CPU Brand String: %s\n", pcm->getCPUBrandString().c_str());
    printf("**************************************************\n");
    #endif

    int* cpu_num_int = (int*) cpu_num;
    int socket = pcm->getSocketId(*cpu_num_int);

    #if PRINT_TITLE == 1
    printf("Executing on Core: %3d (Socket: %2d)\n", *cpu_num_int, socket);
    printf("**************************************************\n");
    printf("\n");
    #endif

#else
    PCM* pcm = NULL;
    int* cpu_num_int = (int*) cpu_num;
    int socket = 0;
#endif

    std::map<std::string, std::map<std::string, Results*>*> kernel_results;

    std::map<std::string, Results*>* load_results = new std::map<std::string, Results*>;
    test_load(pcm, *cpu_num_int, *load_results);
    kernel_results["Load"] = load_results;
    printf("\n");

    std::map<std::string, Results*>* load_results_unroll2 = new std::map<std::string, Results*>;
    test_load_unroll2(pcm, *cpu_num_int, *load_results_unroll2);
    kernel_results["Load (Unroll 2)"] = load_results_unroll2;
    printf("\n");

    std::map<std::string, Results*>* store_results = new std::map<std::string, Results*>;
    test_store(pcm, *cpu_num_int, *store_results);
    kernel_results["Store"] = store_results;
    printf("\n");

    std::map<std::string, Results*>* store_results_unroll2 = new std::map<std::string, Results*>;
    test_store_unroll2(pcm, *cpu_num_int, *store_results_unroll2);
    kernel_results["Store (Unroll 2)"] = store_results_unroll2;
    printf("\n");

    std::map<std::string, Results*>* load_store_results = new std::map<std::string, Results*>;
    test_load_store(pcm, *cpu_num_int, *load_store_results);
    kernel_results["Load/Store"] = load_store_results;
    printf("\n");

    std::map<std::string, Results*>* only_add_results_scalar = new std::map<std::string, Results*>;
    test_only_add_scalar(pcm, *cpu_num_int, *only_add_results_scalar);
    kernel_results["Add (Scalar)"] = only_add_results_scalar;
    printf("\n");

    std::map<std::string, Results*>* only_add_results_scalar_unroll2 = new std::map<std::string, Results*>;
    test_only_add_scalar_unroll2(pcm, *cpu_num_int, *only_add_results_scalar_unroll2);
    kernel_results["Add (Scalar, Unroll 2)"] = only_add_results_scalar_unroll2;
    printf("\n");

    std::map<std::string, Results*>* only_add_results = new std::map<std::string, Results*>;
    test_only_add(pcm, *cpu_num_int, *only_add_results);
    kernel_results["Add"] = only_add_results;
    printf("\n");

    std::map<std::string, Results*>* only_mult_results_scalar = new std::map<std::string, Results*>;
    test_only_mult_scalar(pcm, *cpu_num_int, *only_mult_results_scalar);
    kernel_results["Mult (Scalar)"] = only_mult_results_scalar;
    printf("\n");
    std::map<std::string, Results*>* only_mult_results_scalar_unroll2 = new std::map<std::string, Results*>;
    test_only_mult_scalar_unroll2(pcm, *cpu_num_int, *only_mult_results_scalar_unroll2);
    kernel_results["Mult (Scalar, Unroll 2)"] = only_mult_results_scalar_unroll2;
    printf("\n");

    std::map<std::string, Results*>* only_mult_results_scalar_regRename = new std::map<std::string, Results*>;
    test_only_mult_scalar_regRename(pcm, *cpu_num_int, *only_mult_results_scalar_regRename);
    kernel_results["Mult (Scalar, Reg Rename)"] = only_mult_results_scalar_regRename;
    printf("\n");
    std::map<std::string, Results*>* only_mult_results_scalar_unroll2_regRename = new std::map<std::string, Results*>;
    test_only_mult_scalar_unroll2_regRename(pcm, *cpu_num_int, *only_mult_results_scalar_unroll2_regRename);
    kernel_results["Mult (Scalar, Reg Rename, Unroll 2)"] = only_mult_results_scalar_unroll2_regRename;
    printf("\n");

    std::map<std::string, Results*>* only_mult_results = new std::map<std::string, Results*>;
    test_only_mult(pcm, *cpu_num_int, *only_mult_results);
    kernel_results["Mult"] = only_mult_results;
    printf("\n");
    std::map<std::string, Results*>* only_div_results = new std::map<std::string, Results*>;
    test_only_div(pcm, *cpu_num_int, *only_div_results);
    kernel_results["Div"] = only_div_results;
    printf("\n");
    std::map<std::string, Results*>* only_fma_results = new std::map<std::string, Results*>;
    test_only_fma(pcm, *cpu_num_int, *only_fma_results);
    kernel_results["FMA"] = only_fma_results;
    printf("\n");

    std::map<std::string, Results*>* load_add_store_results = new std::map<std::string, Results*>;
    test_add(pcm, *cpu_num_int, *load_add_store_results);
    kernel_results["Load/Add/Store"] = load_add_store_results;
    printf("\n");
    std::map<std::string, Results*>* load_mult_store_results = new std::map<std::string, Results*>;
    test_mult(pcm, *cpu_num_int, *load_mult_store_results);
    kernel_results["Load/Mult/Store"] = load_mult_store_results;
    printf("\n");
    std::map<std::string, Results*>* load_div_store_results = new std::map<std::string, Results*>;
    test_div(pcm, *cpu_num_int, *load_div_store_results);
    kernel_results["Load/Div/Store"] = load_div_store_results;
    printf("\n");
    std::map<std::string, Results*>* load_fma_store_results = new std::map<std::string, Results*>;
    test_fma(pcm, *cpu_num_int, *load_fma_store_results);
    kernel_results["Load/FMA/Store"] = load_fma_store_results;
    printf("\n");

    std::map<std::string, Results*>* load_add_store_unroll2_results = new std::map<std::string, Results*>;
    test_unroll2_add(pcm, *cpu_num_int, *load_add_store_unroll2_results);
    kernel_results["Load/Add/Store Unroll2"] = load_add_store_unroll2_results;
    printf("\n");

    std::map<std::string, Results*>* load_add_store_nointrin_results = new std::map<std::string, Results*>;
    test_nointrin_add(pcm, *cpu_num_int, *load_add_store_nointrin_results);
    kernel_results["Load/Add/Store No Intrin"] = load_add_store_nointrin_results;

    std::vector<std::string> types;
    types.push_back("int8_t");
    types.push_back("int16_t");
    types.push_back("int32_t");
    types.push_back("int64_t");
    types.push_back("float");
    types.push_back("double");
    types.push_back("x87 Floating Point");

    std::vector<std::string> kernels;
    std::vector<std::string> vec_ext;

    kernels.push_back("Load");                                  vec_ext.push_back("AVX");
    kernels.push_back("Load (Unroll 2)");                       vec_ext.push_back("AVX");
    kernels.push_back("Store");                                 vec_ext.push_back("AVX");
    kernels.push_back("Store (Unroll 2)");                      vec_ext.push_back("AVX");
    kernels.push_back("Add (Scalar)");                          vec_ext.push_back("x86 / x86_64 / x87 / SSE2");
    kernels.push_back("Add (Scalar, Unroll 2)");                vec_ext.push_back("x86 / x86_64 / x87 / SSE2");
    kernels.push_back("Add");                                   vec_ext.push_back("AVX (Float) / AVX2 (Int)");
    kernels.push_back("Mult (Scalar)");                         vec_ext.push_back("x86 / x86_64 / x87 / SSE2");
    kernels.push_back("Mult (Scalar, Unroll 2)");               vec_ext.push_back("x86 / x86_64 / x87 / SSE2");
    kernels.push_back("Mult (Scalar, Reg Rename)");             vec_ext.push_back("x86");
    kernels.push_back("Mult (Scalar, Reg Rename, Unroll 2)");   vec_ext.push_back("x86");
    kernels.push_back("Mult");                                  vec_ext.push_back("AVX (Float) / AVX2 (Int)");
    kernels.push_back("Div");                                   vec_ext.push_back("AVX");
    kernels.push_back("FMA");                                   vec_ext.push_back("FMA");
    kernels.push_back("Load/Store");                            vec_ext.push_back("AVX");
    kernels.push_back("Load/Add/Store");                        vec_ext.push_back("AVX (Float) / AVX2 (Int)");
    kernels.push_back("Load/Mult/Store");                       vec_ext.push_back("AVX (Float) / AVX2 (Int)");
    kernels.push_back("Load/Div/Store");                        vec_ext.push_back("AVX");
    kernels.push_back("Load/FMA/Store");                        vec_ext.push_back("FMA");
    kernels.push_back("Load/Add/Store No Intrin");              vec_ext.push_back("N/A");
    kernels.push_back("Load/Add/Store Unroll2");                vec_ext.push_back("AVX (Float) / AVX2 (Int)");

    //Open CSV File to write
    FILE * csv_file;
    csv_file = fopen("intrin_report.csv", "w");
    
    //=====Write header=====
    //Number of columns = 2 + Number of Benchmarks*2 (1 for Mean and 1 for StdDev)
    //Print Title + Benchmark Names
    fprintf(csv_file, "\"x86_64 Vector Extension Micro-Benchmarks\",");
    for(std::vector<std::string>::iterator it = kernels.begin(); it != kernels.end(); it++)
    {
        fprintf(csv_file, ",\"%s\",", it->c_str());
    }
    fprintf(csv_file, "\n");

    //Print machine information + Vector Extensions
    fprintf(csv_file, "\"%s\",", pcm->getCPUBrandString().c_str());
    for(std::vector<std::string>::iterator it = vec_ext.begin(); it != vec_ext.end(); it++)
    {
        fprintf(csv_file, ",\"%s\",", it->c_str());
    }
    fprintf(csv_file, "\n");

    //Print Hostname, Mean, StdDev Labels
    char hostname[300];
    int got_host = gethostname(hostname, 300);
    if(got_host != 0)
    {
        sprintf(hostname, "Unavailable");
    }

    //Getting time: Example from http://en.cppreference.com/w/cpp/chrono
    auto wall_clock = std::chrono::system_clock::now();
    std::time_t report_gen_time = std::chrono::system_clock::to_time_t(wall_clock);

    fprintf(csv_file, "\"Host: %s, Stimulus Length: %d, Trials: %d, Report Generated: %s\",", hostname, STIM_LEN, TRIALS, ctime(&report_gen_time));
    for(size_t i = 0; i<kernels.size(); i++)
    {
        fprintf(csv_file, ",\"Mean\",\"Std Dev\"");
    }
    fprintf(csv_file, "\n");

    //======Print table======
    //Print Rate (Ms/s)
    for(size_t i = 0; i<types.size(); i++)
    {
        std::string datatype = types[i];

        //Print Descr:
        if(i == 0)
        {
            fprintf(csv_file, "\"Rate (Ms/s)\",\"%s\"", datatype.c_str());
        }
        else
        {
            fprintf(csv_file, ",\"%s\"", datatype.c_str());
        }

        //Print Data
        for(size_t j = 0; j<kernels.size(); j++)
        {
            std::string kernel_name = kernels[j];
            std::map<std::string, std::map<std::string, Results*>*>::iterator result_container_it = kernel_results.find(kernel_name);

            if(result_container_it == kernel_results.end())
            {
                //No such kernel exists.  Print empty values
                fprintf(csv_file, ",,");
            }
            else
            {
                std::map<std::string, Results*>::iterator result_it = result_container_it->second->find(datatype);
                if(result_it == result_container_it->second->end())
                {
                    //No such result exists.  Print empty values
                    fprintf(csv_file, ",,");
                }
                else
                {
                    //Print result (only mean in this case)

                    double mean_exe_time = result_it->second->avg_duration_clock();
                    double scaled_mean = STIM_LEN*1.0/(1000.0*mean_exe_time);
                    fprintf(csv_file, ",%e,", scaled_mean);
                }
            }
        }
        fprintf(csv_file, "\n");
    }

    //Print Execution Time Normalized to 1 Sample
    for(size_t i = 0; i<types.size(); i++)
    {
        std::string datatype = types[i];

        //Print Descr:
        if(i == 0)
        {
            fprintf(csv_file, "\"Normalized Execution Time for 1 Sample (ns)\",\"%s\"", datatype.c_str());
        }
        else
        {
            fprintf(csv_file, ",\"%s\"", datatype.c_str());
        }

        //Print Data
        for(size_t j = 0; j<kernels.size(); j++)
        {
            std::string kernel_name = kernels[j];
            std::map<std::string, std::map<std::string, Results*>*>::iterator result_container_it = kernel_results.find(kernel_name);

            if(result_container_it == kernel_results.end())
            {
                //No such kernel exists.  Print empty values
                fprintf(csv_file, ",,");
            }
            else
            {
                std::map<std::string, Results*>::iterator result_it = result_container_it->second->find(datatype);
                if(result_it == result_container_it->second->end())
                {
                    //No such result exists.  Print empty values
                    fprintf(csv_file, ",,");
                }
                else
                {
                    double mean_exe_time = result_it->second->avg_duration_clock();
                    double scaled_mean = mean_exe_time*1000000/STIM_LEN;

                    double stddev_exe_time = result_it->second->stddev_duration_clock();
                    double scaled_stddev = stddev_exe_time*1000000/STIM_LEN;

                    fprintf(csv_file, ",%e,%e", scaled_mean, scaled_stddev);
                }
            }
        }
        fprintf(csv_file, "\n");
    }

    //Print Energy Use Normalized to 1 Sample
        for(size_t i = 0; i<types.size(); i++)
    {
        std::string datatype = types[i];

        //Print Descr:
        if(i == 0)
        {
            fprintf(csv_file, "\"Normalized CPU Energy Use Time for 1 Sample (nJ)\",\"%s\"", datatype.c_str());
        }
        else
        {
            fprintf(csv_file, ",\"%s\"", datatype.c_str());
        }

        //Print Data
        for(size_t j = 0; j<kernels.size(); j++)
        {
            std::string kernel_name = kernels[j];
            std::map<std::string, std::map<std::string, Results*>*>::iterator result_container_it = kernel_results.find(kernel_name);

            if(result_container_it == kernel_results.end())
            {
                //No such kernel exists.  Print empty values
                fprintf(csv_file, ",,");
            }
            else
            {
                std::map<std::string, Results*>::iterator result_it = result_container_it->second->find(datatype);
                if(result_it == result_container_it->second->end())
                {
                    //No such result exists.  Print empty values
                    fprintf(csv_file, ",,");
                }
                else
                {
                    double mean_cpu_energy = result_it->second->avg_EnergyCPUUsed(socket);
                    double scaled_mean = mean_cpu_energy*1000000000/STIM_LEN;

                    double stddev_cpu_energy = result_it->second->stddev_EnergyCPUUsed(socket);
                    double scaled_stddev = stddev_cpu_energy*1000000000/STIM_LEN;

                    fprintf(csv_file, ",%e,%e", scaled_mean, scaled_stddev);
                }
            }
        }
        fprintf(csv_file, "\n");
    }

    //Print Clk Frequency
    for(size_t i = 0; i<types.size(); i++)
    {
        std::string datatype = types[i];

        //Print Descr:
        if(i == 0)
        {
            fprintf(csv_file, "\"CPU Clk Frequency (MHz)\",\"%s\"", datatype.c_str());
        }
        else
        {
            fprintf(csv_file, ",\"%s\"", datatype.c_str());
        }

        //Print Data
        for(size_t j = 0; j<kernels.size(); j++)
        {
            std::string kernel_name = kernels[j];
            std::map<std::string, std::map<std::string, Results*>*>::iterator result_container_it = kernel_results.find(kernel_name);

            if(result_container_it == kernel_results.end())
            {
                //No such kernel exists.  Print empty values
                fprintf(csv_file, ",,");
            }
            else
            {
                std::map<std::string, Results*>::iterator result_it = result_container_it->second->find(datatype);
                if(result_it == result_container_it->second->end())
                {
                    //No such result exists.  Print empty values
                    fprintf(csv_file, ",,");
                }
                else
                {
                    double avg_cpu_per_dbl = result_it->second->avg_CPUPer(*cpu_num_int);
                    double freq_mean = 1/avg_cpu_per_dbl/1000000;


                    fprintf(csv_file, ",%e,", freq_mean);
                }
            }
        }
        fprintf(csv_file, "\n");
    }

    //Print Clk Per
    for(size_t i = 0; i<types.size(); i++)
    {
        std::string datatype = types[i];

        //Print Descr:
        if(i == 0)
        {
            fprintf(csv_file, "\"CPU Clk Period (ns)\",\"%s\"", datatype.c_str());
        }
        else
        {
            fprintf(csv_file, ",\"%s\"", datatype.c_str());
        }

        //Print Data
        for(size_t j = 0; j<kernels.size(); j++)
        {
            std::string kernel_name = kernels[j];
            std::map<std::string, std::map<std::string, Results*>*>::iterator result_container_it = kernel_results.find(kernel_name);

            if(result_container_it == kernel_results.end())
            {
                //No such kernel exists.  Print empty values
                fprintf(csv_file, ",,");
            }
            else
            {
                std::map<std::string, Results*>::iterator result_it = result_container_it->second->find(datatype);
                if(result_it == result_container_it->second->end())
                {
                    //No such result exists.  Print empty values
                    fprintf(csv_file, ",,");
                }
                else
                {
                    double avg_cpu_per_dbl = result_it->second->avg_CPUPer(*cpu_num_int);
                    double scaled_mean = avg_cpu_per_dbl*1000000000;

                    double stddev_cpu_per_dbl = result_it->second->stddev_CPUPer(*cpu_num_int);
                    double scaled_stddev = stddev_cpu_per_dbl*1000000000;


                    fprintf(csv_file, ",%e,%e", scaled_mean, scaled_stddev);
                }
            }
        }
        fprintf(csv_file, "\n");
    }

    //Close report file
    fclose(csv_file);

    //Cleanup
    for (std::map<std::string, std::map<std::string, Results*>*>::iterator it = kernel_results.begin(); it != kernel_results.end(); it++)
    {
        std::map<std::string, Results*>* sub_result = it->second;

        for (std::map<std::string, Results*>::iterator sub_it = sub_result->begin(); sub_it != sub_result->end(); sub_it++)
        {
            sub_it->second->delete_results();
            delete sub_it->second;
        }

        delete it->second;
    }

    #if PRINT_TITLE == 1
    printf("****** PCM Ceanup ******\n");
    #endif
    //Output from PCM appears when distructor runs

    return NULL;
}

int main(int argc, char *argv[])
{
    //Run these single-threaded benchmarks on CPU 0 (all machines should have CPU 0)

    //Based off of http://man7.org/linux/man-pages/man3/pthread_setaffinity_np.3.html
    //http://man7.org/linux/man-pages/man3/pthread_create.3.html
    //http://man7.org/linux/man-pages/man3/pthread_attr_setaffinity_np.3.html,
    //http://man7.org/linux/man-pages/man3/pthread_join.3.html

#if __APPLE__
#else
    cpu_set_t cpuset;
#endif
    pthread_t thread;
    pthread_attr_t attr;
    void *res;

    int status;

    status = pthread_attr_init(&attr);

    if(status != 0)
    {
        printf("Could not create pthread attributes ... exiting\n");
        exit(1);
    }

    int cpu_number = 0;

#if __APPLE__
    printf("Warning: Running on MacOS - Thread Affinity Not Set.  Disregard Core Number");
#else
    CPU_ZERO(&cpuset);
    CPU_SET(cpu_number, &cpuset);

    status = pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);

    if(status != 0)
    {
        printf("Could not set thread core affinity ... exiting\n");
        exit(1);
    }
#endif

    status = pthread_create(&thread, &attr, &run_benchmarks, &cpu_number);
    if(status != 0)
    {
        printf("Could not create thread ... exiting\n");
        exit(1);
    }

    status = pthread_join(thread, &res);

    if(status != 0)
    {
        printf("Could not join thread ... exiting\n");
        exit(1);
    }

    free(res);

    return 0;
}
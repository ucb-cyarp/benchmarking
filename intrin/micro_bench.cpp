//Kernels Load/Store
#include "load_kernel_asm.h"
#include "load_kernel_asm_unroll2.h"
#include "load_kernel_asm_local_unroll2.h"
#include "load_kernel_asm_local.h"
#include "store_kernel_asm.h"
#include "store_kernel_asm_local.h"
#include "store_kernel_asm_unroll2.h"
#include "store_kernel_asm_local_unroll2.h"
#include "load_store_kernel.h"

#include "load_kernel_scalar_asm.h"
#include "load_kernel_scalar_asm_local.h"
#include "store_kernel_scalar_asm.h"
#include "store_kernel_scalar_asm_local.h"

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
#include "mult_kernel_scalar_asm_unroll4.h"
#include "mult_kernel_asm.h"
#include "mult_kernel_asm_unroll2.h"
#include "div_kernel_asm.h"
#include "div_kernel_scalar_asm.h"
#include "div_kernel_scalar_asm_unroll2.h"
#include "fma_kernel_asm.h"

//Kernels Special Case
#include "load_add_store_nointrin_kernel.h"
#include "load_add_store_unroll2_kernel.h"

//Kerneks Function Call
#include "fctn_call_kernel.h"

#include "benchmark_throughput_test.h"
#include "kernel_runner.h"

//#### Micro Benchmark Suite ####

void test_fctn_call(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Function Call Benchmarks ##########\n");
    Results* res_zero_arg = zero_arg_kernel(pcm, &kernel_zero_arg_fctn, cpu_num, "[x86_64] ===== Zero Arg Function Call =====");
    type_result["Zero Arg"] = res_zero_arg;
}

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
        Results* res_int64_t = load_store_one_arg_kernel<__m256i, int64_t> (pcm, &kernel_asm_mm256_load_si256_int64, cpu_num, "[AVX] ===== Load 4 Signed 64 Bit Integers (_mm256_load_si256) =====");
        type_result["int64_t"] = res_int64_t;
        Results* res_float =   load_store_one_arg_kernel<__m256,  float>   (pcm, &kernel_asm_mm256_load_ps,          cpu_num, "[AVX] ===== Load 8 Packed 32 bit Signed Floating Point Numbers (_mm256_load_ps) =====");
        type_result["float"] = res_float;
        Results* res_double =  load_store_one_arg_kernel<__m256d, double>  (pcm, &kernel_asm_mm256_load_pd,          cpu_num, "[AVX] ===== Load 4 Packed 64 bit Signed Floating Point Numbers (_mm256_load_pd) =====");
        type_result["double"] = res_double;
    #endif
}

void test_load_local(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Load Benchmarks (Local) ##########\n");
    #ifdef __AVX__
        Results* res_int8_t =  load_store_one_arg_kernel<__m256i, int8_t>  (pcm, &kernel_asm_mm256_load_si256_int8_local,  cpu_num, "[AVX] ===== Load 32 Signed Bytes (_mm256_load_si256) =====");
        type_result["int8_t"] = res_int8_t;
        Results* res_int16_t = load_store_one_arg_kernel<__m256i, int16_t> (pcm, &kernel_asm_mm256_load_si256_int16_local, cpu_num, "[AVX] ===== Load 16 Signed 16 Bit Integers(_mm256_load_si256) =====");
        type_result["int16_t"] = res_int16_t;
        Results* res_int32_t = load_store_one_arg_kernel<__m256i, int32_t> (pcm, &kernel_asm_mm256_load_si256_int32_local, cpu_num, "[AVX] ===== Load 8 Signed 32 Bit Integers (_mm256_load_si256) =====");
        type_result["int32_t"] = res_int32_t;
        Results* res_int64_t = load_store_one_arg_kernel<__m256i, int64_t> (pcm, &kernel_asm_mm256_load_si256_int64_local, cpu_num, "[AVX] ===== Load 4 Signed 64 Bit Integers (_mm256_load_si256) =====");
        type_result["int64_t"] = res_int64_t;
        Results* res_float =   load_store_one_arg_kernel<__m256,  float>   (pcm, &kernel_asm_mm256_load_ps_local,          cpu_num, "[AVX] ===== Load 8 Packed 32 bit Signed Floating Point Numbers (_mm256_load_ps) =====");
        type_result["float"] = res_float;
        Results* res_double =  load_store_one_arg_kernel<__m256d, double>  (pcm, &kernel_asm_mm256_load_pd_local,          cpu_num, "[AVX] ===== Load 4 Packed 64 bit Signed Floating Point Numbers (_mm256_load_pd) =====");
        type_result["double"] = res_double;
    #endif
}

void test_load_local_unroll2(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Load Benchmarks (Local, Unroll 2) ##########\n");
    #ifdef __AVX__
        Results* res_int8_t =  load_store_one_arg_kernel<__m256i, int8_t>  (pcm, &kernel_asm_mm256_load_si256_int8_local_unroll2,  cpu_num, "[AVX] ===== Load 32 Signed Bytes (_mm256_load_si256) =====");
        type_result["int8_t"] = res_int8_t;
        Results* res_int16_t = load_store_one_arg_kernel<__m256i, int16_t> (pcm, &kernel_asm_mm256_load_si256_int16_local_unroll2, cpu_num, "[AVX] ===== Load 16 Signed 16 Bit Integers(_mm256_load_si256) =====");
        type_result["int16_t"] = res_int16_t;
        Results* res_int32_t = load_store_one_arg_kernel<__m256i, int32_t> (pcm, &kernel_asm_mm256_load_si256_int32_local_unroll2, cpu_num, "[AVX] ===== Load 8 Signed 32 Bit Integers (_mm256_load_si256) =====");
        type_result["int32_t"] = res_int32_t;
        Results* res_int64_t = load_store_one_arg_kernel<__m256i, int64_t> (pcm, &kernel_asm_mm256_load_si256_int64_local_unroll2, cpu_num, "[AVX] ===== Load 4 Signed 64 Bit Integers (_mm256_load_si256) =====");
        type_result["int64_t"] = res_int64_t;
        Results* res_float =   load_store_one_arg_kernel<__m256,  float>   (pcm, &kernel_asm_mm256_load_ps_local_unroll2,          cpu_num, "[AVX] ===== Load 8 Packed 32 bit Signed Floating Point Numbers (_mm256_load_ps) =====");
        type_result["float"] = res_float;
        Results* res_double =  load_store_one_arg_kernel<__m256d, double>  (pcm, &kernel_asm_mm256_load_pd_local_unroll2,          cpu_num, "[AVX] ===== Load 4 Packed 64 bit Signed Floating Point Numbers (_mm256_load_pd) =====");
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
        Results* res_int64_t = load_store_one_arg_kernel<__m256i, int64_t> (pcm, &kernel_asm_mm256_load_si256_int64_unroll2, cpu_num, "[AVX] ===== Load 4 Signed 64 Bit Integers (_mm256_load_si256) =====");
        type_result["int64_t"] = res_int64_t;
        Results* res_float =   load_store_one_arg_kernel<__m256,  float>   (pcm, &kernel_asm_mm256_load_ps_unroll2,          cpu_num, "[AVX] ===== Load 8 Packed 32 bit Signed Floating Point Numbers (_mm256_load_ps) =====");
        type_result["float"] = res_float;
        Results* res_double =  load_store_one_arg_kernel<__m256d, double>  (pcm, &kernel_asm_mm256_load_pd_unroll2,          cpu_num, "[AVX] ===== Load 4 Packed 64 bit Signed Floating Point Numbers (_mm256_load_pd) =====");
        type_result["double"] = res_double;
    #endif
}

void test_load_scalar(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Load Benchmarks (Scalar) ##########\n");
    Results* res_int8_t =  load_store_one_arg_kernel<__m256i, int8_t>  (pcm, &kernel_asm_load_int8,  cpu_num, "[x86] ===== Load Bytes (movb) =====");
    type_result["int8_t"] = res_int8_t;
    Results* res_int16_t = load_store_one_arg_kernel<__m256i, int16_t> (pcm, &kernel_asm_load_int16, cpu_num, "[x86] ===== Load 16 Bit Integers (movw) =====");
    type_result["int16_t"] = res_int16_t;
    Results* res_int32_t = load_store_one_arg_kernel<__m256i, int32_t> (pcm, &kernel_asm_load_int32, cpu_num, "[x86] ===== Load 32 Bit Integers (movl) =====");
    type_result["int32_t"] = res_int32_t;
    Results* res_int64_t = load_store_one_arg_kernel<__m256i, int64_t> (pcm, &kernel_asm_load_int64, cpu_num, "[x86_64] ===== Load 64 Bit Integers (movq) =====");
    type_result["int64_t"] = res_int64_t;
    #ifdef __SSE2__
        Results* res_float =   load_store_one_arg_kernel<__m256,  float>   (pcm, &kernel_asm_load_ss,  cpu_num, "[SSE2] ===== Load 32 bit Signed Floating Point Numbers (vmovss) =====");
        type_result["float"] = res_float;
        Results* res_double =  load_store_one_arg_kernel<__m256d, double>  (pcm, &kernel_asm_load_sd, cpu_num, "[SSE2] ===== Load 64 bit Signed Floating Point Numbers (vmovsd) =====");
        type_result["double"] = res_double;
    #endif
}

void test_load_scalar_local(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Load Benchmarks (Scalar) ##########\n");
    Results* res_int8_t =  load_store_one_arg_kernel<__m256i, int8_t>  (pcm, &kernel_asm_load_int8_local,  cpu_num, "[x86] ===== Load Bytes (movb) =====");
    type_result["int8_t"] = res_int8_t;
    Results* res_int16_t = load_store_one_arg_kernel<__m256i, int16_t> (pcm, &kernel_asm_load_int16_local, cpu_num, "[x86] ===== Load 16 Bit Integers (movw) =====");
    type_result["int16_t"] = res_int16_t;
    Results* res_int32_t = load_store_one_arg_kernel<__m256i, int32_t> (pcm, &kernel_asm_load_int32_local, cpu_num, "[x86] ===== Load 32 Bit Integers (movl) =====");
    type_result["int32_t"] = res_int32_t;
    Results* res_int64_t = load_store_one_arg_kernel<__m256i, int64_t> (pcm, &kernel_asm_load_int64_local, cpu_num, "[x86_64] ===== Load 64 Bit Integers (movq) =====");
    type_result["int64_t"] = res_int64_t;
    #ifdef __SSE2__
        Results* res_float =   load_store_one_arg_kernel<__m256,  float>   (pcm, &kernel_asm_load_ss_local,  cpu_num, "[SSE2] ===== Load 32 bit Signed Floating Point Numbers (vmovss) =====");
        type_result["float"] = res_float;
        Results* res_double =  load_store_one_arg_kernel<__m256d, double>  (pcm, &kernel_asm_load_sd_local, cpu_num, "[SSE2] ===== Load 64 bit Signed Floating Point Numbers (vmovsd) =====");
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
        Results* res_int64_t = load_store_one_arg_kernel<__m256i, int64_t>  (pcm, &kernel_asm_mm256_store_si256_int64, cpu_num, "[AVX] ===== Store 4 Packed Signed 64 Bit Integers (_mm256_store_si256) =====");
        type_result["int64_t"] = res_int64_t;
        Results* res_float = load_store_one_arg_kernel<__m256, float>     (pcm, &kernel_asm_mm256_store_ps,          cpu_num, "[AVX] ===== Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_store_ps) =====");
        type_result["float"] = res_float;
        Results* res_double = load_store_one_arg_kernel<__m256d, double>   (pcm, &kernel_asm_mm256_store_pd,          cpu_num, "[AVX] ===== Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_store_pd) =====");
        type_result["double"] = res_double;
    #endif
}

void test_store_local(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Store Benchmarks (Local) ##########\n");
    #ifdef __AVX__
        Results* res_int8_t =  load_store_one_arg_kernel<__m256i, int8_t>   (pcm, &kernel_asm_mm256_store_si256_int8_local,  cpu_num, "[AVX] ===== Store 32 Packed Signed Bytes (_mm256_store_si256) =====");
        type_result["int8_t"] = res_int8_t;
        Results* res_int16_t = load_store_one_arg_kernel<__m256i, int16_t>  (pcm, &kernel_asm_mm256_store_si256_int16_local, cpu_num, "[AVX] ===== Store 16 Packed Signed 16 Bit Integers (_mm256_store_si256) =====");
        type_result["int16_t"] = res_int16_t;
        Results* res_int32_t = load_store_one_arg_kernel<__m256i, int32_t>  (pcm, &kernel_asm_mm256_store_si256_int32_local, cpu_num, "[AVX] ===== Store 8 Packed Signed 32 Bit Integers (_mm256_store_si256) =====");
        type_result["int32_t"] = res_int32_t;
        Results* res_int64_t = load_store_one_arg_kernel<__m256i, int64_t>  (pcm, &kernel_asm_mm256_store_si256_int64_local, cpu_num, "[AVX] ===== Store 4 Packed Signed 64 Bit Integers (_mm256_store_si256) =====");
        type_result["int64_t"] = res_int64_t;
        Results* res_float = load_store_one_arg_kernel<__m256, float>     (pcm, &kernel_asm_mm256_store_ps_local,          cpu_num, "[AVX] ===== Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_store_ps) =====");
        type_result["float"] = res_float;
        Results* res_double = load_store_one_arg_kernel<__m256d, double>   (pcm, &kernel_asm_mm256_store_pd_local,          cpu_num, "[AVX] ===== Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_store_pd) =====");
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
        Results* res_int64_t = load_store_one_arg_kernel<__m256i, int64_t>  (pcm, &kernel_asm_mm256_store_si256_int64_unroll2, cpu_num, "[AVX] ===== Store 4 Packed Signed 64 Bit Integers (_mm256_store_si256) =====");
        type_result["int64_t"] = res_int64_t;
        Results* res_float = load_store_one_arg_kernel<__m256, float>     (pcm, &kernel_asm_mm256_store_ps_unroll2,          cpu_num, "[AVX] ===== Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_store_ps) =====");
        type_result["float"] = res_float;
        Results* res_double = load_store_one_arg_kernel<__m256d, double>   (pcm, &kernel_asm_mm256_store_pd_unroll2,          cpu_num, "[AVX] ===== Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_store_pd) =====");
        type_result["double"] = res_double;
    #endif
}

void test_store_local_unroll2(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Store Benchmarks (Local, Unroll 2) ##########\n");
    #ifdef __AVX__
        Results* res_int8_t =  load_store_one_arg_kernel<__m256i, int8_t>   (pcm, &kernel_asm_mm256_store_si256_int8_local_unroll2,  cpu_num, "[AVX] ===== Store 32 Packed Signed Bytes (_mm256_store_si256) =====");
        type_result["int8_t"] = res_int8_t;
        Results* res_int16_t = load_store_one_arg_kernel<__m256i, int16_t>  (pcm, &kernel_asm_mm256_store_si256_int16_local_unroll2, cpu_num, "[AVX] ===== Store 16 Packed Signed 16 Bit Integers (_mm256_store_si256) =====");
        type_result["int16_t"] = res_int16_t;
        Results* res_int32_t = load_store_one_arg_kernel<__m256i, int32_t>  (pcm, &kernel_asm_mm256_store_si256_int32_local_unroll2, cpu_num, "[AVX] ===== Store 8 Packed Signed 32 Bit Integers (_mm256_store_si256) =====");
        type_result["int32_t"] = res_int32_t;
        Results* res_int64_t = load_store_one_arg_kernel<__m256i, int64_t>  (pcm, &kernel_asm_mm256_store_si256_int64_local_unroll2, cpu_num, "[AVX] ===== Store 4 Packed Signed 64 Bit Integers (_mm256_store_si256) =====");
        type_result["int64_t"] = res_int64_t;
        Results* res_float = load_store_one_arg_kernel<__m256, float>     (pcm, &kernel_asm_mm256_store_ps_local_unroll2,          cpu_num, "[AVX] ===== Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_store_ps) =====");
        type_result["float"] = res_float;
        Results* res_double = load_store_one_arg_kernel<__m256d, double>   (pcm, &kernel_asm_mm256_store_pd_local_unroll2,          cpu_num, "[AVX] ===== Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_store_pd) =====");
        type_result["double"] = res_double;
    #endif
}

void test_store_scalar(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Store Benchmarks (Scalar) ##########\n");
    Results* res_int8_t =  load_store_one_arg_kernel<__m256i, int8_t>  (pcm, &kernel_asm_store_int8,  cpu_num, "[x86] ===== Store Bytes (movb) =====");
    type_result["int8_t"] = res_int8_t;
    Results* res_int16_t = load_store_one_arg_kernel<__m256i, int16_t> (pcm, &kernel_asm_store_int16, cpu_num, "[x86] ===== Store 16 Bit Integers (movw) =====");
    type_result["int16_t"] = res_int16_t;
    Results* res_int32_t = load_store_one_arg_kernel<__m256i, int32_t> (pcm, &kernel_asm_store_int32, cpu_num, "[x86] ===== Store 32 Bit Integers (movl) =====");
    type_result["int32_t"] = res_int32_t;
    Results* res_int64_t = load_store_one_arg_kernel<__m256i, int64_t> (pcm, &kernel_asm_store_int64, cpu_num, "[x86_64] ===== Store 64 Bit Integers (movq) =====");
    type_result["int64_t"] = res_int64_t;
    #ifdef __AVX__
        Results* res_float =   load_store_one_arg_kernel<__m256,  float>   (pcm, &kernel_asm_store_ss,  cpu_num, "[AVX] ===== Store 32 bit Signed Floating Point Numbers (vmovss) =====");
        type_result["float"] = res_float;
        Results* res_double =  load_store_one_arg_kernel<__m256d, double>  (pcm, &kernel_asm_store_sd, cpu_num, "[AVX] ===== Store 64 bit Signed Floating Point Numbers (vmovsd) =====");
        type_result["double"] = res_double;
    #endif
}

void test_store_scalar_local(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Store Benchmarks (Scalar, Local) ##########\n");
    Results* res_int8_t =  load_store_one_arg_kernel<__m256i, int8_t>  (pcm, &kernel_asm_store_int8_local,  cpu_num, "[x86] ===== Store Bytes (movb) =====");
    type_result["int8_t"] = res_int8_t;
    Results* res_int16_t = load_store_one_arg_kernel<__m256i, int16_t> (pcm, &kernel_asm_store_int16_local, cpu_num, "[x86] ===== Store 16 Bit Integers (movw) =====");
    type_result["int16_t"] = res_int16_t;
    Results* res_int32_t = load_store_one_arg_kernel<__m256i, int32_t> (pcm, &kernel_asm_store_int32_local, cpu_num, "[x86] ===== Store 32 Bit Integers (movl) =====");
    type_result["int32_t"] = res_int32_t;
    Results* res_int64_t = load_store_one_arg_kernel<__m256i, int64_t> (pcm, &kernel_asm_store_int64_local, cpu_num, "[x86_64] ===== Store 64 Bit Integers (movq) =====");
    type_result["int64_t"] = res_int64_t;
    #ifdef __AVX__
        Results* res_float =   load_store_one_arg_kernel<__m256,  float>   (pcm, &kernel_asm_store_ss_local,  cpu_num, "[AVX] ===== Store 32 bit Signed Floating Point Numbers (vmovss) =====");
        type_result["float"] = res_float;
        Results* res_double =  load_store_one_arg_kernel<__m256d, double>  (pcm, &kernel_asm_store_sd_local, cpu_num, "[AVX] ===== Store 64 bit Signed Floating Point Numbers (vmovsd) =====");
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
        Results* res_int64_t = load_store_two_arg_kernel<__m256i, int64_t> (pcm, &kernel_mm256_load_si256__mm256_store_si256_int64, cpu_num, "[AVX] ===== Load/Store (Copy) 4 Packed Signed 64 Bit Integers (_mm256_load_si256 | _mm256_store_si256) =====");
        type_result["int64_t"] = res_int64_t;
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

    #ifdef __AVX__
    Results* res_single_scalar = zero_arg_kernel(pcm, &kernel_only_asm_add_sp, cpu_num, "[AVX] ===== Add Float Point (single via AVX) =====");
    type_result["float"] = res_single_scalar;
    Results* res_double_scalar = zero_arg_kernel(pcm, &kernel_only_asm_add_dp, cpu_num, "[AVX] ===== Add Floating Point (double via AVX) =====");
    type_result["double"] = res_double_scalar;
    #endif

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

    #ifdef __AVX__
    Results* res_single_scalar = zero_arg_kernel(pcm, &kernel_only_asm_add_sp_unroll2, cpu_num, "[AVX] ===== Add Float Point (single via AVX) =====");
    type_result["float"] = res_single_scalar;
    Results* res_double_scalar = zero_arg_kernel(pcm, &kernel_only_asm_add_dp_unroll2, cpu_num, "[AVX] ===== Add Floating Point (double via AVX) =====");
    type_result["double"] = res_double_scalar;
    #endif

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
        Results* res_int64_t = zero_arg_kernel(pcm, &kernel_only_asm_mm256_add_epi64, cpu_num, "[AVX2] ===== Add 4 Packed 64 bit Signed Integers (_mm256_add_epi64) =====");
        type_result["int64_t"] = res_int64_t;
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

    #ifdef __AVX__
    Results* res_single_scalar = zero_arg_kernel(pcm, &kernel_only_asm_mult_sp, cpu_num, "[AVX] ===== Mult Float Point (single via AVX) =====");
    type_result["float"] = res_single_scalar;
    Results* res_double_scalar = zero_arg_kernel(pcm, &kernel_only_asm_mult_dp, cpu_num, "[AVX] ===== Mult Floating Point (double via AVX) =====");
    type_result["double"] = res_double_scalar;
    #endif

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

    #ifdef __AVX__
    Results* res_single_scalar = zero_arg_kernel(pcm, &kernel_only_asm_mult_sp_unroll2, cpu_num, "[AVX] ===== Mult Float Point (single via AVX) =====");
    type_result["float"] = res_single_scalar;
    Results* res_double_scalar = zero_arg_kernel(pcm, &kernel_only_asm_mult_dp_unroll2, cpu_num, "[AVX] ===== Mult Floating Point (double via AVX) =====");
    type_result["double"] = res_double_scalar;
    #endif

    Results* res_x87_scalar = zero_arg_kernel(pcm, &kernel_only_asm_mult_fp_unroll2, cpu_num, "[x87] ===== Mult Floating Point (via x87) =====");
    type_result["x87 Floating Point"] = res_x87_scalar;
}

void test_only_mult_scalar_unroll2_regRename(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Scalar Mult Benchmarks (Unroll 2, Reg Rename) ##########\n");
    Results* res_int8_t_scalar =  zero_arg_kernel(pcm, &kernel_only_asm_mult_i8_unroll2_regRename,  cpu_num, "[x86] ===== Mult 8 bit Signed Integers =====");
    type_result["int8_t"] = res_int8_t_scalar;
}

void test_only_mult_scalar_unroll4(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Scalar Mult Benchmarks (Unroll 4) ##########\n");
    Results* res_int8_t_scalar =  zero_arg_kernel(pcm, &kernel_only_asm_mult_i8_unroll4,  cpu_num, "[x86] ===== Mult 8 bit Signed Integers =====");
    type_result["int8_t"] = res_int8_t_scalar;
    Results* res_int16_t_scalar = zero_arg_kernel(pcm, &kernel_only_asm_mult_i16_unroll4, cpu_num, "[x86] ===== Mult 16 bit Signed Integers =====");
    type_result["int16_t"] = res_int16_t_scalar;
    Results* res_int32_t_scalar = zero_arg_kernel(pcm, &kernel_only_asm_mult_i32_unroll4, cpu_num, "[x86] ===== Mult 32 bit Signed Integers =====");
    type_result["int32_t"] = res_int32_t_scalar;
    Results* res_int64_t_scalar = zero_arg_kernel(pcm, &kernel_only_asm_mult_i64_unroll4, cpu_num, "[x86_64] ===== Mult 64 bit Signed Integers =====");
    type_result["int64_t"] = res_int64_t_scalar;

    #ifdef __AVX__
    Results* res_single_scalar = zero_arg_kernel(pcm, &kernel_only_asm_mult_sp_unroll4, cpu_num, "[AVX] ===== Mult Float Point (single via AVX) =====");
    type_result["float"] = res_single_scalar;
    Results* res_double_scalar = zero_arg_kernel(pcm, &kernel_only_asm_mult_dp_unroll4, cpu_num, "[AVX] ===== Mult Floating Point (double via AVX) =====");
    type_result["double"] = res_double_scalar;
    #endif

    Results* res_x87_scalar = zero_arg_kernel(pcm, &kernel_only_asm_mult_fp_unroll4, cpu_num, "[x87] ===== Mult Floating Point (via x87) =====");
    type_result["x87 Floating Point"] = res_x87_scalar;
}

void test_only_div_scalar(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Scalar Div Benchmarks ##########\n");
    Results* res_int8_t_scalar =  zero_arg_kernel(pcm, &kernel_only_asm_div_i8,  cpu_num, "[x86] ===== Div 8 bit Signed Integers =====");
    type_result["int8_t"] = res_int8_t_scalar;
    Results* res_int16_t_scalar = zero_arg_kernel(pcm, &kernel_only_asm_div_i16, cpu_num, "[x86] ===== Div 16 bit Signed Integers =====");
    type_result["int16_t"] = res_int16_t_scalar;
    Results* res_int32_t_scalar = zero_arg_kernel(pcm, &kernel_only_asm_div_i32, cpu_num, "[x86] ===== Div 32 bit Signed Integers =====");
    type_result["int32_t"] = res_int32_t_scalar;
    Results* res_int64_t_scalar = zero_arg_kernel(pcm, &kernel_only_asm_div_i64, cpu_num, "[x86_64] ===== Div 64 bit Signed Integers =====");
    type_result["int64_t"] = res_int64_t_scalar;

    #ifdef __AVX__
    Results* res_single_scalar = zero_arg_kernel(pcm, &kernel_only_asm_div_sp, cpu_num, "[AVX] ===== Div Float Point (single via AVX) =====");
    type_result["float"] = res_single_scalar;
    Results* res_double_scalar = zero_arg_kernel(pcm, &kernel_only_asm_div_dp, cpu_num, "[AVX] ===== Div Floating Point (double via AVX) =====");
    type_result["double"] = res_double_scalar;
    #endif

    Results* res_x87_scalar = zero_arg_kernel(pcm, &kernel_only_asm_div_fp, cpu_num, "[x87] ===== Div Floating Point (via x87) =====");
    type_result["x87 Floating Point"] = res_x87_scalar;
}

void test_only_div_scalar_regRename(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Scalar Div Benchmarks (Reg Rename) ##########\n");
    Results* res_int8_t_scalar =  zero_arg_kernel(pcm, &kernel_only_asm_div_i8_regRename,  cpu_num, "[x86] ===== Mult 8 bit Signed Integers =====");
    type_result["int8_t"] = res_int8_t_scalar;
    Results* res_int16_t_scalar = zero_arg_kernel(pcm, &kernel_only_asm_div_i16_regRename, cpu_num, "[x86] ===== Div 16 bit Signed Integers =====");
    type_result["int16_t"] = res_int16_t_scalar;
    Results* res_int32_t_scalar = zero_arg_kernel(pcm, &kernel_only_asm_div_i32_regRename, cpu_num, "[x86] ===== Div 32 bit Signed Integers =====");
    type_result["int32_t"] = res_int32_t_scalar;
    Results* res_int64_t_scalar = zero_arg_kernel(pcm, &kernel_only_asm_div_i64_regRename, cpu_num, "[x86_64] ===== Div 64 bit Signed Integers =====");
    type_result["int64_t"] = res_int64_t_scalar;
}

void test_only_div_scalar_unroll2(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Scalar Div Benchmarks (Unroll 2) ##########\n");
    Results* res_int8_t_scalar =  zero_arg_kernel(pcm, &kernel_only_asm_div_i8_unroll2,  cpu_num, "[x86] ===== Div 8 bit Signed Integers =====");
    type_result["int8_t"] = res_int8_t_scalar;
    Results* res_int16_t_scalar = zero_arg_kernel(pcm, &kernel_only_asm_div_i16_unroll2, cpu_num, "[x86] ===== Div 16 bit Signed Integers =====");
    type_result["int16_t"] = res_int16_t_scalar;
    Results* res_int32_t_scalar = zero_arg_kernel(pcm, &kernel_only_asm_div_i32_unroll2, cpu_num, "[x86] ===== Div 32 bit Signed Integers =====");
    type_result["int32_t"] = res_int32_t_scalar;
    Results* res_int64_t_scalar = zero_arg_kernel(pcm, &kernel_only_asm_div_i64_unroll2, cpu_num, "[x86_64] ===== Div 64 bit Signed Integers =====");
    type_result["int64_t"] = res_int64_t_scalar;

    #ifdef __AVX__
    Results* res_single_scalar = zero_arg_kernel(pcm, &kernel_only_asm_div_sp_unroll2, cpu_num, "[AVX] ===== Div Float Point (single via AVX) =====");
    type_result["float"] = res_single_scalar;
    Results* res_double_scalar = zero_arg_kernel(pcm, &kernel_only_asm_div_dp_unroll2, cpu_num, "[AVX] ===== Div Floating Point (double via AVX) =====");
    type_result["double"] = res_double_scalar;
    #endif

    Results* res_x87_scalar = zero_arg_kernel(pcm, &kernel_only_asm_div_fp_unroll2, cpu_num, "[x87] ===== Div Floating Point (via x87) =====");
    type_result["x87 Floating Point"] = res_x87_scalar;
}

void test_only_div_scalar_unroll2_regRename(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Scalar Div Benchmarks (Unroll 2, Reg Rename) ##########\n");
    Results* res_int8_t_scalar =  zero_arg_kernel(pcm, &kernel_only_asm_div_i8_unroll2_regRename,  cpu_num, "[x86] ===== Mult 8 bit Signed Integers =====");
    type_result["int8_t"] = res_int8_t_scalar;
    Results* res_int16_t_scalar = zero_arg_kernel(pcm, &kernel_only_asm_div_i16_unroll2_regRename, cpu_num, "[x86] ===== Div 16 bit Signed Integers =====");
    type_result["int16_t"] = res_int16_t_scalar;
    Results* res_int32_t_scalar = zero_arg_kernel(pcm, &kernel_only_asm_div_i32_unroll2_regRename, cpu_num, "[x86] ===== Div 32 bit Signed Integers =====");
    type_result["int32_t"] = res_int32_t_scalar;
    Results* res_int64_t_scalar = zero_arg_kernel(pcm, &kernel_only_asm_div_i64_unroll2_regRename, cpu_num, "[x86_64] ===== Div 64 bit Signed Integers =====");
    type_result["int64_t"] = res_int64_t_scalar;
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

void test_only_mult_unroll2(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Multiply Benchmarks (Unroll 2) ##########\n");
    #ifdef __AVX2__
        Results* res_int16_t = zero_arg_kernel(pcm, &kernel_only_asm_mm256_mullo_epi16_unroll2, cpu_num, "[AVX2] ===== Multiply 16 Packed 16 bit Signed Integers -> Produce 32 Bit Intermediates (_mm256_mullo_epi16) =====");
        type_result["int16_t"] = res_int16_t;
        Results* res_int32_t = zero_arg_kernel(pcm, &kernel_only_asm_mm256_mullo_epi32_unroll2, cpu_num, "[AVX2] ===== Multiply 8 Packed 32 bit Signed Integers -> Produce 64 Bit Intermediates  (_mm256_mullo_epi32) =====");
        type_result["int32_t"] = res_int32_t;
    #endif

    #ifdef __AVX__
        Results* res_float   = zero_arg_kernel(pcm, &kernel_only_asm_mm256_mul_ps_unroll2, cpu_num, "[AVX] ===== Multiply 8 Packed 32 bit Signed Floating Point Numbers (_mm256_mul_ps) =====");
        type_result["float"] = res_float;
        Results* res_double  = zero_arg_kernel(pcm, &kernel_only_asm_mm256_mul_pd_unroll2, cpu_num, "[AVX] ===== Multiply 4 Packed 64 bit Signed Floating Point Numbers (_mm256_mul_pd) =====");
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
        Results* res_int64_t = load_store_three_arg_kernel<__m256i, int64_t> (pcm, &kernel_mm256_add_epi32, cpu_num, "[AVX2] ===== Load/Add/Store 4 Packed 64 bit Signed Integers (_mm256_add_epi64) =====");
        type_result["int64_t"] = res_int64_t;
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
    Results* res_int64_t = no_vec_three_arg_kernel<int64_t> (pcm, &kernel_nointrin_mm256_add_epi64, cpu_num, "[No Intrin] ===== Load/Add/Store 4 Packed 64 bit Signed Integers (_mm256_add_epi64) =====");
    type_result["int64_t"] = res_int64_t;

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
        Results* res_int64_t = load_store_three_arg_kernel<__m256i, int64_t> (pcm, &kernel_u2_mm256_add_epi64, cpu_num, "[AVX2] ===== (Unroll 2) Load/Add/Store 4 Packed 64 bit Signed Integers (_mm256_add_epi64) =====");
        type_result["int64_t"] = res_int64_t;
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

//#### Benchmark API Function Overrides ####

std::string getBenchSuiteName(){
    return "x86_64/x87/SSE/AVX/AVX2/FMA Micro-Benchmarks";
}

std::string getReportFileName(){
    return "micro_bench_report";
}

std::string getReportUnitsName(){
    return "STIM_LEN: " + std::to_string(STIM_LEN) + " (Samples/Vector/Trial), TRIALS: " + std::to_string(TRIALS);
}

//The init input function is not used for the micro-benchmark test
void initInput(void* ptr, unsigned long index){}

void getBenchmarksToReport(std::vector<std::string> &kernels, std::vector<std::string> &vec_ext){
    kernels.push_back("Function Call");                         vec_ext.push_back("N/A");
    kernels.push_back("Load (Scalar)");                         vec_ext.push_back("x86 / x86_64 / SSE2");
    kernels.push_back("Load (Scalar, Local)");                  vec_ext.push_back("x86 / x86_64 / SSE2");
    kernels.push_back("Load");                                  vec_ext.push_back("AVX");
    kernels.push_back("Load (Unroll 2)");                       vec_ext.push_back("AVX");
    kernels.push_back("Load (Local)");                          vec_ext.push_back("AVX");
    kernels.push_back("Load (Local, Unroll 2)");                vec_ext.push_back("AVX");
    kernels.push_back("Store (Scalar)");                        vec_ext.push_back("x86 / x86_64 / SSE2");
    kernels.push_back("Store (Scalar, Local)");                 vec_ext.push_back("x86 / x86_64 / SSE2");
    kernels.push_back("Store");                                 vec_ext.push_back("AVX");
    kernels.push_back("Store (Unroll 2)");                      vec_ext.push_back("AVX");
    kernels.push_back("Store (Local)");                         vec_ext.push_back("AVX");
    kernels.push_back("Store (Local, Unroll 2)");               vec_ext.push_back("AVX");
    kernels.push_back("Add (Scalar)");                          vec_ext.push_back("x86 / x86_64 / x87 / AVX");
    kernels.push_back("Add (Scalar, Unroll 2)");                vec_ext.push_back("x86 / x86_64 / x87 / AVX");
    kernels.push_back("Add");                                   vec_ext.push_back("AVX (Float) / AVX2 (Int)");
    kernels.push_back("Mult (Scalar)");                         vec_ext.push_back("x86 / x86_64 / x87 / AVX");
    kernels.push_back("Mult (Scalar, Unroll 2)");               vec_ext.push_back("x86 / x86_64 / x87 / AVX");
    kernels.push_back("Mult (Scalar, Unroll 4)");               vec_ext.push_back("x86 / x86_64 / x87 / AVX");
    kernels.push_back("Mult (Scalar, Reg Rename)");             vec_ext.push_back("x86");
    kernels.push_back("Mult (Scalar, Reg Rename, Unroll 2)");   vec_ext.push_back("x86");
    kernels.push_back("Mult");                                  vec_ext.push_back("AVX (Float) / AVX2 (Int)");
    kernels.push_back("Mult (Unroll 2)");                                  vec_ext.push_back("AVX (Float) / AVX2 (Int)");
    kernels.push_back("Div (Scalar)");                          vec_ext.push_back("x86 / x86_64 / x87 / AVX");
    kernels.push_back("Div (Scalar, Unroll 2)");                vec_ext.push_back("x86 / x86_64 / x87 / AVX");
    kernels.push_back("Div (Scalar, Reg Rename)");              vec_ext.push_back("x86");
    kernels.push_back("Div (Scalar, Reg Rename, Unroll 2)");    vec_ext.push_back("x86");
    kernels.push_back("Div");                                   vec_ext.push_back("AVX");
    kernels.push_back("FMA");                                   vec_ext.push_back("FMA");
    kernels.push_back("Load/Store");                            vec_ext.push_back("AVX");
    kernels.push_back("Load/Add/Store");                        vec_ext.push_back("AVX (Float) / AVX2 (Int)");
    kernels.push_back("Load/Mult/Store");                       vec_ext.push_back("AVX (Float) / AVX2 (Int)");
    kernels.push_back("Load/Div/Store");                        vec_ext.push_back("AVX");
    kernels.push_back("Load/FMA/Store");                        vec_ext.push_back("FMA");
    kernels.push_back("Load/Add/Store No Intrin");              vec_ext.push_back("N/A");
    kernels.push_back("Load/Add/Store Unroll2");                vec_ext.push_back("AVX (Float) / AVX2 (Int)");
}

std::vector<std::string> getVarientsToReport(){
    std::vector<std::string> types;
    types.push_back("int8_t");
    types.push_back("int16_t");
    types.push_back("int32_t");
    types.push_back("int64_t");
    types.push_back("float");
    types.push_back("double");
    types.push_back("x87 Floating Point");
    types.push_back("Zero Arg");

    return types;
}

std::map<std::string, std::map<std::string, Results*>*> runBenchSuite(PCM* pcm, int* cpu_num_int){
    std::map<std::string, std::map<std::string, Results*>*> kernel_results;

    std::map<std::string, Results*>* fctn_call_results = new std::map<std::string, Results*>;
    test_fctn_call(pcm, *cpu_num_int, *fctn_call_results);
    kernel_results["Function Call"] = fctn_call_results;
    printf("\n");

    std::map<std::string, Results*>* load_results_scalar = new std::map<std::string, Results*>;
    test_load_scalar(pcm, *cpu_num_int, *load_results_scalar);
    kernel_results["Load (Scalar)"] = load_results_scalar;
    printf("\n");

    std::map<std::string, Results*>* load_results_scalar_local = new std::map<std::string, Results*>;
    test_load_scalar_local(pcm, *cpu_num_int, *load_results_scalar_local);
    kernel_results["Load (Scalar, Local)"] = load_results_scalar_local;
    printf("\n");

    std::map<std::string, Results*>* load_results = new std::map<std::string, Results*>;
    test_load(pcm, *cpu_num_int, *load_results);
    kernel_results["Load"] = load_results;
    printf("\n");

    std::map<std::string, Results*>* load_results_unroll2 = new std::map<std::string, Results*>;
    test_load_unroll2(pcm, *cpu_num_int, *load_results_unroll2);
    kernel_results["Load (Unroll 2)"] = load_results_unroll2;
    printf("\n");

    std::map<std::string, Results*>* load_results_local = new std::map<std::string, Results*>;
    test_load_local(pcm, *cpu_num_int, *load_results_local);
    kernel_results["Load (Local)"] = load_results_local;
    printf("\n");

    std::map<std::string, Results*>* load_results_local_unroll2 = new std::map<std::string, Results*>;
    test_load_local_unroll2(pcm, *cpu_num_int, *load_results_local_unroll2);
    kernel_results["Load (Local, Unroll 2)"] = load_results_local_unroll2;
    printf("\n");

    std::map<std::string, Results*>* store_results_scalar = new std::map<std::string, Results*>;
    test_store_scalar(pcm, *cpu_num_int, *store_results_scalar);
    kernel_results["Store (Scalar)"] = store_results_scalar;
    printf("\n");

    std::map<std::string, Results*>* store_results_scalar_local = new std::map<std::string, Results*>;
    test_store_scalar_local(pcm, *cpu_num_int, *store_results_scalar_local);
    kernel_results["Store (Scalar, Local)"] = store_results_scalar_local;
    printf("\n");

    std::map<std::string, Results*>* store_results = new std::map<std::string, Results*>;
    test_store(pcm, *cpu_num_int, *store_results);
    kernel_results["Store"] = store_results;
    printf("\n");

    std::map<std::string, Results*>* store_results_unroll2 = new std::map<std::string, Results*>;
    test_store_unroll2(pcm, *cpu_num_int, *store_results_unroll2);
    kernel_results["Store (Unroll 2)"] = store_results_unroll2;
    printf("\n");

    std::map<std::string, Results*>* store_results_local = new std::map<std::string, Results*>;
    test_store_local(pcm, *cpu_num_int, *store_results_local);
    kernel_results["Store (Local)"] = store_results_local;
    printf("\n");

    std::map<std::string, Results*>* store_results_local_unroll2 = new std::map<std::string, Results*>;
    test_store_local_unroll2(pcm, *cpu_num_int, *store_results_local_unroll2);
    kernel_results["Store (Local, Unroll 2)"] = store_results_local_unroll2;
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
    std::map<std::string, Results*>* only_mult_results_scalar_unroll4 = new std::map<std::string, Results*>;
    test_only_mult_scalar_unroll4(pcm, *cpu_num_int, *only_mult_results_scalar_unroll4);
    kernel_results["Mult (Scalar, Unroll 4)"] = only_mult_results_scalar_unroll4;
    printf("\n");

    std::map<std::string, Results*>* only_mult_results = new std::map<std::string, Results*>;
    test_only_mult(pcm, *cpu_num_int, *only_mult_results);
    kernel_results["Mult"] = only_mult_results;
    printf("\n");

    std::map<std::string, Results*>* only_mult_results_unroll2 = new std::map<std::string, Results*>;
    test_only_mult_unroll2(pcm, *cpu_num_int, *only_mult_results_unroll2);
    kernel_results["Mult (Unroll 2)"] = only_mult_results_unroll2;
    printf("\n");

    std::map<std::string, Results*>* only_div_results_scalar = new std::map<std::string, Results*>;
    test_only_div_scalar(pcm, *cpu_num_int, *only_div_results_scalar);
    kernel_results["Div (Scalar)"] = only_div_results_scalar;
    printf("\n");
    std::map<std::string, Results*>* only_div_results_scalar_unroll2 = new std::map<std::string, Results*>;
    test_only_div_scalar_unroll2(pcm, *cpu_num_int, *only_div_results_scalar_unroll2);
    kernel_results["Div (Scalar, Unroll 2)"] = only_div_results_scalar_unroll2;
    printf("\n");
    std::map<std::string, Results*>* only_div_results_scalar_regRename = new std::map<std::string, Results*>;
    test_only_div_scalar_regRename(pcm, *cpu_num_int, *only_div_results_scalar_regRename);
    kernel_results["Div (Scalar, Reg Rename)"] = only_div_results_scalar_regRename;
    printf("\n");
    std::map<std::string, Results*>* only_div_results_scalar_unroll2_regRename = new std::map<std::string, Results*>;
    test_only_div_scalar_unroll2_regRename(pcm, *cpu_num_int, *only_div_results_scalar_unroll2_regRename);
    kernel_results["Div (Scalar, Reg Rename, Unroll 2)"] = only_div_results_scalar_unroll2_regRename;
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

    return kernel_results;
}
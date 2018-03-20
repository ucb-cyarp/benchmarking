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
#include <map>

#include "intrin_bench_default_defines.h"

#include "depends/pcm/cpucounters.h"

#include "kernel_runner.h"
#include "results.h"

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

void test_load(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Load Benchmarks ##########\n");
    #ifdef __AVX__
        Results* res_int8_t =  load_store_one_arg_kernel<__m256i, int8_t>  (pcm, &kernel_mm256_load_si256_int8,  cpu_num, "[AVX] ===== Load 32 Signed Bytes [Store in Local Var] (_mm256_load_si256) =====");
        type_result["int8_t"] = res_int8_t;
        Results* res_int16_t = load_store_one_arg_kernel<__m256i, int16_t> (pcm, &kernel_mm256_load_si256_int16, cpu_num, "[AVX] ===== Load 16 Signed 16 Bit Integers [Store in Local Var] (_mm256_load_si256) =====");
        type_result["int16_t"] = res_int16_t;
        Results* res_int32_t = load_store_one_arg_kernel<__m256i, int32_t> (pcm, &kernel_mm256_load_si256_int32, cpu_num, "[AVX] ===== Load 8 Signed 32 Bit Integers [Store in Local Var] (_mm256_load_si256) =====");
        type_result["int32_t"] = res_int32_t;
        Results* res_float =   load_store_one_arg_kernel<__m256,  float>   (pcm, &kernel_mm256_load_ps,          cpu_num, "[AVX] ===== Load 8 Packed 32 bit Signed Floating Point Numbers [Store in Local Var] (_mm256_load_ps) =====");
        type_result["float"] = res_float;
        Results* res_double =  load_store_one_arg_kernel<__m256d, double>  (pcm, &kernel_mm256_load_pd,          cpu_num, "[AVX] ===== Load 4 Packed 64 bit Signed Floating Point Numbers [Store in Local Var] (_mm256_load_pd) =====");
        type_result["double"] = res_double;
    #endif
}

void test_store(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Store Benchmarks ##########\n");
    #ifdef __AVX__
        Results* res_int8_t =  load_store_one_arg_kernel<__m256i, int8_t>   (pcm, &kernel_mm256_store_si256_int8,  cpu_num, "[AVX] ===== Store 32 Packed Signed Bytes (_mm256_store_si256) =====");
        type_result["int8_t"] = res_int8_t;
        Results* res_int16_t = load_store_one_arg_kernel<__m256i, int16_t>  (pcm, &kernel_mm256_store_si256_int16, cpu_num, "[AVX] ===== Store 16 Packed Signed 16 Bit Integers (_mm256_store_si256) =====");
        type_result["int16_t"] = res_int16_t;
        Results* res_int32_t = load_store_one_arg_kernel<__m256i, int32_t>  (pcm, &kernel_mm256_store_si256_int32, cpu_num, "[AVX] ===== Store 8 Packed Signed 32 Bit Integers (_mm256_store_si256) =====");
        type_result["int32_t"] = res_int32_t;
        Results* res_float = load_store_one_arg_kernel<__m256, float>     (pcm, &kernel_mm256_store_ps,          cpu_num, "[AVX] ===== Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_store_ps) =====");
        type_result["float"] = res_float;
        Results* res_double = load_store_one_arg_kernel<__m256d, double>   (pcm, &kernel_mm256_store_pd,          cpu_num, "[AVX] ===== Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_store_pd) =====");
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

void test_only_add(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Add Benchmarks ##########\n");
    #ifdef __AVX2__
        Results* res_int8_t =  zero_arg_kernel(pcm, &kernel_only_mm256_add_epi8,  cpu_num, "[AVX2] ===== Add 32 Packed 8 bit Signed Integers [Store in Local Var] (_mm256_add_epi8) =====");
        type_result["int8_t"] = res_int8_t;
        Results* res_int16_t = zero_arg_kernel(pcm, &kernel_only_mm256_add_epi16, cpu_num, "[AVX2] ===== Add 16 Packed 16 bit Signed Integers [Store in Local Var] (_mm256_add_epi16) =====");
        type_result["int16_t"] = res_int16_t;
        Results* res_int32_t = zero_arg_kernel(pcm, &kernel_only_mm256_add_epi32, cpu_num, "[AVX2] ===== Add 8 Packed 32 bit Signed Integers [Store in Local Var] (_mm256_add_epi32) =====");
        type_result["int32_t"] = res_int32_t;
    #endif

    #ifdef __AVX__
        Results* res_float =   zero_arg_kernel(pcm, &kernel_only_mm256_add_ps,    cpu_num, "[AVX] ===== Add 8 Packed 32 bit Signed Floating Point Numbers [Store in Local Var] (_mm256_add_ps) =====");
        type_result["float"] = res_float;
        Results* res_double =  zero_arg_kernel(pcm, &kernel_only_mm256_add_pd,    cpu_num, "[AVX] ===== Add 4 Packed 64 bit Signed Floating Point Numbers [Store in Local Var] (_mm256_add_pd) =====");
        type_result["double"] = res_double;
    #endif
}

void test_only_mult(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Multiply Benchmarks ##########\n");
    #ifdef __AVX2__
        Results* res_int16_t = zero_arg_kernel(pcm, &kernel_only_mm256_mullo_epi16, cpu_num, "[AVX2] ===== Multiply 16 Packed 16 bit Signed Integers -> Produce 32 Bit Intermediates -> Store Lower 16 Bits of Intermediates [In Local Var] (_mm256_mullo_epi16) =====");
        type_result["int16_t"] = res_int16_t;
        Results* res_int32_t = zero_arg_kernel(pcm, &kernel_only_mm256_mullo_epi32, cpu_num, "[AVX2] ===== Multiply 8 Packed 32 bit Signed Integers -> Produce 64 Bit Intermediates -> Store Lower 32 Bits of Intermediates [In Local Var] (_mm256_mullo_epi32) =====");
        type_result["int32_t"] = res_int32_t;
    #endif

    #ifdef __AVX__
        Results* res_float   = zero_arg_kernel(pcm, &kernel_only_mm256_mul_ps, cpu_num, "[AVX] ===== Multiply 8 Packed 32 bit Signed Floating Point Numbers [Store In Local Var] (_mm256_mul_ps) =====");
        type_result["float"] = res_float;
        Results* res_double  = zero_arg_kernel(pcm, &kernel_only_mm256_mul_pd, cpu_num, "[AVX] ===== Multiply 4 Packed 64 bit Signed Floating Point Numbers [Store In Local Var] (_mm256_mul_pd) =====");
        type_result["double"] = res_double;
    #endif
}

void test_only_div(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## Divide Benchmarks ##########\n");
    #ifdef __AVX__
        Results* res_float = zero_arg_kernel(pcm, &kernel_only_mm256_div_ps, cpu_num, "[AVX] ===== Divide 8 Packed 32 bit Signed Floating Point Numbers [Store In Local Var] (_mm256_div_ps) =====");
        type_result["float"] = res_float;
        Results* res_double = zero_arg_kernel(pcm, &kernel_only_mm256_div_pd, cpu_num, "[AVX] ===== Divide 4 Packed 64 bit Signed Floating Point Numbers [Store In Local Var] (_mm256_div_pd) =====");
        type_result["double"] = res_double;
    #endif
}

void test_only_fma(PCM* pcm, int cpu_num, std::map<std::string, Results*>& type_result)
{
    printf("########## FMA Benchmarks ##########\n");
    #ifdef __FMA__
        Results* res_float   = zero_arg_kernel(pcm, &kernel_only_mm256_fmadd_ps, cpu_num, "[FMA] ===== FMA 8 Packed 32 bit Signed Floating Point Numbers [Store in Local Var] (_mm256_fmadd_ps) =====");
        type_result["float"] = res_float;
        Results* res_double  = zero_arg_kernel(pcm, &kernel_only_mm256_fmadd_pd, cpu_num, "[FMA] ===== FMA 4 Packed 64 bit Signed Floating Point Numbers [Store in Local Var] (_mm256_fmadd_pd) =====");
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

//Initialize PCM
//Based off opcm/pcm example pcm-power.cpp
PCM* init_PCM()
{
    set_signal_handlers();

    PCM * m = PCM::getInstance();
    //m->allowMultipleInstances();
    m->disableJKTWorkaround();

    //Configure PCM
    const int cpu_model = m->getCPUModel();
    if (!(m->hasPCICFGUncore()))
    {
        std::cerr << "Unsupported processor model (" << cpu_model << ")." << std::endl;
        exit(1);
    }

    printf("**************************************************\n");
    printf("Resetting PMU\n");
    m->resetPMU();

    int default_freq_band[3] = { 12, 20, 40 };
    
    int imc_profile = -1; //Do not gather DRAM statistics (for now)
    int pcu_profile = 5; //Get frequency Change statistics
    int* freq_band = default_freq_band;

    //Configure PCM PCU Monitoring
    if (PCM::Success != m->programServerUncorePowerMetrics(imc_profile, pcu_profile, freq_band))
    {
        #ifdef _MSC_VER
        std::cerr << "You must have signed msr.sys driver in your current directory and have administrator rights to run this program" << std::endl;
        #elif defined(__linux__)
        std::cerr << "You need to be root and loaded 'msr' Linux kernel module to execute the program. You may load the 'msr' module with 'modprobe msr'. \n";
        #endif
        exit(EXIT_FAILURE);
    }

    if (m->program() != PCM::Success) exit(EXIT_FAILURE);

    m->setBlocked(true);

    return m;
}

void* run_benchmarks(void* cpu_num)
{
    #if PRINT_TITLE == 1
    printf("SSE/AVX/FMA Intrinsic Tester\n");
    printf("STIM_LEN: %d (Samples/Vector/Trial), TRIALS: %d\n", STIM_LEN, TRIALS);
    #endif

    printf("\n");
    printf("****** Platform Information Provided by PCM ******\n");
    PCM* pcm = init_PCM();

    printf("**************************************************\n");
    printf("CPU Brand String: %s\n", pcm->getCPUBrandString().c_str());
    printf("**************************************************\n");

    int* cpu_num_int = (int*) cpu_num;
    int socket = pcm->getSocketId(*cpu_num_int);
    printf("Executing on Core: %3d (Socket: %2d)\n", *cpu_num_int, socket);
    printf("**************************************************\n");
    printf("\n");

    std::map<std::string, std::map<std::string, Results*>*> kernel_results;

    std::map<std::string, Results*>* load_results = new std::map<std::string, Results*>;
    test_load(pcm, *cpu_num_int, *load_results);
    kernel_results["Load"] = load_results;
    printf("\n");
    std::map<std::string, Results*>* store_results = new std::map<std::string, Results*>;
    test_store(pcm, *cpu_num_int, *store_results);
    kernel_results["Store"] = store_results;
    printf("\n");
    std::map<std::string, Results*>* load_store_results = new std::map<std::string, Results*>;
    test_load_store(pcm, *cpu_num_int, *load_store_results);
    kernel_results["Load/Store"] = load_store_results;
    printf("\n");

    std::map<std::string, Results*>* only_add_results = new std::map<std::string, Results*>;
    test_only_add(pcm, *cpu_num_int, *only_add_results);
    kernel_results["Add"] = only_add_results;
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
    types.push_back("float");
    types.push_back("double");

    std::vector<std::string> kernels;
    kernels.push_back("Load");
    kernels.push_back("Store");
    kernels.push_back("Add");
    kernels.push_back("Mult");
    kernels.push_back("Div");
    kernels.push_back("FMA");

    std::vector<std::string> vec_ext;
    vec_ext.push_back("AVX");
    vec_ext.push_back("AVX");
    vec_ext.push_back("AVX (Float) / AVX2 (Int)");
    vec_ext.push_back("AVX (Float) / AVX2 (Int)");
    vec_ext.push_back("AVX (Float) / AVX2 (Int)");
    vec_ext.push_back("FMA");

    //TODO: Print header

    //TODO: Print table



    //Cleanup
    for (std::map<std::str, std::map<std::string, Results*>*>::iterator it = kernel_results.begin(); it != kernel_results.end(); it++)
    {
        std::map<std::string, Results*>* sub_result = it->second;

        for (std::map<std::string, Results*>::iterator sub_it = sub_result->begin(); sub_it != sub_result->end(); sub_it++)
        {
            sub_it->second->delete_results();
            delete sub_it->second;
        }

        delete it->second;
    }

    printf("****** PCM Ceanup ******\n");
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

    cpu_set_t cpuset;
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

    CPU_ZERO(&cpuset);
    CPU_SET(cpu_number, &cpuset);

    status = pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);

    if(status != 0)
    {
        printf("Could not set thread core affinity ... exiting\n");
        exit(1);
    }

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
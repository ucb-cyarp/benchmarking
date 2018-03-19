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

#include "intrin_bench_default_defines.h"

#include "depends/pcm/cpucounters.h"

#include "kernel_runner.h"

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

void test_load(PCM* pcm)
{
    printf("########## Load Benchmarks ##########\n");
    #ifdef __AVX__
        load_store_one_arg_kernel<__m256i, int8_t>  (pcm, &kernel_mm256_load_si256_int8,  "[AVX] ===== Load 32 Signed Bytes [Store in Local Var] (_mm256_load_si256) =====");
        load_store_one_arg_kernel<__m256i, int16_t> (pcm, &kernel_mm256_load_si256_int16, "[AVX] ===== Load 16 Signed 16 Bit Integers [Store in Local Var] (_mm256_load_si256) =====");
        load_store_one_arg_kernel<__m256i, int32_t> (pcm, &kernel_mm256_load_si256_int32, "[AVX] ===== Load 8 Signed 32 Bit Integers [Store in Local Var] (_mm256_load_si256) =====");
        load_store_one_arg_kernel<__m256,  float>   (pcm, &kernel_mm256_load_ps,          "[AVX] ===== Load 8 Packed 32 bit Signed Floating Point Numbers [Store in Local Var] (_mm256_load_ps) =====");
        load_store_one_arg_kernel<__m256d, double>  (pcm, &kernel_mm256_load_pd,          "[AVX] ===== Load 4 Packed 64 bit Signed Floating Point Numbers [Store in Local Var] (_mm256_load_pd) =====");
    #endif
}

void test_store(PCM* pcm)
{
    printf("########## Store Benchmarks ##########\n");
    #ifdef __AVX__
        load_store_one_arg_kernel<__m256i, int8_t>   (pcm, &kernel_mm256_store_si256_int8,  "[AVX] ===== Store 32 Packed Signed Bytes (_mm256_store_si256) =====");
        load_store_one_arg_kernel<__m256i, int16_t>  (pcm, &kernel_mm256_store_si256_int16, "[AVX] ===== Store 16 Packed Signed 16 Bit Integers (_mm256_store_si256) =====");
        load_store_one_arg_kernel<__m256i, int32_t>  (pcm, &kernel_mm256_store_si256_int32, "[AVX] ===== Store 8 Packed Signed 32 Bit Integers (_mm256_store_si256) =====");
        load_store_one_arg_kernel<__m256, float>     (pcm, &kernel_mm256_store_ps,          "[AVX] ===== Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_store_ps) =====");
        load_store_one_arg_kernel<__m256d, double>   (pcm, &kernel_mm256_store_pd,          "[AVX] ===== Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_store_pd) =====");
    #endif
}

void test_load_store(PCM* pcm)
{
    printf("########## Load/Store Benchmarks ##########\n");
    #ifdef __AVX__
        load_store_two_arg_kernel<__m256i, int8_t>  (pcm, &kernel_mm256_load_si256__mm256_store_si256_int8,  "[AVX] ===== Load/Store (Copy) 32 Packed Signed Bytes (_mm256_load_si256 | _mm256_store_si256) =====");
        load_store_two_arg_kernel<__m256i, int16_t> (pcm, &kernel_mm256_load_si256__mm256_store_si256_int16, "[AVX] ===== Load/Store (Copy) 16 Packed Signed 16 Bit Integers (_mm256_load_si256 | _mm256_store_si256) =====");
        load_store_two_arg_kernel<__m256i, int32_t> (pcm, &kernel_mm256_load_si256__mm256_store_si256_int32, "[AVX] ===== Load/Store (Copy) 8 Packed Signed 32 Bit Integers (_mm256_load_si256 | _mm256_store_si256) =====");
        load_store_two_arg_kernel<__m256,  float>   (pcm, &kernel_mm256_load_ps__mm256_store_ps,             "[AVX] ===== Load/Store (Copy) 8 Packed 32 bit Signed Floating Point Numbers (_mm256_load_ps | _mm256_store_ps) =====");
        load_store_two_arg_kernel<__m256d, double>  (pcm, &kernel_mm256_load_pd__mm256_store_pd,             "[AVX] ===== Load/Store (Copy) 4 Packed 64 bit Signed Floating Point Numbers (_mm256_load_pd | _mm256_store_pd) =====");
    #endif
}

void test_only_add(PCM* pcm)
{
    printf("########## Add Benchmarks ##########\n");
    #ifdef __AVX2__
        zero_arg_kernel(pcm, &kernel_only_mm256_add_epi8,  "[AVX2] ===== Add 32 Packed 8 bit Signed Integers [Store in Local Var] (_mm256_add_epi8) =====");
        zero_arg_kernel(pcm, &kernel_only_mm256_add_epi16, "[AVX2] ===== Add 16 Packed 16 bit Signed Integers [Store in Local Var] (_mm256_add_epi16) =====");
        zero_arg_kernel(pcm, &kernel_only_mm256_add_epi32, "[AVX2] ===== Add 8 Packed 32 bit Signed Integers [Store in Local Var] (_mm256_add_epi32) =====");
    #endif

    #ifdef __AVX__
        zero_arg_kernel(pcm, &kernel_only_mm256_add_ps,    "[AVX] ===== Add 8 Packed 32 bit Signed Floating Point Numbers [Store in Local Var] (_mm256_add_ps) =====");
        zero_arg_kernel(pcm, &kernel_only_mm256_add_pd,    "[AVX] ===== Add 4 Packed 64 bit Signed Floating Point Numbers [Store in Local Var] (_mm256_add_pd) =====");
    #endif
}

void test_only_mult(PCM* pcm)
{
    printf("########## Multiply Benchmarks ##########\n");
    #ifdef __AVX2__
        zero_arg_kernel(pcm, &kernel_only_mm256_mullo_epi16, "[AVX2] ===== Multiply 16 Packed 16 bit Signed Integers -> Produce 32 Bit Intermediates -> Store Lower 16 Bits of Intermediates [In Local Var] (_mm256_mullo_epi16) =====");
        zero_arg_kernel(pcm, &kernel_only_mm256_mullo_epi32, "[AVX2] ===== Multiply 8 Packed 32 bit Signed Integers -> Produce 64 Bit Intermediates -> Store Lower 32 Bits of Intermediates [In Local Var] (_mm256_mullo_epi32) =====");
    #endif

    #ifdef __AVX__
        zero_arg_kernel(pcm, &kernel_only_mm256_mul_ps, "[AVX] ===== Multiply 8 Packed 32 bit Signed Floating Point Numbers [Store In Local Var] (_mm256_mul_ps) =====");
        zero_arg_kernel(pcm, &kernel_only_mm256_mul_pd, "[AVX] ===== Multiply 4 Packed 64 bit Signed Floating Point Numbers [Store In Local Var] (_mm256_mul_pd) =====");
    #endif
}

void test_only_div(PCM* pcm)
{
    printf("########## Divide Benchmarks ##########\n");
    #ifdef __AVX__
        zero_arg_kernel(pcm, &kernel_only_mm256_div_ps, "[AVX] ===== Divide 8 Packed 32 bit Signed Floating Point Numbers [Store In Local Var] (_mm256_div_ps) =====");
        zero_arg_kernel(pcm, &kernel_only_mm256_div_pd, "[AVX] ===== Divide 4 Packed 64 bit Signed Floating Point Numbers [Store In Local Var] (_mm256_div_pd) =====");
    #endif
}

void test_only_fma(PCM* pcm)
{
    printf("########## FMA Benchmarks ##########\n");
    #ifdef __FMA__
        zero_arg_kernel(pcm, &kernel_only_mm256_fmadd_ps, "[FMA] ===== FMA 8 Packed 32 bit Signed Floating Point Numbers [Store in Local Var] (_mm256_fmadd_ps) =====");
        zero_arg_kernel(pcm, &kernel_only_mm256_fmadd_pd, "[FMA] ===== FMA 4 Packed 64 bit Signed Floating Point Numbers [Store in Local Var] (_mm256_fmadd_pd) =====");
    #endif
}

void test_add(PCM* pcm)
{
    printf("########## Load/Add/Store Benchmarks ##########\n");
    #ifdef __AVX2__
        load_store_three_arg_kernel<__m256i, int8_t>  (pcm, &kernel_mm256_add_epi8,  "[AVX2] ===== Load/Add/Store 32 Packed 8 bit Signed Integers (_mm256_add_epi8) =====");
        load_store_three_arg_kernel<__m256i, int16_t> (pcm, &kernel_mm256_add_epi16, "[AVX2] ===== Load/Add/Store 16 Packed 16 bit Signed Integers (_mm256_add_epi16) =====");
        load_store_three_arg_kernel<__m256i, int32_t> (pcm, &kernel_mm256_add_epi32, "[AVX2] ===== Load/Add/Store 8 Packed 32 bit Signed Integers (_mm256_add_epi32) =====");
    #endif

    #ifdef __AVX__
        load_store_three_arg_kernel<__m256,  float>   (pcm, &kernel_mm256_add_ps,    "[AVX] ===== Load/Add/Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_add_ps) =====");
        load_store_three_arg_kernel<__m256d, double>  (pcm, &kernel_mm256_add_pd,    "[AVX] ===== Load/Add/Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_add_pd) =====");
    #endif
}

void test_nointrin_add(PCM* pcm)
{
    printf("########## Load/Add/Store (No Intrinsics) Benchmarks ##########\n");
    no_vec_three_arg_kernel<int8_t>  (pcm, &kernel_nointrin_mm256_add_epi8,  "[No Intrin] ===== Load/Add/Store 32 Packed 8 bit Signed Integers (_mm256_add_epi8) =====");
    no_vec_three_arg_kernel<int16_t> (pcm, &kernel_nointrin_mm256_add_epi16, "[No Intrin] ===== Load/Add/Store 16 Packed 16 bit Signed Integers (_mm256_add_epi16) =====");
    no_vec_three_arg_kernel<int32_t> (pcm, &kernel_nointrin_mm256_add_epi32, "[No Intrin] ===== Load/Add/Store 8 Packed 32 bit Signed Integers (_mm256_add_epi32) =====");

    no_vec_three_arg_kernel<float>   (pcm, &kernel_nointrin_mm256_add_ps,    "[No Intrin] ===== Load/Add/Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_add_ps) =====");
    no_vec_three_arg_kernel<double>  (pcm, &kernel_nointrin_mm256_add_pd,    "[No Intrin] ===== Load/Add/Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_add_pd) =====");
}

void test_unroll2_add(PCM* pcm)
{
    printf("########## Load/Add/Store (Unrolled by 2) Benchmarks ##########\n");
    #ifdef __AVX2__
        load_store_three_arg_kernel<__m256i, int8_t>  (pcm, &kernel_u2_mm256_add_epi8,  "[AVX2] ===== (Unroll 2) Load/Add/Store 32 Packed 8 bit Signed Integers (_mm256_add_epi8) =====");
        load_store_three_arg_kernel<__m256i, int16_t> (pcm, &kernel_u2_mm256_add_epi16, "[AVX2] ===== (Unroll 2) Load/Add/Store 16 Packed 16 bit Signed Integers (_mm256_add_epi16) =====");
        load_store_three_arg_kernel<__m256i, int32_t> (pcm, &kernel_u2_mm256_add_epi32, "[AVX2] ===== (Unroll 2) Load/Add/Store 8 Packed 32 bit Signed Integers (_mm256_add_epi32) =====");
    #endif

    #ifdef __AVX__
        load_store_three_arg_kernel<__m256,  float>   (pcm, &kernel_u2_mm256_add_ps,    "[AVX] ===== (Unroll 2) Load/Add/Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_add_ps) =====");
        load_store_three_arg_kernel<__m256d, double>  (pcm, &kernel_u2_mm256_add_pd,    "[AVX] ===== (Unroll 2) Load/Add/Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_add_pd) =====");
    #endif
}

void test_mult(PCM* pcm)
{
    printf("########## Load/Multiply/Store Benchmarks ##########\n");
    #ifdef __AVX2__
        load_store_three_arg_kernel<__m256i, int16_t> (pcm, &kernel_mm256_mullo_epi16, "[AVX2] ===== Load/Multiply 16 Packed 16 bit Signed Integers -> Produce 32 Bit Intermediates -> Store Lower 16 Bits of Intermediates (_mm256_mullo_epi16) =====");
        load_store_three_arg_kernel<__m256i, int32_t> (pcm, &kernel_mm256_mullo_epi32, "[AVX2] ===== Load/Multiply 8 Packed 32 bit Signed Integers -> Produce 64 Bit Intermediates -> Store Lower 32 Bits of Intermediates (_mm256_mullo_epi32) =====");
    #endif

    #ifdef __AVX__
        load_store_three_arg_kernel<__m256,  float>   (pcm, &kernel_mm256_mul_ps,      "[AVX] ===== Load/Multiply/Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_mul_ps) =====");
        load_store_three_arg_kernel<__m256d, double>  (pcm, &kernel_mm256_mul_pd,      "[AVX] ===== Load/Multiply/Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_mul_pd) =====");
    #endif
}

void test_div(PCM* pcm)
{
    printf("########## Load/Divide/Store Benchmarks ##########\n");
    #ifdef __AVX__
        load_store_three_arg_kernel<__m256,  float>  (pcm, &kernel_mm256_div_ps, "[AVX] ===== Load/Divide/Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_div_ps) =====");
        load_store_three_arg_kernel<__m256d, double> (pcm, &kernel_mm256_div_pd, "[AVX] ===== Load/Divide/Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_div_pd) =====");
    #endif
}

void test_fma(PCM* pcm)
{
    printf("########## Load/FMA/Store Benchmarks ##########\n");
    #ifdef __FMA__
        load_store_four_arg_kernel<__m256,  float>  (pcm, &kernel_mm256_fmadd_ps,    "[FMA] ===== Load/FMA/Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_fmadd_ps) =====");
        load_store_four_arg_kernel<__m256d, double> (pcm, &kernel_mm256_fmadd_pd,    "[FMA] ===== Load/FMA/Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_fmadd_pd) =====");
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

void* run_benchmarks(void* null)
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
    printf("\n");

    test_load(pcm);
    printf("\n");
    test_store(pcm);
    printf("\n");
    test_load_store(pcm);
    printf("\n");

    test_only_add(pcm);
    printf("\n");
    test_only_mult(pcm);
    printf("\n");
    test_only_div(pcm);
    printf("\n");
    test_only_fma(pcm);
    printf("\n");

    test_add(pcm);
    printf("\n");
    test_mult(pcm);
    printf("\n");
    test_div(pcm);
    printf("\n");
    test_fma(pcm);
    printf("\n");

    test_unroll2_add(pcm);
    printf("\n");

    test_nointrin_add(pcm);

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


    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);

    status = pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);

    if(status != 0)
    {
        printf("Could not set thread core affinity ... exiting\n");
        exit(1);
    }

    status = pthread_create(&thread, &attr, &run_benchmarks, NULL);
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
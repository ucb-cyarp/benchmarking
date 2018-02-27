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

#include <chrono>
#include <ctime>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <iostream>
#include <cstdint>

//Include Intrinsics
#include <x86intrin.h>
#include <immintrin.h>
#include <mm_malloc.h>
// #include <mmintrin.h>
// #include <emmintrin.h>
// #include <xmmintrin.h>

//Set default options
#ifndef STIM_LEN
    #define STIM_LEN 160000000
#endif

#ifndef TRIALS
    #define TRIALS 10
#endif

#ifndef PRINT_TITLE
    #define PRINT_TITLE 1
#endif

#ifndef PRINT_HEADER
    #define PRINT_HEADER 1
#endif

#ifndef PRINT_TRIALS
    #define PRINT_TRIALS 1
#endif

#ifndef PRINT_STATS
    #define PRINT_STATS 1
#endif

void statistics(std::chrono::duration<double, std::ratio<1, 1000>>* durations, double* durations_clock)
{
    //Print Average & StdDev
    double avg_duration = 0;
    double avg_duration_clock = 0;
    for(size_t i = 0; i < TRIALS; i++)
    {
        avg_duration += (double) durations[i].count();
        avg_duration_clock += (double) durations_clock[i];
    }
    avg_duration /= TRIALS;
    avg_duration_clock /= TRIALS;

    double std_dev_duration = 0;
    double std_dev_duration_clock = 0;
    for(size_t i = 0; i < TRIALS; i++)
    {
        double tmp = durations[i].count() - avg_duration;
        double tmp_clock = durations_clock[i] - avg_duration_clock;
        tmp = tmp*tmp;
        tmp_clock = tmp_clock*tmp_clock;

        std_dev_duration += tmp;
        std_dev_duration_clock += tmp_clock;
    }
    std_dev_duration /= (TRIALS - 1);
    std_dev_duration_clock /= (TRIALS - 1);
    std_dev_duration = sqrt(std_dev_duration);
    std_dev_duration_clock = sqrt(std_dev_duration_clock);

    printf("High Res Clock - Sample Mean (ms): %f, Sample Std Dev: %f\n", avg_duration, std_dev_duration);
    printf("         Clock - Sample Mean (ms): %f, Sample Std Dev: %f\n", avg_duration_clock, std_dev_duration_clock);

    printf("High Res Clock - Sample Mean (MS/s): %f\n", STIM_LEN*1.0/(1000.0*avg_duration));
    printf("         Clock - Sample Mean (MS/s): %f\n", STIM_LEN*1.0/(1000.0*avg_duration_clock));
}

//==========_mm256_add_epi8==========
void kernel_mm256_add_epi8( __m256i* a, __m256i* b, __m256i* c)
{
    for(int i = 0; i<STIM_LEN/32; i++)
    {
        __m256i a_val = _mm256_load_si256(a+i);
        __m256i b_val = _mm256_load_si256(b+i);
        __m256i c_val = _mm256_add_epi8(a_val, b_val);
        _mm256_store_si256(c+i, c_val);
    }
}

void test_mm256_add_epi8()
{
    #if PRINT_HEADER == 1
        printf("Add 32 Packed 8 bit Signed Integers (_mm256_add_epi8)\n");
    #endif

    //Allocate timer arrays
    std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];
    double durations_clock[TRIALS];
    double durations_rdtsc[TRIALS];

    for(int trial = 0; trial<TRIALS; trial++)
    {
        //Allocate the arrays to operate over
        //These arrays need to be aligned to the vector register size of 256a
        void* a = _mm_malloc (STIM_LEN*sizeof(int8_t), 32);
        void* b = _mm_malloc (STIM_LEN*sizeof(int8_t), 32);
        void* c = _mm_malloc (STIM_LEN*sizeof(int8_t), 32);

        __m256i* a_m256 = (__m256i * ) a;
        __m256i* b_m256 = (__m256i * ) b;
        __m256i* c_m256 = (__m256i * ) c;

        //Start Timer
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        clock_t start_clock = clock();
        uint64_t start_rdtsc = _rdtsc();

        kernel_mm256_add_epi8(a_m256, b_m256, c_m256);

        //Stop Timer and Report Time
        uint64_t stop_rdtsc = _rdtsc();
        clock_t stop_clock = clock();
        std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();

        durations[trial] = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(stop-start);
        durations_clock[trial] = 1000.0 * (stop_clock - start_clock) / CLOCKS_PER_SEC;
        durations_rdtsc[trial] =  (stop_rdtsc - start_rdtsc);

        #if PRINT_TRIALS == 1
            printf("Trial %6d: %f, %f, %f\n", trial, durations[trial], durations_clock[trial], durations_rdtsc[trial]);
        #endif 

        //Clean up
        _mm_free(a);
        _mm_free(b);
        _mm_free(c);
    }

    #if PRINT_STATS == 1
    statistics(durations, durations_clock);
    #endif
}

//==========_mm256_add_epi16==========
void kernel_mm256_add_epi16( __m256i* a, __m256i* b, __m256i* c)
{
    for(int i = 0; i<STIM_LEN/16; i++)
    {
        __m256i a_val = _mm256_load_si256(a+i);
        __m256i b_val = _mm256_load_si256(b+i);
        __m256i c_val = _mm256_add_epi16(a_val, b_val);
        _mm256_store_si256(c+i, c_val);
    }
}

void test_mm256_add_epi16()
{
    #if PRINT_HEADER == 1
        printf("Add 16 Packed 16 bit Signed Integers (_mm256_add_epi16)\n");
    #endif

    //Allocate timer arrays
    std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];
    double durations_clock[TRIALS];
    double durations_rdtsc[TRIALS];

    for(int trial = 0; trial<TRIALS; trial++)
    {
        //Allocate the arrays to operate over
        //These arrays need to be aligned to the vector register size of 256a
        void* a = _mm_malloc (STIM_LEN*sizeof(int16_t), 32);
        void* b = _mm_malloc (STIM_LEN*sizeof(int16_t), 32);
        void* c = _mm_malloc (STIM_LEN*sizeof(int16_t), 32);

        __m256i* a_m256 = (__m256i * ) a;
        __m256i* b_m256 = (__m256i * ) b;
        __m256i* c_m256 = (__m256i * ) c;

        //Start Timer
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        clock_t start_clock = clock();
        uint64_t start_rdtsc = _rdtsc();

        kernel_mm256_add_epi16(a_m256, b_m256, c_m256);

        //Stop Timer and Report Time
        uint64_t stop_rdtsc = _rdtsc();
        clock_t stop_clock = clock();
        std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();

        durations[trial] = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(stop-start);
        durations_clock[trial] = 1000.0 * (stop_clock - start_clock) / CLOCKS_PER_SEC;
        durations_rdtsc[trial] =  (stop_rdtsc - start_rdtsc);

        #if PRINT_TRIALS == 1
            printf("Trial %6d: %f, %f, %f\n", trial, durations[trial], durations_clock[trial], durations_rdtsc[trial]);
        #endif 

        //Clean up
        _mm_free(a);
        _mm_free(b);
        _mm_free(c);
    }

    #if PRINT_STATS == 1
    statistics(durations, durations_clock);
    #endif
}

//==========_mm256_add_epi32==========
void kernel_mm256_add_epi32( __m256i* a, __m256i* b, __m256i* c)
{
    for(int i = 0; i<STIM_LEN/8; i++)
    {
        __m256i a_val = _mm256_load_si256(a+i);
        __m256i b_val = _mm256_load_si256(b+i);
        __m256i c_val = _mm256_add_epi32(a_val, b_val);
        _mm256_store_si256(c+i, c_val);
    }
}

void test_mm256_add_epi32()
{
    #if PRINT_HEADER == 1
        printf("Add 8 Packed 32 bit Signed Integers (_mm256_add_epi32)\n");
    #endif

    //Allocate timer arrays
    std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];
    double durations_clock[TRIALS];
    double durations_rdtsc[TRIALS];

    for(int trial = 0; trial<TRIALS; trial++)
    {
        //Allocate the arrays to operate over
        //These arrays need to be aligned to the vector register size of 256a
        void* a = _mm_malloc (STIM_LEN*sizeof(int32_t), 32);
        void* b = _mm_malloc (STIM_LEN*sizeof(int32_t), 32);
        void* c = _mm_malloc (STIM_LEN*sizeof(int32_t), 32);

        __m256i* a_m256 = (__m256i * ) a;
        __m256i* b_m256 = (__m256i * ) b;
        __m256i* c_m256 = (__m256i * ) c;

        //Start Timer
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        clock_t start_clock = clock();
        uint64_t start_rdtsc = _rdtsc();

        kernel_mm256_add_epi32(a_m256, b_m256, c_m256);

        //Stop Timer and Report Time
        uint64_t stop_rdtsc = _rdtsc();
        clock_t stop_clock = clock();
        std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();

        durations[trial] = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(stop-start);
        durations_clock[trial] = 1000.0 * (stop_clock - start_clock) / CLOCKS_PER_SEC;
        durations_rdtsc[trial] =  (stop_rdtsc - start_rdtsc);

        #if PRINT_TRIALS == 1
            printf("Trial %6d: %f, %f, %f\n", trial, durations[trial], durations_clock[trial], durations_rdtsc[trial]);
        #endif 

        //Clean up
        _mm_free(a);
        _mm_free(b);
        _mm_free(c);
    }

    #if PRINT_STATS == 1
    statistics(durations, durations_clock);
    #endif
}

//==========_mm256_add_ps==========
void kernel_mm256_add_ps( __m256* a, __m256* b, __m256* c)
{
    float* a_float = (float*) a;
    float* b_float = (float*) b;
    float* c_float = (float*) c;

    for(int i = 0; i<STIM_LEN; i+=8)
    {
        __m256 a_val = _mm256_load_ps(a_float+i);
        __m256 b_val = _mm256_load_ps(b_float+i);
        __m256 c_val = _mm256_add_ps(a_val, b_val);
        _mm256_store_ps(c_float+i, c_val);
    }
}

void test_mm256_add_ps()
{
    #if PRINT_HEADER == 1
        printf("Add 8 Packed 32 bit Signed Floating Point Numbers (_mm256_add_ps)\n");
    #endif

    //Allocate timer arrays
    std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];
    double durations_clock[TRIALS];
    double durations_rdtsc[TRIALS];

    for(int trial = 0; trial<TRIALS; trial++)
    {
        //Allocate the arrays to operate over
        //These arrays need to be aligned to the vector register size of 256a
        void* a = _mm_malloc (STIM_LEN*sizeof(float), 32);
        void* b = _mm_malloc (STIM_LEN*sizeof(float), 32);
        void* c = _mm_malloc (STIM_LEN*sizeof(float), 32);

        __m256* a_m256 = (__m256 * ) a;
        __m256* b_m256 = (__m256 * ) b;
        __m256* c_m256 = (__m256 * ) c;

        //Start Timer
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        clock_t start_clock = clock();
        uint64_t start_rdtsc = _rdtsc();

        kernel_mm256_add_ps(a_m256, b_m256, c_m256);

        //Stop Timer and Report Time
        uint64_t stop_rdtsc = _rdtsc();
        clock_t stop_clock = clock();
        std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();

        durations[trial] = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(stop-start);
        durations_clock[trial] = 1000.0 * (stop_clock - start_clock) / CLOCKS_PER_SEC;
        durations_rdtsc[trial] =  (stop_rdtsc - start_rdtsc);

        #if PRINT_TRIALS == 1
            printf("Trial %6d: %f, %f, %f\n", trial, durations[trial], durations_clock[trial], durations_rdtsc[trial]);
        #endif 

        //Clean up
        _mm_free(a);
        _mm_free(b);
        _mm_free(c);
    }

    #if PRINT_STATS == 1
    statistics(durations, durations_clock);
    #endif
}

int main(int argc, char *argv[])
{
    #if PRINT_TITLE == 1
    printf("SSE/AVX/FMA Intrinsic Tester\n");
    printf("STIM_LEN: %d, TRIALS: %d\n", STIM_LEN, TRIALS);
    #endif

    test_mm256_add_epi8();
    test_mm256_add_epi16();
    test_mm256_add_epi32();
    test_mm256_add_ps();
}
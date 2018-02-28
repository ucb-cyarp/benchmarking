#include "intrin_bench_default_defines.h"

#include "statistics.h"

//==========_mm256_mullo_epi16==========
void kernel_mm256_mullo_epi16( __m256i* a, __m256i* b, __m256i* c)
{
    for(int i = 0; i<STIM_LEN/16; i++)
    {
        __m256i a_val = _mm256_load_si256(a+i);
        __m256i b_val = _mm256_load_si256(b+i);
        __m256i c_val = _mm256_mullo_epi16(a_val, b_val);
        _mm256_store_si256(c+i, c_val);
    }
}

void test_mm256_mullo_epi16()
{
    #if PRINT_HEADER == 1
        printf("Multiply 16 Packed 16 bit Signed Integers -> Produce 32 Bit Intermediates -> Store Lower 16 Bits of Intermediates (_mm256_mullo_epi16)\n");
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

        kernel_mm256_mullo_epi16(a_m256, b_m256, c_m256);

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

//==========_mm256_mullo_epi32==========
void kernel_mm256_mullo_epi32( __m256i* a, __m256i* b, __m256i* c)
{
    for(int i = 0; i<STIM_LEN/8; i++)
    {
        __m256i a_val = _mm256_load_si256(a+i);
        __m256i b_val = _mm256_load_si256(b+i);
        __m256i c_val = _mm256_mullo_epi32(a_val, b_val);
        _mm256_store_si256(c+i, c_val);
    }
}

void test_mm256_mullo_epi32()
{
    #if PRINT_HEADER == 1
        printf("Multiply 8 Packed 32 bit Signed Integers -> Produce 64 Bit Intermediates -> Store Lower 32 Bits of Intermediates (_mm256_mullo_epi32)\n");
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

        kernel_mm256_mullo_epi32(a_m256, b_m256, c_m256);

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

//==========_mm256_mul_ps==========
void kernel_mm256_mul_ps( __m256* a, __m256* b, __m256* c)
{
    float* a_float = (float*) a;
    float* b_float = (float*) b;
    float* c_float = (float*) c;

    for(int i = 0; i<STIM_LEN; i+=8)
    {
        __m256 a_val = _mm256_load_ps(a_float+i);
        __m256 b_val = _mm256_load_ps(b_float+i);
        __m256 c_val = _mm256_mul_ps(a_val, b_val);
        _mm256_store_ps(c_float+i, c_val);
    }
}

void test_mm256_mul_ps()
{
    #if PRINT_HEADER == 1
        printf("Multiply 8 Packed 32 bit Signed Floating Point Numbers (_mm256_mul_ps)\n");
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

        kernel_mm256_mul_ps(a_m256, b_m256, c_m256);

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

//==========_mm256_mul_pd==========
void kernel_mm256_mul_pd( __m256d* a, __m256d* b, __m256d* c)
{
    double* a_double = (double*) a;
    double* b_double = (double*) b;
    double* c_double = (double*) c;

    for(int i = 0; i<STIM_LEN; i+=4)
    {
        __m256d a_val = _mm256_load_pd(a_double+i);
        __m256d b_val = _mm256_load_pd(b_double+i);
        __m256d c_val = _mm256_mul_pd(a_val, b_val);
        _mm256_store_pd(c_double+i, c_val);
    }
}

void test_mm256_mul_pd()
{
    #if PRINT_HEADER == 1
        printf("Multiply 4 Packed 64 bit Signed Floating Point Numbers (_mm256_mul_pd)\n");
    #endif

    //Allocate timer arrays
    std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];
    double durations_clock[TRIALS];
    double durations_rdtsc[TRIALS];

    for(int trial = 0; trial<TRIALS; trial++)
    {
        //Allocate the arrays to operate over
        //These arrays need to be aligned to the vector register size of 256a
        void* a = _mm_malloc (STIM_LEN*sizeof(double), 32);
        void* b = _mm_malloc (STIM_LEN*sizeof(double), 32);
        void* c = _mm_malloc (STIM_LEN*sizeof(double), 32);

        __m256d* a_m256d = (__m256d * ) a;
        __m256d* b_m256d = (__m256d * ) b;
        __m256d* c_m256d = (__m256d * ) c;

        //Start Timer
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        clock_t start_clock = clock();
        uint64_t start_rdtsc = _rdtsc();

        kernel_mm256_mul_pd(a_m256d, b_m256d, c_m256d);

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
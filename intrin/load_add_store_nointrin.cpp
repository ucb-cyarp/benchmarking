#include "intrin_bench_default_defines.h"
#include "load_add_store_nointrin_kernel.h"

#include "statistics.h"

//==========_mm256_add_epi8==========
void test_nointrin_mm256_add_epi8()
{
    #if PRINT_HEADER == 1
        printf("[No Intrin] ===== Load/Add/Store 32 Packed 8 bit Signed Integers (_mm256_add_epi8) =====\n");
    #endif

    //Allocate timer arrays
    std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];
    double durations_clock[TRIALS];
    double durations_rdtsc[TRIALS];

    for(int trial = 0; trial<TRIALS; trial++)
    {
        //Allocate the arrays to operate over
        //These arrays need to be aligned to the vector register size of 256a
        void* a = malloc (STIM_LEN*sizeof(int8_t));
        void* b = malloc (STIM_LEN*sizeof(int8_t));
        void* c = malloc (STIM_LEN*sizeof(int8_t));

        int8_t* a_int8 = (int8_t * ) a;
        int8_t* b_int8 = (int8_t * ) b;
        int8_t* c_int8 = (int8_t * ) c;

        //Start Timer
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        clock_t start_clock = clock();
        uint64_t start_rdtsc = _rdtsc();

        kernel_nointrin_mm256_add_epi8(a_int8, b_int8, c_int8);

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
        free(a);
        free(b);
        free(c);
    }

    #if PRINT_STATS == 1
    statistics(durations, durations_clock);
    #endif
}

//==========_mm256_add_epi16==========
void test_nointrin_mm256_add_epi16()
{
    #if PRINT_HEADER == 1
        printf("[No Intrin] ===== Load/Add/Store 16 Packed 16 bit Signed Integers (_mm256_add_epi16) =====\n");
    #endif

    //Allocate timer arrays
    std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];
    double durations_clock[TRIALS];
    double durations_rdtsc[TRIALS];

    for(int trial = 0; trial<TRIALS; trial++)
    {
        //Allocate the arrays to operate over
        //These arrays need to be aligned to the vector register size of 256a
        void* a = malloc (STIM_LEN*sizeof(int16_t));
        void* b = malloc (STIM_LEN*sizeof(int16_t));
        void* c = malloc (STIM_LEN*sizeof(int16_t));

        int16_t* a_int16 = (int16_t * ) a;
        int16_t* b_int16 = (int16_t * ) b;
        int16_t* c_int16 = (int16_t * ) c;

        //Start Timer
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        clock_t start_clock = clock();
        uint64_t start_rdtsc = _rdtsc();

        kernel_nointrin_mm256_add_epi16(a_int16, b_int16, c_int16);

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
        free(a);
        free(b);
        free(c);
    }

    #if PRINT_STATS == 1
    statistics(durations, durations_clock);
    #endif
}

//==========_mm256_add_epi32==========
void test_nointrin_mm256_add_epi32()
{
    #if PRINT_HEADER == 1
        printf("[No Intrin] ===== Load/Add/Store 8 Packed 32 bit Signed Integers (_mm256_add_epi32) =====\n");
    #endif

    //Allocate timer arrays
    std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];
    double durations_clock[TRIALS];
    double durations_rdtsc[TRIALS];

    for(int trial = 0; trial<TRIALS; trial++)
    {
        //Allocate the arrays to operate over
        //These arrays need to be aligned to the vector register size of 256a
        void* a = malloc (STIM_LEN*sizeof(int32_t));
        void* b = malloc (STIM_LEN*sizeof(int32_t));
        void* c = malloc (STIM_LEN*sizeof(int32_t));

        int32_t* a_int32 = (int32_t * ) a;
        int32_t* b_int32 = (int32_t * ) b;
        int32_t* c_int32 = (int32_t * ) c;

        //Start Timer
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        clock_t start_clock = clock();
        uint64_t start_rdtsc = _rdtsc();

        kernel_nointrin_mm256_add_epi32(a_int32, b_int32, c_int32);

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
        free(a);
        free(b);
        free(c);
    }

    #if PRINT_STATS == 1
    statistics(durations, durations_clock);
    #endif
}

//==========_mm256_add_ps==========
void test_nointrin_mm256_add_ps()
{
    #if PRINT_HEADER == 1
        printf("[No Intrin] ===== Load/Add/Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_add_ps) =====\n");
    #endif

    //Allocate timer arrays
    std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];
    double durations_clock[TRIALS];
    double durations_rdtsc[TRIALS];

    for(int trial = 0; trial<TRIALS; trial++)
    {
        //Allocate the arrays to operate over
        //These arrays need to be aligned to the vector register size of 256a
        void* a = malloc (STIM_LEN*sizeof(float));
        void* b = malloc (STIM_LEN*sizeof(float));
        void* c = malloc (STIM_LEN*sizeof(float));

        float* a_float = (float * ) a;
        float* b_float = (float * ) b;
        float* c_float = (float * ) c;

        //Start Timer
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        clock_t start_clock = clock();
        uint64_t start_rdtsc = _rdtsc();

        kernel_nointrin_mm256_add_ps(a_float, b_float, c_float);

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
        free(a);
        free(b);
        free(c);
    }

    #if PRINT_STATS == 1
    statistics(durations, durations_clock);
    #endif
}

//==========_mm256_add_pd==========
void test_nointrin_mm256_add_pd()
{
    #if PRINT_HEADER == 1
        printf("[No Intrin] ===== Load/Add/Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_add_pd) =====\n");
    #endif

    //Allocate timer arrays
    std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];
    double durations_clock[TRIALS];
    double durations_rdtsc[TRIALS];

    for(int trial = 0; trial<TRIALS; trial++)
    {
        //Allocate the arrays to operate over
        //These arrays need to be aligned to the vector register size of 256a
        void* a = malloc (STIM_LEN*sizeof(double));
        void* b = malloc (STIM_LEN*sizeof(double));
        void* c = malloc (STIM_LEN*sizeof(double));

        double* a_double = (double * ) a;
        double* b_double = (double * ) b;
        double* c_double = (double * ) c;

        //Start Timer
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        clock_t start_clock = clock();
        uint64_t start_rdtsc = _rdtsc();

        kernel_nointrin_mm256_add_pd(a_double, b_double, c_double);

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
        free(a);
        free(b);
        free(c);
    }

    #if PRINT_STATS == 1
    statistics(durations, durations_clock);
    #endif
}

#include "intrin_bench_default_defines.h"
#include "load_kernel.h"

#include "statistics.h"

#ifdef __AVX__
    //==========_mm256_load_si256 int8_t==========
    void test_mm256_load_si256_int8()
    {
        #if PRINT_HEADER == 1
            printf("[AVX] ===== Load 32 Signed Bytes [Store in Local Var] (_mm256_load_si256) =====\n");
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

            __m256i* a_m256 = (__m256i * ) a;

            //Start Timer
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            clock_t start_clock = clock();
            uint64_t start_rdtsc = _rdtsc();

            kernel_mm256_load_si256_int8(a_m256);

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
        }

        #if PRINT_STATS == 1
        statistics(durations, durations_clock);
        #endif
    }

    //==========_mm256_load_si256 int16_t==========
    void test_mm256_load_si256_int16()
    {
        #if PRINT_HEADER == 1
            printf("[AVX] ===== Load 16 Signed 16 Bit Integers [Store in Local Var] (_mm256_load_si256) =====\n");
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

            __m256i* a_m256 = (__m256i * ) a;

            //Start Timer
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            clock_t start_clock = clock();
            uint64_t start_rdtsc = _rdtsc();

            kernel_mm256_load_si256_int16(a_m256);

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
        }

        #if PRINT_STATS == 1
        statistics(durations, durations_clock);
        #endif
    }

    //==========_mm256_load_si256 int32_t==========
    void test_mm256_load_si256_int32()
    {
        #if PRINT_HEADER == 1
            printf("[AVX] ===== Load 8 Signed 32 Bit Integers [Store in Local Var] (_mm256_load_si256) =====\n");
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

            __m256i* a_m256 = (__m256i * ) a;

            //Start Timer
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            clock_t start_clock = clock();
            uint64_t start_rdtsc = _rdtsc();

            kernel_mm256_load_si256_int32(a_m256);

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
        }

        #if PRINT_STATS == 1
        statistics(durations, durations_clock);
        #endif
    }

    //==========_mm256_load_ps==========
    void test_mm256_load_ps()
    {
        #if PRINT_HEADER == 1
            printf("[AVX] ===== Load 8 Packed 32 bit Signed Floating Point Numbers [Store in Local Var] (_mm256_load_ps) =====\n");
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

            __m256* a_m256 = (__m256 * ) a;

            //Start Timer
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            clock_t start_clock = clock();
            uint64_t start_rdtsc = _rdtsc();

            kernel_mm256_load_ps(a_m256);

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
        }

        #if PRINT_STATS == 1
        statistics(durations, durations_clock);
        #endif
    }

    //==========_mm256_load_pd==========
    void test_mm256_load_pd()
    {
        #if PRINT_HEADER == 1
            printf("[AVX] ===== Load 4 Packed 64 bit Signed Floating Point Numbers [Store in Local Var] (_mm256_load_pd) =====\n");
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

            __m256d* a_m256d = (__m256d * ) a;

            //Start Timer
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            clock_t start_clock = clock();
            uint64_t start_rdtsc = _rdtsc();

            kernel_mm256_load_pd(a_m256d);

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
        }

        #if PRINT_STATS == 1
        statistics(durations, durations_clock);
        #endif
    }
#endif

#include "intrin_bench_default_defines.h"
#include "add_kernel.h"

#include "statistics.h"

#ifdef __AVX2__
    //==========_mm256_add_epi8==========
    void test_only_mm256_add_epi8()
    {
        #if PRINT_HEADER == 1
            printf("[AVX2] ===== Add 32 Packed 8 bit Signed Integers [Store in Local Var] (_mm256_add_epi8) =====\n");
        #endif

        //Allocate timer arrays
        std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];
        double durations_clock[TRIALS];
        double durations_rdtsc[TRIALS];

        for(int trial = 0; trial<TRIALS; trial++)
        {
            //Start Timer
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            clock_t start_clock = clock();
            uint64_t start_rdtsc = _rdtsc();

            kernel_only_mm256_add_epi8();

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
        }

        #if PRINT_STATS == 1
        statistics(durations, durations_clock);
        #endif
    }

    //==========_mm256_add_epi16==========
    void test_only_mm256_add_epi16()
    {
        #if PRINT_HEADER == 1
            printf("[AVX2] ===== Add 16 Packed 16 bit Signed Integers [Store in Local Var] (_mm256_add_epi16) =====\n");
        #endif

        //Allocate timer arrays
        std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];
        double durations_clock[TRIALS];
        double durations_rdtsc[TRIALS];

        for(int trial = 0; trial<TRIALS; trial++)
        {
            //Start Timer
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            clock_t start_clock = clock();
            uint64_t start_rdtsc = _rdtsc();

            kernel_only_mm256_add_epi16();

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
        }

        #if PRINT_STATS == 1
        statistics(durations, durations_clock);
        #endif
    }

    //==========_mm256_add_epi32==========
    void test_only_mm256_add_epi32()
    {
        #if PRINT_HEADER == 1
            printf("[AVX2] ===== Add 8 Packed 32 bit Signed Integers [Store in Local Var] (_mm256_add_epi32) =====\n");
        #endif

        //Allocate timer arrays
        std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];
        double durations_clock[TRIALS];
        double durations_rdtsc[TRIALS];

        for(int trial = 0; trial<TRIALS; trial++)
        {
            //Start Timer
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            clock_t start_clock = clock();
            uint64_t start_rdtsc = _rdtsc();

            kernel_only_mm256_add_epi32();

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
        }

        #if PRINT_STATS == 1
        statistics(durations, durations_clock);
        #endif
    }
#endif

#ifdef __AVX__
    //==========_mm256_add_ps==========
    void test_only_mm256_add_ps()
    {
        #if PRINT_HEADER == 1
            printf("[AVX] ===== Add 8 Packed 32 bit Signed Floating Point Numbers [Store in Local Var] (_mm256_add_ps) =====\n");
        #endif

        //Allocate timer arrays
        std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];
        double durations_clock[TRIALS];
        double durations_rdtsc[TRIALS];

        for(int trial = 0; trial<TRIALS; trial++)
        {
            //Start Timer
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            clock_t start_clock = clock();
            uint64_t start_rdtsc = _rdtsc();

            kernel_only_mm256_add_ps();

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
        }

        #if PRINT_STATS == 1
        statistics(durations, durations_clock);
        #endif
    }

    //==========_mm256_add_pd==========
    void test_only_mm256_add_pd()
    {
        #if PRINT_HEADER == 1
            printf("[AVX] ===== Add 4 Packed 64 bit Signed Floating Point Numbers [Store in Local Var] (_mm256_add_pd) =====\n");
        #endif

        //Allocate timer arrays
        std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];
        double durations_clock[TRIALS];
        double durations_rdtsc[TRIALS];

        for(int trial = 0; trial<TRIALS; trial++)
        {
            //Start Timer
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            clock_t start_clock = clock();
            uint64_t start_rdtsc = _rdtsc();

            kernel_only_mm256_add_pd();

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
        }

        #if PRINT_STATS == 1
        statistics(durations, durations_clock);
        #endif
    }
#endif

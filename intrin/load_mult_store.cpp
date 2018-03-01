#include "intrin_bench_default_defines.h"
#include "load_mult_store_kernel.h"

#include "statistics.h"

#ifdef __AVX2__
    //==========_mm256_mullo_epi16==========
    void test_mm256_mullo_epi16()
    {
        #if PRINT_HEADER == 1
            printf("[AVX2] ===== Load/Multiply 16 Packed 16 bit Signed Integers -> Produce 32 Bit Intermediates -> Store Lower 16 Bits of Intermediates (_mm256_mullo_epi16) =====\n");
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
    void test_mm256_mullo_epi32()
    {
        #if PRINT_HEADER == 1
            printf("[AVX2] ===== Load/Multiply 8 Packed 32 bit Signed Integers -> Produce 64 Bit Intermediates -> Store Lower 32 Bits of Intermediates (_mm256_mullo_epi32) =====\n");
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
#endif

#ifdef __AVX__
    //==========_mm256_mul_ps==========
    void test_mm256_mul_ps()
    {
        #if PRINT_HEADER == 1
            printf("[AVX] ===== Load/Multiply/Store 8 Packed 32 bit Signed Floating Point Numbers (_mm256_mul_ps) =====\n");
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
    void test_mm256_mul_pd()
    {
        #if PRINT_HEADER == 1
            printf("[AVX] ===== Load/Multiply/Store 4 Packed 64 bit Signed Floating Point Numbers (_mm256_mul_pd) =====\n");
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
#endif

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
        printf("Multiply 16 Packed 16 bit Signed Integers -> Produce 32 Bit Intermediate -> Store Lower 16 Bits of Intermediate (_mm256_mullo_epi16)\n");
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

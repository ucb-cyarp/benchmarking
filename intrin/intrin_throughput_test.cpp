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

#include "intrin_bench_default_defines.h"
#include "load_add_store.h"

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
    test_mm256_add_pd();
}
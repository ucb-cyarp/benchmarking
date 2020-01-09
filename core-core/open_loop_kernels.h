#ifndef _H_OPEN_LOOP_KERNELS
    #define _H_OPEN_LOOP_KERNELS

    #include "results.h"
    #include "profiler.h"

    #include "kernel_server_runner.h"

    #include "print_results.h"

    #include <cstdlib>

    #include "mallocHelpers.h"
    #include "fifo_helpers.h"
    #include "reporting_helpers.h"

    #include <algorithm>

    void run_open_loop_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, std::vector<int32_t> balance_nops, std::vector<int> initial_nops, int alignment, int64_t max_block_transfers, FILE* file, std::ofstream* raw_file);

#endif
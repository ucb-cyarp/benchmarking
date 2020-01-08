#ifndef _H_CLOSED_LOOP_KERNELS
    #define _H_CLOSED_LOOP_KERNELS

    #include "results.h"
    #include "profiler.h"

    #include "kernel_server_runner.h"

    #include "print_results.h"

    #include <cstdlib>

    #include "mallocHelpers.h"
    #include "fifo_helpers.h"
    #include "reporting_helpers.h"

    #include <algorithm>

    void run_closed_loop_bang_control_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, std::vector<int32_t> server_control_period, std::vector<int32_t> client_control_period, std::vector<int32_t> control_gain, int alignment, int64_t max_block_transfers, FILE* file, std::ofstream* raw_file);
#endif
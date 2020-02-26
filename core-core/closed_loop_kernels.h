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

    void run_closed_loop_bang_control_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, std::vector<int32_t> server_control_period, std::vector<int32_t> client_control_period, std::vector<float> control_gain, std::vector<float> initial_nops, int alignment, int64_t max_block_transfers, FILE* file, std::ofstream* raw_file);
    void run_closed_loop_pi_control_rate_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, std::vector<int32_t> server_control_periods, std::vector<int32_t> client_control_periods, std::vector<float> control_gains_p, std::vector<float> control_gains_i, std::vector<float> initial_nops, int alignment, int64_t max_block_transfers, FILE* file, std::ofstream* raw_file);
#endif
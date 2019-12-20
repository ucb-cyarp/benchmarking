#ifndef _H_FIFO_KERNELS
    #define _H_FIFO_KERNELS

    #include "results.h"
    #include "profiler.h"

    #include "kernel_server_runner.h"

    #include "bandwidth_circular_fifo_kernel.h"
    #include "bandwidth_circular_fifo_blocked_kernel.h"
    #include "bandwidth_circular_fifo_read_limit_kernel.h"
    #include "bandwidth_circular_fifo_blocked_cachedptr_kernel.h"

    #include "print_results.h"

    #include <cstdlib>

    #include "mallocHelpers.h"
    #include "fifo_helpers.h"
    #include "reporting_helpers.h"

    #include <algorithm>

    //MAKE A 2D Table
    void run_bandwidth_fifo_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, std::vector<int32_t> max_writes_per_transaction, FILE* file = NULL, std::ofstream* raw_file=NULL);

    //MAKE A 2D Table
    void run_bandwidth_fifo_blocked_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, FILE* file = NULL, std::ofstream* raw_file=NULL);

    //MAKE A 2D Table
    void run_bandwidth_fifo_read_limit_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, std::vector<int32_t> max_elements_per_transaction, FILE* file = NULL, std::ofstream* raw_file=NULL);

    //MAKE A 2D Table
    void run_bandwidth_fifo_blocked_cachedptr_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, FILE* file = NULL, std::ofstream* raw_file=NULL);
#endif
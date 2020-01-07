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

    void run_open_loop_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, std::vector<int32_t> max_writes_per_transaction, std::vector<int32_t> ballance_nops, FILE* file = NULL, std::ofstream* raw_file=NULL);

#endif
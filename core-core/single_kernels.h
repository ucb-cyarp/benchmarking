#ifndef _H_SINGLE_KERNELS
    #define _H_SINGLE_KERNELS

    #include "results.h"
    #include "profiler.h"

    #include "kernel_server_runner.h"
    
    #include "latency_single_kernel.h"
    #include "latency_dual_kernel.h"
    
    #include "print_results.h"

    #include <cstdlib>

    #include "mallocHelpers.h"

    #include <atomic>
    #include <ctime>

    #include "reporting_helpers.h"

    Results* run_latency_single_kernel(Profiler* profiler, int cpu_a, int cpu_b);

    Results* run_latency_dual_kernel(Profiler* profiler, int cpu_a, int cpu_b);
#endif

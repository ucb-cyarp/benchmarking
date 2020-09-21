#ifndef _H_ARRAY_KERNELS
    #define _H_ARRAY_KERNELS

    #include "results.h"
    #include "profiler.h"

    #include "kernel_server_runner.h"

    #include "latency_single_array_kernel.h"
    #include "latency_dual_array_kernel.h"
    #include "latency_flow_ctrl_kernel.h"
    #include "latency_flow_ctrl_blocked_read_kernel.h"

    #include "print_results.h"

    #include <cstdlib>

    #include "mallocHelpers.h"

    #include <atomic>
    #include <algorithm>

    #include "reporting_helpers.h"

    //==== Single Array ====
    void run_latency_single_array_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, FILE* file=NULL, std::ofstream* raw_file=NULL);
    void run_latency_single_array_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, int cpu_d, std::vector<size_t> array_lengths, FILE* file_a=NULL, FILE* file_b=NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL);
    /**
     * Note: This version tests fanin & fanout.  Since the single array kernel operates by
     * "ping pong-ing" between the client and server threads, each cycle contains a fan-out
     * and a fan-in.  The reported result (since it is reported as 1 way) averages the effect
     * of fanout and fanin
     */
    void run_latency_single_array_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, std::vector<size_t> array_lengths, FILE* file_a=NULL, FILE* file_b=NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL);

    //==== Dual array ====
    void run_latency_dual_array_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, FILE* file = NULL, std::ofstream* raw_file=NULL);
    void run_latency_dual_array_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, int cpu_d, std::vector<size_t> array_lengths, FILE* file_a = NULL, FILE* file_b = NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL);
    void run_latency_dual_array_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, std::vector<size_t> array_lengths, FILE* file_a = NULL, FILE* file_b = NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL);
    void run_latency_flow_ctrl_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, FILE* file = NULL, std::ofstream* raw_file=NULL);
    void run_latency_flow_ctrl_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, int cpu_d, std::vector<size_t> array_lengths, FILE* file_a = NULL, FILE* file_b = NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL);

    /**
     * This version tests for fan-in when the comunication mechanism uses the flow control model.
     * 2 servers feed to 1 client.  The servers conduct the measurment
     */
    void run_latency_flow_ctrl_fanin_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, std::vector<size_t> array_lengths, FILE* file_a = NULL, FILE* file_b = NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL);
    /**
     * This version tests for fan-out when the comunication mechanism uses the flow control model.
     * 1 server feed to 2 clients.  The clients conduct the measurment
     * 
     * TODO: Small inaccuracy may occur on the first transaction due to servers starting first
     */
    void run_latency_flow_ctrl_fanout_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, std::vector<size_t> array_lengths, FILE* file_a = NULL, FILE* file_b = NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL);
    void run_latency_flow_ctrl_blocked_read_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, FILE* file = NULL, std::ofstream* raw_file=NULL);
    void run_latency_flow_ctrl_blocked_read_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, int cpu_d, std::vector<size_t> array_lengths, FILE* file_a = NULL, FILE* file_b = NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL);
    void run_latency_flow_ctrl_blocked_read_fanin_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, std::vector<size_t> array_lengths, FILE* file_a = NULL, FILE* file_b = NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL);
    void run_latency_flow_ctrl_blocked_read_fanout_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, std::vector<size_t> array_lengths, FILE* file_a = NULL, FILE* file_b = NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL);
#endif
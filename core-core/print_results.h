#ifndef _H_PRINT_RESULTS
    #define _H_PRINT_RESULTS

    #include "results.h"
    #include "profiler.h"

    #include "kernel_server_runner.h"
    #include "latency_single_kernel.h"
    #include "intrin_bench_default_defines_and_imports_cpp.h"

    void print_results_standalone(Results &results, int bytes_per_element, int stim_len);
    void print_results(Results &results, int bytes_per_element, int stim_len, int length, std::string format, FILE* file=NULL, std::ofstream* raw_file=NULL);
    void print_results(Results &results, int core0, int core1, int bytes_per_element, int stim_len, int length, std::string format, FILE* file=NULL, std::ofstream* raw_file=NULL);
    void print_results(Results &results, int bytes_per_element, int stim_len, int length, int max_write_per_transaction, std::string format, FILE* file=NULL, std::ofstream* raw_file=NULL);
    void print_results_blocked_fifo_standalone(Results &results, int elements_per_trial, int block_length, int bytes_per_element);
    void print_results_blocked_fifo(Results &results, int elements_per_trial, int length, int block_length, int bytes_per_element, std::string format, FILE* file=NULL, std::ofstream* raw_file=NULL);
    void print_results_fifoless_standalone(Results &results);
    void print_results_open_loop(Results &results, size_t array_length, int32_t block_length, int32_t balancing_nops, int initial_nops, std::string format, FILE* file, std::ofstream* raw_file);
#endif
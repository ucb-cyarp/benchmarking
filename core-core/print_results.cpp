#include "print_results.h"

/**
 * Prints a single result for most benchmarks which focus on latency and data rate.
 * Results across parameters not printed in a single table, each result (configuration)
 * has its own mini table.
 */
void print_results_standalone(Results &results, int bytes_per_element, int stim_len)
{
    double avg_duration_ms = results.avg_duration();
    double stddev_duration_ms = results.stddev_duration();
    double avg_latency_ns = avg_duration_ms*1000000/stim_len;
    double stddev_latency_ns = stddev_duration_ms*1000000/stim_len;

    double transactions_rate_msps = stim_len/(1000.0*avg_duration_ms);

    double data_rate_mbps = 8.0*bytes_per_element*stim_len/(1000.0*avg_duration_ms);

    printf("        =======================================================\n");
    printf("        Metric                   |     Avg      |    StdDev    \n");
    printf("        =======================================================\n");
    //printf("        Duration (ms)            |%14.6f|%14.6f\n", stim_len, avg_duration_ms, stddev_duration_ms);
    printf("        One Way Latency (ns)     |%14.6f|%14.6f\n", avg_latency_ns, stddev_latency_ns);
    printf("        Transaction Rate (MT/s)  |%14.6f|\n", transactions_rate_msps);
    printf("        Data Rate (Mbps)         |%14.6f|\n", data_rate_mbps);
    printf("        =======================================================\n");
}

/**
 * Prints a result for most benchmarks which focus on latency and data rate.
 * Results are printed as a row in a table which contains results for other configurations
 * Results are also written to the summary CSV file and the raw CSV file
 */
void print_results(Results &results, int bytes_per_element, int stim_len, int length, std::string format, FILE* file, std::ofstream* raw_file)
{
    double avg_duration_ms = results.avg_duration();
    double stddev_duration_ms = results.stddev_duration();
    double avg_latency_ns = avg_duration_ms*1000000/stim_len;
    double stddev_latency_ns = stddev_duration_ms*1000000/stim_len;

    double transactions_rate_msps = stim_len/(1000.0*avg_duration_ms);

    double data_rate_mbps = 8.0*bytes_per_element*stim_len/(1000.0*avg_duration_ms);

    #if PRINT_STATS == 1
    printf(format.c_str(), length, avg_latency_ns, stddev_latency_ns, transactions_rate_msps, data_rate_mbps);
    #endif

    #if WRITE_CSV == 1
    fprintf(file, "%d, %f, %f, %f, %f\n", length, avg_latency_ns, stddev_latency_ns, transactions_rate_msps, data_rate_mbps);
    results.write_durations_and_benchmark_specific_results(*raw_file, {""}, {std::to_string(length)}, false);
    #endif
}

/**
 * Prints a result for most benchmarks which focus on latency and data rate.
 * 
 * This version includes the cores involved in the transfer and is used when multiple communication streams are occuring and the performance
 * of each must be distinctly reported
 * 
 * Results are printed as a row in a table which contains results for other configurations
 * Results are also written to the summary CSV file and the raw CSV file
 */
void print_results(Results &results, int core0, int core1, int bytes_per_element, int stim_len, int length, std::string format, FILE* file, std::ofstream* raw_file)
{
    double avg_duration_ms = results.avg_duration();
    double stddev_duration_ms = results.stddev_duration();
    double avg_latency_ns = avg_duration_ms*1000000/stim_len;
    double stddev_latency_ns = stddev_duration_ms*1000000/stim_len;

    double transactions_rate_msps = stim_len/(1000.0*avg_duration_ms);

    double data_rate_mbps = 8.0*bytes_per_element*stim_len/(1000.0*avg_duration_ms);

    #if PRINT_STATS == 1
    printf(format.c_str(), core0, core1, length, avg_latency_ns, stddev_latency_ns, transactions_rate_msps, data_rate_mbps);
    #endif

    #if WRITE_CSV == 1
    fprintf(file, "%d, %f, %f, %f, %f\n", length, avg_latency_ns, stddev_latency_ns, transactions_rate_msps, data_rate_mbps);
    results.write_durations_and_benchmark_specific_results(*raw_file, {""}, {std::to_string(length)}, false);
    #endif
}

/**
 * Prints a result for most benchmarks which focus on latency and data rate.
 * Results are printed as a row in a table which contains results for other configurations
 * Results are also written to the summary CSV file and the raw CSV file
 */
void print_results(Results &results, int bytes_per_element, int stim_len, int length, int max_write_per_transaction, std::string format, FILE* file, std::ofstream* raw_file)
{
    double avg_duration_ms = results.avg_duration();
    double stddev_duration_ms = results.stddev_duration();
    double avg_latency_ns = avg_duration_ms*1000000/stim_len;
    double stddev_latency_ns = stddev_duration_ms*1000000/stim_len;

    double transactions_rate_msps = stim_len/(1000.0*avg_duration_ms);

    double data_rate_mbps = 8.0*bytes_per_element*stim_len/(1000.0*avg_duration_ms);

    #if PRINT_STATS == 1
    printf(format.c_str(), data_rate_mbps);
    #endif

    #if WRITE_CSV == 1
    fprintf(file, "%f", data_rate_mbps);
    results.write_durations_and_benchmark_specific_results(*raw_file, {"", ""}, {std::to_string(length), std::to_string(max_write_per_transaction)}, false);
    #endif
}

/**
 * Prints a single result for a blocked FIFO benchmark.
 * Results across parameters not printed in a single table, each result (configuration)
 * has its own mini table.
 */
void print_results_blocked_fifo_standalone(Results &results, int elements_per_trial, int block_length, int bytes_per_element)
{
    //The actual bytes per trial length will actually be larger if the block length does not equally divide the bytes per trial.
    //Does one more block in this case which fills the remaining slots and adds a few more.
    //The number of unfilled slots without the extra block is target_stim_len%block_len.
    //The amount over the target stim length with the extra block is the block_len - unfilled slots
    int stim_len = elements_per_trial;
    if(stim_len%block_length != 0)
    {
        stim_len = stim_len + block_length - stim_len%block_length;
    }

    double avg_duration_ms = results.avg_duration();
    double stddev_duration_ms = results.stddev_duration();
    double avg_latency_ns = avg_duration_ms*1000000/stim_len;
    double stddev_latency_ns = stddev_duration_ms*1000000/stim_len;

    double transactions_rate_msps = stim_len/(1000.0*avg_duration_ms);

    double data_rate_mbps = 8.0*bytes_per_element*stim_len/(1000.0*avg_duration_ms);

    printf("        =======================================================\n");
    printf("        Metric                   |     Avg      |    StdDev    \n");
    printf("        =======================================================\n");
    //printf("        Duration (ms)            |%14.6f|%14.6f\n", stim_len, avg_duration_ms, stddev_duration_ms);
    printf("        One Way Latency (ns)     |%14.6f|%14.6f\n", avg_latency_ns, stddev_latency_ns);
    printf("        Transaction Rate (MT/s)  |%14.6f|\n", transactions_rate_msps);
    printf("        Data Rate (Mbps)         |%14.6f|\n", data_rate_mbps);
    printf("        =======================================================\n");
}

/**
 * Prints a result for the blocked FIFO benchmark
 * Results are printed as a row in a table which contains results for other configurations
 * Results are also written to the summary CSV file and the raw CSV file
 */
void print_results_blocked_fifo(Results &results, int elements_per_trial, int length, int block_length, int bytes_per_element, std::string format, FILE* file, std::ofstream* raw_file)
{
    //The actual stim length will actually be larger if the block length does not equally divide the stim length.
    //Does one more block in this case which fills the remaining slots and adds a few more.
    //The number of unfilled slots without the extra block is target_stim_len%block_len.
    //The amount over the target stim length with the extra block is the block_len - unfilled slots
    int stim_len = elements_per_trial;
    if(stim_len%block_length != 0)
    {
        stim_len = stim_len + block_length - stim_len%block_length;
    }

    double avg_duration_ms = results.avg_duration();
    double stddev_duration_ms = results.stddev_duration();
    double avg_latency_ns = avg_duration_ms*1000000/stim_len;
    double stddev_latency_ns = stddev_duration_ms*1000000/stim_len;

    double transactions_rate_msps = stim_len/(1000.0*avg_duration_ms);

    double data_rate_mbps = 8.0*bytes_per_element*stim_len/(1000.0*avg_duration_ms);

    #if PRINT_STATS == 1
    printf(format.c_str(), data_rate_mbps);
    #endif

    #if WRITE_CSV == 1
    fprintf(file, "%f", data_rate_mbps);
    results.write_durations_and_benchmark_specific_results(*raw_file, {"", ""}, {std::to_string(length), std::to_string(block_length)}, false);
    #endif
}

void print_results_fifoless_standalone(Results &results)
{
    double avg_duration_ms = results.avg_duration();
    double stddev_duration_ms = results.stddev_duration();

    printf("        =======================================================\n");
    printf("        Metric                   |     Avg      |    StdDev    \n");
    printf("        =======================================================\n");
    printf("        Duration (ms)            |%14.6e|%14.6e\n", avg_duration_ms, stddev_duration_ms);
    printf("        =======================================================\n");
}

void print_results_open_loop(Results &results, size_t array_length, int32_t block_length, int32_t balancing_nops, int initial_nops, std::string format, FILE* file, std::ofstream* raw_file)
{
    double avg_duration_ms = results.avg_duration();
    double stddev_duration_ms = results.stddev_duration();

    #if PRINT_STATS == 1
    printf(format.c_str(), array_length, block_length, balancing_nops, initial_nops, avg_duration_ms, stddev_duration_ms);
    #endif

    #if WRITE_CSV == 1
    fprintf(file, "%ld,%d,%d,%d,%f,%f\n", array_length, block_length, balancing_nops, initial_nops, avg_duration_ms, stddev_duration_ms);
    results.write_durations_and_benchmark_specific_results(*raw_file, {"", "", ""}, {std::to_string(array_length), std::to_string(block_length), std::to_string(balancing_nops), std::to_string(initial_nops)}, false);
    #endif
}

void print_results_closed_loop(Results &results, size_t array_length, int32_t block_length, int32_t server_control_period, int32_t client_control_period, int32_t control_gain, int initial_nop, std::string format, FILE* file, std::ofstream* raw_file)
{
    double avg_duration_ms = results.avg_duration();
    double stddev_duration_ms = results.stddev_duration();

    #if PRINT_STATS == 1
    printf(format.c_str(), array_length, block_length, server_control_period, client_control_period, control_gain, initial_nop, avg_duration_ms, stddev_duration_ms);
    #endif

    #if WRITE_CSV == 1
    fprintf(file, "%ld,%d,%d,%d,%d,%d,%f,%f\n", array_length, block_length, server_control_period, client_control_period, control_gain, initial_nop, avg_duration_ms, stddev_duration_ms);
    results.write_durations_and_benchmark_specific_results(*raw_file, {"", "", "", "", "", ""}, {std::to_string(array_length), std::to_string(block_length), std::to_string(server_control_period), std::to_string(client_control_period), std::to_string(control_gain), std::to_string(initial_nop)}, false);
    #endif
}
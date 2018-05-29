#ifndef _H_PRINT_RESULTS
    #define _H_PRINT_RESULTS

    #include "cpucounters.h"
    #include "results.h"
    #include "pcm_helper.h"

    #include "kernel_server_runner.h"
    #include "latency_single_kernel.h"

    void print_results(Results* results, int bytes_per_transfer, int stim_len)
    {
        double avg_duration_ms = results->avg_duration();
        double stddev_duration_ms = results->stddev_duration();
        double avg_latency_ns = avg_duration_ms*1000000/stim_len;
        double stddev_latency_ns = stddev_duration_ms*1000000/stim_len;

        double transactions_rate_msps = stim_len/(1000.0*avg_duration_ms);

        double data_rate_mbps = 8.0*bytes_per_transfer*stim_len/(1000.0*avg_duration_ms);

        printf("        =======================================================\n");
        printf("        Metric                   |     Avg      |    StdDev    \n");
        printf("        =======================================================\n");
        //printf("        Duration (ms)            |%14.6f|%14.6f\n", stim_len, avg_duration_ms, stddev_duration_ms);
        printf("        One Way Latency (ns)     |%14.6f|%14.6f\n", avg_latency_ns, stddev_latency_ns);
        printf("        Transaction Rate (MT/s)  |%14.6f|\n", transactions_rate_msps);
        printf("        Data Rate (Mbps)         |%14.6f|\n", data_rate_mbps);
        printf("        =======================================================\n");
    }

    void print_results(Results* results, int bytes_per_transfer, int stim_len, int length, std::string format, FILE* file=NULL, std::ofstream* raw_file=NULL)
    {
        double avg_duration_ms = results->avg_duration();
        double stddev_duration_ms = results->stddev_duration();
        double avg_latency_ns = avg_duration_ms*1000000/stim_len;
        double stddev_latency_ns = stddev_duration_ms*1000000/stim_len;

        double transactions_rate_msps = stim_len/(1000.0*avg_duration_ms);

        double data_rate_mbps = 8.0*bytes_per_transfer*stim_len/(1000.0*avg_duration_ms);

        #if PRINT_STATS == 1
        printf(format.c_str(), length, avg_latency_ns, stddev_latency_ns, transactions_rate_msps, data_rate_mbps);
        #endif

        #if WRITE_CSV == 1
        fprintf(file, "%d, %f, %f, %f, %f\n", length, avg_latency_ns, stddev_latency_ns, transactions_rate_msps, data_rate_mbps);
        results->write_durations(*raw_file, "", length, false);
        #endif
    }

    void print_results(Results* results, int core0, int core1, int bytes_per_transfer, int stim_len, int length, std::string format, FILE* file=NULL, std::ofstream* raw_file=NULL)
    {
        double avg_duration_ms = results->avg_duration();
        double stddev_duration_ms = results->stddev_duration();
        double avg_latency_ns = avg_duration_ms*1000000/stim_len;
        double stddev_latency_ns = stddev_duration_ms*1000000/stim_len;

        double transactions_rate_msps = stim_len/(1000.0*avg_duration_ms);

        double data_rate_mbps = 8.0*bytes_per_transfer*stim_len/(1000.0*avg_duration_ms);

        #if PRINT_STATS == 1
        printf(format.c_str(), core0, core1, length, avg_latency_ns, stddev_latency_ns, transactions_rate_msps, data_rate_mbps);
        #endif

        #if WRITE_CSV == 1
        fprintf(file, "%d, %f, %f, %f, %f\n", length, avg_latency_ns, stddev_latency_ns, transactions_rate_msps, data_rate_mbps);
        results->write_durations(*raw_file, "", length, false);
        #endif
    }

    void print_results(Results* results, int bytes_per_transfer, int stim_len, int length, int max_write_per_transaction, std::string format, FILE* file=NULL, std::ofstream* raw_file=NULL)
    {
        double avg_duration_ms = results->avg_duration();
        double stddev_duration_ms = results->stddev_duration();
        double avg_latency_ns = avg_duration_ms*1000000/stim_len;
        double stddev_latency_ns = stddev_duration_ms*1000000/stim_len;

        double transactions_rate_msps = stim_len/(1000.0*avg_duration_ms);

        double data_rate_mbps = 8.0*bytes_per_transfer*stim_len/(1000.0*avg_duration_ms);

        #if PRINT_STATS == 1
        printf(format.c_str(), data_rate_mbps);
        #endif

        #if WRITE_CSV == 1
        fprintf(file, "%f", data_rate_mbps);
        results->write_durations(*raw_file, "", length, "", max_write_per_transaction, false);
        #endif
    }

    void print_results_blocked_fifo(Results* results, int bytes_per_transfer, int target_stim_len, int block_length)
    {
        //The actual stim length will actually be larger if the block length does not equally divide the stim length.
        //Does one more block in this case which fills the remaining slots and adds a few more.
        //The number of unfilled slots without the extra block is target_stim_len%block_len.
        //The amount over the target stim length with the extra block is the block_len - unfilled slots
        int stim_len = target_stim_len;
        if(target_stim_len%block_length != 0)
        {
            stim_len = target_stim_len + block_length - target_stim_len%block_length;
        }

        double avg_duration_ms = results->avg_duration();
        double stddev_duration_ms = results->stddev_duration();
        double avg_latency_ns = avg_duration_ms*1000000/stim_len;
        double stddev_latency_ns = stddev_duration_ms*1000000/stim_len;

        double transactions_rate_msps = stim_len/(1000.0*avg_duration_ms);

        double data_rate_mbps = 8.0*bytes_per_transfer*stim_len/(1000.0*avg_duration_ms);

        printf("        =======================================================\n");
        printf("        Metric                   |     Avg      |    StdDev    \n");
        printf("        =======================================================\n");
        //printf("        Duration (ms)            |%14.6f|%14.6f\n", stim_len, avg_duration_ms, stddev_duration_ms);
        printf("        One Way Latency (ns)     |%14.6f|%14.6f\n", avg_latency_ns, stddev_latency_ns);
        printf("        Transaction Rate (MT/s)  |%14.6f|\n", transactions_rate_msps);
        printf("        Data Rate (Mbps)         |%14.6f|\n", data_rate_mbps);
        printf("        =======================================================\n");
    }

    void print_results_blocked_fifo(Results* results, int bytes_per_transfer, int target_stim_len, int length, int block_length, std::string format, FILE* file=NULL, std::ofstream* raw_file=NULL)
    {
        //The actual stim length will actually be larger if the block length does not equally divide the stim length.
        //Does one more block in this case which fills the remaining slots and adds a few more.
        //The number of unfilled slots without the extra block is target_stim_len%block_len.
        //The amount over the target stim length with the extra block is the block_len - unfilled slots
        int stim_len = target_stim_len;
        if(target_stim_len%block_length != 0)
        {
            stim_len = target_stim_len + block_length - target_stim_len%block_length;
        }

        double avg_duration_ms = results->avg_duration();
        double stddev_duration_ms = results->stddev_duration();
        double avg_latency_ns = avg_duration_ms*1000000/stim_len;
        double stddev_latency_ns = stddev_duration_ms*1000000/stim_len;

        double transactions_rate_msps = stim_len/(1000.0*avg_duration_ms);

        double data_rate_mbps = 8.0*bytes_per_transfer*stim_len/(1000.0*avg_duration_ms);

        #if PRINT_STATS == 1
        printf(format.c_str(), data_rate_mbps);
        #endif

        #if WRITE_CSV == 1
        fprintf(file, "%f", data_rate_mbps);
        results->write_durations(*raw_file, "", length, "", block_length, false);
        #endif
    }
#endif
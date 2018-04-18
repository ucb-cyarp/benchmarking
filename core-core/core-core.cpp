/*
 * Benchmark for Core-Core Data Transfer
 * 
 * Creates 2 Threads and Places them on Seperate Cores
 * 
 * Test 1: Latency (Single Element -> Both Threads Read and Write to Same Address)
 *     Two thread are run on different cores, each accessing a single 
 *     element (1 byte, 2 byte, 4 byte, 8 byte) of shared memory.
 * 
 *     The thread logic is as such:
 *         The thread starts a local timer and then enters a loop
 * 
 *         The thread has a local counter and a pointer to the shared memory location
 *         The thread is polling on the shared memory location until it is greater than its counter (should be 1 greater than its counter)
 *         When this happens, it sets its counter to the value in shared memory +1 (should be +2 from previous counter value)
 *         The thread then writes this value to the shared memory location
 *         If the new counter value is greater than a threshold, the loop terminates
 * 
 *         Note: Counter represents the number of core-core transfers (not round trip)
 * 
 *         When the loop terminates, the timer is stopped.
 *         The round trip latency is calculated as the total time / (counter / 2)
 *         The unidirectional latency is calculated as the total time / counter
 * 
 * Test 1.1: Latency (Dual Elements -> Each Thread Reads from One Address and Writes to Another, No Thread Reads/Writes to Same Address)
 *     Two thread are run on different cores, each accessing 2 
 *     elements (1 byte, 2 byte, 4 byte, 8 byte) of shared memory.
 *     Each thead is the primary owner of one of these elements.
 * 
 *     The thread logic is as such:
 *         The thread starts a local timer and then enters a loop
 * 
 *         The thread has a local counter and a pointer to its own piece of shared memory and a pointer to the other thread's piece of shared memory
 *         The thread is polling on the other thread's memory location until it is greater than its counter (should be 1 greater than its counter)
 *         When this happens, it sets its counter to the value in the other thread's shared memory +1 (should be +2 from previous counter value)
 *         The thread then writes this value to its own shared memory location
 *         If the new counter value is greater than a threshold, the loop terminates
 * 
 *         When the loop terminates, the timer is stopped.
 *         The round trip latency is calculated as the total time / (counter / 2)
 *         The unidirectional latency is calculated as the total time / counter
 * 
 * Test 2: Latency Multiple Elements
 *       Very similar to the latency test except that additional shared memory is more than a single element.
 *       When polling on the array, all elements must be greater than the counter before the counter is incremented and new values are written to shared memory.
 *       The recieving node poles the first memory location until it has the expected value.  It then begins polling the next value.  It does this until it 
 *       reaches the last address
 * 
 *       Note: This may not measure the true peak bandwidth if cache coherence does not guarentee changes are represented in order.
 * 
 *       Note: This also does not account for pipelining of computation and data transfer (ie. if 2 elements can be used while the rest are transfering in)
 *                   * One way to answer this problem may be to have a counter of number of times we need to pole for each element
 * 
 * Compiler Parameters (defined as macros)
 *     - STIM_LEN:     Stimulus Length
 *     - TRIALS:       Number of Trial Runs
 *     - PRINT_TITLE:  1 to Print Title Text
 *     - PRINT_HEADER: 1 to Print Test Header
 *     - PRINT_TRIALS: 1 to Print Each Trial
 *     - PRINT_STATS:  1 to Print Statistics
 * 
 * Arguments
 *     
 */

#include <pthread.h>
#include <stdio.h>
#include <map>
#include <unistd.h>

#include <chrono>
#include <ctime>

#include "intrin_bench_default_defines.h"

#include "cpucounters.h"
#include "results.h"
#include "pcm_helper.h"

#include "kernel_server_runner.h"

#include "latency_single_kernel.h"
#include "latency_dual_kernel.h"
#include "latency_single_array_kernel.h"
#include "latency_dual_array_kernel.h"
#include "latency_flow_ctrl_kernel.h"
#include "latency_flow_ctrl_blocked_read_kernel.h"
#include "bandwidth_circular_fifo_kernel.h"
#include "bandwidth_circular_fifo_blocked_kernel.h"

#ifndef PRINT_FULL_STATS
    #define PRINT_FULL_STATS 0
#endif

#ifndef WRITE_CSV
    #define WRITE_CSV 1
#endif

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

Results* run_latency_single_kernel(PCM* pcm, int cpu_a, int cpu_b)
{
    //=====Test 1=====
    #if PRINT_TITLE == 1
    printf("\n");
    printf("Single Memory Location\n");
    #endif

    //Initialize
    int32_t* shared_loc = new int32_t;

    //Init to 0
    *shared_loc = 0;

    LatencySingleKernelArgs* arg_a = new LatencySingleKernelArgs();
    arg_a->init_counter = -1; //(server)
    arg_a->shared_ptr = shared_loc;

    LatencySingleKernelArgs* arg_b = new LatencySingleKernelArgs();
    arg_b->init_counter = 0; //(client)
    arg_b->shared_ptr = shared_loc;

    void* reset_arg = shared_loc; //Argument for reset function is shared location


    Results* results = execute_kernel(pcm, latency_single_kernel, latency_single_kernel_reset, arg_a, arg_b, reset_arg, cpu_a, cpu_b);

    #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1
        #if USE_PCM == 1
                std::vector<int> sockets;
                int socket_a = pcm->getSocketId(cpu_a);
                int socket_b = pcm->getSocketId(cpu_b);

                sockets.push_back(socket_a);
                if(socket_b != socket_a)
                {
                    sockets.push_back(socket_b);
                }

                std::vector<int> cores;
                cores.push_back(cpu_a);
                cores.push_back(cpu_b);
        
                #if PRINT_FULL_STATS == 1
                    results->print_statistics(sockets, cores, STIM_LEN);
                #endif

                #if PRINT_STATS == 1
                print_results(results, sizeof(*shared_loc), STIM_LEN);
                #endif

        #else
                #if PRINT_FULL_STATS
                results->print_statistics(0, cpu_a, STIM_LEN);
                #endif

                print_results(results, sizeof(*shared_loc), STIM_LEN);
        #endif
    #endif

    //Clean Up
    delete shared_loc;
    delete arg_a;
    delete arg_b;

    return results;
}

Results* run_latency_dual_kernel(PCM* pcm, int cpu_a, int cpu_b)
{
#if PRINT_TITLE == 1
    printf("\n");
    printf("Dual Memory Locations\n");
    #endif

    //Initialize
    int32_t* shared_loc_a = new int32_t;
    int32_t* shared_loc_b = new int32_t;

    //Init to 0
    *shared_loc_a = 0;
    *shared_loc_b = 0;

    LatencyDualKernelArgs* arg_a = new LatencyDualKernelArgs();
    arg_a->init_counter = -1; //(server)
    arg_a->my_shared_ptr = shared_loc_a;
    arg_a->other_shared_ptr = shared_loc_b;

    LatencyDualKernelArgs* arg_b = new LatencyDualKernelArgs();
    arg_b->init_counter = 0; //(client)
    arg_b->my_shared_ptr = shared_loc_b; //Swapped from above
    arg_b->other_shared_ptr = shared_loc_a;

    LatencyDualKernelResetArgs* reset_arg = new LatencyDualKernelResetArgs();
    reset_arg->shared_ptr_a = shared_loc_a;
    reset_arg->shared_ptr_b = shared_loc_b;

    Results* results = execute_kernel(pcm, latency_dual_kernel, latency_dual_kernel_reset, arg_a, arg_b, reset_arg, cpu_a, cpu_b);

    #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1
        #if USE_PCM == 1
                std::vector<int> sockets;
                int socket_a = pcm->getSocketId(cpu_a);
                int socket_b = pcm->getSocketId(cpu_b);

                sockets.push_back(socket_a);
                if(socket_b != socket_a)
                {
                    sockets.push_back(socket_b);
                }

                std::vector<int> cores;
                cores.push_back(cpu_a);
                cores.push_back(cpu_b);
        
                #if PRINT_FULL_STATS == 1
                    results->print_statistics(sockets, cores, STIM_LEN);
                #endif

                #if PRINT_STATS == 1
                print_results(results, sizeof(*shared_loc_a), STIM_LEN);
                #endif

        #else
                #if PRINT_FULL_STATS
                results->print_statistics(0, cpu_a, STIM_LEN);
                #endif

                print_results(results, sizeof(*shared_loc_a), STIM_LEN);
        #endif
    #endif

    //Clean Up
    delete shared_loc_a;
    delete shared_loc_b;
    delete arg_a;
    delete arg_b;
    delete reset_arg;

    return results;
}

Results* run_latency_single_array_kernel(PCM* pcm, int cpu_a, int cpu_b, size_t array_length, bool report_standalone=true, std::string format = "", FILE* file=NULL, std::ofstream* raw_file=NULL)
{
    //=====Test 2=====
    #if PRINT_TITLE == 1
    if(report_standalone)
    {
        printf("\n");
        printf("Single Memory Location - Array\n");
        printf("Array Length: %lu int32_t Elements\n", array_length);
    }
    #endif

    //Initialize
    int32_t* shared_loc = new int32_t[array_length];

    //Init to 0
    for(size_t i = 0; i < array_length; i++)
    {
        shared_loc[i] = 0;
    }

    LatencySingleArrayKernelArgs* arg_a = new LatencySingleArrayKernelArgs();
    arg_a->init_counter = -1; //(server)
    arg_a->shared_ptr = shared_loc;
    arg_a->length = array_length;

    LatencySingleArrayKernelArgs* arg_b = new LatencySingleArrayKernelArgs();
    arg_b->init_counter = 0; //(client)
    arg_b->shared_ptr = shared_loc;
    arg_b->length = array_length;

    LatencySingleArrayKernelResetArgs* reset_arg = new LatencySingleArrayKernelResetArgs();
    reset_arg->shared_ptr = shared_loc;
    reset_arg->length = array_length;

    Results* results = execute_kernel(pcm, latency_single_array_kernel, latency_single_array_kernel_reset, arg_a, arg_b, reset_arg, cpu_a, cpu_b);

    #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1 || WRITE_CSV == 1
        if(report_standalone)
        {
            #if USE_PCM == 1
                    std::vector<int> sockets;
                    int socket_a = pcm->getSocketId(cpu_a);
                    int socket_b = pcm->getSocketId(cpu_b);

                    sockets.push_back(socket_a);
                    if(socket_b != socket_a)
                    {
                        sockets.push_back(socket_b);
                    }

                    std::vector<int> cores;
                    cores.push_back(cpu_a);
                    cores.push_back(cpu_b);
            
                    #if PRINT_FULL_STATS == 1
                        results->print_statistics(sockets, cores, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                    print_results(results, sizeof(*shared_loc)*array_length, STIM_LEN);
                    #endif

            #else
                    #if PRINT_FULL_STATS
                    results->print_statistics(0, cpu_a, STIM_LEN);
                    #endif

                    print_results(results, sizeof(*shared_loc)*array_length, STIM_LEN);
            #endif
        }
        else
        {
            print_results(results, sizeof(*shared_loc)*array_length, STIM_LEN, array_length, format, file, raw_file);
        }
    #endif

    //Clean Up
    delete[] shared_loc;
    delete arg_a;
    delete arg_b;
    delete reset_arg;

    return results;
}

void run_latency_single_array_kernel(PCM* pcm, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, FILE* file=NULL, std::ofstream* raw_file=NULL)
{
    //Print header
    printf("Single Memory Location - Array\n");
    printf("        ==========================================================================================\n");
    printf("          Transfer Length  |   One Way Latency (ns)   | Transaction Rate (MT/s) | Data Rate (Mbps)\n");
    printf("        (int32_t Elements) |       Avg, StdDev        |                         |                 \n");
    printf("        ==========================================================================================\n");

    #if WRITE_CSV == 1
    fprintf(file, "\"Transfer Length (int32_t Elements)\", \"One Way Latency (ns) - Avg\", \"One Way Latency (ns) - StdDev\", \"Transaction Rate (MT/s)\", \"Data Rate (Mbps)\"\n");
    fflush(file);
    *raw_file << "\"Transfer Length (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
    #endif

    std::string format = "        %18d | %11.6f, %11.6f | %23.6f | %15.6f \n";

    for(int i = 0; i<array_lengths.size(); i++)
    {
        size_t array_length = array_lengths[i];
        Results* latency_single_array_kernel_results = run_latency_single_array_kernel(pcm, cpu_a, cpu_b, array_length, false, format, file, raw_file);

        #if WRITE_CSV == 1
        fflush(file);
        #endif

        //Cleanup
        latency_single_array_kernel_results->delete_results();
        delete latency_single_array_kernel_results;
    }

    printf("        ==========================================================================================\n");
}

Results* run_latency_dual_array_kernel(PCM* pcm, int cpu_a, int cpu_b, size_t array_length, bool report_standalone=true, std::string format = "", FILE* file = NULL, std::ofstream* raw_file=NULL)
{
    //=====Test 2=====
    #if PRINT_TITLE == 1
    if(report_standalone)
    {
        printf("\n");
        printf("Dual Memory Location - Array\n");
        printf("Array Length: %lu int32_t Elements\n", array_length);
    }
    #endif

    //Initialize
    int32_t* shared_loc_a = new int32_t[array_length];
    int32_t* shared_loc_b = new int32_t[array_length];

    //Init to 0
    for(size_t i = 0; i < array_length; i++)
    {
        shared_loc_a[i] = 0;
        shared_loc_b[i] = 0;
    }

    LatencyDualArrayKernelArgs* arg_a = new LatencyDualArrayKernelArgs();
    arg_a->init_counter = -1; //(server)
    arg_a->my_shared_ptr = shared_loc_a;
    arg_a->other_shared_ptr = shared_loc_b;
    arg_a->length = array_length;

    LatencyDualArrayKernelArgs* arg_b = new LatencyDualArrayKernelArgs();
    arg_b->init_counter = 0; //(client)
    arg_b->my_shared_ptr = shared_loc_b; //Swapped from server
    arg_b->other_shared_ptr = shared_loc_a;
    arg_b->length = array_length;

    LatencyDualArrayKernelResetArgs* reset_arg = new LatencyDualArrayKernelResetArgs();
    reset_arg->shared_ptr_a = shared_loc_a;
    reset_arg->shared_ptr_b = shared_loc_b;
    reset_arg->length = array_length;

    Results* results = execute_kernel(pcm, latency_dual_array_kernel, latency_dual_array_kernel_reset, arg_a, arg_b, reset_arg, cpu_a, cpu_b);

    #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1 || WRITE_CSV == 1
        if(report_standalone)
        {
            #if USE_PCM == 1
                    std::vector<int> sockets;
                    int socket_a = pcm->getSocketId(cpu_a);
                    int socket_b = pcm->getSocketId(cpu_b);

                    sockets.push_back(socket_a);
                    if(socket_b != socket_a)
                    {
                        sockets.push_back(socket_b);
                    }

                    std::vector<int> cores;
                    cores.push_back(cpu_a);
                    cores.push_back(cpu_b);
            
                    #if PRINT_FULL_STATS == 1
                        results->print_statistics(sockets, cores, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                    print_results(results, sizeof(*shared_loc_a)*array_length, STIM_LEN);
                    #endif

            #else
                    #if PRINT_FULL_STATS
                    results->print_statistics(0, cpu_a, STIM_LEN);
                    #endif

                    print_results(results, sizeof(*shared_loc)*array_length, STIM_LEN);
            #endif
        }
        else
        {
            print_results(results, sizeof(*shared_loc_a)*array_length, STIM_LEN, array_length, format, file, raw_file);
        }
    #endif

    //Clean Up
    delete[] shared_loc_a;
    delete[] shared_loc_b;
    delete arg_a;
    delete arg_b;
    delete reset_arg;

    return results;
}

void run_latency_dual_array_kernel(PCM* pcm, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, FILE* file = NULL, std::ofstream* raw_file=NULL)
{
    //Print header
    printf("Dual Memory Location - Array\n");
    printf("        ==========================================================================================\n");
    printf("          Transfer Length  |   One Way Latency (ns)   | Transaction Rate (MT/s) | Data Rate (Mbps)\n");
    printf("        (int32_t Elements) |       Avg, StdDev        |                         |                 \n");
    printf("        ==========================================================================================\n");

    #if WRITE_CSV == 1
    fprintf(file, "\"Transfer Length (int32_t Elements)\", \"One Way Latency (ns) - Avg\", \"One Way Latency (ns) - StdDev\", \"Transaction Rate (MT/s)\", \"Data Rate (Mbps)\"\n");
    fflush(file);
    *raw_file << "\"Transfer Length (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
    #endif

    std::string format = "        %18d | %11.6f, %11.6f | %23.6f | %15.6f \n";

    for(int i = 0; i<array_lengths.size(); i++)
    {
        size_t array_length = array_lengths[i];
        Results* latency_dual_array_kernel_results = run_latency_dual_array_kernel(pcm, cpu_a, cpu_b, array_length, false, format, file, raw_file);

        #if WRITE_CSV == 1
        fflush(file);
        #endif

        //Cleanup
        latency_dual_array_kernel_results->delete_results();
        delete latency_dual_array_kernel_results;
    }

    printf("        ==========================================================================================\n");
}

Results* run_latency_flow_ctrl_kernel(PCM* pcm, int cpu_a, int cpu_b, size_t array_length, bool report_standalone=true, std::string format = "", FILE* file=NULL, std::ofstream* raw_file=NULL)
{
    #if PRINT_TITLE == 1
    if(report_standalone)
    {
        printf("\n");
        printf("Flow Control - Array\n");
        printf("Array Length: %lu int32_t Elements\n", array_length);
    }
    #endif

    //Initialize
    int32_t* shared_array_loc = new int32_t[array_length];
    int32_t* shared_ack_loc = new int32_t;

    //Init to 0
    for(size_t i = 0; i < array_length; i++)
    {
        shared_array_loc[i] = 0;
    }

    LatencyFlowCtrlKernelArgs* args = new LatencyFlowCtrlKernelArgs();
    args->array_shared_ptr = shared_array_loc;
    args->ack_shared_ptr = shared_ack_loc;
    args->length = array_length;

    Results* results = execute_client_server_kernel(pcm, latency_flow_ctrl_server_kernel, latency_flow_ctrl_client_kernel, latency_flow_ctrl_kernel_reset, args, args, args, cpu_a, cpu_b);

    #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1 || WRITE_CSV == 1
        if(report_standalone)
        {
            #if USE_PCM == 1
                    std::vector<int> sockets;
                    int socket_a = pcm->getSocketId(cpu_a);
                    int socket_b = pcm->getSocketId(cpu_b);

                    sockets.push_back(socket_a);
                    if(socket_b != socket_a)
                    {
                        sockets.push_back(socket_b);
                    }

                    std::vector<int> cores;
                    cores.push_back(cpu_a);
                    cores.push_back(cpu_b);
            
                    #if PRINT_FULL_STATS == 1
                        results->print_statistics(sockets, cores, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                    print_results(results, sizeof(*shared_array_loc)*array_length, STIM_LEN/2); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                    #endif

            #else
                    #if PRINT_FULL_STATS
                    results->print_statistics(0, cpu_a, STIM_LEN);
                    #endif

                    print_results(results, sizeof(*shared_loc)*array_length, STIM_LEN/2); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
            #endif
        }
        else
        {
            print_results(results, sizeof(*shared_array_loc)*array_length, STIM_LEN/2, array_length, format, file, raw_file); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
        }
    #endif

    //Clean Up
    delete[] shared_array_loc;
    delete shared_ack_loc;
    delete args;

    return results;
}

void run_latency_flow_ctrl_kernel(PCM* pcm, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, FILE* file = NULL, std::ofstream* raw_file=NULL)
{
    //Print header
    printf("Flow Control - Array\n");
    printf("        ==========================================================================================\n");
    printf("          Transfer Length  |  Round Trip Latency (ns) | Transaction Rate (MT/s) | Data Rate (Mbps)\n");
    printf("        (int32_t Elements) |       Avg, StdDev        |                         |                 \n");
    printf("        ==========================================================================================\n");

    #if WRITE_CSV == 1
    fprintf(file, "\"Transfer Length (int32_t Elements)\", \"Round Trip Latency (ns) - Avg\", \"Round Trip Latency (ns) - StdDev\", \"Transaction Rate (MT/s)\", \"Data Rate (Mbps)\"\n");
    fflush(file);
    *raw_file << "\"Transfer Length (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
    #endif

    std::string format = "        %18d | %11.6f, %11.6f | %23.6f | %15.6f \n";

    for(int i = 0; i<array_lengths.size(); i++)
    {
        size_t array_length = array_lengths[i];
        Results* latency_fifo_kernel_results = run_latency_flow_ctrl_kernel(pcm, cpu_a, cpu_b, array_length, false, format, file, raw_file);

        #if WRITE_CSV == 1
        fflush(file);
        #endif

        //Cleanup
        latency_fifo_kernel_results->delete_results();
        delete latency_fifo_kernel_results;
    }

    printf("        ==========================================================================================\n");
}

Results* run_latency_flow_ctrl_blocked_read_kernel(PCM* pcm, int cpu_a, int cpu_b, size_t array_length, bool report_standalone=true, std::string format = "", FILE* file=NULL, std::ofstream* raw_file=NULL)
{
    #if PRINT_TITLE == 1
    if(report_standalone)
    {
        printf("\n");
        printf("Flow Control Blocked Read - Array\n");
        printf("Array Length: %lu int32_t Elements\n", array_length);
    }
    #endif

    //Initialize
    int32_t* shared_array_loc = new int32_t[array_length];
    int32_t* shared_ack_loc = new int32_t;
    int32_t* shared_valid_loc = new int32_t;

    //Init to 0
    for(size_t i = 0; i < array_length; i++)
    {
        shared_array_loc[i] = 0;
    }

    LatencyFlowCtrlBlockedReadKernelArgs* args = new LatencyFlowCtrlBlockedReadKernelArgs();
    args->array_shared_ptr = shared_array_loc;
    args->ack_shared_ptr = shared_ack_loc;
    args->valid_shared_ptr = shared_valid_loc;
    args->length = array_length;

    Results* results = execute_client_server_kernel(pcm, latency_flow_ctrl_blocked_read_server_kernel, latency_flow_ctrl_blocked_read_client_kernel, latency_flow_ctrl_blocked_read_kernel_reset, args, args, args, cpu_a, cpu_b);

    #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1 || WRITE_CSV == 1
        if(report_standalone)
        {
            #if USE_PCM == 1
                    std::vector<int> sockets;
                    int socket_a = pcm->getSocketId(cpu_a);
                    int socket_b = pcm->getSocketId(cpu_b);

                    sockets.push_back(socket_a);
                    if(socket_b != socket_a)
                    {
                        sockets.push_back(socket_b);
                    }

                    std::vector<int> cores;
                    cores.push_back(cpu_a);
                    cores.push_back(cpu_b);
            
                    #if PRINT_FULL_STATS == 1
                        results->print_statistics(sockets, cores, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                    print_results(results, sizeof(*shared_array_loc)*array_length, STIM_LEN/2); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                    #endif

            #else
                    #if PRINT_FULL_STATS
                    results->print_statistics(0, cpu_a, STIM_LEN);
                    #endif

                    print_results(results, sizeof(*shared_loc)*array_length, STIM_LEN/2); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
            #endif
        }
        else
        {
            print_results(results, sizeof(*shared_array_loc)*array_length, STIM_LEN/2, array_length, format, file, raw_file); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
        }
    #endif

    //Clean Up
    delete[] shared_array_loc;
    delete shared_ack_loc;
    delete shared_valid_loc;
    delete args;

    return results;
}

void run_latency_flow_ctrl_blocked_read_kernel(PCM* pcm, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, FILE* file = NULL, std::ofstream* raw_file=NULL)
{
    //Print header
    printf("Flow Control Blocked Read - Array\n");
    printf("        ==========================================================================================\n");
    printf("          Transfer Length  |  Round Trip Latency (ns) | Transaction Rate (MT/s) | Data Rate (Mbps)\n");
    printf("        (int32_t Elements) |       Avg, StdDev        |                         |                 \n");
    printf("        ==========================================================================================\n");

    #if WRITE_CSV == 1
    fprintf(file, "\"Transfer Length (int32_t Elements)\", \"Round Trip Latency (ns) - Avg\", \"Round Trip Latency (ns) - StdDev\", \"Transaction Rate (MT/s)\", \"Data Rate (Mbps)\"\n");
    fflush(file);
    *raw_file << "\"Transfer Length (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
    #endif

    std::string format = "        %18d | %11.6f, %11.6f | %23.6f | %15.6f \n";

    for(int i = 0; i<array_lengths.size(); i++)
    {
        size_t array_length = array_lengths[i];
        Results* latency_fifo_kernel_results = run_latency_flow_ctrl_blocked_read_kernel(pcm, cpu_a, cpu_b, array_length, false, format, file, raw_file);

        #if WRITE_CSV == 1
        fflush(file);
        #endif

        //Cleanup
        latency_fifo_kernel_results->delete_results();
        delete latency_fifo_kernel_results;
    }

    printf("        ==========================================================================================\n");
}

Results* run_bandwidth_fifo_kernel(PCM* pcm, int cpu_a, int cpu_b, size_t array_length, int32_t max_write_per_transaction, bool report_standalone=true, std::string format = "", FILE* file=NULL, std::ofstream* raw_file=NULL)
{
    #if PRINT_TITLE == 1
    if(report_standalone)
    {
        printf("\n");
        printf("FIFO - Array\n");
        printf("Array Length: %lu int32_t Elements, Max Write Per Transaction: %d\n", array_length, max_write_per_transaction);
    }
    #endif

    //Initialize
    int32_t* shared_array_loc = new int32_t[array_length];
    int32_t* shared_write_id_loc = new int32_t;
    int32_t* shared_read_id_loc = new int32_t;

    //Init to 0
    for(size_t i = 0; i < array_length; i++)
    {
        shared_array_loc[i] = 0;
    }

    BandwidthCircularFifoKernelArgs* args = new BandwidthCircularFifoKernelArgs();
    args->array_shared_ptr = shared_array_loc;
    args->write_pos_shared_ptr = shared_write_id_loc;
    args->read_pos_shared_ptr = shared_read_id_loc;
    args->length = array_length;
    args->max_write_per_transaction = max_write_per_transaction;

    Results* results = execute_client_server_kernel(pcm, bandwidth_circular_fifo_server_kernel, bandwidth_circular_fifo_client_kernel, bandwidth_circular_fifo_kernel_reset, args, args, args, cpu_a, cpu_b);

    #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1 || WRITE_CSV == 1
        if(report_standalone)
        {
            #if USE_PCM == 1
                    std::vector<int> sockets;
                    int socket_a = pcm->getSocketId(cpu_a);
                    int socket_b = pcm->getSocketId(cpu_b);

                    sockets.push_back(socket_a);
                    if(socket_b != socket_a)
                    {
                        sockets.push_back(socket_b);
                    }

                    std::vector<int> cores;
                    cores.push_back(cpu_a);
                    cores.push_back(cpu_b);
            
                    #if PRINT_FULL_STATS == 1
                        results->print_statistics(sockets, cores, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                    print_results(results, sizeof(*shared_array_loc), STIM_LEN);
                    #endif

            #else
                    #if PRINT_FULL_STATS
                    results->print_statistics(0, cpu_a, STIM_LEN);
                    #endif

                    print_results(results, sizeof(*shared_loc), STIM_LEN); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
            #endif
        }
        else
        {
            print_results(results, sizeof(*shared_array_loc), STIM_LEN, array_length, max_write_per_transaction, format, file, raw_file); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
        }
    #endif

    //Clean Up
    delete[] shared_array_loc;
    delete shared_write_id_loc;
    delete shared_read_id_loc;
    delete args;

    return results;
}

//MAKE A 2D Table
void run_bandwidth_fifo_kernel(PCM* pcm, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, std::vector<int32_t> max_writes_per_transaction, FILE* file = NULL, std::ofstream* raw_file=NULL)
{
    int32_t data_col_width = 10;

    //Print header
    printf("FIFO - Array\n");
    printf("        Lengths in int32_t Elements, Data Rates in Mbps\n");
    printf("        ===========================");
    for(int i = 0; i<max_writes_per_transaction.size(); i++)
    {
        for(int j = 0; j<data_col_width; j++)
        {
            printf("=");
        }
    }
    printf("\n");
    printf("        Array Len \\ Max Trans. Len ");
    for(int i = 0; i<max_writes_per_transaction.size(); i++)
    {
        printf("|%9.2d", max_writes_per_transaction[i]); //12 becaused of spaces
    }
    printf("\n");
    printf("        ===========================");
    for(int i = 0; i<max_writes_per_transaction.size(); i++)
    {
        for(int j = 0; j<data_col_width; j++) 
        {
            printf("=");
        }
    }

    #if WRITE_CSV == 1
    fprintf(file, "\"Array Len \\ Max Trans. Len (int32_t elements)\"");//Command inserted below
    for(int i = 0; i<max_writes_per_transaction.size(); i++)
    {
        fprintf(file, ",%d", max_writes_per_transaction[i]);
    }
    //fprintf(file, "\n"); //Done below
    fflush(file);
    *raw_file << "\"Transfer Length (int32_t Elements)\",\"Max Writes Per Transaction (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
    #endif

    std::string format = "|%9.2f";

    for(int i = 0; i<array_lengths.size(); i++)
    {
        size_t array_length = array_lengths[i];

        //Print the newlinem indent and new array length
        printf("\n        %27lu", array_length);
        #if WRITE_CSV == 1
        fprintf(file, "\n%lu", array_length);
        fflush(file);
        #endif

        for(int j = 0; j<max_writes_per_transaction.size(); j++)
        {
            int32_t max_writes = max_writes_per_transaction[j];

            if(max_writes <= array_length) //Check if we should bother running this case
            {
                #if WRITE_CSV == 1
                fprintf(file, ",");
                fflush(file);
                #endif

                Results* latency_fifo_kernel_results = run_bandwidth_fifo_kernel(pcm, cpu_a, cpu_b, array_length, max_writes, false, format, file, raw_file);

                //Cleanup
                latency_fifo_kernel_results->delete_results();
                delete latency_fifo_kernel_results;
            }
            else
            {
                //We did not run this test case
                #if WRITE_CSV == 1
                fprintf(file, ",%d", 0);
                fflush(file);
                #endif

                printf(format.c_str(), 0);
            }
        }
    }

    //Print the newline
    #if WRITE_CSV == 1
    fprintf(file, "\n");
    fflush(file);
    #endif

    printf("\n        ===========================");
    for(int i = 0; i<max_writes_per_transaction.size(); i++)
    {
        for(int j = 0; j<data_col_width; j++)
        {
            printf("=");
        }
    }
    printf("\n");
}

Results* run_bandwidth_fifo_blocked_kernel(PCM* pcm, int cpu_a, int cpu_b, size_t array_length, int32_t block_length, bool report_standalone=true, std::string format = "", FILE* file=NULL, std::ofstream* raw_file=NULL)
{
    #if PRINT_TITLE == 1
    if(report_standalone)
    {
        printf("\n");
        printf("FIFO - Array - Blocked Transfers\n");
        printf("Array Length: %lu int32_t Elements, Reads/Write Per Transaction: %d\n", array_length, block_length);
    }
    #endif

    //Initialize
    int32_t* shared_array_loc = new int32_t[array_length];
    int32_t* shared_write_id_loc = new int32_t;
    int32_t* shared_read_id_loc = new int32_t;

    //Init to 0
    for(size_t i = 0; i < array_length; i++)
    {
        shared_array_loc[i] = 0;
    }

    BandwidthCircularFifoBlockedKernelArgs* args = new BandwidthCircularFifoBlockedKernelArgs();
    args->array_shared_ptr = shared_array_loc;
    args->write_pos_shared_ptr = shared_write_id_loc;
    args->read_pos_shared_ptr = shared_read_id_loc;
    args->length = array_length;
    args->block_length = block_length;

    Results* results = execute_client_server_kernel(pcm, bandwidth_circular_fifo_blocked_server_kernel, bandwidth_circular_fifo_blocked_client_kernel, bandwidth_circular_fifo_blocked_kernel_reset, args, args, args, cpu_a, cpu_b);

    #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1 || WRITE_CSV == 1
        if(report_standalone)
        {
            #if USE_PCM == 1
                    std::vector<int> sockets;
                    int socket_a = pcm->getSocketId(cpu_a);
                    int socket_b = pcm->getSocketId(cpu_b);

                    sockets.push_back(socket_a);
                    if(socket_b != socket_a)
                    {
                        sockets.push_back(socket_b);
                    }

                    std::vector<int> cores;
                    cores.push_back(cpu_a);
                    cores.push_back(cpu_b);
            
                    #if PRINT_FULL_STATS == 1
                        results->print_statistics(sockets, cores, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                    print_results(results, sizeof(*shared_array_loc), STIM_LEN);
                    #endif

            #else
                    #if PRINT_FULL_STATS
                    results->print_statistics(0, cpu_a, STIM_LEN);
                    #endif

                    print_results(results, sizeof(*shared_array_loc), STIM_LEN); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
            #endif
        }
        else
        {
            print_results(results, sizeof(*shared_array_loc), STIM_LEN, array_length, block_length, format, file, raw_file); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
        }
    #endif

    //Clean Up
    delete[] shared_array_loc;
    delete shared_write_id_loc;
    delete shared_read_id_loc;
    delete args;

    return results;
}

//MAKE A 2D Table
void run_bandwidth_fifo_blocked_kernel(PCM* pcm, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, FILE* file = NULL, std::ofstream* raw_file=NULL)
{
    int32_t data_col_width = 10;

    //Print header
    printf("FIFO - Array\n");
    printf("        Lengths in int32_t Elements, Data Rates in Mbps\n");
    printf("        ===========================");
    for(int i = 0; i<block_lengths.size(); i++)
    {
        for(int j = 0; j<data_col_width; j++)
        {
            printf("=");
        }
    }
    printf("\n");
    printf("        Array Len \\ Max Trans. Len ");
    for(int i = 0; i<block_lengths.size(); i++)
    {
        printf("|%9.2d", block_lengths[i]); //12 becaused of spaces
    }
    printf("\n");
    printf("        ===========================");
    for(int i = 0; i<block_lengths.size(); i++)
    {
        for(int j = 0; j<data_col_width; j++) 
        {
            printf("=");
        }
    }

    #if WRITE_CSV == 1
    fprintf(file, "\"Array Len \\ Max Trans. Len (int32_t elements)\"");//Command inserted below
    for(int i = 0; i<block_lengths.size(); i++)
    {
        fprintf(file, ",%d", block_lengths[i]);
    }
    //fprintf(file, "\n"); //Done below
    fflush(file);
    *raw_file << "\"Transfer Length (int32_t Elements)\",\"Reads/Writes Per Transaction (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
    #endif

    std::string format = "|%9.2f";

    for(int i = 0; i<array_lengths.size(); i++)
    {
        size_t array_length = array_lengths[i];

        //Print the newlinem indent and new array length
        printf("\n        %27lu", array_length);
        #if WRITE_CSV == 1
        fprintf(file, "\n%lu", array_length);
        fflush(file);
        #endif

        for(int j = 0; j<block_lengths.size(); j++)
        {
            int32_t block_length = block_lengths[j];

            if(block_length <= array_length) //Check if we should bother running this case
            {
                #if WRITE_CSV == 1
                fprintf(file, ",");
                fflush(file);
                #endif

                Results* latency_fifo_kernel_results = run_bandwidth_fifo_kernel(pcm, cpu_a, cpu_b, array_length, block_length, false, format, file, raw_file);

                //Cleanup
                latency_fifo_kernel_results->delete_results();
                delete latency_fifo_kernel_results;
            }
            else
            {
                //We did not run this test case
                #if WRITE_CSV == 1
                fprintf(file, ",%d", 0);
                fflush(file);
                #endif

                printf(format.c_str(), 0);
            }
        }
    }

    //Print the newline
    #if WRITE_CSV == 1
    fprintf(file, "\n");
    fflush(file);
    #endif

    printf("\n        ===========================");
    for(int i = 0; i<array_lengths.size(); i++)
    {
        for(int j = 0; j<data_col_width; j++)
        {
            printf("=");
        }
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    //Run these single-threaded benchmarks on CPU 0 (all machines should have CPU 0)

    //Based off of http://man7.org/linux/man-pages/man3/pthread_setaffinity_np.3.html
    //http://man7.org/linux/man-pages/man3/pthread_create.3.html
    //http://man7.org/linux/man-pages/man3/pthread_attr_setaffinity_np.3.html,
    //http://man7.org/linux/man-pages/man3/pthread_join.3.html

    if(argc < 3)
    {
        printf("core-core core-a core-b\n    core-a: Processor ID for first processor core\n    core-b: Processor ID for second processor core\n");
        exit(1);
    }
    std::string cpu_a_str(argv[1]);
    int cpu_a = std::stoi(cpu_a_str);

    std::string cpu_b_str(argv[2]);
    int cpu_b = std::stoi(cpu_b_str);

    #if PRINT_TITLE == 1
        printf("Core-Core Communication Test: From CPU%d to CPU%d\n", cpu_a, cpu_b);
        printf("STIM_LEN: %d (Samples/Vector/Trial), TRIALS: %d\n", STIM_LEN, TRIALS);
    #endif

    #if USE_PCM == 1
        bool print_topology = false;
        #if PRINT_TITLE == 1
        print_topology = true;
        #endif

        #if PRINT_TITLE == 1
        printf("\n");
        printf("****** Platform Information Provided by PCM ******\n");
        #endif

        PCM* pcm = init_PCM(print_topology);

        #if PRINT_TITLE == 1
        printf("**************************************************\n");
        printf("CPU Brand String: %s\n", pcm->getCPUBrandString().c_str());
        printf("**************************************************\n");
        #endif

        #if PRINT_TITLE == 1

        printf("\n");
        int cpu_a_socket = pcm->getSocketId(cpu_a);
        int cpu_a_core = pcm->getCoreId(cpu_a);
        int cpu_a_tile = pcm->getTileId(cpu_a);
        printf("CPU A = Logical CPU#: %d, Socket #: %d, Physical Core #: %d, L2 Tile #: %d\n", cpu_a, cpu_a_socket, cpu_a_core, cpu_a_tile);
        int cpu_b_socket = pcm->getSocketId(cpu_b);
        int cpu_b_core = pcm->getCoreId(cpu_b);
        int cpu_b_tile = pcm->getTileId(cpu_b);
        printf("CPU B = Logical CPU#: %d, Socket #: %d, Physical Core #: %d, L2 Tile #: %d\n", cpu_b, cpu_b_socket, cpu_b_core, cpu_b_tile);
        #endif

    #endif

    //=====Test 1=====
    Results* latency_single_kernel_results = run_latency_single_kernel(pcm, cpu_a, cpu_b);
    latency_single_kernel_results->delete_results();
    delete latency_single_kernel_results;

    //=====Test 1.1=====
    Results* latency_dual_kernel_results = run_latency_dual_kernel(pcm, cpu_a, cpu_b);
    latency_dual_kernel_results->delete_results();
    delete latency_dual_kernel_results;

    //=====Test 2=====
    //std::vector<size_t> array_sizes = {1, 2, 4, 8, 16, 32, 64};
    std::vector<size_t> array_sizes;
    size_t start = 1;
    size_t stop = 129;
    // size_t stop = 65;

    for(int i = start; i < stop; i++)
    {
        array_sizes.push_back(i);
    }

    printf("\n");

    FILE* single_array_csv_file = NULL;
    std::ofstream single_array_raw_csv_file;
    #if WRITE_CSV == 1
    single_array_csv_file = fopen("report_single_array.csv", "w");
    single_array_raw_csv_file.open("report_single_array_raw.csv", std::ofstream::out);
    #endif

    run_latency_single_array_kernel(pcm, cpu_a, cpu_b, array_sizes, single_array_csv_file, &single_array_raw_csv_file);
    
    fclose(single_array_csv_file);
    single_array_raw_csv_file.close();

    printf("\n");

    FILE* dual_array_csv_file = NULL;
    std::ofstream dual_array_raw_csv_file;
    #if WRITE_CSV == 1
    dual_array_csv_file = fopen("report_dual_array.csv", "w");
    dual_array_raw_csv_file.open("report_dual_array_raw.csv", std::ofstream::out);
    #endif

    run_latency_dual_array_kernel(pcm, cpu_a, cpu_b, array_sizes, dual_array_csv_file, &dual_array_raw_csv_file);

    fclose(dual_array_csv_file);
    dual_array_raw_csv_file.close();

    printf("\n");

    FILE* flow_ctrl_array_csv_file = NULL;
    std::ofstream flow_ctrl_array_raw_csv_file;
    #if WRITE_CSV == 1
    flow_ctrl_array_csv_file = fopen("report_flow_ctrl_array.csv", "w");
    flow_ctrl_array_raw_csv_file.open("report_flow_ctrl_array_raw.csv", std::ofstream::out);
    #endif

    run_latency_flow_ctrl_kernel(pcm, cpu_a, cpu_b, array_sizes, flow_ctrl_array_csv_file, &flow_ctrl_array_raw_csv_file);

    fclose(flow_ctrl_array_csv_file);
    flow_ctrl_array_raw_csv_file.close();

    printf("\n");

    FILE* flow_ctrl_blocked_read_array_csv_file = NULL;
    std::ofstream flow_ctrl_blocked_read_array_raw_csv_file;
    #if WRITE_CSV == 1
    flow_ctrl_blocked_read_array_csv_file = fopen("report_flow_ctrl_blocked_read_array.csv", "w");
    flow_ctrl_blocked_read_array_raw_csv_file.open("report_flow_ctrl_blocked_read_array_raw.csv", std::ofstream::out);
    #endif

    run_latency_flow_ctrl_blocked_read_kernel(pcm, cpu_a, cpu_b, array_sizes, flow_ctrl_blocked_read_array_csv_file, &flow_ctrl_blocked_read_array_raw_csv_file);

    fclose(flow_ctrl_blocked_read_array_csv_file);
    flow_ctrl_blocked_read_array_raw_csv_file.close();

    printf("\n");

    std::vector<int32_t> transaction_sizes;
    for(int i = start; i < stop; i++)
    {
        transaction_sizes.push_back(i);
    }

    FILE* fifo_array_csv_file = NULL;
    std::ofstream fifo_array_raw_csv_file;
    #if WRITE_CSV == 1
    fifo_array_csv_file = fopen("report_fifo_array.csv", "w");
    fifo_array_raw_csv_file.open("report_fifo_array_raw.csv", std::ofstream::out);
    #endif

    run_bandwidth_fifo_kernel(pcm, cpu_a, cpu_b, array_sizes, transaction_sizes, fifo_array_csv_file, &fifo_array_raw_csv_file);

    fclose(fifo_array_csv_file);
    fifo_array_raw_csv_file.close();

    printf("\n");

    FILE* fifo_blocked_array_csv_file = NULL;
    std::ofstream fifo_blocked_array_raw_csv_file;
    #if WRITE_CSV == 1
    fifo_blocked_array_csv_file = fopen("report_fifo_blocked_array.csv", "w");
    fifo_blocked_array_raw_csv_file.open("report_fifo_blocked_array_raw.csv", std::ofstream::out);
    #endif

    run_bandwidth_fifo_blocked_kernel(pcm, cpu_a, cpu_b, array_sizes, transaction_sizes, fifo_blocked_array_csv_file, &fifo_blocked_array_raw_csv_file);

    fclose(fifo_blocked_array_csv_file);
    fifo_blocked_array_raw_csv_file.close();

    return 0;
}
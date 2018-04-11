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

#ifndef PRINT_FULL_STATS
    #define PRINT_FULL_STATS 0
#endif

#ifndef WRITE_CSV
    #define WRITE_CSV 1
#endif

void print_results(Results* results, int bytes_per_transfer)
{
    double avg_duration_ms = results->avg_duration();
    double stddev_duration_ms = results->stddev_duration();
    double avg_latency_ns = avg_duration_ms*1000000/STIM_LEN;
    double stddev_latency_ns = stddev_duration_ms*1000000/STIM_LEN;

    double transactions_rate_msps = STIM_LEN/(1000.0*avg_duration_ms);

    double data_rate_mbps = 8.0*bytes_per_transfer*STIM_LEN/(1000.0*avg_duration_ms);

    printf("        =======================================================\n");
    printf("        Metric                   |     Avg      |    StdDev    \n");
    printf("        =======================================================\n");
    //printf("        Duration (ms)            |%14.6f|%14.6f\n", STIM_LEN, avg_duration_ms, stddev_duration_ms);
    printf("        One Way Latency (ns)     |%14.6f|%14.6f\n", avg_latency_ns, stddev_latency_ns);
    printf("        Transaction Rate (MT/s)  |%14.6f|\n", transactions_rate_msps);
    printf("        Data Rate (Mbps)         |%14.6f|\n", data_rate_mbps);
    printf("        =======================================================\n");
}

void print_results(Results* results, int bytes_per_transfer, int length, std::string format, FILE* file=NULL)
{
    double avg_duration_ms = results->avg_duration();
    double stddev_duration_ms = results->stddev_duration();
    double avg_latency_ns = avg_duration_ms*1000000/STIM_LEN;
    double stddev_latency_ns = stddev_duration_ms*1000000/STIM_LEN;

    double transactions_rate_msps = STIM_LEN/(1000.0*avg_duration_ms);

    double data_rate_mbps = 8.0*bytes_per_transfer*STIM_LEN/(1000.0*avg_duration_ms);

    #if PRINT_STATS == 1
    printf(format.c_str(), length, avg_latency_ns, stddev_latency_ns, transactions_rate_msps, data_rate_mbps);
    #endif

    #if WRITE_CSV == 1
    fprintf(file, "%d, %f, %f, %f, %f\n", length, avg_latency_ns, stddev_latency_ns, transactions_rate_msps, data_rate_mbps);
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
                print_results(results, sizeof(*shared_loc));
                #endif

        #else
                #if PRINT_FULL_STATS
                results->print_statistics(0, cpu_a, STIM_LEN);
                #endif

                print_results(results, sizeof(*shared_loc));
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
                print_results(results, sizeof(*shared_loc_a));
                #endif

        #else
                #if PRINT_FULL_STATS
                results->print_statistics(0, cpu_a, STIM_LEN);
                #endif

                print_results(results, sizeof(*shared_loc_a));
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

Results* run_latency_single_array_kernel(PCM* pcm, int cpu_a, int cpu_b, size_t array_length, bool report_standalone=true, std::string format = "", FILE* file=NULL)
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
                    print_results(results, sizeof(*shared_loc)*array_length);
                    #endif

            #else
                    #if PRINT_FULL_STATS
                    results->print_statistics(0, cpu_a, STIM_LEN);
                    #endif

                    print_results(results, sizeof(*shared_loc)*array_length);
            #endif
        }
        else
        {
            print_results(results, sizeof(*shared_loc)*array_length, array_length, format, file);
        }
    #endif

    //Clean Up
    delete[] shared_loc;
    delete arg_a;
    delete arg_b;
    delete reset_arg;

    return results;
}

void run_latency_single_array_kernel(PCM* pcm, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, FILE* file=NULL)
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
    #endif

    std::string format = "        %18d | %11.6f, %11.6f | %23.6f | %15.6f \n";

    for(int i = 0; i<array_lengths.size(); i++)
    {
        size_t array_length = array_lengths[i];
        Results* latency_single_array_kernel_results = run_latency_single_array_kernel(pcm, cpu_a, cpu_b, array_length, false, format, file);

        #if WRITE_CSV == 1
        fflush(file);
        #endif

        //Cleanup
        latency_single_array_kernel_results->delete_results();
        delete latency_single_array_kernel_results;
    }

    printf("        ==========================================================================================\n");
}

Results* run_latency_dual_array_kernel(PCM* pcm, int cpu_a, int cpu_b, size_t array_length, bool report_standalone=true, std::string format = "", FILE* file = NULL)
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
                    print_results(results, sizeof(*shared_loc_a)*array_length);
                    #endif

            #else
                    #if PRINT_FULL_STATS
                    results->print_statistics(0, cpu_a, STIM_LEN);
                    #endif

                    print_results(results, sizeof(*shared_loc)*array_length);
            #endif
        }
        else
        {
            print_results(results, sizeof(*shared_loc_a)*array_length, array_length, format, file);
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

void run_latency_dual_array_kernel(PCM* pcm, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, FILE* file = NULL)
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
    #endif

    std::string format = "        %18d | %11.6f, %11.6f | %23.6f | %15.6f \n";

    for(int i = 0; i<array_lengths.size(); i++)
    {
        size_t array_length = array_lengths[i];
        Results* latency_dual_array_kernel_results = run_latency_dual_array_kernel(pcm, cpu_a, cpu_b, array_length, false, format, file);

        #if WRITE_CSV == 1
        fflush(file);
        #endif

        //Cleanup
        latency_dual_array_kernel_results->delete_results();
        delete latency_dual_array_kernel_results;
    }

    printf("        ==========================================================================================\n");
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
    size_t stop = 65;

    for(int i = start; i < stop; i++)
    {
        array_sizes.push_back(i);
    }

    printf("\n");

    FILE* single_array_csv_file = NULL;
    #if WRITE_CSV == 1
    single_array_csv_file = fopen("report_single_array.csv", "w");
    #endif

    run_latency_single_array_kernel(pcm, cpu_a, cpu_b, array_sizes, single_array_csv_file);
    
    fclose(single_array_csv_file);

    printf("\n");

    FILE* dual_array_csv_file = NULL;
    #if WRITE_CSV == 1
    dual_array_csv_file = fopen("report_dual_array.csv", "w");
    #endif

    run_latency_dual_array_kernel(pcm, cpu_a, cpu_b, array_sizes, dual_array_csv_file);

    fclose(dual_array_csv_file);

    return 0;
}
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
#include "bandwidth_circular_fifo_read_limit_kernel.h"

#ifndef PRINT_FULL_STATS
    #define PRINT_FULL_STATS 0
#endif

#ifndef WRITE_CSV
    #define WRITE_CSV 1
#endif

#include "print_results.h"
#include "single_kernels.h"
#include "array_kernels.h"
#include "fifo_kernels.h"

int main(int argc, char *argv[])
{
    //Run these single-threaded benchmarks on CPU 0 (all machines should have CPU 0)

    //Based off of http://man7.org/linux/man-pages/man3/pthread_setaffinity_np.3.html
    //http://man7.org/linux/man-pages/man3/pthread_create.3.html
    //http://man7.org/linux/man-pages/man3/pthread_attr_setaffinity_np.3.html,
    //http://man7.org/linux/man-pages/man3/pthread_join.3.html

    if(argc < 5)
    {
        printf("core-core core-a core-b core-c core-d\n    core-a: Processor ID for first processor core\n    core-b: Processor ID for second processor core\n    core-c: Processor ID for third processor core\n    core-d: Processor ID for fourth processor core\n");
        exit(1);
    }
    std::string cpu_a_str(argv[1]);
    int cpu_a = std::stoi(cpu_a_str);

    std::string cpu_b_str(argv[2]);
    int cpu_b = std::stoi(cpu_b_str);

    std::string cpu_a_str(argv[3]);
    int cpu_c = std::stoi(cpu_a_str);

    std::string cpu_b_str(argv[4]);
    int cpu_d = std::stoi(cpu_b_str);

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
    #else
        PCM* pcm = NULL;
    #endif

    //Setup for array runs
    //std::vector<size_t> array_sizes = {1, 2, 4, 8, 16, 32, 64};
    std::vector<size_t> array_sizes;
    size_t start = 1;
    size_t stop = 257;
    // size_t stop = 129;
    // size_t stop = 65;

    for(int i = start; i < stop; i++)
    {
        array_sizes.push_back(i);
    }

    //=====Test 1 - Latency Single Shared Element=====
    Results* latency_single_kernel_results = run_latency_single_kernel(pcm, cpu_a, cpu_b);
    latency_single_kernel_results->delete_results();
    delete latency_single_kernel_results;

    //=====Test 1.1 - Latency Dual Elements=====
    Results* latency_dual_kernel_results = run_latency_dual_kernel(pcm, cpu_a, cpu_b);
    latency_dual_kernel_results->delete_results();
    delete latency_dual_kernel_results;

    //=====Test 2 - Latency Single Shared Array=====

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

    //=====Test 2.1 - Latency Dual Arrays=====
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

    //=====Test 2.2 - Latency Flow Control Array=====
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

    //=====Test 2.3 - Latency Flow Control Blocked Array=====
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

    //=====Test 3 - Bandwidth FIFO=====
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

    //=====Test 3.1 - Bandwidth FIFO Blocked=====
    FILE* fifo_blocked_array_csv_file = NULL;
    std::ofstream fifo_blocked_array_raw_csv_file;
    #if WRITE_CSV == 1
    fifo_blocked_array_csv_file = fopen("report_fifo_blocked_array.csv", "w");
    fifo_blocked_array_raw_csv_file.open("report_fifo_blocked_array_raw.csv", std::ofstream::out);
    #endif

    run_bandwidth_fifo_blocked_kernel(pcm, cpu_a, cpu_b, array_sizes, transaction_sizes, fifo_blocked_array_csv_file, &fifo_blocked_array_raw_csv_file);

    fclose(fifo_blocked_array_csv_file);
    fifo_blocked_array_raw_csv_file.close();

    printf("\n");

    //=====Test 3.2 - Bandwidth FIFO Read Limit=====
    FILE* fifo_read_limit_array_csv_file = NULL;
    std::ofstream fifo_read_limit_array_raw_csv_file;
    #if WRITE_CSV == 1
    fifo_read_limit_array_csv_file = fopen("report_fifo_read_limit_array.csv", "w");
    fifo_read_limit_array_raw_csv_file.open("report_fifo_read_limit_array_raw.csv", std::ofstream::out);
    #endif

    run_bandwidth_fifo_read_limit_kernel(pcm, cpu_a, cpu_b, array_sizes, transaction_sizes, fifo_read_limit_array_csv_file, &fifo_read_limit_array_raw_csv_file);

    fclose(fifo_read_limit_array_csv_file);
    fifo_read_limit_array_raw_csv_file.close();

    //########## Parallel Runs ##########

    return 0;
}
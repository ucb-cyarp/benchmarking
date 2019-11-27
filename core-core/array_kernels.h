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

    Results* run_latency_single_array_kernel(Profiler* profiler, int cpu_a, int cpu_b, size_t array_length, bool report_standalone=true, std::string format = "", FILE* file=NULL, std::ofstream* raw_file=NULL)
    {
        //=====Test 2=====
        #if PRINT_TITLE == 1
        if(report_standalone)
        {
            printf("\n");
            time_t currentTime = time(NULL);
            struct tm * localT;
            localT = localtime(&currentTime);
            printf("Single Memory Location - Array | Start Time: %s", asctime(localT));
            printf("Array Length: %lu int32_t Elements\n", array_length);
        }
        #endif

        //Initialize
        size_t amountToAlloc = array_length*sizeof(std::atomic_int32_t);
        if(amountToAlloc % CACHE_LINE_SIZE != 0){
            amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
        }
        std::atomic_int32_t* shared_loc = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);

        //Init to 0
        for(size_t i = 0; i < array_length; i++)
        {
            std::atomic_init(shared_loc+i, 0);
            if(!std::atomic_is_lock_free(shared_loc+i)){
                printf("Atomic is not lock free and was expected to be");
                exit(1);
            }
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

        Results* results = execute_kernel(profiler, latency_single_array_kernel, latency_single_array_kernel_reset, arg_a, arg_b, reset_arg, cpu_a, cpu_b);

        #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1 || WRITE_CSV == 1
            if(report_standalone)
            {
                if(!profiler->cpuTopology.empty()){
                    std::vector<int> sockets;
                    int socket_a = profiler->cpuTopology[cpu_a].socket;
                    int socket_b = profiler->cpuTopology[cpu_b].socket;
                    sockets.push_back(socket_a);
                    if(socket_b != socket_a)
                    {
                        sockets.push_back(socket_b);
                    }

                    std::vector<int> cores;
                    int core_a = profiler->cpuTopology[cpu_a].core;
                    int core_b = profiler->cpuTopology[cpu_b].core;
                    cores.push_back(core_a);
                    if(core_a != core_b){
                        cores.push_back(core_b);
                    }

                    std::vector<int> dies;
                    int die_a = profiler->cpuTopology[cpu_a].die;
                    int die_b = profiler->cpuTopology[cpu_b].die;
                    dies.push_back(die_a);
                    if(die_a != die_b){
                        dies.push_back(die_b);
                    }

                    std::vector<int> threads;
                    threads.push_back(cpu_a);
                    if(cpu_a != cpu_b){
                        threads.push_back(cpu_b);
                    }
                    
                    #if PRINT_FULL_STATS == 1
                        results->print_statistics(sockets, dies, cores, threads, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                    print_results(results, sizeof(*shared_loc)*array_length, STIM_LEN);
                    #endif
                }else{
                        #if PRINT_FULL_STATS
                        results->print_statistics(0, 0, 0, cpu_a, STIM_LEN);
                        #endif

                        print_results(results, sizeof(*shared_loc)*array_length, STIM_LEN);
                }
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

    SimultaniousResults* run_latency_single_array_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, int cpu_d, size_t array_length, bool report_standalone=true, std::string format = "", FILE* file_a=NULL, FILE* file_b=NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL)
    {
        //=====Test 2=====
        #if PRINT_TITLE == 1
        if(report_standalone)
        {
            printf("\n");
            time_t currentTime = time(NULL);
            struct tm * localT;
            localT = localtime(&currentTime);
            printf("Single Memory Location - Simultanious | Start Time: %s", asctime(localT));
            printf("Array Length: %lu int32_t Elements\n", array_length);
        }
        #endif

        //Initialize
        size_t amountToAlloc = array_length*sizeof(std::atomic_int32_t);
        if(amountToAlloc % CACHE_LINE_SIZE != 0){
            amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
        }
        std::atomic_int32_t* shared_loc_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);
        std::atomic_int32_t* shared_loc_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_c);

        //Init to 0
        for(size_t i = 0; i < array_length; i++)
        {
            std::atomic_init(shared_loc_1+i, 0);
            if(!std::atomic_is_lock_free(shared_loc_1+i)){
                printf("Atomic is not lock free and was expected to be");
                exit(1);
            }
            std::atomic_init(shared_loc_2+i, 0);
            if(!std::atomic_is_lock_free(shared_loc_2+i)){
                printf("Atomic is not lock free and was expected to be");
                exit(1);
            }
        }

        LatencySingleArrayKernelArgs* arg_a = new LatencySingleArrayKernelArgs();
        arg_a->init_counter = -1; //(server)
        arg_a->shared_ptr = shared_loc_1;
        arg_a->length = array_length;

        LatencySingleArrayKernelArgs* arg_b = new LatencySingleArrayKernelArgs();
        arg_b->init_counter = 0; //(client)
        arg_b->shared_ptr = shared_loc_1;
        arg_b->length = array_length;

        LatencySingleArrayKernelArgs* arg_c = new LatencySingleArrayKernelArgs();
        arg_c->init_counter = -1; //(server)
        arg_c->shared_ptr = shared_loc_2;
        arg_c->length = array_length;

        LatencySingleArrayKernelArgs* arg_d = new LatencySingleArrayKernelArgs();
        arg_d->init_counter = 0; //(client)
        arg_d->shared_ptr = shared_loc_2;
        arg_d->length = array_length;

        LatencySingleArrayKernelResetArgs* reset_arg_1 = new LatencySingleArrayKernelResetArgs();
        reset_arg_1->shared_ptr = shared_loc_1;
        reset_arg_1->length = array_length;

        LatencySingleArrayKernelResetArgs* reset_arg_2 = new LatencySingleArrayKernelResetArgs();
        reset_arg_2->shared_ptr = shared_loc_2;
        reset_arg_2->length = array_length;

        SimultaniousResults* results = execute_kernel(profiler, latency_single_array_kernel, latency_single_array_kernel_reset, arg_a, arg_b, arg_c, arg_d, reset_arg_1, reset_arg_2, cpu_a, cpu_b, cpu_c, cpu_d);

        #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1 || WRITE_CSV == 1
            if(report_standalone)
            {
                if(!profiler->cpuTopology.empty()){
                    std::set<int> sockets_set;
                    sockets_set.insert(profiler->cpuTopology[cpu_a].socket);
                    sockets_set.insert(profiler->cpuTopology[cpu_b].socket);
                    sockets_set.insert(profiler->cpuTopology[cpu_c].socket);
                    sockets_set.insert(profiler->cpuTopology[cpu_d].socket);
                    std::vector<int> sockets(sockets_set.begin(), sockets_set.end());

                    std::set<int> cores_set;
                    cores_set.insert(profiler->cpuTopology[cpu_a].core);
                    cores_set.insert(profiler->cpuTopology[cpu_b].core);
                    cores_set.insert(profiler->cpuTopology[cpu_c].core);
                    cores_set.insert(profiler->cpuTopology[cpu_d].core);
                    std::vector<int> cores(cores_set.begin(), cores_set.end());

                    std::set<int> dies_set;
                    dies_set.insert(profiler->cpuTopology[cpu_a].die);
                    dies_set.insert(profiler->cpuTopology[cpu_b].die);
                    dies_set.insert(profiler->cpuTopology[cpu_c].die);
                    dies_set.insert(profiler->cpuTopology[cpu_d].die);
                    std::vector<int> dies(dies_set.begin(), dies_set.end());

                    std::set<int> threads_set;
                    threads_set.insert(cpu_a);
                    threads_set.insert(cpu_b);
                    threads_set.insert(cpu_c);
                    threads_set.insert(cpu_d);
                    std::vector<int> threads(threads_set.begin(), threads_set.end());
                
                    #if PRINT_FULL_STATS == 1
                        printf("Thread Pair 1 (A/B)\n");
                        results->results_a->print_statistics(sockets, dies, cores, threads, STIM_LEN);
                        printf("Thread Pair 2 (C/D)\n");
                        results->results_b->print_statistics(sockets, dies, cores, threads, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        printf("Thread Pair 1 (A/B)\n");
                        print_results(results->results_a, sizeof(*shared_loc_1)*array_length, STIM_LEN);
                        printf("Thread Pair 2 (C/D)\n");
                        print_results(results->results_b, sizeof(*shared_loc_2)*array_length, STIM_LEN);
                    #endif

                }else{
                    #if PRINT_FULL_STATS
                        printf("Thread Pair 1 (A/B)\n");
                        results->results_a->print_statistics(0, 0, 0, cpu_a, STIM_LEN);
                        printf("Thread Pair 2 (C/D)\n");
                        results->results_b->print_statistics(0, 0, 0, cpu_c, STIM_LEN);
                    #endif

                    print_results(results->results_a, sizeof(*shared_loc_1)*array_length, STIM_LEN);
                    print_results(results->results_b, sizeof(*shared_loc_2)*array_length, STIM_LEN);
                }
            }
            else
            {
                print_results(results->results_a, cpu_a, cpu_b, sizeof(*shared_loc_1)*array_length, STIM_LEN, array_length, format, file_a, raw_file_a);
                print_results(results->results_b, cpu_c, cpu_d, sizeof(*shared_loc_2)*array_length, STIM_LEN, array_length, format, file_b, raw_file_b);
            }
        #endif

        //Clean Up
        free(shared_loc_1);
        free(shared_loc_2);
        delete arg_a;
        delete arg_b;
        delete arg_c;
        delete arg_d;
        delete reset_arg_1;
        delete reset_arg_2;

        return results;
    }

    /**
     * Note: This version tests fanin & fanout.  Since the single array kernel operates by
     * "ping pong-ing" between the client and server threads, each cycle contains a fan-out
     * and a fan-in.  The reported result (since it is reported as 1 way) averages the effect
     * of fanout and fanin
     */
    SimultaniousResults* run_latency_single_array_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, size_t array_length, bool report_standalone=true, std::string format = "", FILE* file_a=NULL, FILE* file_b=NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL)
    {
        //=====Test 2=====
        #if PRINT_TITLE == 1
        if(report_standalone)
        {
            printf("\n");
            time_t currentTime = time(NULL);
            struct tm * localT;
            localT = localtime(&currentTime);
            printf("Single Memory Location - Fan-in/Fan-out - Array | Start Time: %s", asctime(localT));
            printf("Array Length: %lu int32_t Elements\n", array_length);
        }
        #endif

        //Initialize
        size_t amountToAlloc = array_length*sizeof(std::atomic_int32_t);
        if(amountToAlloc % CACHE_LINE_SIZE != 0){
            amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
        }
        std::atomic_int32_t* shared_loc_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);
        std::atomic_int32_t* shared_loc_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_b);

        //Init to 0
        for(size_t i = 0; i < array_length; i++)
        {
            std::atomic_init(shared_loc_1+i, 0);
            if(!std::atomic_is_lock_free(shared_loc_1+i)){
                printf("Atomic is not lock free and was expected to be");
                exit(1);
            }

            std::atomic_init(shared_loc_2+i, 0);
            if(!std::atomic_is_lock_free(shared_loc_2+i)){
                printf("Atomic is not lock free and was expected to be");
                exit(1);
            }
        }

        //Construct 2 servers and 1 client.  Servers will measure.
        //Both fan-in and fan-out is measured 
        LatencySingleArrayKernelArgs* arg_a = new LatencySingleArrayKernelArgs();
        arg_a->init_counter = -1; //(server)
        arg_a->shared_ptr = shared_loc_1;
        arg_a->length = array_length;

        LatencySingleArrayKernelArgs* arg_b = new LatencySingleArrayKernelArgs();
        arg_b->init_counter = -1; //(server)
        arg_b->shared_ptr = shared_loc_2;
        arg_b->length = array_length;

        LatencySingleArrayJoinKernelArgs* arg_c = new LatencySingleArrayJoinKernelArgs();
        arg_c->init_counter_a = 0; //(client)
        arg_c->init_counter_b = 0; //(client)
        arg_c->shared_ptr_a = shared_loc_1;
        arg_c->shared_ptr_b = shared_loc_2;
        arg_c->length_a = array_length;
        arg_c->length_b = array_length;

        LatencySingleArrayJoinKernelResetArgs* reset_arg_1 = new LatencySingleArrayJoinKernelResetArgs();
        reset_arg_1->shared_ptr_a = shared_loc_1;
        reset_arg_1->shared_ptr_b = shared_loc_2;
        reset_arg_1->length_a = array_length;
        reset_arg_1->length_b = array_length;

        SimultaniousResults* results = execute_kernel_fanin_server_measure(profiler, latency_single_array_kernel, latency_single_array_join_kernel, latency_single_array_join_kernel_reset, arg_a, arg_b, arg_c, reset_arg_1, cpu_a, cpu_b, cpu_c);

        #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1 || WRITE_CSV == 1
            if(report_standalone)
            {
                if(!profiler->cpuTopology.empty()){
                    std::set<int> sockets_set;
                    sockets_set.insert(profiler->cpuTopology[cpu_a].socket);
                    sockets_set.insert(profiler->cpuTopology[cpu_b].socket);
                    sockets_set.insert(profiler->cpuTopology[cpu_c].socket);
                    std::vector<int> sockets(sockets_set.begin(), sockets_set.end());

                    std::set<int> cores_set;
                    cores_set.insert(profiler->cpuTopology[cpu_a].core);
                    cores_set.insert(profiler->cpuTopology[cpu_b].core);
                    cores_set.insert(profiler->cpuTopology[cpu_c].core);
                    std::vector<int> cores(cores_set.begin(), cores_set.end());

                    std::set<int> dies_set;
                    dies_set.insert(profiler->cpuTopology[cpu_a].die);
                    dies_set.insert(profiler->cpuTopology[cpu_b].die);
                    dies_set.insert(profiler->cpuTopology[cpu_c].die);
                    std::vector<int> dies(dies_set.begin(), dies_set.end());

                    std::set<int> threads_set;
                    threads_set.insert(cpu_a);
                    threads_set.insert(cpu_b);
                    threads_set.insert(cpu_c);
                    std::vector<int> threads(threads_set.begin(), threads_set.end());
            
                    #if PRINT_FULL_STATS == 1
                        printf("Thread Pair 1 (A/C)\n");
                        results->results_a->print_statistics(sockets, dies, cores, threads, STIM_LEN);
                        printf("Thread Pair 2 (B/C)\n");
                        results->results_b->print_statistics(sockets, dies, cores, threads, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                    printf("Thread Pair 1 (A/C)\n");
                    print_results(results->results_a, sizeof(*shared_loc_1)*array_length, STIM_LEN);
                    printf("Thread Pair 2 (B/C)\n");
                    print_results(results->results_b, sizeof(*shared_loc_2)*array_length, STIM_LEN);
                    #endif

                }else{
                        #if PRINT_FULL_STATS
                        printf("Thread Pair 1 (A/C)\n");
                        results->results_a->print_statistics(0, 0, 0, cpu_a, STIM_LEN);
                        printf("Thread Pair 2 (B/C)\n");
                        results->results_b->print_statistics(0, 0, 0, cpu_c, STIM_LEN);
                        #endif

                        print_results(results->results_a, sizeof(*shared_loc_1)*array_length, STIM_LEN);
                        print_results(results->results_b, sizeof(*shared_loc_2)*array_length, STIM_LEN);
                }
            }
            else
            {
                print_results(results->results_a, cpu_a, cpu_c, sizeof(*shared_loc_1)*array_length, STIM_LEN, array_length, format, file_a, raw_file_a);
                print_results(results->results_b, cpu_b, cpu_c, sizeof(*shared_loc_2)*array_length, STIM_LEN, array_length, format, file_b, raw_file_b);
            }
        #endif

        //Clean Up
        free(shared_loc_1);
        free(shared_loc_2);
        delete arg_a;
        delete arg_b;
        delete arg_c;
        delete reset_arg_1;

        return results;
    }

    void run_latency_single_array_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, FILE* file=NULL, std::ofstream* raw_file=NULL)
    {
        //Print header
        time_t currentTime = time(NULL);
        struct tm * localT;
        localT = localtime(&currentTime);
        printf("Single Memory Location - Array | Start Time: %s", asctime(localT));
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
            Results* latency_single_array_kernel_results = run_latency_single_array_kernel(profiler, cpu_a, cpu_b, array_length, false, format, file, raw_file);

            #if WRITE_CSV == 1
            fflush(file);
            #endif

            //Cleanup
            delete latency_single_array_kernel_results;
        }

        printf("        ==========================================================================================\n");
    }

    void run_latency_single_array_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, int cpu_d, std::vector<size_t> array_lengths, FILE* file_a=NULL, FILE* file_b=NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL)
    {
        //Print header
        time_t currentTime = time(NULL);
        struct tm * localT;
        localT = localtime(&currentTime);
        printf("Single Memory Location - Simultanious - Array | Start Time: %s", asctime(localT));
        printf("        ========================================================================================================\n");
        printf("         CPU <-> CPU |  Transfer Length  |   One Way Latency (ns)   | Transaction Rate (MT/s) | Data Rate (Mbps)\n");
        printf("                     |(int32_t Elements) |       Avg, StdDev        |                         |                 \n");
        printf("        ========================================================================================================\n");

        #if WRITE_CSV == 1
        fprintf(file_a, "\"Transfer Length (int32_t Elements)\", \"One Way Latency (ns) - Avg\", \"One Way Latency (ns) - StdDev\", \"Transaction Rate (MT/s)\", \"Data Rate (Mbps)\"\n");
        fflush(file_a);
        fprintf(file_b, "\"Transfer Length (int32_t Elements)\", \"One Way Latency (ns) - Avg\", \"One Way Latency (ns) - StdDev\", \"Transaction Rate (MT/s)\", \"Data Rate (Mbps)\"\n");
        fflush(file_b);
        *raw_file_a << "\"Transfer Length (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
        *raw_file_b << "\"Transfer Length (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
        #endif

        std::string format = "         %3d <-> %-3d | %17d | %11.6f, %11.6f | %23.6f | %15.6f \n";

        for(int i = 0; i<array_lengths.size(); i++)
        {
            size_t array_length = array_lengths[i];
            SimultaniousResults* latency_single_array_kernel_results_container = run_latency_single_array_kernel(profiler, cpu_a, cpu_b, cpu_c, cpu_d, array_length, false, format, file_a, file_b, raw_file_a, raw_file_b);

            #if WRITE_CSV == 1
            fflush(file_a);
            fflush(file_b);
            #endif

            //Cleanup
            delete latency_single_array_kernel_results_container->results_a;
            delete latency_single_array_kernel_results_container->results_b;

            delete latency_single_array_kernel_results_container;
        }

        printf("        ==========================================================================================\n");
    }

    void run_latency_single_array_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, std::vector<size_t> array_lengths, FILE* file_a=NULL, FILE* file_b=NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL)
    {
        //Print header
        time_t currentTime = time(NULL);
        struct tm * localT;
        localT = localtime(&currentTime);
        printf("Single Memory Location - Fan-in/Fan-out - Array | Start Time: %s", asctime(localT));
        printf("        ========================================================================================================\n");
        printf("         CPU <-> CPU |  Transfer Length  |   One Way Latency (ns)   | Transaction Rate (MT/s) | Data Rate (Mbps)\n");
        printf("                     |(int32_t Elements) |       Avg, StdDev        |                         |                 \n");
        printf("        ========================================================================================================\n");

        #if WRITE_CSV == 1
        fprintf(file_a, "\"Transfer Length (int32_t Elements)\", \"One Way Latency (ns) - Avg\", \"One Way Latency (ns) - StdDev\", \"Transaction Rate (MT/s)\", \"Data Rate (Mbps)\"\n");
        fflush(file_a);
        fprintf(file_b, "\"Transfer Length (int32_t Elements)\", \"One Way Latency (ns) - Avg\", \"One Way Latency (ns) - StdDev\", \"Transaction Rate (MT/s)\", \"Data Rate (Mbps)\"\n");
        fflush(file_b);
        *raw_file_a << "\"Transfer Length (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
        *raw_file_b << "\"Transfer Length (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
        #endif

        std::string format = "         %3d <-> %-3d | %17d | %11.6f, %11.6f | %23.6f | %15.6f \n";

        for(int i = 0; i<array_lengths.size(); i++)
        {
            size_t array_length = array_lengths[i];
            SimultaniousResults* latency_single_array_kernel_results_container = run_latency_single_array_kernel(profiler, cpu_a, cpu_b, cpu_c, array_length, false, format, file_a, file_b, raw_file_a, raw_file_b);

            #if WRITE_CSV == 1
            fflush(file_a);
            fflush(file_b);
            #endif

            //Cleanup
            delete latency_single_array_kernel_results_container->results_a;
            delete latency_single_array_kernel_results_container->results_b;

            delete latency_single_array_kernel_results_container;
        }

        printf("        ==========================================================================================\n");
    }

    Results* run_latency_dual_array_kernel(Profiler* profiler, int cpu_a, int cpu_b, size_t array_length, bool report_standalone=true, std::string format = "", FILE* file = NULL, std::ofstream* raw_file=NULL)
    {
        //=====Test 2=====
        #if PRINT_TITLE == 1
        if(report_standalone)
        {
            printf("\n");
            time_t currentTime = time(NULL);
            struct tm * localT;
            localT = localtime(&currentTime);
            printf("Dual Memory Location - Array | Start Time: %s", asctime(localT));
            printf("Array Length: %lu int32_t Elements\n", array_length);
        }
        #endif

        //Initialize
        size_t amountToAlloc = array_length*sizeof(std::atomic_int32_t);
        if(amountToAlloc % CACHE_LINE_SIZE != 0){
            amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
        }
        std::atomic_int32_t* shared_loc_a = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);
        std::atomic_int32_t* shared_loc_b = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_b);

        //Init to 0
        for(size_t i = 0; i < array_length; i++)
        {
            std::atomic_init(shared_loc_a+i, 0);
            if(!std::atomic_is_lock_free(shared_loc_a+i)){
                printf("Atomic is not lock free and was expected to be");
                exit(1);
            }
            std::atomic_init(shared_loc_b+i, 0);
            if(!std::atomic_is_lock_free(shared_loc_b+i)){
                printf("Atomic is not lock free and was expected to be");
                exit(1);
            }
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

        Results* results = execute_kernel(profiler, latency_dual_array_kernel, latency_dual_array_kernel_reset, arg_a, arg_b, reset_arg, cpu_a, cpu_b);

        #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1 || WRITE_CSV == 1
            if(report_standalone)
            {
                if(!profiler->cpuTopology.empty()){
                    std::vector<int> sockets;
                    int socket_a = profiler->cpuTopology[cpu_a].socket;
                    int socket_b = profiler->cpuTopology[cpu_b].socket;
                    sockets.push_back(socket_a);
                    if(socket_b != socket_a)
                    {
                        sockets.push_back(socket_b);
                    }

                    std::vector<int> cores;
                    int core_a = profiler->cpuTopology[cpu_a].core;
                    int core_b = profiler->cpuTopology[cpu_b].core;
                    cores.push_back(core_a);
                    if(core_a != core_b){
                        cores.push_back(core_b);
                    }

                    std::vector<int> dies;
                    int die_a = profiler->cpuTopology[cpu_a].die;
                    int die_b = profiler->cpuTopology[cpu_b].die;
                    dies.push_back(die_a);
                    if(die_a != die_b){
                        dies.push_back(die_b);
                    }

                    std::vector<int> threads;
                    threads.push_back(cpu_a);
                    if(cpu_a != cpu_b){
                        threads.push_back(cpu_b);
                    }
            
                    #if PRINT_FULL_STATS == 1
                        results->print_statistics(sockets, dies, cores, threads, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        print_results(results, sizeof(*shared_loc_a)*array_length, STIM_LEN);
                    #endif

                }else{
                    #if PRINT_FULL_STATS
                        results->print_statistics(0, 0, 0, cpu_a, STIM_LEN);
                    #endif

                    print_results(results, sizeof(*shared_loc_a)*array_length, STIM_LEN);
                }
            }
            else
            {
                print_results(results, sizeof(*shared_loc_a)*array_length, STIM_LEN, array_length, format, file, raw_file);
            }
        #endif

        //Clean Up
        free(shared_loc_a);
        free(shared_loc_b);
        delete arg_a;
        delete arg_b;
        delete reset_arg;

        return results;
    }

    SimultaniousResults* run_latency_dual_array_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, int cpu_d, size_t array_length, bool report_standalone=true, std::string format = "", FILE* file_a = NULL, FILE* file_b = NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL)
    {
        //=====Test 2=====
        #if PRINT_TITLE == 1
        if(report_standalone)
        {
            printf("\n");
            time_t currentTime = time(NULL);
            struct tm * localT;
            localT = localtime(&currentTime);
            printf("Dual Memory Location - Simultanious - Array | Start Time: %s", asctime(localT));
            printf("Array Length: %lu int32_t Elements\n", array_length);
        }
        #endif

        //Initialize
        size_t amountToAlloc = array_length*sizeof(std::atomic_int32_t);
        if(amountToAlloc % CACHE_LINE_SIZE != 0){
            amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
        }
        std::atomic_int32_t* shared_loc_a_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);
        std::atomic_int32_t* shared_loc_b_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_b);

        std::atomic_int32_t* shared_loc_a_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_c);
        std::atomic_int32_t* shared_loc_b_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_d);

        //Init to 0
        for(size_t i = 0; i < array_length; i++)
        {
            std::atomic_init(shared_loc_a_1+i, 0);
            if(!std::atomic_is_lock_free(shared_loc_a_1+i)){
                printf("Atomic is not lock free and was expected to be");
                exit(1);
            }
            std::atomic_init(shared_loc_b_1+i, 0);
            if(!std::atomic_is_lock_free(shared_loc_b_1+i)){
                printf("Atomic is not lock free and was expected to be");
                exit(1);
            }

            std::atomic_init(shared_loc_a_2+i, 0);
            if(!std::atomic_is_lock_free(shared_loc_a_2+i)){
                printf("Atomic is not lock free and was expected to be");
                exit(1);
            }
            std::atomic_init(shared_loc_b_2+i, 0);
            if(!std::atomic_is_lock_free(shared_loc_b_2+i)){
                printf("Atomic is not lock free and was expected to be");
                exit(1);
            }
        }

        LatencyDualArrayKernelArgs* arg_a = new LatencyDualArrayKernelArgs();
        arg_a->init_counter = -1; //(server)
        arg_a->my_shared_ptr = shared_loc_a_1;
        arg_a->other_shared_ptr = shared_loc_b_1;
        arg_a->length = array_length;

        LatencyDualArrayKernelArgs* arg_b = new LatencyDualArrayKernelArgs();
        arg_b->init_counter = 0; //(client)
        arg_b->my_shared_ptr = shared_loc_b_1; //Swapped from server
        arg_b->other_shared_ptr = shared_loc_a_1;
        arg_b->length = array_length;

        LatencyDualArrayKernelArgs* arg_c = new LatencyDualArrayKernelArgs();
        arg_c->init_counter = -1; //(server)
        arg_c->my_shared_ptr = shared_loc_a_2;
        arg_c->other_shared_ptr = shared_loc_b_2;
        arg_c->length = array_length;

        LatencyDualArrayKernelArgs* arg_d = new LatencyDualArrayKernelArgs();
        arg_d->init_counter = 0; //(client)
        arg_d->my_shared_ptr = shared_loc_b_2; //Swapped from server
        arg_d->other_shared_ptr = shared_loc_a_2;
        arg_d->length = array_length;

        LatencyDualArrayKernelResetArgs* reset_arg_1 = new LatencyDualArrayKernelResetArgs();
        reset_arg_1->shared_ptr_a = shared_loc_a_1;
        reset_arg_1->shared_ptr_b = shared_loc_b_1;
        reset_arg_1->length = array_length;

        LatencyDualArrayKernelResetArgs* reset_arg_2 = new LatencyDualArrayKernelResetArgs();
        reset_arg_2->shared_ptr_a = shared_loc_a_2;
        reset_arg_2->shared_ptr_b = shared_loc_b_2;
        reset_arg_2->length = array_length;

        SimultaniousResults* results = execute_kernel(profiler, latency_dual_array_kernel, latency_dual_array_kernel_reset, arg_a, arg_b, arg_c, arg_d, reset_arg_1, reset_arg_2, cpu_a, cpu_b, cpu_c, cpu_d);

        #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1 || WRITE_CSV == 1
            if(report_standalone)
            {
                if(!profiler->cpuTopology.empty()){
                    std::set<int> sockets_set;
                    sockets_set.insert(profiler->cpuTopology[cpu_a].socket);
                    sockets_set.insert(profiler->cpuTopology[cpu_b].socket);
                    sockets_set.insert(profiler->cpuTopology[cpu_c].socket);
                    sockets_set.insert(profiler->cpuTopology[cpu_d].socket);
                    std::vector<int> sockets(sockets_set.begin(), sockets_set.end());

                    std::set<int> cores_set;
                    cores_set.insert(profiler->cpuTopology[cpu_a].core);
                    cores_set.insert(profiler->cpuTopology[cpu_b].core);
                    cores_set.insert(profiler->cpuTopology[cpu_c].core);
                    cores_set.insert(profiler->cpuTopology[cpu_d].core);
                    std::vector<int> cores(cores_set.begin(), cores_set.end());

                    std::set<int> dies_set;
                    dies_set.insert(profiler->cpuTopology[cpu_a].die);
                    dies_set.insert(profiler->cpuTopology[cpu_b].die);
                    dies_set.insert(profiler->cpuTopology[cpu_c].die);
                    dies_set.insert(profiler->cpuTopology[cpu_d].die);
                    std::vector<int> dies(dies_set.begin(), dies_set.end());

                    std::set<int> threads_set;
                    threads_set.insert(cpu_a);
                    threads_set.insert(cpu_b);
                    threads_set.insert(cpu_c);
                    threads_set.insert(cpu_d);
                    std::vector<int> threads(threads_set.begin(), threads_set.end());
                
                    #if PRINT_FULL_STATS == 1
                        printf("Thread Pair 1 (A/B)\n");
                        results->results_a->print_statistics(sockets, dies, cores, threads, STIM_LEN);
                        printf("Thread Pair 2 (C/D)\n");
                        results->results_b->print_statistics(sockets, dies, cores, threads, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        printf("Thread Pair 1 (A/B)\n");
                        print_results(results->results_a, sizeof(*shared_loc_a_1)*array_length, STIM_LEN);
                        printf("Thread Pair 2 (C/D)\n");
                        print_results(results->results_b, sizeof(*shared_loc_a_2)*array_length, STIM_LEN);
                    #endif
                }else{
                    #if PRINT_FULL_STATS
                        printf("Thread Pair 1 (A/B)\n");
                        results->results_a->print_statistics(0, 0, 0, cpu_a, STIM_LEN);
                        printf("Thread Pair 2 (C/D)\n");
                        results->results_b->print_statistics(0, 0, 0, cpu_c, STIM_LEN);
                    #endif

                    printf("Thread Pair 1 (A/B)\n");
                    print_results(results->results_a, sizeof(*shared_loc_a_1)*array_length, STIM_LEN);
                    printf("Thread Pair 2 (C/D)\n");
                    print_results(results->results_b, sizeof(*shared_loc_a_2)*array_length, STIM_LEN);
                }
            }
            else
            {
                print_results(results->results_a, cpu_a, cpu_b, sizeof(*shared_loc_a_1)*array_length, STIM_LEN, array_length, format, file_a, raw_file_a);
                print_results(results->results_b, cpu_c, cpu_d, sizeof(*shared_loc_a_2)*array_length, STIM_LEN, array_length, format, file_b, raw_file_b);
            }
        #endif

        //Clean Up
        free(shared_loc_a_1);
        free(shared_loc_b_1);
        free(shared_loc_a_2);
        free(shared_loc_b_2);
        delete arg_a;
        delete arg_b;
        delete reset_arg_1;
        delete reset_arg_2;

        return results;
    }

    SimultaniousResults* run_latency_dual_array_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, size_t array_length, bool report_standalone=true, std::string format = "", FILE* file_a = NULL, FILE* file_b = NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL)
    {
        //=====Test 2=====
        #if PRINT_TITLE == 1
        if(report_standalone)
        {
            printf("\n");
            time_t currentTime = time(NULL);
            struct tm * localT;
            localT = localtime(&currentTime);
            printf("Dual Memory Location - Fan-in/Fan-out - Array | Start Time: %s", asctime(localT));
            printf("Array Length: %lu int32_t Elements\n", array_length);
        }
        #endif

        //Initialize
        size_t amountToAlloc = array_length*sizeof(std::atomic_int32_t);
        if(amountToAlloc % CACHE_LINE_SIZE != 0){
            amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
        }
        std::atomic_int32_t* shared_loc_a_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);
        std::atomic_int32_t* shared_loc_b_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_c);

        std::atomic_int32_t* shared_loc_a_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_b);
        std::atomic_int32_t* shared_loc_b_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_c);

        //Init to 0
        for(size_t i = 0; i < array_length; i++)
        {
            std::atomic_init(shared_loc_a_1+i, 0);
            if(!std::atomic_is_lock_free(shared_loc_a_1+i)){
                printf("Atomic is not lock free and was expected to be");
                exit(1);
            }
            std::atomic_init(shared_loc_b_1+i, 0);
            if(!std::atomic_is_lock_free(shared_loc_b_1+i)){
                printf("Atomic is not lock free and was expected to be");
                exit(1);
            }

            std::atomic_init(shared_loc_a_2+i, 0);
            if(!std::atomic_is_lock_free(shared_loc_a_2+i)){
                printf("Atomic is not lock free and was expected to be");
                exit(1);
            }
            std::atomic_init(shared_loc_b_2+i, 0);
            if(!std::atomic_is_lock_free(shared_loc_b_2+i)){
                printf("Atomic is not lock free and was expected to be");
                exit(1);
            }
        }

        //2 Servers, 1 Client - Servers will measure
        //SERVERS
        LatencyDualArrayKernelArgs* arg_a = new LatencyDualArrayKernelArgs();
        arg_a->init_counter = -1; //(server)
        arg_a->my_shared_ptr = shared_loc_a_1;
        arg_a->other_shared_ptr = shared_loc_b_1;
        arg_a->length = array_length;

        LatencyDualArrayKernelArgs* arg_b = new LatencyDualArrayKernelArgs();
        arg_b->init_counter = -1; //(server)
        arg_b->my_shared_ptr = shared_loc_a_2; //Swapped from server
        arg_b->other_shared_ptr = shared_loc_b_2;
        arg_b->length = array_length;

        //CLIENT
        LatencyDualArrayJoinKernelArgs* arg_c = new LatencyDualArrayJoinKernelArgs();
        arg_c->init_counter_a = 0; //(client)
        arg_c->my_shared_ptr_a = shared_loc_b_1; //Swapped from server
        arg_c->other_shared_ptr_a = shared_loc_a_1;
        arg_c->length_a = array_length;
        arg_c->init_counter_b = 0; //(client)
        arg_c->my_shared_ptr_b = shared_loc_b_2; //Swapped from server
        arg_c->other_shared_ptr_b = shared_loc_a_2;
        arg_c->length_b = array_length;

        LatencyDualArrayJoinKernelResetArgs* reset_arg_1 = new LatencyDualArrayJoinKernelResetArgs();
        reset_arg_1->shared_ptr_a = shared_loc_a_1;
        reset_arg_1->shared_ptr_b = shared_loc_b_1;
        reset_arg_1->shared_ptr_c = shared_loc_a_2;
        reset_arg_1->shared_ptr_d = shared_loc_b_2;
        reset_arg_1->length_ab = array_length;
        reset_arg_1->length_cd = array_length;

        SimultaniousResults* results = execute_kernel_fanin_server_measure(profiler, latency_dual_array_kernel, latency_dual_array_join_kernel, latency_dual_array_join_kernel_reset, arg_a, arg_b, arg_c, reset_arg_1, cpu_a, cpu_b, cpu_c);

        #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1 || WRITE_CSV == 1
            if(report_standalone)
            {
                if(!profiler->cpuTopology.empty()){
                    std::set<int> sockets_set;
                    sockets_set.insert(profiler->cpuTopology[cpu_a].socket);
                    sockets_set.insert(profiler->cpuTopology[cpu_b].socket);
                    sockets_set.insert(profiler->cpuTopology[cpu_c].socket);
                    std::vector<int> sockets(sockets_set.begin(), sockets_set.end());

                    std::set<int> cores_set;
                    cores_set.insert(profiler->cpuTopology[cpu_a].core);
                    cores_set.insert(profiler->cpuTopology[cpu_b].core);
                    cores_set.insert(profiler->cpuTopology[cpu_c].core);
                    std::vector<int> cores(cores_set.begin(), cores_set.end());

                    std::set<int> dies_set;
                    dies_set.insert(profiler->cpuTopology[cpu_a].die);
                    dies_set.insert(profiler->cpuTopology[cpu_b].die);
                    dies_set.insert(profiler->cpuTopology[cpu_c].die);
                    std::vector<int> dies(dies_set.begin(), dies_set.end());

                    std::set<int> threads_set;
                    threads_set.insert(cpu_a);
                    threads_set.insert(cpu_b);
                    threads_set.insert(cpu_c);
                    std::vector<int> threads(threads_set.begin(), threads_set.end());
            
                    #if PRINT_FULL_STATS == 1
                        printf("Thread Pair 1 (A/C)\n");
                        results->results_a->print_statistics(sockets, dies, cores, threads, STIM_LEN);
                        printf("Thread Pair 2 (B/C)\n");
                        results->results_b->print_statistics(sockets, dies, cores, threads, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                    printf("Thread Pair 1 (A/C)\n");
                    print_results(results->results_a, sizeof(*shared_loc_a_1)*array_length, STIM_LEN);
                    printf("Thread Pair 2 (B/C)\n");
                    print_results(results->results_b, sizeof(*shared_loc_a_2)*array_length, STIM_LEN);
                    #endif
                }else{
                    #if PRINT_FULL_STATS
                    printf("Thread Pair 1 (A/C)\n");
                    results->results_a->print_statistics(0, 0, 0, cpu_a, STIM_LEN);
                    printf("Thread Pair 2 (B/C)\n");
                    results->results_b->print_statistics(0, 0, 0, cpu_c, STIM_LEN);
                    #endif

                    printf("Thread Pair 1 (A/C)\n");
                    print_results(results->results_a, sizeof(*shared_loc_a_1)*array_length, STIM_LEN);
                    printf("Thread Pair 2 (B/C)\n");
                    print_results(results->results_b, sizeof(*shared_loc_a_2)*array_length, STIM_LEN);
                }
            }
            else
            {
                print_results(results->results_a, cpu_a, cpu_c, sizeof(*shared_loc_a_1)*array_length, STIM_LEN, array_length, format, file_a, raw_file_a);
                print_results(results->results_b, cpu_b, cpu_c, sizeof(*shared_loc_a_2)*array_length, STIM_LEN, array_length, format, file_b, raw_file_b);
            }
        #endif

        //Clean Up
        free(shared_loc_a_1);
        free(shared_loc_b_1);
        free(shared_loc_a_2);
        free(shared_loc_b_2);
        delete arg_a;
        delete arg_b;
        delete reset_arg_1;

        return results;
    }

    void run_latency_dual_array_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, FILE* file = NULL, std::ofstream* raw_file=NULL)
    {
        //Print header
        time_t currentTime = time(NULL);
        struct tm * localT;
        localT = localtime(&currentTime);
        printf("Dual Memory Location - Array | Start Time: %s", asctime(localT));
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
            Results* latency_dual_array_kernel_results = run_latency_dual_array_kernel(profiler, cpu_a, cpu_b, array_length, false, format, file, raw_file);

            #if WRITE_CSV == 1
            fflush(file);
            #endif

            //Cleanup
            delete latency_dual_array_kernel_results;
        }

        printf("        ==========================================================================================\n");
    }

    void run_latency_dual_array_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, int cpu_d, std::vector<size_t> array_lengths, FILE* file_a = NULL, FILE* file_b = NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL)
    {
        //Print header
        time_t currentTime = time(NULL);
        struct tm * localT;
        localT = localtime(&currentTime);
        printf("Dual Memory Location - Simultanious - Array | Start Time: %s", asctime(localT));
        printf("        ========================================================================================================\n");
        printf("         CPU <-> CPU |  Transfer Length  |   One Way Latency (ns)   | Transaction Rate (MT/s) | Data Rate (Mbps)\n");
        printf("                     |(int32_t Elements) |       Avg, StdDev        |                         |                 \n");
        printf("        ========================================================================================================\n");

        #if WRITE_CSV == 1
        fprintf(file_a, "\"Transfer Length (int32_t Elements)\", \"One Way Latency (ns) - Avg\", \"One Way Latency (ns) - StdDev\", \"Transaction Rate (MT/s)\", \"Data Rate (Mbps)\"\n");
        fflush(file_a);
        fprintf(file_b, "\"Transfer Length (int32_t Elements)\", \"One Way Latency (ns) - Avg\", \"One Way Latency (ns) - StdDev\", \"Transaction Rate (MT/s)\", \"Data Rate (Mbps)\"\n");
        fflush(file_b);
        *raw_file_a << "\"Transfer Length (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
        *raw_file_b << "\"Transfer Length (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
        #endif

        std::string format = "         %3d <-> %-3d | %17d | %11.6f, %11.6f | %23.6f | %15.6f \n";

        for(int i = 0; i<array_lengths.size(); i++)
        {
            size_t array_length = array_lengths[i];
            SimultaniousResults* latency_dual_array_kernel_results_container = run_latency_dual_array_kernel(profiler, cpu_a, cpu_b, cpu_c, cpu_d, array_length, false, format, file_a, file_b, raw_file_a, raw_file_b);

            #if WRITE_CSV == 1
            fflush(file_a);
            fflush(file_b);
            #endif

            //Cleanup
            delete latency_dual_array_kernel_results_container->results_a;
            delete latency_dual_array_kernel_results_container->results_b;

            delete latency_dual_array_kernel_results_container;
        }

        printf("        ==========================================================================================\n");
    }

    void run_latency_dual_array_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, std::vector<size_t> array_lengths, FILE* file_a = NULL, FILE* file_b = NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL)
    {
        //Print header
        time_t currentTime = time(NULL);
        struct tm * localT;
        localT = localtime(&currentTime);
        printf("Dual Memory Location - Fan-in/Fan-out - Array | Start Time: %s", asctime(localT));
        printf("        ========================================================================================================\n");
        printf("         CPU <-> CPU |  Transfer Length  |   One Way Latency (ns)   | Transaction Rate (MT/s) | Data Rate (Mbps)\n");
        printf("                     |(int32_t Elements) |       Avg, StdDev        |                         |                 \n");
        printf("        ========================================================================================================\n");

        #if WRITE_CSV == 1
        fprintf(file_a, "\"Transfer Length (int32_t Elements)\", \"One Way Latency (ns) - Avg\", \"One Way Latency (ns) - StdDev\", \"Transaction Rate (MT/s)\", \"Data Rate (Mbps)\"\n");
        fflush(file_a);
        fprintf(file_b, "\"Transfer Length (int32_t Elements)\", \"One Way Latency (ns) - Avg\", \"One Way Latency (ns) - StdDev\", \"Transaction Rate (MT/s)\", \"Data Rate (Mbps)\"\n");
        fflush(file_b);
        *raw_file_a << "\"Transfer Length (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
        *raw_file_b << "\"Transfer Length (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
        #endif

        std::string format = "         %3d <-> %-3d | %17d | %11.6f, %11.6f | %23.6f | %15.6f \n";

        for(int i = 0; i<array_lengths.size(); i++)
        {
            size_t array_length = array_lengths[i];
            SimultaniousResults* latency_dual_array_kernel_results_container = run_latency_dual_array_kernel(profiler, cpu_a, cpu_b, cpu_c, array_length, false, format, file_a, file_b, raw_file_a, raw_file_b);

            #if WRITE_CSV == 1
            fflush(file_a);
            fflush(file_b);
            #endif

            //Cleanup
            delete latency_dual_array_kernel_results_container->results_a;
            delete latency_dual_array_kernel_results_container->results_b;

            delete latency_dual_array_kernel_results_container;
        }

        printf("        ==========================================================================================\n");
    }

    Results* run_latency_flow_ctrl_kernel(Profiler* profiler, int cpu_a, int cpu_b, size_t array_length, bool report_standalone=true, std::string format = "", FILE* file=NULL, std::ofstream* raw_file=NULL)
    {
        #if PRINT_TITLE == 1
        if(report_standalone)
        {
            printf("\n");
            time_t currentTime = time(NULL);
            struct tm * localT;
            localT = localtime(&currentTime);
            printf("Flow Control - Array | Start Time: %s", asctime(localT));
            printf("Array Length: %lu int32_t Elements\n", array_length);
        }
        #endif

        //Initialize
        size_t amountToAlloc = array_length*sizeof(int32_t);
        if(amountToAlloc % CACHE_LINE_SIZE != 0){
            amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
        }
        int32_t* shared_array_loc = (int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);

        size_t amountToAllocCursors = sizeof(std::atomic_int32_t);
        if(amountToAllocCursors % CACHE_LINE_SIZE != 0){
            amountToAllocCursors += (CACHE_LINE_SIZE - (amountToAllocCursors % CACHE_LINE_SIZE));
        }
        std::atomic_int32_t* shared_ack_loc = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_b);
        std::atomic_init(shared_ack_loc, 0);
        if(!std::atomic_is_lock_free(shared_ack_loc)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }

        //Init to 0
        for(size_t i = 0; i < array_length; i++)
        {
            shared_array_loc[i] = 0;
        }

        LatencyFlowCtrlKernelArgs* args = new LatencyFlowCtrlKernelArgs();
        args->array_shared_ptr = shared_array_loc;
        args->ack_shared_ptr = shared_ack_loc;
        args->length = array_length;

        Results* results = execute_client_server_kernel(profiler, latency_flow_ctrl_server_kernel, latency_flow_ctrl_client_kernel, latency_flow_ctrl_kernel_reset, args, args, args, cpu_a, cpu_b);

        #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1 || WRITE_CSV == 1
            if(report_standalone)
            {
                if(!profiler->cpuTopology.empty()){
                    std::vector<int> sockets;
                    int socket_a = profiler->cpuTopology[cpu_a].socket;
                    int socket_b = profiler->cpuTopology[cpu_b].socket;
                    sockets.push_back(socket_a);
                    if(socket_b != socket_a)
                    {
                        sockets.push_back(socket_b);
                    }

                    std::vector<int> cores;
                    int core_a = profiler->cpuTopology[cpu_a].core;
                    int core_b = profiler->cpuTopology[cpu_b].core;
                    cores.push_back(core_a);
                    if(core_a != core_b){
                        cores.push_back(core_b);
                    }

                    std::vector<int> dies;
                    int die_a = profiler->cpuTopology[cpu_a].die;
                    int die_b = profiler->cpuTopology[cpu_b].die;
                    dies.push_back(die_a);
                    if(die_a != die_b){
                        dies.push_back(die_b);
                    }

                    std::vector<int> threads;
                    threads.push_back(cpu_a);
                    if(cpu_a != cpu_b){
                        threads.push_back(cpu_b);
                    }
                
                    #if PRINT_FULL_STATS == 1
                        results->print_statistics(sockets, dies, cores, threads, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        print_results(results, sizeof(*shared_array_loc)*array_length, STIM_LEN/2); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                    #endif

                }else{
                        #if PRINT_FULL_STATS
                            results->print_statistics(0, 0, 0, cpu_a, STIM_LEN);
                        #endif

                        print_results(results, sizeof(*shared_array_loc)*array_length, STIM_LEN/2); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                }
            }
            else
            {
                print_results(results, sizeof(*shared_array_loc)*array_length, STIM_LEN/2, array_length, format, file, raw_file); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
            }
        #endif

        //Clean Up
        free(shared_array_loc);
        free(shared_ack_loc);
        delete args;

        return results;
    }

    SimultaniousResults* run_latency_flow_ctrl_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, int cpu_d, size_t array_length, bool report_standalone=true, std::string format = "", FILE* file_a=NULL, FILE* file_b=NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL)
    {
        #if PRINT_TITLE == 1
        if(report_standalone)
        {
            printf("\n");
            time_t currentTime = time(NULL);
            struct tm * localT;
            localT = localtime(&currentTime);
            printf("Flow Control - Simultanious - Array | Start Time: %s", asctime(localT));
            printf("Array Length: %lu int32_t Elements\n", array_length);
        }
        #endif

        //Initialize
        size_t amountToAlloc = array_length*sizeof(int32_t);
        if(amountToAlloc % CACHE_LINE_SIZE != 0){
            amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
        }
        int32_t* shared_array_loc_1 = (int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);
        int32_t* shared_array_loc_2 = (int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_c);

        size_t amountToAllocCursors = sizeof(std::atomic_int32_t);
        if(amountToAllocCursors % CACHE_LINE_SIZE != 0){
            amountToAllocCursors += (CACHE_LINE_SIZE - (amountToAllocCursors % CACHE_LINE_SIZE));
        }
        std::atomic_int32_t* shared_ack_loc_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_b);
        std::atomic_int32_t* shared_ack_loc_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_d);
        std::atomic_init(shared_ack_loc_1, 0);
        if(!std::atomic_is_lock_free(shared_ack_loc_1)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }
        std::atomic_init(shared_ack_loc_2, 0);
        if(!std::atomic_is_lock_free(shared_ack_loc_2)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }

        //Init to 0
        for(size_t i = 0; i < array_length; i++)
        {
            shared_array_loc_1[i] = 0;
            shared_array_loc_2[i] = 0;
        }

        LatencyFlowCtrlKernelArgs* args_1 = new LatencyFlowCtrlKernelArgs();
        args_1->array_shared_ptr = shared_array_loc_1;
        args_1->ack_shared_ptr = shared_ack_loc_1;
        args_1->length = array_length;

        LatencyFlowCtrlKernelArgs* args_2 = new LatencyFlowCtrlKernelArgs();
        args_2->array_shared_ptr = shared_array_loc_2;
        args_2->ack_shared_ptr = shared_ack_loc_2;
        args_2->length = array_length;

        //                                                                                                                                                                          S1,     S2,     C1,     C2,     R1,     R2
        SimultaniousResults* results = execute_client_server_kernel(profiler, latency_flow_ctrl_server_kernel, latency_flow_ctrl_client_kernel, latency_flow_ctrl_kernel_reset, args_1, args_2, args_1, args_2, args_1, args_2, cpu_a, cpu_b, cpu_c, cpu_d);

        #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1 || WRITE_CSV == 1
            if(report_standalone)
            {
                if(!profiler->cpuTopology.empty()){
                    std::set<int> sockets_set;
                    sockets_set.insert(profiler->cpuTopology[cpu_a].socket);
                    sockets_set.insert(profiler->cpuTopology[cpu_b].socket);
                    sockets_set.insert(profiler->cpuTopology[cpu_c].socket);
                    sockets_set.insert(profiler->cpuTopology[cpu_d].socket);
                    std::vector<int> sockets(sockets_set.begin(), sockets_set.end());

                    std::set<int> cores_set;
                    cores_set.insert(profiler->cpuTopology[cpu_a].core);
                    cores_set.insert(profiler->cpuTopology[cpu_b].core);
                    cores_set.insert(profiler->cpuTopology[cpu_c].core);
                    cores_set.insert(profiler->cpuTopology[cpu_d].core);
                    std::vector<int> cores(cores_set.begin(), cores_set.end());

                    std::set<int> dies_set;
                    dies_set.insert(profiler->cpuTopology[cpu_a].die);
                    dies_set.insert(profiler->cpuTopology[cpu_b].die);
                    dies_set.insert(profiler->cpuTopology[cpu_c].die);
                    dies_set.insert(profiler->cpuTopology[cpu_d].die);
                    std::vector<int> dies(dies_set.begin(), dies_set.end());

                    std::set<int> threads_set;
                    threads_set.insert(cpu_a);
                    threads_set.insert(cpu_b);
                    threads_set.insert(cpu_c);
                    threads_set.insert(cpu_d);
                    std::vector<int> threads(threads_set.begin(), threads_set.end());
            
                    #if PRINT_FULL_STATS == 1
                        printf("Thread Pair 1 (A/B)\n");
                        results->results_a->print_statistics(sockets, dies, cores, threads, STIM_LEN);
                        printf("Thread Pair 2 (C/D)\n");
                        results->results_b->print_statistics(sockets, dies, cores, threads, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        printf("Thread Pair 1 (A/B)\n");
                        print_results(results->results_a, sizeof(*shared_array_loc_1)*array_length, STIM_LEN/2); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                        printf("Thread Pair 2 (C/D)\n");
                        print_results(results->results_b, sizeof(*shared_array_loc_2)*array_length, STIM_LEN/2);
                    #endif
                }else{
                    #if PRINT_FULL_STATS
                        printf("Thread Pair 1 (A/B)\n");
                        results->results_a->print_statistics(0, 0, 0, cpu_a, STIM_LEN);
                        printf("Thread Pair 2 (C/D)\n");
                        results->results_b->print_statistics(0, 0, 0, cpu_c, STIM_LEN);
                    #endif

                    printf("Thread Pair 1 (A/B)\n");
                    print_results(results->results_a, sizeof(*shared_array_loc_1)*array_length, STIM_LEN/2); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                    printf("Thread Pair 2 (C/D)\n");
                    print_results(results->results_b, sizeof(*shared_array_loc_2)*array_length, STIM_LEN/2);
                }
            }
            else
            {
                print_results(results->results_a, cpu_a, cpu_b, sizeof(*shared_array_loc_1)*array_length, STIM_LEN/2, array_length, format, file_a, raw_file_a); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                print_results(results->results_b, cpu_c, cpu_d, sizeof(*shared_array_loc_2)*array_length, STIM_LEN/2, array_length, format, file_b, raw_file_b);
            }
        #endif

        //Clean Up
        free(shared_array_loc_1);
        free(shared_array_loc_2);
        free(shared_ack_loc_1);
        free(shared_ack_loc_2);
        delete args_1;
        delete args_2;

        return results;
    }

    /**
     * This version tests for fan-in when the comunication mechanism uses the flow control model.
     * 2 servers feed to 1 client.  The servers conduct the measurment
     */
    SimultaniousResults* run_latency_flow_ctrl_fanin_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, size_t array_length, bool report_standalone=true, std::string format = "", FILE* file_a=NULL, FILE* file_b=NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL)
    {
        #if PRINT_TITLE == 1
        if(report_standalone)
        {
            printf("\n");
            time_t currentTime = time(NULL);
            struct tm * localT;
            localT = localtime(&currentTime);
            printf("Flow Control - Fan-in - Array | Start Time: %s", asctime(localT));
            printf("Array Length: %lu int32_t Elements\n", array_length);
        }
        #endif

        //Initialize
        size_t amountToAlloc = array_length*sizeof(int32_t);
        if(amountToAlloc % CACHE_LINE_SIZE != 0){
            amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
        }
        int32_t* shared_array_loc_1 = (int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);
        int32_t* shared_array_loc_2 = (int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_b);

        size_t amountToAllocCursors = sizeof(std::atomic_int32_t);
        if(amountToAllocCursors % CACHE_LINE_SIZE != 0){
            amountToAllocCursors += (CACHE_LINE_SIZE - (amountToAllocCursors % CACHE_LINE_SIZE));
        }
        std::atomic_int32_t* shared_ack_loc_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_c);
        std::atomic_int32_t* shared_ack_loc_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_c);
        std::atomic_init(shared_ack_loc_1, 0);
        if(!std::atomic_is_lock_free(shared_ack_loc_1)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }
        std::atomic_init(shared_ack_loc_2, 0);
        if(!std::atomic_is_lock_free(shared_ack_loc_2)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }

        //Init to 0
        for(size_t i = 0; i < array_length; i++)
        {
            shared_array_loc_1[i] = 0;
            shared_array_loc_2[i] = 0;
        }

        //2 Servers
        LatencyFlowCtrlKernelArgs* args_a = new LatencyFlowCtrlKernelArgs();
        args_a->array_shared_ptr = shared_array_loc_1;
        args_a->ack_shared_ptr = shared_ack_loc_1;
        args_a->length = array_length;

        LatencyFlowCtrlKernelArgs* args_b = new LatencyFlowCtrlKernelArgs();
        args_b->array_shared_ptr = shared_array_loc_2;
        args_b->ack_shared_ptr = shared_ack_loc_2;
        args_b->length = array_length;

        //1 Client
        LatencyFlowCtrlJoinKernelArgs* args_c = new LatencyFlowCtrlJoinKernelArgs();
        args_c->array_shared_ptr_a = shared_array_loc_1;
        args_c->ack_shared_ptr_a = shared_ack_loc_1;
        args_c->length_a = array_length;

        args_c->array_shared_ptr_b = shared_array_loc_2;
        args_c->ack_shared_ptr_b = shared_ack_loc_2;
        args_c->length_b = array_length;

        SimultaniousResults* results = execute_kernel_fanin_server_measure(profiler, latency_flow_ctrl_server_kernel, latency_flow_ctrl_client_join_kernel, latency_flow_ctrl_join_kernel_reset, args_a, args_b, args_c, args_c, cpu_a, cpu_b, cpu_c); //Reset args are same as client join args

        #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1 || WRITE_CSV == 1
            if(report_standalone)
            {
                if(!profiler->cpuTopology.empty()){
                    std::set<int> sockets_set;
                    sockets_set.insert(profiler->cpuTopology[cpu_a].socket);
                    sockets_set.insert(profiler->cpuTopology[cpu_b].socket);
                    sockets_set.insert(profiler->cpuTopology[cpu_c].socket);
                    std::vector<int> sockets(sockets_set.begin(), sockets_set.end());

                    std::set<int> cores_set;
                    cores_set.insert(profiler->cpuTopology[cpu_a].core);
                    cores_set.insert(profiler->cpuTopology[cpu_b].core);
                    cores_set.insert(profiler->cpuTopology[cpu_c].core);
                    std::vector<int> cores(cores_set.begin(), cores_set.end());

                    std::set<int> dies_set;
                    dies_set.insert(profiler->cpuTopology[cpu_a].die);
                    dies_set.insert(profiler->cpuTopology[cpu_b].die);
                    dies_set.insert(profiler->cpuTopology[cpu_c].die);
                    std::vector<int> dies(dies_set.begin(), dies_set.end());

                    std::set<int> threads_set;
                    threads_set.insert(cpu_a);
                    threads_set.insert(cpu_b);
                    threads_set.insert(cpu_c);
                    std::vector<int> threads(threads_set.begin(), threads_set.end());
            
                    #if PRINT_FULL_STATS == 1
                        printf("Thread Pair 1 (A/C)\n");
                        results->results_a->print_statistics(sockets, dies, cores, threads, STIM_LEN);
                        printf("Thread Pair 2 (B/C)\n");
                        results->results_b->print_statistics(sockets, dies, cores, threads, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        printf("Thread Pair 1 (A/C)\n");
                        print_results(results->results_a, sizeof(*shared_array_loc_1)*array_length, STIM_LEN/2); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                        printf("Thread Pair 2 (B/C)\n");
                        print_results(results->results_b, sizeof(*shared_array_loc_2)*array_length, STIM_LEN/2);
                    #endif

                }else{
                    #if PRINT_FULL_STATS
                        printf("Thread Pair 1 (A/C)\n");
                        results->results_a->print_statistics(0, 0, 0, cpu_a, STIM_LEN);
                        printf("Thread Pair 2 (B/C)\n");
                        results->results_b->print_statistics(0, 0, 0, cpu_c, STIM_LEN);
                    #endif

                    printf("Thread Pair 1 (A/C)\n");
                    print_results(results->results_a, sizeof(*shared_array_loc_1)*array_length, STIM_LEN/2); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                    printf("Thread Pair 2 (B/C)\n");
                    print_results(results->results_b, sizeof(*shared_array_loc_2)*array_length, STIM_LEN/2);
                }
            }
            else
            {
                print_results(results->results_a, cpu_a, cpu_c, sizeof(*shared_array_loc_1)*array_length, STIM_LEN/2, array_length, format, file_a, raw_file_a); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                print_results(results->results_b, cpu_b, cpu_c, sizeof(*shared_array_loc_2)*array_length, STIM_LEN/2, array_length, format, file_b, raw_file_b);
            }
        #endif

        //Clean Up
        free(shared_array_loc_1);
        free(shared_array_loc_2);
        free(shared_ack_loc_1);
        free(shared_ack_loc_2);
        delete args_a;
        delete args_b;
        delete args_c;

        return results;
    }

    /**
     * This version tests for fan-out when the comunication mechanism uses the flow control model.
     * 1 server feed to 2 clients.  The clients conduct the measurment
     * 
     * TODO: Small inaccuracy may occur on the first transaction due to servers starting first
     */
    SimultaniousResults* run_latency_flow_ctrl_fanout_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, size_t array_length, bool report_standalone=true, std::string format = "", FILE* file_a=NULL, FILE* file_b=NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL)
    {
        #if PRINT_TITLE == 1
        if(report_standalone)
        {
            printf("\n");
            time_t currentTime = time(NULL);
            struct tm * localT;
            localT = localtime(&currentTime);
            printf("Flow Control - Fan-out - Array | Start Time: %s", asctime(localT));
            printf("Array Length: %lu int32_t Elements\n", array_length);
        }
        #endif

        //Initialize
        size_t amountToAlloc = array_length*sizeof(int32_t);
        if(amountToAlloc % CACHE_LINE_SIZE != 0){
            amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
        }
        int32_t* shared_array_loc_1 = (int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);
        int32_t* shared_array_loc_2 = (int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);

        size_t amountToAllocCursors = sizeof(std::atomic_int32_t);
        if(amountToAllocCursors % CACHE_LINE_SIZE != 0){
            amountToAllocCursors += (CACHE_LINE_SIZE - (amountToAllocCursors % CACHE_LINE_SIZE));
        }
        std::atomic_int32_t* shared_ack_loc_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_b);
        std::atomic_int32_t* shared_ack_loc_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_c);
        std::atomic_init(shared_ack_loc_1, 0);
        if(!std::atomic_is_lock_free(shared_ack_loc_1)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }
        std::atomic_init(shared_ack_loc_2, 0);
        if(!std::atomic_is_lock_free(shared_ack_loc_2)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }

        //Init to 0
        for(size_t i = 0; i < array_length; i++)
        {
            shared_array_loc_1[i] = 0;
            shared_array_loc_2[i] = 0;
        }

        //1 Server
        LatencyFlowCtrlJoinKernelArgs* args_a = new LatencyFlowCtrlJoinKernelArgs();
        args_a->array_shared_ptr_a = shared_array_loc_1;
        args_a->ack_shared_ptr_a = shared_ack_loc_1;
        args_a->length_a = array_length;

        args_a->array_shared_ptr_b = shared_array_loc_2;
        args_a->ack_shared_ptr_b = shared_ack_loc_2;
        args_a->length_b = array_length;

        //2 Clients
        LatencyFlowCtrlKernelArgs* args_b = new LatencyFlowCtrlKernelArgs();
        args_b->array_shared_ptr = shared_array_loc_1;
        args_b->ack_shared_ptr = shared_ack_loc_1;
        args_b->length = array_length;

        LatencyFlowCtrlKernelArgs* args_c = new LatencyFlowCtrlKernelArgs();
        args_c->array_shared_ptr = shared_array_loc_2;
        args_c->ack_shared_ptr = shared_ack_loc_2;
        args_c->length = array_length;

        SimultaniousResults* results = execute_kernel_fanout_client_measure(profiler, latency_flow_ctrl_server_join_kernel, latency_flow_ctrl_client_kernel, latency_flow_ctrl_join_kernel_reset, args_a, args_b, args_c, args_a, cpu_a, cpu_b, cpu_c); //Reset args are same as server join args

        #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1 || WRITE_CSV == 1
            if(report_standalone)
            {
                if(!profiler->cpuTopology.empty()){
                    std::set<int> sockets_set;
                    sockets_set.insert(profiler->cpuTopology[cpu_a].socket);
                    sockets_set.insert(profiler->cpuTopology[cpu_b].socket);
                    sockets_set.insert(profiler->cpuTopology[cpu_c].socket);
                    std::vector<int> sockets(sockets_set.begin(), sockets_set.end());

                    std::set<int> cores_set;
                    cores_set.insert(profiler->cpuTopology[cpu_a].core);
                    cores_set.insert(profiler->cpuTopology[cpu_b].core);
                    cores_set.insert(profiler->cpuTopology[cpu_c].core);
                    std::vector<int> cores(cores_set.begin(), cores_set.end());

                    std::set<int> dies_set;
                    dies_set.insert(profiler->cpuTopology[cpu_a].die);
                    dies_set.insert(profiler->cpuTopology[cpu_b].die);
                    dies_set.insert(profiler->cpuTopology[cpu_c].die);
                    std::vector<int> dies(dies_set.begin(), dies_set.end());

                    std::set<int> threads_set;
                    threads_set.insert(cpu_a);
                    threads_set.insert(cpu_b);
                    threads_set.insert(cpu_c);
                    std::vector<int> threads(threads_set.begin(), threads_set.end());
            
                    #if PRINT_FULL_STATS == 1
                        printf("Thread Pair 1 (A/B)\n");
                        results->results_a->print_statistics(sockets, dies, cores, threads, STIM_LEN);
                        printf("Thread Pair 2 (A/C)\n");
                        results->results_b->print_statistics(sockets, dies, cores, threads, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        printf("Thread Pair 1 (A/B)\n");
                        print_results(results->results_a, sizeof(*shared_array_loc_1)*array_length, STIM_LEN/2); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                        printf("Thread Pair 2 (A/C)\n");
                        print_results(results->results_b, sizeof(*shared_array_loc_2)*array_length, STIM_LEN/2);
                    #endif

                }else{
                        #if PRINT_FULL_STATS
                            printf("Thread Pair 1 (A/B)\n");
                            results->results_a->print_statistics(0, 0, 0, cpu_a, STIM_LEN);
                            printf("Thread Pair 2 (A/C)\n");
                            results->results_b->print_statistics(0, 0, 0, cpu_c, STIM_LEN);
                        #endif

                        printf("Thread Pair 1 (A/B)\n");
                        print_results(results->results_a, sizeof(*shared_array_loc_1)*array_length, STIM_LEN/2); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                        printf("Thread Pair 2 (A/C)\n");
                        print_results(results->results_b, sizeof(*shared_array_loc_2)*array_length, STIM_LEN/2);
                }
            }
            else
            {
                print_results(results->results_a, cpu_a, cpu_b, sizeof(*shared_array_loc_1)*array_length, STIM_LEN/2, array_length, format, file_a, raw_file_a); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                print_results(results->results_b, cpu_a, cpu_c, sizeof(*shared_array_loc_2)*array_length, STIM_LEN/2, array_length, format, file_b, raw_file_b);
            }
        #endif

        //Clean Up
        free(shared_array_loc_1);
        free(shared_array_loc_2);
        free(shared_ack_loc_1);
        free(shared_ack_loc_2);
        delete args_a;
        delete args_b;
        delete args_c;

        return results;
    }

    void run_latency_flow_ctrl_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, FILE* file = NULL, std::ofstream* raw_file=NULL)
    {
        //Print header
        time_t currentTime = time(NULL);
        struct tm * localT;
        localT = localtime(&currentTime);
        printf("Flow Control - Array | Start Time: %s", asctime(localT));
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
            Results* latency_fifo_kernel_results = run_latency_flow_ctrl_kernel(profiler, cpu_a, cpu_b, array_length, false, format, file, raw_file);

            #if WRITE_CSV == 1
            fflush(file);
            #endif

            //Cleanup
            delete latency_fifo_kernel_results;
        }

        printf("        ==========================================================================================\n");
    }

    void run_latency_flow_ctrl_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, int cpu_d, std::vector<size_t> array_lengths, FILE* file_a = NULL, FILE* file_b = NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL)
    {
        //Print header
    time_t currentTime = time(NULL);
        struct tm * localT;
        localT = localtime(&currentTime);
        printf("Flow Control - Simultanious - Array | Start Time: %s", asctime(localT));
        printf("        ========================================================================================================\n");
        printf("         CPU <-> CPU |  Transfer Length  |   One Way Latency (ns)   | Transaction Rate (MT/s) | Data Rate (Mbps)\n");
        printf("                     |(int32_t Elements) |       Avg, StdDev        |                         |                 \n");
        printf("        ========================================================================================================\n");

        #if WRITE_CSV == 1
        fprintf(file_a, "\"Transfer Length (int32_t Elements)\", \"Round Trip Latency (ns) - Avg\", \"Round Trip Latency (ns) - StdDev\", \"Transaction Rate (MT/s)\", \"Data Rate (Mbps)\"\n");
        fflush(file_a);
        fprintf(file_b, "\"Transfer Length (int32_t Elements)\", \"Round Trip Latency (ns) - Avg\", \"Round Trip Latency (ns) - StdDev\", \"Transaction Rate (MT/s)\", \"Data Rate (Mbps)\"\n");
        fflush(file_b);
        *raw_file_a << "\"Transfer Length (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
        *raw_file_b << "\"Transfer Length (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
        #endif

        std::string format = "         %3d <-> %-3d | %17d | %11.6f, %11.6f | %23.6f | %15.6f \n";

        for(int i = 0; i<array_lengths.size(); i++)
        {
            size_t array_length = array_lengths[i];
            SimultaniousResults* latency_fifo_kernel_results = run_latency_flow_ctrl_kernel(profiler, cpu_a, cpu_b, cpu_c, cpu_d, array_length, false, format, file_a, file_b, raw_file_a, raw_file_b);

            #if WRITE_CSV == 1
            fflush(file_a);
            fflush(file_b);
            #endif

            //Cleanup
            delete latency_fifo_kernel_results->results_a;
            delete latency_fifo_kernel_results->results_b;
            delete latency_fifo_kernel_results;
        }

        printf("        ==========================================================================================\n");
    }

    void run_latency_flow_ctrl_fanin_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, std::vector<size_t> array_lengths, FILE* file_a = NULL, FILE* file_b = NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL)
    {
        //Print header
        time_t currentTime = time(NULL);
        struct tm * localT;
        localT = localtime(&currentTime);
        printf("Flow Control - Fan-in - Array | Start Time: %s", asctime(localT));
        printf("        ========================================================================================================\n");
        printf("         CPU <-> CPU |  Transfer Length  |   One Way Latency (ns)   | Transaction Rate (MT/s) | Data Rate (Mbps)\n");
        printf("                     |(int32_t Elements) |       Avg, StdDev        |                         |                 \n");
        printf("        ========================================================================================================\n");

        #if WRITE_CSV == 1
        fprintf(file_a, "\"Transfer Length (int32_t Elements)\", \"Round Trip Latency (ns) - Avg\", \"Round Trip Latency (ns) - StdDev\", \"Transaction Rate (MT/s)\", \"Data Rate (Mbps)\"\n");
        fflush(file_a);
        fprintf(file_b, "\"Transfer Length (int32_t Elements)\", \"Round Trip Latency (ns) - Avg\", \"Round Trip Latency (ns) - StdDev\", \"Transaction Rate (MT/s)\", \"Data Rate (Mbps)\"\n");
        fflush(file_b);
        *raw_file_a << "\"Transfer Length (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
        *raw_file_b << "\"Transfer Length (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
        #endif

        std::string format = "          %3d -> %-3d | %17d | %11.6f, %11.6f | %23.6f | %15.6f \n";

        for(int i = 0; i<array_lengths.size(); i++)
        {
            size_t array_length = array_lengths[i];
            SimultaniousResults* latency_fifo_kernel_results = run_latency_flow_ctrl_fanin_kernel(profiler, cpu_a, cpu_b, cpu_c, array_length, false, format, file_a, file_b, raw_file_a, raw_file_b);

            #if WRITE_CSV == 1
            fflush(file_a);
            fflush(file_b);
            #endif

            //Cleanup
            delete latency_fifo_kernel_results->results_a;
            delete latency_fifo_kernel_results->results_b;
            delete latency_fifo_kernel_results;
        }

        printf("        ==========================================================================================\n");
    }

    void run_latency_flow_ctrl_fanout_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, std::vector<size_t> array_lengths, FILE* file_a = NULL, FILE* file_b = NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL)
    {
        //Print header
        time_t currentTime = time(NULL);
        struct tm * localT;
        localT = localtime(&currentTime);
        printf("Flow Control - Fan-out - Array | Start Time: %s", asctime(localT));
        printf("        ========================================================================================================\n");
        printf("         CPU <-> CPU |  Transfer Length  |   One Way Latency (ns)   | Transaction Rate (MT/s) | Data Rate (Mbps)\n");
        printf("                     |(int32_t Elements) |       Avg, StdDev        |                         |                 \n");
        printf("        ========================================================================================================\n");

        #if WRITE_CSV == 1
        fprintf(file_a, "\"Transfer Length (int32_t Elements)\", \"Round Trip Latency (ns) - Avg\", \"Round Trip Latency (ns) - StdDev\", \"Transaction Rate (MT/s)\", \"Data Rate (Mbps)\"\n");
        fflush(file_a);
        fprintf(file_b, "\"Transfer Length (int32_t Elements)\", \"Round Trip Latency (ns) - Avg\", \"Round Trip Latency (ns) - StdDev\", \"Transaction Rate (MT/s)\", \"Data Rate (Mbps)\"\n");
        fflush(file_b);
        *raw_file_a << "\"Transfer Length (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
        *raw_file_b << "\"Transfer Length (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
        #endif

        std::string format = "          %3d -> %-3d | %17d | %11.6f, %11.6f | %23.6f | %15.6f \n";

        for(int i = 0; i<array_lengths.size(); i++)
        {
            size_t array_length = array_lengths[i];
            SimultaniousResults* latency_fifo_kernel_results = run_latency_flow_ctrl_fanout_kernel(profiler, cpu_a, cpu_b, cpu_c, array_length, false, format, file_a, file_b, raw_file_a, raw_file_b);

            #if WRITE_CSV == 1
            fflush(file_a);
            fflush(file_b);
            #endif

            //Cleanup
            delete latency_fifo_kernel_results->results_a;
            delete latency_fifo_kernel_results->results_b;
            delete latency_fifo_kernel_results;
        }

        printf("        ==========================================================================================\n");
    }

    Results* run_latency_flow_ctrl_blocked_read_kernel(Profiler* profiler, int cpu_a, int cpu_b, size_t array_length, bool report_standalone=true, std::string format = "", FILE* file=NULL, std::ofstream* raw_file=NULL)
    {
        #if PRINT_TITLE == 1
        if(report_standalone)
        {
            printf("\n");
            time_t currentTime = time(NULL);
            struct tm * localT;
            localT = localtime(&currentTime);
            printf("Flow Control Blocked Read - Array | Start Time: %s", asctime(localT));
            printf("Array Length: %lu int32_t Elements\n", array_length);
        }
        #endif

        //Initialize
        size_t amountToAlloc = array_length*sizeof(int32_t);
        if(amountToAlloc % CACHE_LINE_SIZE != 0){
            amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
        }
        int32_t* shared_array_loc = (int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);

        size_t amountToAllocCursors = sizeof(std::atomic_int32_t);
        if(amountToAllocCursors % CACHE_LINE_SIZE != 0){
            amountToAllocCursors += (CACHE_LINE_SIZE - (amountToAllocCursors % CACHE_LINE_SIZE));
        }
        std::atomic_int32_t* shared_ack_loc = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_b);
        std::atomic_int32_t* shared_valid_loc = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_a);
        std::atomic_init(shared_ack_loc, 0);
        if(!std::atomic_is_lock_free(shared_ack_loc)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }
        std::atomic_init(shared_valid_loc, 0);
        if(!std::atomic_is_lock_free(shared_valid_loc)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }

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

        Results* results = execute_client_server_kernel(profiler, latency_flow_ctrl_blocked_read_server_kernel, latency_flow_ctrl_blocked_read_client_kernel, latency_flow_ctrl_blocked_read_kernel_reset, args, args, args, cpu_a, cpu_b);

        #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1 || WRITE_CSV == 1
            if(report_standalone)
            {
                if(!profiler->cpuTopology.empty()){
                    std::vector<int> sockets;
                    int socket_a = profiler->cpuTopology[cpu_a].socket;
                    int socket_b = profiler->cpuTopology[cpu_b].socket;
                    sockets.push_back(socket_a);
                    if(socket_b != socket_a)
                    {
                        sockets.push_back(socket_b);
                    }

                    std::vector<int> cores;
                    int core_a = profiler->cpuTopology[cpu_a].core;
                    int core_b = profiler->cpuTopology[cpu_b].core;
                    cores.push_back(core_a);
                    if(core_a != core_b){
                        cores.push_back(core_b);
                    }

                    std::vector<int> dies;
                    int die_a = profiler->cpuTopology[cpu_a].die;
                    int die_b = profiler->cpuTopology[cpu_b].die;
                    dies.push_back(die_a);
                    if(die_a != die_b){
                        dies.push_back(die_b);
                    }

                    std::vector<int> threads;
                    threads.push_back(cpu_a);
                    if(cpu_a != cpu_b){
                        threads.push_back(cpu_b);
                    }
            
                    #if PRINT_FULL_STATS == 1
                        results->print_statistics(sockets, dies, cores, threads, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        print_results(results, sizeof(*shared_array_loc)*array_length, STIM_LEN/2); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                    #endif
                }else{
                    #if PRINT_FULL_STATS
                        results->print_statistics(0, 0, 0, cpu_a, STIM_LEN);
                    #endif

                    print_results(results, sizeof(*shared_array_loc)*array_length, STIM_LEN/2); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                }
            }
            else
            {
                print_results(results, sizeof(*shared_array_loc)*array_length, STIM_LEN/2, array_length, format, file, raw_file); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
            }
        #endif

        //Clean Up
        free(shared_array_loc);
        free(shared_ack_loc);
        free(shared_valid_loc);
        delete args;

        return results;
    }

    SimultaniousResults* run_latency_flow_ctrl_blocked_read_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, int cpu_d, size_t array_length, bool report_standalone=true, std::string format = "", FILE* file_a=NULL, FILE* file_b=NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL)
    {
        #if PRINT_TITLE == 1
        if(report_standalone)
        {
            printf("\n");
            time_t currentTime = time(NULL);
            struct tm * localT;
            localT = localtime(&currentTime);
            printf("Flow Control Blocked Read - Simultanious - Array | Start Time: %s", asctime(localT));
            printf("Flow Control Blocked Read - Simultanious - Array\n");
            printf("Array Length: %lu int32_t Elements\n", array_length);
        }
        #endif

        //Initialize
        size_t amountToAlloc = array_length*sizeof(int32_t);
        if(amountToAlloc % CACHE_LINE_SIZE != 0){
            amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
        }
        int32_t* shared_array_loc_1 = (int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);
        int32_t* shared_array_loc_2 = (int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_c);

        size_t amountToAllocCursors = sizeof(std::atomic_int32_t);
        if(amountToAllocCursors % CACHE_LINE_SIZE != 0){
            amountToAllocCursors += (CACHE_LINE_SIZE - (amountToAllocCursors % CACHE_LINE_SIZE));
        }
        std::atomic_int32_t* shared_ack_loc_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_b);
        std::atomic_int32_t* shared_valid_loc_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_a);
        std::atomic_int32_t* shared_ack_loc_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_d);
        std::atomic_int32_t* shared_valid_loc_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_c);
        std::atomic_init(shared_ack_loc_1, 0);
        if(!std::atomic_is_lock_free(shared_ack_loc_1)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }
        std::atomic_init(shared_valid_loc_1, 0);
        if(!std::atomic_is_lock_free(shared_valid_loc_1)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }
        std::atomic_init(shared_ack_loc_2, 0);
        if(!std::atomic_is_lock_free(shared_ack_loc_2)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }
        std::atomic_init(shared_valid_loc_2, 0);
        if(!std::atomic_is_lock_free(shared_valid_loc_2)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }

        //Init to 0
        for(size_t i = 0; i < array_length; i++)
        {
            shared_array_loc_1[i] = 0;
            shared_array_loc_2[i] = 0;
        }

        LatencyFlowCtrlBlockedReadKernelArgs* args_1 = new LatencyFlowCtrlBlockedReadKernelArgs();
        args_1->array_shared_ptr = shared_array_loc_1;
        args_1->ack_shared_ptr = shared_ack_loc_1;
        args_1->valid_shared_ptr = shared_valid_loc_1;
        args_1->length = array_length;

        LatencyFlowCtrlBlockedReadKernelArgs* args_2 = new LatencyFlowCtrlBlockedReadKernelArgs();
        args_2->array_shared_ptr = shared_array_loc_2;
        args_2->ack_shared_ptr = shared_ack_loc_2;
        args_2->valid_shared_ptr = shared_valid_loc_2;
        args_2->length = array_length;

        //                                                                                                                                                                                                                 S1,     S2,     C1,     C2,     R1,     R2
        SimultaniousResults* results = execute_client_server_kernel(profiler, latency_flow_ctrl_blocked_read_server_kernel, latency_flow_ctrl_blocked_read_client_kernel, latency_flow_ctrl_blocked_read_kernel_reset, args_1, args_2, args_1, args_2, args_1, args_2, cpu_a, cpu_b, cpu_c, cpu_d);

        #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1 || WRITE_CSV == 1
            if(report_standalone)
            {
                if(!profiler->cpuTopology.empty()){
                    std::set<int> sockets_set;
                    sockets_set.insert(profiler->cpuTopology[cpu_a].socket);
                    sockets_set.insert(profiler->cpuTopology[cpu_b].socket);
                    sockets_set.insert(profiler->cpuTopology[cpu_c].socket);
                    sockets_set.insert(profiler->cpuTopology[cpu_d].socket);
                    std::vector<int> sockets(sockets_set.begin(), sockets_set.end());

                    std::set<int> cores_set;
                    cores_set.insert(profiler->cpuTopology[cpu_a].core);
                    cores_set.insert(profiler->cpuTopology[cpu_b].core);
                    cores_set.insert(profiler->cpuTopology[cpu_c].core);
                    cores_set.insert(profiler->cpuTopology[cpu_d].core);
                    std::vector<int> cores(cores_set.begin(), cores_set.end());

                    std::set<int> dies_set;
                    dies_set.insert(profiler->cpuTopology[cpu_a].die);
                    dies_set.insert(profiler->cpuTopology[cpu_b].die);
                    dies_set.insert(profiler->cpuTopology[cpu_c].die);
                    dies_set.insert(profiler->cpuTopology[cpu_d].die);
                    std::vector<int> dies(dies_set.begin(), dies_set.end());

                    std::set<int> threads_set;
                    threads_set.insert(cpu_a);
                    threads_set.insert(cpu_b);
                    threads_set.insert(cpu_c);
                    threads_set.insert(cpu_d);
                    std::vector<int> threads(threads_set.begin(), threads_set.end());
            
                    #if PRINT_FULL_STATS == 1
                        printf("Thread Pair 1 (A/B)\n");
                        results->results_a->print_statistics(sockets, dies, cores, threads, STIM_LEN);
                        printf("Thread Pair 2 (C/D)\n");
                        results->results_b->print_statistics(sockets, dies, cores, threads, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        printf("Thread Pair 1 (A/B)\n");
                        print_results(results->results_a, sizeof(*shared_array_loc_1)*array_length, STIM_LEN/2); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                        printf("Thread Pair 2 (C/D)\n");
                        print_results(results->results_b, sizeof(*shared_array_loc_2)*array_length, STIM_LEN/2);
                    #endif
                }else{
                    #if PRINT_FULL_STATS
                        printf("Thread Pair 1 (A/B)\n");
                        results->results_a->print_statistics(0, 0, 0, cpu_a, STIM_LEN);
                        printf("Thread Pair 2 (C/D)\n");
                        results->results_b->print_statistics(0, 0, 0, cpu_c, STIM_LEN);
                    #endif

                    printf("Thread Pair 1 (A/B)\n");
                    print_results(results->results_a, sizeof(*shared_array_loc_1)*array_length, STIM_LEN/2); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                    printf("Thread Pair 2 (C/D)\n");
                    print_results(results->results_b, sizeof(*shared_array_loc_2)*array_length, STIM_LEN/2);
                }
            }
            else
            {
                print_results(results->results_a, cpu_a, cpu_b, sizeof(*shared_array_loc_1)*array_length, STIM_LEN/2, array_length, format, file_a, raw_file_a); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                print_results(results->results_b, cpu_c, cpu_d, sizeof(*shared_array_loc_2)*array_length, STIM_LEN/2, array_length, format, file_b, raw_file_b);
            }
        #endif

        //Clean Up
        free(shared_array_loc_1);
        free(shared_array_loc_2);
        free(shared_ack_loc_1);
        free(shared_ack_loc_2);
        free(shared_valid_loc_1);
        free(shared_valid_loc_2);
        delete args_1;
        delete args_2;

        return results;
    }

    SimultaniousResults* run_latency_flow_ctrl_blocked_read_fanin_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, size_t array_length, bool report_standalone=true, std::string format = "", FILE* file_a=NULL, FILE* file_b=NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL)
    {
        #if PRINT_TITLE == 1
        if(report_standalone)
        {
            printf("\n");
            time_t currentTime = time(NULL);
            struct tm * localT;
            localT = localtime(&currentTime);
            printf("Flow Control Blocked Read - Fan-in - Array | Start Time: %s", asctime(localT));
            printf("Array Length: %lu int32_t Elements\n", array_length);
        }
        #endif

        //Initialize
        size_t amountToAlloc = array_length*sizeof(int32_t);
        if(amountToAlloc % CACHE_LINE_SIZE != 0){
            amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
        }
        int32_t* shared_array_loc_1 = (int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);
        int32_t* shared_array_loc_2 = (int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_b);

        size_t amountToAllocCursors = sizeof(std::atomic_int32_t);
        if(amountToAllocCursors % CACHE_LINE_SIZE != 0){
            amountToAllocCursors += (CACHE_LINE_SIZE - (amountToAllocCursors % CACHE_LINE_SIZE));
        }
        std::atomic_int32_t* shared_ack_loc_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_c);
        std::atomic_int32_t* shared_valid_loc_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_a);
        std::atomic_int32_t* shared_ack_loc_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_c);
        std::atomic_int32_t* shared_valid_loc_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_b);
        std::atomic_init(shared_ack_loc_1, 0);
        if(!std::atomic_is_lock_free(shared_ack_loc_1)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }
        std::atomic_init(shared_valid_loc_1, 0);
        if(!std::atomic_is_lock_free(shared_valid_loc_1)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }
        std::atomic_init(shared_ack_loc_2, 0);
        if(!std::atomic_is_lock_free(shared_ack_loc_2)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }
        std::atomic_init(shared_valid_loc_2, 0);
        if(!std::atomic_is_lock_free(shared_valid_loc_2)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }

        //Init to 0
        for(size_t i = 0; i < array_length; i++)
        {
            shared_array_loc_1[i] = 0;
            shared_array_loc_2[i] = 0;
        }

        //Server Args
        LatencyFlowCtrlBlockedReadKernelArgs* srv_args_1 = new LatencyFlowCtrlBlockedReadKernelArgs();
        srv_args_1->array_shared_ptr = shared_array_loc_1;
        srv_args_1->ack_shared_ptr = shared_ack_loc_1;
        srv_args_1->valid_shared_ptr = shared_valid_loc_1;
        srv_args_1->length = array_length;

        LatencyFlowCtrlBlockedReadKernelArgs* srv_args_2 = new LatencyFlowCtrlBlockedReadKernelArgs();
        srv_args_2->array_shared_ptr = shared_array_loc_2;
        srv_args_2->ack_shared_ptr = shared_ack_loc_2;
        srv_args_2->valid_shared_ptr = shared_valid_loc_2;
        srv_args_2->length = array_length;

        //Client Arg
        LatencyFlowCtrlBlockedReadJoinKernelArgs* cli_args_1 = new LatencyFlowCtrlBlockedReadJoinKernelArgs();
        cli_args_1->array_shared_ptr_a = shared_array_loc_1;
        cli_args_1->ack_shared_ptr_a = shared_ack_loc_1;
        cli_args_1->valid_shared_ptr_a = shared_valid_loc_1;
        cli_args_1->length_a = array_length;
        cli_args_1->array_shared_ptr_b = shared_array_loc_2;
        cli_args_1->ack_shared_ptr_b = shared_ack_loc_2;
        cli_args_1->valid_shared_ptr_b = shared_valid_loc_2;
        cli_args_1->length_b = array_length;


        SimultaniousResults* results = execute_kernel_fanin_server_measure(profiler, latency_flow_ctrl_blocked_read_server_kernel, latency_flow_ctrl_blocked_read_client_join_kernel, latency_flow_ctrl_blocked_read_join_kernel_reset, srv_args_1, srv_args_2, cli_args_1, cli_args_1, cpu_a, cpu_b, cpu_c); //Reset arg is cli args

        #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1 || WRITE_CSV == 1
            if(report_standalone)
            {
                if(!profiler->cpuTopology.empty()){
                    std::set<int> sockets_set;
                    sockets_set.insert(profiler->cpuTopology[cpu_a].socket);
                    sockets_set.insert(profiler->cpuTopology[cpu_b].socket);
                    sockets_set.insert(profiler->cpuTopology[cpu_c].socket);
                    std::vector<int> sockets(sockets_set.begin(), sockets_set.end());

                    std::set<int> cores_set;
                    cores_set.insert(profiler->cpuTopology[cpu_a].core);
                    cores_set.insert(profiler->cpuTopology[cpu_b].core);
                    cores_set.insert(profiler->cpuTopology[cpu_c].core);
                    std::vector<int> cores(cores_set.begin(), cores_set.end());

                    std::set<int> dies_set;
                    dies_set.insert(profiler->cpuTopology[cpu_a].die);
                    dies_set.insert(profiler->cpuTopology[cpu_b].die);
                    dies_set.insert(profiler->cpuTopology[cpu_c].die);
                    std::vector<int> dies(dies_set.begin(), dies_set.end());

                    std::set<int> threads_set;
                    threads_set.insert(cpu_a);
                    threads_set.insert(cpu_b);
                    threads_set.insert(cpu_c);
                    std::vector<int> threads(threads_set.begin(), threads_set.end());
            
                    #if PRINT_FULL_STATS == 1
                        printf("Thread Pair 1 (A/C)\n");
                        results->results_a->print_statistics(sockets, dies, cores, threads, STIM_LEN);
                        printf("Thread Pair 2 (B/C)\n");
                        results->results_b->print_statistics(sockets, dies, cores, threads, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                    printf("Thread Pair 1 (A/C)\n");
                    print_results(results->results_a, sizeof(*shared_array_loc_1)*array_length, STIM_LEN/2); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                    printf("Thread Pair 2 (B/C)\n");
                    print_results(results->results_b, sizeof(*shared_array_loc_2)*array_length, STIM_LEN/2);
                    #endif
                }else{
                    #if PRINT_FULL_STATS
                    printf("Thread Pair 1 (A/C)\n");
                    results->results_a->print_statistics(0, 0, 0, cpu_a, STIM_LEN);
                    printf("Thread Pair 2 (B/C)\n");
                    results->results_b->print_statistics(0, 0, 0, cpu_b, STIM_LEN);
                    #endif

                    printf("Thread Pair 1 (A/C)\n");
                    print_results(results->results_a, sizeof(*shared_array_loc_1)*array_length, STIM_LEN/2); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                    printf("Thread Pair 2 (B/C)\n");
                    print_results(results->results_b, sizeof(*shared_array_loc_2)*array_length, STIM_LEN/2);
                }
            }
            else
            {
                print_results(results->results_a, cpu_a, cpu_c, sizeof(*shared_array_loc_1)*array_length, STIM_LEN/2, array_length, format, file_a, raw_file_a); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                print_results(results->results_b, cpu_b, cpu_c, sizeof(*shared_array_loc_2)*array_length, STIM_LEN/2, array_length, format, file_b, raw_file_b);
            }
        #endif

        //Clean Up
        free(shared_array_loc_1);
        free(shared_array_loc_2);
        free(shared_ack_loc_1);
        free(shared_ack_loc_2);
        free(shared_valid_loc_1);
        free(shared_valid_loc_2);
        delete srv_args_1;
        delete srv_args_2;
        delete cli_args_1;

        return results;
    }

    SimultaniousResults* run_latency_flow_ctrl_blocked_read_fanout_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, size_t array_length, bool report_standalone=true, std::string format = "", FILE* file_a=NULL, FILE* file_b=NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL)
    {
        #if PRINT_TITLE == 1
        if(report_standalone)
        {
            printf("\n");
            time_t currentTime = time(NULL);
            struct tm * localT;
            localT = localtime(&currentTime);
            printf("Flow Control Blocked Read - Fan-in - Array | Start Time: %s", asctime(localT));
            printf("Flow Control Blocked Read - Fan-in - Array\n");
            printf("Array Length: %lu int32_t Elements\n", array_length);
        }
        #endif

        //Initialize
        size_t amountToAlloc = array_length*sizeof(int32_t);
        if(amountToAlloc % CACHE_LINE_SIZE != 0){
            amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
        }
        int32_t* shared_array_loc_1 = (int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);
        int32_t* shared_array_loc_2 = (int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);

        size_t amountToAllocCursors = sizeof(std::atomic_int32_t);
        if(amountToAllocCursors % CACHE_LINE_SIZE != 0){
            amountToAllocCursors += (CACHE_LINE_SIZE - (amountToAllocCursors % CACHE_LINE_SIZE));
        }
        std::atomic_int32_t* shared_ack_loc_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_b);
        std::atomic_int32_t* shared_valid_loc_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_a);
        std::atomic_int32_t* shared_ack_loc_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_c);
        std::atomic_int32_t* shared_valid_loc_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_a);
        std::atomic_init(shared_ack_loc_1, 0);
        if(!std::atomic_is_lock_free(shared_ack_loc_1)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }
        std::atomic_init(shared_valid_loc_1, 0);
        if(!std::atomic_is_lock_free(shared_valid_loc_1)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }
        std::atomic_init(shared_ack_loc_2, 0);
        if(!std::atomic_is_lock_free(shared_ack_loc_2)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }
        std::atomic_init(shared_valid_loc_2, 0);
        if(!std::atomic_is_lock_free(shared_valid_loc_2)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }

        //Init to 0
        for(size_t i = 0; i < array_length; i++)
        {
            shared_array_loc_1[i] = 0;
            shared_array_loc_2[i] = 0;
        }

        //Server Arg
        LatencyFlowCtrlBlockedReadJoinKernelArgs* srv_args_1 = new LatencyFlowCtrlBlockedReadJoinKernelArgs();
        srv_args_1->array_shared_ptr_a = shared_array_loc_1;
        srv_args_1->ack_shared_ptr_a = shared_ack_loc_1;
        srv_args_1->valid_shared_ptr_a = shared_valid_loc_1;
        srv_args_1->length_a = array_length;
        srv_args_1->array_shared_ptr_b = shared_array_loc_2;
        srv_args_1->ack_shared_ptr_b = shared_ack_loc_2;
        srv_args_1->valid_shared_ptr_b = shared_valid_loc_2;
        srv_args_1->length_b = array_length;

        //Client Args
        LatencyFlowCtrlBlockedReadKernelArgs* cli_args_1 = new LatencyFlowCtrlBlockedReadKernelArgs();
        cli_args_1->array_shared_ptr = shared_array_loc_1;
        cli_args_1->ack_shared_ptr = shared_ack_loc_1;
        cli_args_1->valid_shared_ptr = shared_valid_loc_1;
        cli_args_1->length = array_length;

        LatencyFlowCtrlBlockedReadKernelArgs* cli_args_2 = new LatencyFlowCtrlBlockedReadKernelArgs();
        cli_args_2->array_shared_ptr = shared_array_loc_2;
        cli_args_2->ack_shared_ptr = shared_ack_loc_2;
        cli_args_2->valid_shared_ptr = shared_valid_loc_2;
        cli_args_2->length = array_length;

        SimultaniousResults* results = execute_kernel_fanout_client_measure(profiler, latency_flow_ctrl_blocked_read_server_join_kernel, latency_flow_ctrl_blocked_read_client_kernel, latency_flow_ctrl_blocked_read_join_kernel_reset, srv_args_1, cli_args_1, cli_args_2, srv_args_1, cpu_a, cpu_b, cpu_c); //Reset arg is srv args

        #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1 || WRITE_CSV == 1
            if(report_standalone)
            {
                if(!profiler->cpuTopology.empty()){
                    std::set<int> sockets_set;
                    sockets_set.insert(profiler->cpuTopology[cpu_a].socket);
                    sockets_set.insert(profiler->cpuTopology[cpu_b].socket);
                    sockets_set.insert(profiler->cpuTopology[cpu_c].socket);
                    std::vector<int> sockets(sockets_set.begin(), sockets_set.end());

                    std::set<int> cores_set;
                    cores_set.insert(profiler->cpuTopology[cpu_a].core);
                    cores_set.insert(profiler->cpuTopology[cpu_b].core);
                    cores_set.insert(profiler->cpuTopology[cpu_c].core);
                    std::vector<int> cores(cores_set.begin(), cores_set.end());

                    std::set<int> dies_set;
                    dies_set.insert(profiler->cpuTopology[cpu_a].die);
                    dies_set.insert(profiler->cpuTopology[cpu_b].die);
                    dies_set.insert(profiler->cpuTopology[cpu_c].die);
                    std::vector<int> dies(dies_set.begin(), dies_set.end());

                    std::set<int> threads_set;
                    threads_set.insert(cpu_a);
                    threads_set.insert(cpu_b);
                    threads_set.insert(cpu_c);
                    std::vector<int> threads(threads_set.begin(), threads_set.end());
            
                    #if PRINT_FULL_STATS == 1
                        printf("Thread Pair 1 (A/B)\n");
                        results->results_a->print_statistics(sockets, dies, cores, threads, STIM_LEN);
                        printf("Thread Pair 2 (A/C)\n");
                        results->results_b->print_statistics(sockets, dies, cores, threads, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                    printf("Thread Pair 1 (A/B)\n");
                    print_results(results->results_a, sizeof(*shared_array_loc_1)*array_length, STIM_LEN/2); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                    printf("Thread Pair 2 (A/C)\n");
                    print_results(results->results_b, sizeof(*shared_array_loc_2)*array_length, STIM_LEN/2);
                    #endif

                }else{
                    #if PRINT_FULL_STATS
                    printf("Thread Pair 1 (A/B)\n");
                    results->results_a->print_statistics(0, 0, 0, cpu_a, STIM_LEN);
                    printf("Thread Pair 2 (A/C)\n");
                    results->results_b->print_statistics(0, 0, 0, cpu_b, STIM_LEN);
                    #endif

                    printf("Thread Pair 1 (A/B)\n");
                    print_results(results->results_a, sizeof(*shared_array_loc_1)*array_length, STIM_LEN/2); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                    printf("Thread Pair 2 (A/C)\n");
                    print_results(results->results_b, sizeof(*shared_array_loc_2)*array_length, STIM_LEN/2);
                }
            }
            else
            {
                print_results(results->results_a, cpu_a, cpu_b, sizeof(*shared_array_loc_1)*array_length, STIM_LEN/2, array_length, format, file_a, raw_file_a); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                print_results(results->results_b, cpu_a, cpu_c, sizeof(*shared_array_loc_2)*array_length, STIM_LEN/2, array_length, format, file_b, raw_file_b);
            }
        #endif

        //Clean Up
        free(shared_array_loc_1);
        free(shared_array_loc_2);
        free(shared_ack_loc_1);
        free(shared_ack_loc_2);
        free(shared_valid_loc_1);
        free(shared_valid_loc_2);
        delete srv_args_1;
        delete cli_args_1;
        delete cli_args_2;

        return results;
    }

    void run_latency_flow_ctrl_blocked_read_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, FILE* file = NULL, std::ofstream* raw_file=NULL)
    {
        //Print header
        time_t currentTime = time(NULL);
        struct tm * localT;
        localT = localtime(&currentTime);
        printf("Flow Control Blocked Read - Array | Start Time: %s", asctime(localT));
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
            Results* latency_fifo_kernel_results = run_latency_flow_ctrl_blocked_read_kernel(profiler, cpu_a, cpu_b, array_length, false, format, file, raw_file);

            #if WRITE_CSV == 1
            fflush(file);
            #endif

            //Cleanup
            delete latency_fifo_kernel_results;
        }

        printf("        ==========================================================================================\n");
    }

    void run_latency_flow_ctrl_blocked_read_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, int cpu_d, std::vector<size_t> array_lengths, FILE* file_a = NULL, FILE* file_b = NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL)
    {
        //Print header
        time_t currentTime = time(NULL);
        struct tm * localT;
        localT = localtime(&currentTime);
        printf("Flow Control Blocked Read - Simultanious - Array | Start Time: %s", asctime(localT));
        printf("        ========================================================================================================\n");
        printf("         CPU <-> CPU |  Transfer Length  |   One Way Latency (ns)   | Transaction Rate (MT/s) | Data Rate (Mbps)\n");
        printf("                     |(int32_t Elements) |       Avg, StdDev        |                         |                 \n");
        printf("        ========================================================================================================\n");

        #if WRITE_CSV == 1
        fprintf(file_a, "\"Transfer Length (int32_t Elements)\", \"Round Trip Latency (ns) - Avg\", \"Round Trip Latency (ns) - StdDev\", \"Transaction Rate (MT/s)\", \"Data Rate (Mbps)\"\n");
        fflush(file_a);
        fprintf(file_b, "\"Transfer Length (int32_t Elements)\", \"Round Trip Latency (ns) - Avg\", \"Round Trip Latency (ns) - StdDev\", \"Transaction Rate (MT/s)\", \"Data Rate (Mbps)\"\n");
        fflush(file_b);
        *raw_file_a << "\"Transfer Length (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
        *raw_file_b << "\"Transfer Length (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
        #endif

        std::string format = "         %3d <-> %-3d | %17d | %11.6f, %11.6f | %23.6f | %15.6f \n";

        for(int i = 0; i<array_lengths.size(); i++)
        {
            size_t array_length = array_lengths[i];
            SimultaniousResults* latency_fifo_kernel_results = run_latency_flow_ctrl_blocked_read_kernel(profiler, cpu_a, cpu_b, cpu_c, cpu_d, array_length, false, format, file_a, file_b, raw_file_a, raw_file_b);

            #if WRITE_CSV == 1
            fflush(file_a);
            fflush(file_b);
            #endif

            //Cleanup
            delete latency_fifo_kernel_results->results_a;
            delete latency_fifo_kernel_results->results_b;
            delete latency_fifo_kernel_results;
        }

        printf("        ==========================================================================================\n");
    }

    void run_latency_flow_ctrl_blocked_read_fanin_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, std::vector<size_t> array_lengths, FILE* file_a = NULL, FILE* file_b = NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL)
    {
        //Print header
        time_t currentTime = time(NULL);
        struct tm * localT;
        localT = localtime(&currentTime);
        printf("Flow Control Blocked Read - Fan-in - Array | Start Time: %s", asctime(localT));
        printf("        ========================================================================================================\n");
        printf("         CPU <-> CPU |  Transfer Length  |   One Way Latency (ns)   | Transaction Rate (MT/s) | Data Rate (Mbps)\n");
        printf("                     |(int32_t Elements) |       Avg, StdDev        |                         |                 \n");
        printf("        ========================================================================================================\n");

        #if WRITE_CSV == 1
        fprintf(file_a, "\"Transfer Length (int32_t Elements)\", \"Round Trip Latency (ns) - Avg\", \"Round Trip Latency (ns) - StdDev\", \"Transaction Rate (MT/s)\", \"Data Rate (Mbps)\"\n");
        fflush(file_a);
        fprintf(file_b, "\"Transfer Length (int32_t Elements)\", \"Round Trip Latency (ns) - Avg\", \"Round Trip Latency (ns) - StdDev\", \"Transaction Rate (MT/s)\", \"Data Rate (Mbps)\"\n");
        fflush(file_b);
        *raw_file_a << "\"Transfer Length (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
        *raw_file_b << "\"Transfer Length (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
        #endif

        std::string format = "          %3d -> %-3d | %17d | %11.6f, %11.6f | %23.6f | %15.6f \n";

        for(int i = 0; i<array_lengths.size(); i++)
        {
            size_t array_length = array_lengths[i];
            SimultaniousResults* latency_fifo_kernel_results = run_latency_flow_ctrl_blocked_read_fanin_kernel(profiler, cpu_a, cpu_b, cpu_c, array_length, false, format, file_a, file_b, raw_file_a, raw_file_b);

            #if WRITE_CSV == 1
            fflush(file_a);
            fflush(file_b);
            #endif

            //Cleanup
            delete latency_fifo_kernel_results->results_a;
            delete latency_fifo_kernel_results->results_b;
            delete latency_fifo_kernel_results;
        }

        printf("        ==========================================================================================\n");
    }

    void run_latency_flow_ctrl_blocked_read_fanout_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, std::vector<size_t> array_lengths, FILE* file_a = NULL, FILE* file_b = NULL, std::ofstream* raw_file_a=NULL, std::ofstream* raw_file_b=NULL)
    {
        //Print header
        time_t currentTime = time(NULL);
        struct tm * localT;
        localT = localtime(&currentTime);
        printf("Flow Control Blocked Read - Fan-out - Array | Start Time: %s", asctime(localT));
        printf("        ========================================================================================================\n");
        printf("         CPU <-> CPU |  Transfer Length  |   One Way Latency (ns)   | Transaction Rate (MT/s) | Data Rate (Mbps)\n");
        printf("                     |(int32_t Elements) |       Avg, StdDev        |                         |                 \n");
        printf("        ========================================================================================================\n");

        #if WRITE_CSV == 1
        fprintf(file_a, "\"Transfer Length (int32_t Elements)\", \"Round Trip Latency (ns) - Avg\", \"Round Trip Latency (ns) - StdDev\", \"Transaction Rate (MT/s)\", \"Data Rate (Mbps)\"\n");
        fflush(file_a);
        fprintf(file_b, "\"Transfer Length (int32_t Elements)\", \"Round Trip Latency (ns) - Avg\", \"Round Trip Latency (ns) - StdDev\", \"Transaction Rate (MT/s)\", \"Data Rate (Mbps)\"\n");
        fflush(file_b);
        *raw_file_a << "\"Transfer Length (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
        *raw_file_b << "\"Transfer Length (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
        #endif

        std::string format = "          %3d -> %-3d | %17d | %11.6f, %11.6f | %23.6f | %15.6f \n";

        for(int i = 0; i<array_lengths.size(); i++)
        {
            size_t array_length = array_lengths[i];
            SimultaniousResults* latency_fifo_kernel_results = run_latency_flow_ctrl_blocked_read_fanout_kernel(profiler, cpu_a, cpu_b, cpu_c, array_length, false, format, file_a, file_b, raw_file_a, raw_file_b);

            #if WRITE_CSV == 1
            fflush(file_a);
            fflush(file_b);
            #endif

            //Cleanup
            delete latency_fifo_kernel_results->results_a;
            delete latency_fifo_kernel_results->results_b;
            delete latency_fifo_kernel_results;
        }

        printf("        ==========================================================================================\n");
    }
#endif
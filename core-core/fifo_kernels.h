#ifndef _H_FIFO_KERNELS
    #define _H_FIFO_KERNELS

    #include "results.h"
    #include "profiler.h"

    #include "kernel_server_runner.h"

    #include "bandwidth_circular_fifo_kernel.h"
    #include "bandwidth_circular_fifo_blocked_kernel.h"
    #include "bandwidth_circular_fifo_read_limit_kernel.h"
    #include "bandwidth_circular_fifo_blocked_cachedptr_kernel.h"

    #include "print_results.h"

    #include <cstdlib>

    Results* run_bandwidth_fifo_kernel(Profiler* profiler, int cpu_a, int cpu_b, size_t array_length, int32_t max_write_per_transaction, bool report_standalone=true, std::string format = "", FILE* file=NULL, std::ofstream* raw_file=NULL)
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
        size_t amountToAlloc = array_length*sizeof(int32_t);
        if(amountToAlloc % CACHE_LINE_SIZE != 0){
            amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
        }
        int32_t* shared_array_loc = (int32_t*) aligned_alloc(CACHE_LINE_SIZE, amountToAlloc);

        size_t amountToAllocCursors = sizeof(int32_t);
        if(amountToAllocCursors % CACHE_LINE_SIZE != 0){
            amountToAllocCursors += (CACHE_LINE_SIZE - (amountToAllocCursors % CACHE_LINE_SIZE));
        }
        int32_t* shared_write_id_loc = (int32_t*) aligned_alloc(CACHE_LINE_SIZE, amountToAllocCursors);
        int32_t* shared_read_id_loc = (int32_t*) aligned_alloc(CACHE_LINE_SIZE, amountToAllocCursors);

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

        Results* results = execute_client_server_kernel(profiler, bandwidth_circular_fifo_server_kernel, bandwidth_circular_fifo_client_kernel, bandwidth_circular_fifo_kernel_reset, args, args, args, cpu_a, cpu_b);

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
                        print_results(results, sizeof(*shared_array_loc), STIM_LEN*array_length);
                    #endif
                }else{
                    #if PRINT_FULL_STATS
                        results->print_statistics(0, 0, 0, cpu_a, STIM_LEN);
                    #endif

                    print_results(results, sizeof(*shared_array_loc), STIM_LEN*array_length); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                }
            }
            else
            {
                print_results(results, sizeof(*shared_array_loc), STIM_LEN*array_length, array_length, max_write_per_transaction, format, file, raw_file); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
            }
        #endif

        //Clean Up
        free(shared_array_loc);
        free(shared_write_id_loc);
        free(shared_read_id_loc);
        delete args;

        return results;
    }

    //MAKE A 2D Table
    void run_bandwidth_fifo_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, std::vector<int32_t> max_writes_per_transaction, FILE* file = NULL, std::ofstream* raw_file=NULL)
    {
        int32_t data_col_width = 10;

        //Print header
        #if PRINT_TITLE == 1
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
        #endif

        #if PRINT_STATS == 1
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
        #endif

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
            #if PRINT_STATS == 1
            printf("\n        %27lu", array_length);
            #endif

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

                    Results* latency_fifo_kernel_results = run_bandwidth_fifo_kernel(profiler, cpu_a, cpu_b, array_length, max_writes, false, format, file, raw_file);

                    //Cleanup
                    delete latency_fifo_kernel_results;
                }
                else
                {
                    //We did not run this test case
                    #if WRITE_CSV == 1
                    fprintf(file, ",%d", 0);
                    fflush(file);
                    #endif

                    #if PRINT_STATS == 1
                    printf(format.c_str(), 0);
                    #endif
                }
            }
        }

        //Print the newline
        #if WRITE_CSV == 1
        fprintf(file, "\n");
        fflush(file);
        #endif

        #if PRINT_TITLE == 1
        printf("\n        ===========================");
        for(int i = 0; i<max_writes_per_transaction.size(); i++)
        {
            for(int j = 0; j<data_col_width; j++)
            {
                printf("=");
            }
        }
        printf("\n");
        #endif
    }

    Results* run_bandwidth_fifo_blocked_kernel(Profiler* profiler, int cpu_a, int cpu_b, size_t array_length, int32_t block_length, bool report_standalone=true, std::string format = "", FILE* file=NULL, std::ofstream* raw_file=NULL)
    {
        #if PRINT_TITLE == 1
        if(report_standalone)
        {
            printf("\n");
            printf("FIFO - Array - Blocked Transfers\n");
            printf("Array Length: %lu int32_t Elements, Block Length Transaction: %d\n", array_length, block_length);
        }
        #endif

        //Initialize
        size_t amountToAlloc = array_length*sizeof(int32_t);
        if(amountToAlloc % CACHE_LINE_SIZE != 0){
            amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
        }
        int32_t* shared_array_loc = (int32_t*) aligned_alloc(CACHE_LINE_SIZE, amountToAlloc);

        size_t amountToAllocCursors = sizeof(int32_t);
        if(amountToAllocCursors % CACHE_LINE_SIZE != 0){
            amountToAllocCursors += (CACHE_LINE_SIZE - (amountToAllocCursors % CACHE_LINE_SIZE));
        }
        int32_t* shared_write_id_loc = (int32_t*) aligned_alloc(CACHE_LINE_SIZE, amountToAllocCursors);
        int32_t* shared_read_id_loc = (int32_t*) aligned_alloc(CACHE_LINE_SIZE, amountToAllocCursors);

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

        Results* results = execute_client_server_kernel(profiler, bandwidth_circular_fifo_blocked_server_kernel, bandwidth_circular_fifo_blocked_client_kernel, bandwidth_circular_fifo_blocked_kernel_reset, args, args, args, cpu_a, cpu_b);

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
                    print_results_blocked_fifo(results, array_length*STIM_LEN, block_length, sizeof(*shared_array_loc));
                    #endif
                }else{
                    #if PRINT_FULL_STATS
                        results->print_statistics(0, 0, 0, cpu_a, STIM_LEN);
                    #endif

                    print_results_blocked_fifo(results, array_length*STIM_LEN, block_length, sizeof(*shared_array_loc)); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                }
            }
            else
            {
                print_results_blocked_fifo(results, array_length*STIM_LEN, array_length, block_length, sizeof(*shared_array_loc), format, file, raw_file); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
            }
        #endif

        //Clean Up
        free(shared_array_loc);
        free(shared_write_id_loc);
        free(shared_read_id_loc);
        delete args;

        return results;
    }

    //MAKE A 2D Table
    void run_bandwidth_fifo_blocked_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, FILE* file = NULL, std::ofstream* raw_file=NULL)
    {
        int32_t data_col_width = 10;

        //Print header
        #if PRINT_TITLE == 1
        printf("FIFO - Array - Blocked Transfers\n");
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
        #endif

        #if PRINT_STATS == 1
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
        #endif

        #if WRITE_CSV == 1
        fprintf(file, "\"Array Len \\ Block Length (int32_t elements)\"");//Command inserted below
        for(int i = 0; i<block_lengths.size(); i++)
        {
            fprintf(file, ",%d", block_lengths[i]);
        }
        //fprintf(file, "\n"); //Done below
        fflush(file);
        *raw_file << "\"Transfer Length (int32_t Elements)\",\"Block Length (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
        #endif

        std::string format = "|%9.2f";

        for(int i = 0; i<array_lengths.size(); i++)
        {
            size_t array_length = array_lengths[i];

            //Print the newlinem indent and new array length
            #if PRINT_STATS == 1
            printf("\n        %27lu", array_length);
            #endif

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

                    Results* latency_fifo_kernel_results = run_bandwidth_fifo_blocked_kernel(profiler, cpu_a, cpu_b, array_length, block_length, false, format, file, raw_file);

                    //Cleanup
                    delete latency_fifo_kernel_results;
                }
                else
                {
                    //We did not run this test case
                    #if WRITE_CSV == 1
                    fprintf(file, ",%d", 0);
                    fflush(file);
                    #endif

                    #if PRINT_STATS == 1
                    printf(format.c_str(), 0);
                    #endif
                }
            }
        }

        //Print the newline
        #if WRITE_CSV == 1
        fprintf(file, "\n");
        fflush(file);
        #endif

        #if PRINT_TITLE == 1
        printf("\n        ===========================");
        for(int i = 0; i<array_lengths.size(); i++)
        {
            for(int j = 0; j<data_col_width; j++)
            {
                printf("=");
            }
        }
        printf("\n");
        #endif
    }

    Results* run_bandwidth_fifo_read_limit_kernel(Profiler* profiler, int cpu_a, int cpu_b, size_t array_length, int32_t max_elements_per_transaction, bool report_standalone=true, std::string format = "", FILE* file=NULL, std::ofstream* raw_file=NULL)
    {
        #if PRINT_TITLE == 1
        if(report_standalone)
        {
            printf("\n");
            printf("FIFO - Array - Read and Write Limit\n");
            printf("Array Length: %lu int32_t Elements, Max Read|Write Per Transaction: %d\n", array_length, max_elements_per_transaction);
        }
        #endif

        //Initialize
        size_t amountToAlloc = array_length*sizeof(int32_t);
        if(amountToAlloc % CACHE_LINE_SIZE != 0){
            amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
        }
        int32_t* shared_array_loc = (int32_t*) aligned_alloc(CACHE_LINE_SIZE, amountToAlloc);

        size_t amountToAllocCursors = sizeof(int32_t);
        if(amountToAllocCursors % CACHE_LINE_SIZE != 0){
            amountToAllocCursors += (CACHE_LINE_SIZE - (amountToAllocCursors % CACHE_LINE_SIZE));
        }
        int32_t* shared_write_id_loc = (int32_t*) aligned_alloc(CACHE_LINE_SIZE, amountToAllocCursors);
        int32_t* shared_read_id_loc = (int32_t*) aligned_alloc(CACHE_LINE_SIZE, amountToAllocCursors);

        //Init to 0
        for(size_t i = 0; i < array_length; i++)
        {
            shared_array_loc[i] = 0;
        }

        BandwidthCircularFifoReadLimitKernelArgs* args = new BandwidthCircularFifoReadLimitKernelArgs();
        args->array_shared_ptr = shared_array_loc;
        args->write_pos_shared_ptr = shared_write_id_loc;
        args->read_pos_shared_ptr = shared_read_id_loc;
        args->length = array_length;
        args->max_elements_per_transaction = max_elements_per_transaction;

        Results* results = execute_client_server_kernel(profiler, bandwidth_circular_fifo_read_limit_server_kernel, bandwidth_circular_fifo_read_limit_client_kernel, bandwidth_circular_fifo_read_limit_kernel_reset, args, args, args, cpu_a, cpu_b);

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
                        print_results(results, sizeof(*shared_array_loc), STIM_LEN*array_length);
                    #endif

                }else{
                    #if PRINT_FULL_STATS
                    results->print_statistics(0, 0, 0, cpu_a, STIM_LEN);
                    #endif

                    print_results(results, sizeof(*shared_array_loc), STIM_LEN*array_length); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                }
            }
            else
            {
                print_results(results, sizeof(*shared_array_loc), STIM_LEN*array_length, array_length, max_elements_per_transaction, format, file, raw_file); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
            }
        #endif

        //Clean Up
        free(shared_array_loc);
        free(shared_write_id_loc);
        free(shared_read_id_loc);
        delete args;

        return results;
    }

    //MAKE A 2D Table
    void run_bandwidth_fifo_read_limit_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, std::vector<int32_t> max_elements_per_transaction, FILE* file = NULL, std::ofstream* raw_file=NULL)
    {
        int32_t data_col_width = 10;

        //Print header
        #if PRINT_TITLE == 1
        printf("FIFO - Array - Read and Write Limited\n");
        printf("        Lengths in int32_t Elements, Data Rates in Mbps\n");
        printf("        ===========================");
        for(int i = 0; i<max_elements_per_transaction.size(); i++)
        {
            for(int j = 0; j<data_col_width; j++)
            {
                printf("=");
            }
        }
        printf("\n");
        #endif

        #if PRINT_STATS == 1
        printf("        Array Len \\ Max Trans. Len ");
        for(int i = 0; i<max_elements_per_transaction.size(); i++)
        {
            printf("|%9.2d", max_elements_per_transaction[i]); //12 becaused of spaces
        }
        printf("\n");
        printf("        ===========================");
        for(int i = 0; i<max_elements_per_transaction.size(); i++)
        {
            for(int j = 0; j<data_col_width; j++) 
            {
                printf("=");
            }
        }
        #endif

        #if WRITE_CSV == 1
        fprintf(file, "\"Array Len \\ Max Trans. Len (int32_t elements)\"");//Command inserted below
        for(int i = 0; i<max_elements_per_transaction.size(); i++)
        {
            fprintf(file, ",%d", max_elements_per_transaction[i]);
        }
        //fprintf(file, "\n"); //Done below
        fflush(file);
        *raw_file << "\"Transfer Length (int32_t Elements)\",\"Max Reads|Writes Per Transaction (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
        #endif

        std::string format = "|%9.2f";

        for(int i = 0; i<array_lengths.size(); i++)
        {
            size_t array_length = array_lengths[i];

            //Print the newlinem indent and new array length
            #if PRINT_STATS == 1
            printf("\n        %27lu", array_length);
            #endif 

            #if WRITE_CSV == 1
            fprintf(file, "\n%lu", array_length);
            fflush(file);
            #endif

            for(int j = 0; j<max_elements_per_transaction.size(); j++)
            {
                int32_t max_elements = max_elements_per_transaction[j];

                if(max_elements <= array_length) //Check if we should bother running this case
                {
                    #if WRITE_CSV == 1
                    fprintf(file, ",");
                    fflush(file);
                    #endif

                    Results* latency_fifo_kernel_results = run_bandwidth_fifo_read_limit_kernel(profiler, cpu_a, cpu_b, array_length, max_elements, false, format, file, raw_file);

                    //Cleanup
                    delete latency_fifo_kernel_results;
                }
                else
                {
                    //We did not run this test case
                    #if WRITE_CSV == 1
                    fprintf(file, ",%d", 0);
                    fflush(file);
                    #endif

                    #if PRINT_STATS == 1
                    printf(format.c_str(), 0);
                    #endif
                }
            }
        }

        //Print the newline
        #if WRITE_CSV == 1
        fprintf(file, "\n");
        fflush(file);
        #endif

        #if PRINT_TITLE == 1
        printf("\n        ===========================");
        for(int i = 0; i<max_elements_per_transaction.size(); i++)
        {
            for(int j = 0; j<data_col_width; j++)
            {
                printf("=");
            }
        }
        printf("\n");
        #endif
    }

    Results* run_bandwidth_fifo_blocked_cachedptr_kernel(Profiler* profiler, int cpu_a, int cpu_b, size_t array_length, int32_t block_length, bool report_standalone=true, std::string format = "", FILE* file=NULL, std::ofstream* raw_file=NULL)
    {
        #if PRINT_TITLE == 1
        if(report_standalone)
        {
            printf("\n");
            printf("FIFO - Array - Blocked Transfers Cached Indexes\n");
            printf("Array Length: %lu int32_t Elements, Block Length Transaction: %d\n", array_length, block_length);
        }
        #endif

        //Initialize
        size_t amountToAlloc = array_length*sizeof(int32_t);
        if(amountToAlloc % CACHE_LINE_SIZE != 0){
            amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
        }
        int32_t* shared_array_loc = (int32_t*) aligned_alloc(CACHE_LINE_SIZE, amountToAlloc);

        size_t amountToAllocCursors = sizeof(int32_t);
        if(amountToAllocCursors % CACHE_LINE_SIZE != 0){
            amountToAllocCursors += (CACHE_LINE_SIZE - (amountToAllocCursors % CACHE_LINE_SIZE));
        }
        int32_t* shared_write_id_loc = (int32_t*) aligned_alloc(CACHE_LINE_SIZE, amountToAllocCursors);
        int32_t* shared_read_id_loc = (int32_t*) aligned_alloc(CACHE_LINE_SIZE, amountToAllocCursors);

        //Init to 0
        for(size_t i = 0; i < array_length; i++)
        {
            shared_array_loc[i] = 0;
        }

        BandwidthCircularFifoBlockedCachedPtrKernelArgs* args = new BandwidthCircularFifoBlockedCachedPtrKernelArgs();
        args->array_shared_ptr = shared_array_loc;
        args->write_pos_shared_ptr = shared_write_id_loc;
        args->read_pos_shared_ptr = shared_read_id_loc;
        args->length = array_length;
        args->block_length = block_length;

        Results* results = execute_client_server_kernel(profiler, bandwidth_circular_fifo_blocked_cachedptr_server_kernel, bandwidth_circular_fifo_blocked_cachedptr_client_kernel, bandwidth_circular_fifo_blocked_cachedptr_kernel_reset, args, args, args, cpu_a, cpu_b);

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
                    print_results_blocked_fifo(results, array_length*STIM_LEN, block_length, sizeof(*shared_array_loc));
                    #endif
                }else{
                    #if PRINT_FULL_STATS
                        results->print_statistics(0, 0, 0, cpu_a, STIM_LEN);
                    #endif

                    print_results_blocked_fifo(results, array_length*STIM_LEN, block_length, sizeof(*shared_array_loc)); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                }
            }
            else
            {
                print_results_blocked_fifo(results, array_length*STIM_LEN, array_length, block_length, sizeof(*shared_array_loc), format, file, raw_file); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
            }
        #endif

        //Clean Up
        free(shared_array_loc);
        free(shared_write_id_loc);
        free(shared_read_id_loc);
        delete args;

        return results;
    }

    //MAKE A 2D Table
    void run_bandwidth_fifo_blocked_cachedptr_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, FILE* file = NULL, std::ofstream* raw_file=NULL)
    {
        int32_t data_col_width = 10;

        //Print header
        #if PRINT_TITLE == 1
        printf("FIFO - Array - Blocked Transfers Cached Indexes\n");
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
        #endif

        #if PRINT_STATS == 1
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
        #endif

        #if WRITE_CSV == 1
        fprintf(file, "\"Array Len \\ Block Length (int32_t elements)\"");//Command inserted below
        for(int i = 0; i<block_lengths.size(); i++)
        {
            fprintf(file, ",%d", block_lengths[i]);
        }
        //fprintf(file, "\n"); //Done below
        fflush(file);
        *raw_file << "\"Transfer Length (int32_t Elements)\",\"Block Length (int32_t Elements)\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
        #endif

        std::string format = "|%9.2f";

        for(int i = 0; i<array_lengths.size(); i++)
        {
            size_t array_length = array_lengths[i];

            //Print the newlinem indent and new array length
            #if PRINT_STATS == 1
            printf("\n        %27lu", array_length);
            #endif

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

                    Results* latency_fifo_kernel_results = run_bandwidth_fifo_blocked_cachedptr_kernel(profiler, cpu_a, cpu_b, array_length, block_length, false, format, file, raw_file);

                    //Cleanup
                    delete latency_fifo_kernel_results;
                }
                else
                {
                    //We did not run this test case
                    #if WRITE_CSV == 1
                    fprintf(file, ",%d", 0);
                    fflush(file);
                    #endif

                    #if PRINT_STATS == 1
                    printf(format.c_str(), 0);
                    #endif
                }
            }
        }

        //Print the newline
        #if WRITE_CSV == 1
        fprintf(file, "\n");
        fflush(file);
        #endif

        #if PRINT_TITLE == 1
        printf("\n        ===========================");
        for(int i = 0; i<array_lengths.size(); i++)
        {
            for(int j = 0; j<data_col_width; j++)
            {
                printf("=");
            }
        }
        printf("\n");
        #endif
    }

#endif
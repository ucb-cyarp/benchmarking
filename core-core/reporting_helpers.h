#ifndef REPORTING_HELPERS_H
    #define REPORTING_HELPERS_H

    #include <string>
    #include <cstdio>
    #include <iostream>
    #include "kernel_server_runner.h"
    #include "print_results.h"
    #include "intrin_bench_default_defines_and_imports_cpp.h"
    
    void printTitleArray(bool report_standalone, std::string title, size_t array_length);

    void writeCSVHeader(FILE* file, std::ofstream* raw_file);

    //Return format
    std::string tableHeaderArray1Stream(std::string title, FILE* file, std::ofstream* raw_file);

    //Return format
    std::string tableHeaderArray2Streams(std::string title, FILE* file_a, FILE* file_b, std::ofstream* raw_file_a, std::ofstream* raw_file_b, bool bidirectional);

    void tableFooter();

    template <typename elementType>
    void exportResultsSingle2Core(Profiler* profiler, int cpu_a, int cpu_b, Results &results){
        #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1
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
                    results.print_statistics(sockets, dies, cores, threads, STIM_LEN);
                #endif

                #if PRINT_STATS == 1
                    print_results(results, sizeof(elementType), STIM_LEN);
                #endif
            }else{
                #if PRINT_FULL_STATS
                    results.print_statistics(0, 0, 0, cpu_a, STIM_LEN);
                #endif

                #if PRINT_STATS == 1
                    print_results(results, sizeof(elementType), STIM_LEN);
                #endif
            }
        #endif
    }

    /**
     * Note that, for the array benchmarks, the stim_len is the number of one-way transactions that occure.
     * 
     * So, for a stim_len of 100, there are 50 transactions a->b and 50 transactions b->a
     * When the time is divided by the stim_len, we get the time for a one way transaction (assuming symmetry)
     * 
     * For the flow control versions, the communication is inherently asemetric.  In this case, we do not wish to count
     * the transactions in the reverse direction (the ack).  Each flow control transaction consits of 2 one-way transactions
     * Thus, when reporting results, the stimulus length is divided by 2.
     * 
     * The decision on whether or not to divide by 2 is controlled by the divBy2 parameter
     */
    template <typename elementType>
    void exportResultsArray2Core(bool report_standalone, Profiler* profiler, int cpu_a, int cpu_b, Results &results, size_t array_length, std::string format, FILE* file, std::ofstream* raw_file, bool divBy2){
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
                        results.print_statistics(sockets, dies, cores, threads, divBy2 ? STIM_LEN/2 : STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        print_results(results, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN);
                    #endif
                }else{
                    #if PRINT_FULL_STATS
                        results.print_statistics(0, 0, 0, cpu_a, divBy2 ? STIM_LEN/2 : STIM_LEN);
                    #endif

                    print_results(results, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN);
                }
            }
            else
            {
                print_results(results, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN, array_length, format, file, raw_file);
            }
        #endif
    }

    template <typename elementType>
    void exportResultsArray4Core(bool report_standalone, Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, int cpu_d, SimultaniousResults &results, size_t array_length, std::string format, FILE* file_a, FILE* file_b, std::ofstream* raw_file_a, std::ofstream* raw_file_b, bool divBy2){
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
                        results.results_a.print_statistics(sockets, dies, cores, threads, divBy2 ? STIM_LEN/2 : STIM_LEN);
                        printf("Thread Pair 2 (C/D)\n");
                        results.results_b.print_statistics(sockets, dies, cores, threads, divBy2 ? STIM_LEN/2 : STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        printf("Thread Pair 1 (A/B)\n");
                        print_results(results.results_a, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN);
                        printf("Thread Pair 2 (C/D)\n");
                        print_results(results.results_b, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN);
                    #endif

                }else{
                    #if PRINT_FULL_STATS == 1
                        printf("Thread Pair 1 (A/B)\n");
                        results.results_a.print_statistics(0, 0, 0, cpu_a, divBy2 ? STIM_LEN/2 : STIM_LEN);
                        printf("Thread Pair 2 (C/D)\n");
                        results.results_b.print_statistics(0, 0, 0, cpu_c, divBy2 ? STIM_LEN/2 : STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        printf("Thread Pair 1 (A/B)\n");
                        print_results(results.results_a, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN);
                        printf("Thread Pair 2 (C/D)\n");
                        print_results(results.results_b, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN);
                    #endif
                }
            }
            else
            {
                print_results(results.results_a, cpu_a, cpu_b, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN, array_length, format, file_a, raw_file_a);
                print_results(results.results_b, cpu_c, cpu_d, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN, array_length, format, file_b, raw_file_b);
            }
        #endif
    }

    template <typename elementType>
    void exportResultsArray3CoreFanInFanOut(bool report_standalone, Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, SimultaniousResults &results, size_t array_length, std::string format, FILE* file_a, FILE* file_b, std::ofstream* raw_file_a, std::ofstream* raw_file_b, bool divBy2){
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
                        results.results_a.print_statistics(sockets, dies, cores, threads, divBy2 ? STIM_LEN/2 : STIM_LEN);
                        printf("Thread Pair 2 (B/C)\n");
                        results.results_b.print_statistics(sockets, dies, cores, threads, divBy2 ? STIM_LEN/2 : STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        printf("Thread Pair 1 (A/C)\n");
                        print_results(results.results_a, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN);
                        printf("Thread Pair 2 (B/C)\n");
                        print_results(results.results_b, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN);
                    #endif

                }else{
                    #if PRINT_FULL_STATS == 1
                        printf("Thread Pair 1 (A/C)\n");
                        results.results_a.print_statistics(0, 0, 0, cpu_a, divBy2 ? STIM_LEN/2 : STIM_LEN);
                        printf("Thread Pair 2 (B/C)\n");
                        results.results_b.print_statistics(0, 0, 0, cpu_c, divBy2 ? STIM_LEN/2 : STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        printf("Thread Pair 1 (A/C)\n");
                        print_results(results.results_a, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN);
                        printf("Thread Pair 2 (B/C)\n");
                        print_results(results.results_b, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN);
                    #endif
                }
            }
            else
            {
                print_results(results.results_a, cpu_a, cpu_c, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN, array_length, format, file_a, raw_file_a);
                print_results(results.results_b, cpu_b, cpu_c, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN, array_length, format, file_b, raw_file_b);
            }
        #endif
    }

    template <typename elementType>
    void exportResultsArray3CoreFanOut(bool report_standalone, Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, SimultaniousResults &results, size_t array_length, std::string format, FILE* file_a, FILE* file_b, std::ofstream* raw_file_a, std::ofstream* raw_file_b, bool divBy2){
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
                        results.results_a.print_statistics(sockets, dies, cores, threads, divBy2 ? STIM_LEN/2 : STIM_LEN);
                        printf("Thread Pair 2 (A/C)\n");
                        results.results_b.print_statistics(sockets, dies, cores, threads, divBy2 ? STIM_LEN/2 : STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        printf("Thread Pair 1 (A/B)\n");
                        print_results(results.results_a, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN); //Div by 2 is because the counter increments for each direction of the flow control transaction (transmit and ack)
                        printf("Thread Pair 2 (A/C)\n");
                        print_results(results.results_b, sizeof(elementType)*array_length,divBy2 ? STIM_LEN/2 : STIM_LEN);
                    #endif

                }else{
                        #if PRINT_FULL_STATS == 1
                            printf("Thread Pair 1 (A/B)\n");
                            results.results_a.print_statistics(0, 0, 0, cpu_a, divBy2 ? STIM_LEN/2 : STIM_LEN);
                            printf("Thread Pair 2 (A/C)\n");
                            results.results_b.print_statistics(0, 0, 0, cpu_c, divBy2 ? STIM_LEN/2 : STIM_LEN);
                        #endif

                        #if PRINT_STATS == 1
                            printf("Thread Pair 1 (A/B)\n");
                            print_results(results.results_a, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN); //Div by 2 is because the counter increments for each direction of the flow control transaction (transmit and ack)
                            printf("Thread Pair 2 (A/C)\n");
                            print_results(results.results_b, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN);
                        #endif
                }
            }
            else
            {
                print_results(results.results_a, cpu_a, cpu_b, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN, array_length, format, file_a, raw_file_a); //Div by 2 is because the counter increments for each direction of the flow control transaction (transmit and ack)
                print_results(results.results_b, cpu_a, cpu_c, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN, array_length, format, file_b, raw_file_b);
            }
        #endif
    }

    /**
     * Note that the STIM_LEN for the FIFO benchmark signifies how many times an entire FIFO Buffer's worth of data is transfered.  The number of indevidual elements is therefore the length of the array*STIM_LEN
     * The number of bytes sent in a trial is elementSize*array_length*STIM_LENGTH
     * 
     * This is meant to parallel the array benchmarks where the STIM_LEN specifies the number of one way transactions (typically of the entire array)
     */
    template <typename elementType>
    void exportResultsFIFO2Core(bool report_standalone, Profiler* profiler, int cpu_a, int cpu_b, Results &results, size_t array_length, int writes_per_transaction, std::string format, FILE* file, std::ofstream* raw_file){
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
                        results.print_statistics(sockets, dies, cores, threads, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        print_results(results, sizeof(elementType), STIM_LEN*array_length);
                    #endif
                }else{
                    #if PRINT_FULL_STATS
                        results.print_statistics(0, 0, 0, cpu_a, STIM_LEN);
                    #endif

                    print_results(results, sizeof(elementType), STIM_LEN*array_length);
                }
            }
            else
            {
                print_results(results, sizeof(elementType), STIM_LEN*array_length, array_length, writes_per_transaction, format, file, raw_file);
            }
        #endif
    }

    template <typename elementType>
    void exportResultsBlockedFIFO2Core(bool report_standalone, Profiler* profiler, int cpu_a, int cpu_b, Results &results, size_t array_length, int block_length, std::string format, FILE* file, std::ofstream* raw_file){
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
                        results.print_statistics(sockets, dies, cores, threads, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        print_results_blocked_fifo(results, array_length*STIM_LEN, block_length, sizeof(elementType));
                    #endif
                }else{
                    #if PRINT_FULL_STATS == 1
                        results.print_statistics(0, 0, 0, cpu_a, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        print_results_blocked_fifo(results, array_length*STIM_LEN, block_length, sizeof(elementType));
                    #endif
                }
            }
            else
            {
                print_results_blocked_fifo(results, array_length*STIM_LEN, array_length, block_length, sizeof(elementType), format, file, raw_file);
            }
        #endif
    }

    void printWriteFIFOTable2Core();

    void printTitleFIFOPoint(bool report_standalone, std::string title, size_t array_length, std::string second_param_label, int32_t second_param);

    void printTitleFIFO(std::string title, int columns, int column_width);
    
    std::string printAndWriteHeadersFIFO(std::string secondary_label_printcsv, std::string secondary_label_rawcsv, std::vector<int32_t> secondary_dimension_items, int data_col_width, FILE* file, std::ofstream* raw_file);

    template <typename elementType>
    void printWriteFIFOTable2Core(bool report_standalone, Profiler* profiler, int cpu_a, int cpu_b, std::string title, std::string secondary_dimension_label, std::vector<Results> results_vec, std::vector<size_t> array_lengths, std::vector<int32_t> writes_per_transactions, int data_col_width, std::string format, FILE* file, std::ofstream* raw_file){
        int result_idx = 0;

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

            for(int j = 0; j<writes_per_transactions.size(); j++)
            {
                int32_t writes_per_transaction = writes_per_transactions[j];

                if(writes_per_transaction <= array_length) //Check if we should bother running this case
                {
                    #if WRITE_CSV == 1
                    fprintf(file, ",");
                    fflush(file);
                    #endif

                    //Print/Write individual results
                    printTitleFIFOPoint(report_standalone, title, array_length, secondary_dimension_label, writes_per_transaction);
                    exportResultsFIFO2Core<elementType>(report_standalone, profiler, cpu_a, cpu_b, results_vec[result_idx], array_length, writes_per_transaction, format, file, raw_file);

                    result_idx++;
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
        for(int i = 0; i<writes_per_transactions.size(); i++)
        {
            for(int j = 0; j<data_col_width; j++)
            {
                printf("=");
            }
        }
        printf("\n");
        #endif
    }

    template <typename elementType>
    void printWriteBlockedFIFOTable2Core(bool report_standalone, Profiler* profiler, int cpu_a, int cpu_b, std::string title, std::string secondary_dimension_label, std::vector<Results> results_vec, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, int data_col_width, std::string format, FILE* file, std::ofstream* raw_file){
        int result_idx = 0;

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

                    //Print/Write individual results
                    printTitleFIFOPoint(report_standalone, title, array_length, secondary_dimension_label, block_length);
                    exportResultsBlockedFIFO2Core<elementType>(report_standalone, profiler, cpu_a, cpu_b, results_vec[result_idx], array_length, block_length, format, file, raw_file);

                    result_idx++;
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
        for(int i = 0; i<block_lengths.size(); i++)
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

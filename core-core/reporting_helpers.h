#ifndef REPORTING_HELPERS_H
    #define REPORTING_HELPERS_H

    #include <string>
    #include <cstdio>
    #include <iostream>
    #include <set>
    #include "kernel_server_runner.h"
    #include "print_results.h"
    #include "intrin_bench_default_defines_and_imports_cpp.h"

    void printTitle(std::string title);
    
    void printTitleArray(bool report_standalone, std::string title, size_t array_length);

    void writeCSVHeader(FILE* file, std::ofstream* raw_file);

    //Return format
    std::string tableHeaderArray1Stream(std::string title, FILE* file, std::ofstream* raw_file);

    //Return format
    std::string tableHeaderArray2Streams(std::string title, FILE* file_a, FILE* file_b, std::ofstream* raw_file_a, std::ofstream* raw_file_b, bool bidirectional);

    void tableFooter();

    void getGranularityIndexsOfInterest(std::vector<int> cpus, Profiler *profiler, std::vector<int> &sockets, std::vector<int> &cores, std::vector<int> &dies, std::vector<int> &threads);

    /**
     * Prints a result of a "single" address transaction benchmark.
     * 
     * The proper mode of printing and exporting is selected given the configuration options (ex. standalone, print stats, ...).
     */
    template <typename elementType>
    void exportResultsSingle2Core(Profiler* profiler, int cpu_a, int cpu_b, Results &results){
        #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1
            if(!profiler->cpuTopology.empty()){
                std::vector<int> sockets;
                std::vector<int> cores;
                std::vector<int> dies;
                std::vector<int> threads;

                getGranularityIndexsOfInterest({cpu_a, cpu_b}, profiler, sockets, cores, dies, threads);
        
                #if PRINT_FULL_STATS == 1
                    results.print_statistics(sockets, dies, cores, threads, STIM_LEN);
                #endif

                #if PRINT_STATS == 1
                    print_results_standalone(results, sizeof(elementType), STIM_LEN);
                #endif
            }else{
                #if PRINT_FULL_STATS == 1
                    results.print_statistics(0, 0, 0, cpu_a, STIM_LEN);
                #endif

                #if PRINT_STATS == 1
                    print_results_standalone(results, sizeof(elementType), STIM_LEN);
                #endif
            }
        #endif
    }

    /**
     * Prints a result of an array benchmark as well as writing it to the summary and raw CSV files.
     * 
     * The amount of reporting is configured using compile and runtime parameters (report_standalone, PRINT_STATS, ...).
     * 
     * This function is typically called in a loop to print/export all of the results (all of the configurations) of
     * a particular benchmark run.
     * 
     * ----------
     * 
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
                    std::vector<int> cores;
                    std::vector<int> dies;
                    std::vector<int> threads;

                    getGranularityIndexsOfInterest({cpu_a, cpu_b}, profiler, sockets, cores, dies, threads);
                    
                    #if PRINT_FULL_STATS == 1
                        results.print_statistics(sockets, dies, cores, threads, divBy2 ? STIM_LEN/2 : STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        print_results_standalone(results, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN);
                    #endif
                }else{
                    #if PRINT_FULL_STATS == 1
                        results.print_statistics_standalone(0, 0, 0, cpu_a, divBy2 ? STIM_LEN/2 : STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        print_results_standalone(results, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN);
                    #endif
                }
            }
            else
            {
                print_results(results, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN, array_length, format, file, raw_file);
            }
        #endif
    }

    /**
     * Prints a result of a 4 core array benchmark as well as writing it to the summary and raw CSV files.
     * 
     * The amount of reporting is configured using compile and runtime parameters (report_standalone, PRINT_STATS, ...).
     * 
     * This function is typically called in a loop to print/export all of the results (all of the configurations) of
     * a particular benchmark run.
     */
    template <typename elementType>
    void exportResultsArray4Core(bool report_standalone, Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, int cpu_d, SimultaniousResults &results, size_t array_length, std::string format, FILE* file_a, FILE* file_b, std::ofstream* raw_file_a, std::ofstream* raw_file_b, bool divBy2){
        #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1 || WRITE_CSV == 1
            if(report_standalone)
            {
                if(!profiler->cpuTopology.empty()){
                    std::vector<int> sockets;
                    std::vector<int> cores;
                    std::vector<int> dies;
                    std::vector<int> threads;

                    getGranularityIndexsOfInterest({cpu_a, cpu_b, cpu_c, cpu_d}, profiler, sockets, cores, dies, threads);
                
                    #if PRINT_FULL_STATS == 1
                        printf("Thread Pair 1 (A/B)\n");
                        results.results_a.print_statistics(sockets, dies, cores, threads, divBy2 ? STIM_LEN/2 : STIM_LEN);
                        printf("Thread Pair 2 (C/D)\n");
                        results.results_b.print_statistics(sockets, dies, cores, threads, divBy2 ? STIM_LEN/2 : STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        printf("Thread Pair 1 (A/B)\n");
                        print_results_standalone(results.results_a, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN);
                        printf("Thread Pair 2 (C/D)\n");
                        print_results_standalone(results.results_b, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN);
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
                        print_results_standalone(results.results_a, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN);
                        printf("Thread Pair 2 (C/D)\n");
                        print_results_standalone(results.results_b, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN);
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

    /**
     * Prints a result of a 3 core (fanin/fanout) array benchmark as well as writing it to the summary and raw CSV files.
     * 
     * The amount of reporting is configured using compile and runtime parameters (report_standalone, PRINT_STATS, ...).
     * 
     * This function is typically called in a loop to print/export all of the results (all of the configurations) of
     * a particular benchmark run.
     */
    template <typename elementType>
    void exportResultsArray3CoreFanInFanOut(bool report_standalone, Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, SimultaniousResults &results, size_t array_length, std::string format, FILE* file_a, FILE* file_b, std::ofstream* raw_file_a, std::ofstream* raw_file_b, bool divBy2){
        #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1 || WRITE_CSV == 1
            if(report_standalone)
            {
                if(!profiler->cpuTopology.empty()){
                    std::vector<int> sockets;
                    std::vector<int> cores;
                    std::vector<int> dies;
                    std::vector<int> threads;

                    getGranularityIndexsOfInterest({cpu_a, cpu_b, cpu_c}, profiler, sockets, cores, dies, threads);
            
                    #if PRINT_FULL_STATS == 1
                        printf("Thread Pair 1 (A/C)\n");
                        results.results_a.print_statistics(sockets, dies, cores, threads, divBy2 ? STIM_LEN/2 : STIM_LEN);
                        printf("Thread Pair 2 (B/C)\n");
                        results.results_b.print_statistics(sockets, dies, cores, threads, divBy2 ? STIM_LEN/2 : STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        printf("Thread Pair 1 (A/C)\n");
                        print_results_standalone(results.results_a, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN);
                        printf("Thread Pair 2 (B/C)\n");
                        print_results_standalone(results.results_b, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN);
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
                        print_results_standalone(results.results_a, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN);
                        printf("Thread Pair 2 (B/C)\n");
                        print_results_standalone(results.results_b, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN);
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

    /**
     * Prints a result of a 3 core (fanout) array benchmark as well as writing it to the summary and raw CSV files.
     * 
     * The amount of reporting is configured using compile and runtime parameters (report_standalone, PRINT_STATS, ...).
     * 
     * This function is typically called in a loop to print/export all of the results (all of the configurations) of
     * a particular benchmark run.
     */
    template <typename elementType>
    void exportResultsArray3CoreFanOut(bool report_standalone, Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, SimultaniousResults &results, size_t array_length, std::string format, FILE* file_a, FILE* file_b, std::ofstream* raw_file_a, std::ofstream* raw_file_b, bool divBy2){
        #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1 || WRITE_CSV == 1
            if(report_standalone)
            {
                if(!profiler->cpuTopology.empty()){
                    std::vector<int> sockets;
                    std::vector<int> cores;
                    std::vector<int> dies;
                    std::vector<int> threads;

                    getGranularityIndexsOfInterest({cpu_a, cpu_b, cpu_c}, profiler, sockets, cores, dies, threads);
            
                    #if PRINT_FULL_STATS == 1
                        printf("Thread Pair 1 (A/B)\n");
                        results.results_a.print_statistics(sockets, dies, cores, threads, divBy2 ? STIM_LEN/2 : STIM_LEN);
                        printf("Thread Pair 2 (A/C)\n");
                        results.results_b.print_statistics(sockets, dies, cores, threads, divBy2 ? STIM_LEN/2 : STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        printf("Thread Pair 1 (A/B)\n");
                        print_results_standalone(results.results_a, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN); //Div by 2 is because the counter increments for each direction of the flow control transaction (transmit and ack)
                        printf("Thread Pair 2 (A/C)\n");
                        print_results_standalone(results.results_b, sizeof(elementType)*array_length,divBy2 ? STIM_LEN/2 : STIM_LEN);
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
                            print_results_standalone(results.results_a, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN); //Div by 2 is because the counter increments for each direction of the flow control transaction (transmit and ack)
                            printf("Thread Pair 2 (A/C)\n");
                            print_results_standalone(results.results_b, sizeof(elementType)*array_length, divBy2 ? STIM_LEN/2 : STIM_LEN);
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
     * Prints a result of a 2 core FIFO benchmark as well as writing it to the summary and raw CSV files.
     * 
     * The amount of reporting is configured using compile and runtime parameters (report_standalone, PRINT_STATS, ...).
     * 
     * This function is typically called in a loop to print/export all of the results (all of the configurations) of
     * a particular benchmark run.
     *
     * ---------
     * 
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
                    std::vector<int> cores;
                    std::vector<int> dies;
                    std::vector<int> threads;

                    getGranularityIndexsOfInterest({cpu_a, cpu_b}, profiler, sockets, cores, dies, threads);

                    #if PRINT_FULL_STATS == 1
                        results.print_statistics(sockets, dies, cores, threads, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        print_results_standalone(results, sizeof(elementType), STIM_LEN*array_length);
                    #endif
                }else{
                    #if PRINT_FULL_STATS == 1
                        results.print_statistics(0, 0, 0, cpu_a, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        print_results_standalone(results, sizeof(elementType), STIM_LEN*array_length);
                    #endif
                }
            }
            else
            {
                print_results(results, sizeof(elementType), STIM_LEN*array_length, array_length, writes_per_transaction, format, file, raw_file);
            }
        #endif
    }

    /**
     * Prints a result of a 2 core Blocked FIFO benchmark as well as writing it to the summary and raw CSV files.
     * 
     * The amount of reporting is configured using compile and runtime parameters (report_standalone, PRINT_STATS, ...).
     * 
     * This function is typically called in a loop to print/export all of the results (all of the configurations) of
     * a particular benchmark run.
     */
    template <typename elementType>
    void exportResultsBlockedFIFO2Core(bool report_standalone, Profiler* profiler, int cpu_a, int cpu_b, Results &results, size_t array_length, int block_length, std::string format, FILE* file, std::ofstream* raw_file){
        #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1 || WRITE_CSV == 1
            if(report_standalone)
            {
                if(!profiler->cpuTopology.empty()){
                    std::vector<int> sockets;
                    std::vector<int> cores;
                    std::vector<int> dies;
                    std::vector<int> threads;

                    getGranularityIndexsOfInterest({cpu_a, cpu_b}, profiler, sockets, cores, dies, threads);

                    #if PRINT_FULL_STATS == 1
                        results.print_statistics(sockets, dies, cores, threads, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        print_results_blocked_fifo_standalone(results, array_length*STIM_LEN, block_length, sizeof(elementType));
                    #endif
                }else{
                    #if PRINT_FULL_STATS == 1
                        results.print_statistics(0, 0, 0, cpu_a, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        print_results_blocked_fifo_standalone(results, array_length*STIM_LEN, block_length, sizeof(elementType));
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

    /**
     * Prints (and writes to the summary CSV file) a table of the results for a 2 Core FIFO benchmark.
     * 
     * Also populates the raw CSV file with results from the benchmark
     * 
     * Accompishes this by repeatedly calling the exportResults function
     */
    template <typename elementType>
    void printWriteFIFOTable2Core(bool report_standalone, Profiler* profiler, int cpu_a, int cpu_b, std::string title, std::string secondary_dimension_label, std::vector<Results> results_vec, std::vector<size_t> array_lengths, std::vector<int32_t> writes_per_transactions, int data_col_width, std::string format, FILE* file, std::ofstream* raw_file){
        int result_idx = 0;

        for(int i = 0; i<array_lengths.size(); i++)
        {
            size_t array_length = array_lengths[i];

            //Print the newline indent and new array length
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

    /**
     * Prints (and writes to the summary CSV file) a table of the results for a 2 Core Blocked FIFO benchmark.
     * 
     * Also populates the raw CSV file with results from the benchmark
     * 
     * Accompishes this by repeatedly calling the exportResults function
     */
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

    
    void writeCSVSummaryHeaderOpenLoop(FILE* file);
    std::string tableHeaderOpenLoop(std::string title, FILE* file);
    void printTitleOpenLoopPoint(bool report_standalone, std::string title, size_t array_length, size_t block_length, int balancing_nops, int initial_nops);

    void writeRawHeaderOpenLoop(std::vector<std::shared_ptr<BenchmarkSpecificResult>> implSpecificResults, std::ofstream* raw_file);

    /**
     * Prints a result of a 2 core open loop benchmark as well as writing it to the summary and raw CSV files.
     * 
     * The amount of reporting is configured using compile and runtime parameters (report_standalone, PRINT_STATS, ...).
     * 
     * This function is typically called in a loop to print/export all of the results (all of the configurations) of
     * a particular benchmark run.
     */
    template <typename elementType>
    void exportResultsOpenLoop(bool report_standalone, Profiler* profiler, std::vector<int> cpus, Results &results, size_t array_length, int32_t block_size, int32_t balance_nops, int initial_nops, std::string format, FILE* file, std::ofstream* raw_file){
        #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1 || WRITE_CSV == 1
            if(report_standalone)
            {
                if(!profiler->cpuTopology.empty()){
                    std::vector<int> sockets;
                    std::vector<int> cores;
                    std::vector<int> dies;
                    std::vector<int> threads;

                    getGranularityIndexsOfInterest(cpus, profiler, sockets, cores, dies, threads);

                    #if PRINT_FULL_STATS == 1
                        results.print_statistics(sockets, dies, cores, threads, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        print_results_fifoless_standalone(results);
                    #endif
                }else{
                    #if PRINT_FULL_STATS == 1
                        results.print_statistics(0, 0, 0, cpu_a, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        print_results_fifoless_standalone(results);
                    #endif
                }
            }
            else
            {
                print_results_open_loop(results, array_length, block_size, balance_nops, initial_nops, format, file, raw_file);
            }
        #endif
    }

    /**
     * Prints (and writes to the summary CSV file) a table of the results for an open loop benchmark.
     * 
     * Also populates the raw CSV file with results from the benchmark
     * 
     * Accompishes this by repeatedly calling the exportResults function
     */
    template <typename elementType>
    void printWriteOpenLoop2CoreResults(bool report_standalone, Profiler* profiler, std::vector<int> cpus, std::string title, std::vector<Results> results_vec, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, std::vector<int32_t> balance_nops, std::vector<int> initial_nops, std::string format, FILE* file, std::ofstream* raw_file){
        for(int i = 0; i<array_lengths.size(); i++)
        {
            size_t array_length = array_lengths[i];
            for(int j = 0; j<block_lengths.size(); j++)
            {
                int32_t block_length = block_lengths[j];
                for(int k = 0; k<balance_nops.size(); k++){
                    int32_t balance_nop = balance_nops[k];
                    for(int l = 0; l<initial_nops.size(); l++){
                        int initial_nop = initial_nops[l];
                        int idx = l+
                                k*initial_nops.size()+
                                j*initial_nops.size()*balance_nops.size()+
                                i*initial_nops.size()*balance_nops.size()*block_lengths.size();

                        //In this version, a row is created for each configuration (>2 dimensions)

                        //Print/Write individual results
                        printTitleOpenLoopPoint(report_standalone, title, array_length, block_length, balance_nop, initial_nop);
                        exportResultsOpenLoop<elementType>(report_standalone, profiler, cpus, results_vec[idx], array_length, block_length, balance_nop, initial_nop, format, file, raw_file);
                    }
                }
            }
        }

        //Print the newline
        #if WRITE_CSV == 1
        fprintf(file, "\n");
        fflush(file);
        #endif

        #if PRINT_TITLE == 1
        tableFooter();
        #endif
    }

    std::string tableHeaderClosedLoop(std::string title, FILE* file);

    void writeRawHeaderClosedLoop(std::vector<std::shared_ptr<BenchmarkSpecificResult>> implSpecificResults, std::ofstream* raw_file);

    void printTitleClosedLoopPoint(bool report_standalone, std::string title, size_t array_length, size_t block_length, int32_t server_control_period, int32_t client_control_period, int32_t control_gain, int initial_nop);

    /**
     * Prints a result of a 2 core open loop benchmark as well as writing it to the summary and raw CSV files.
     * 
     * The amount of reporting is configured using compile and runtime parameters (report_standalone, PRINT_STATS, ...).
     * 
     * This function is typically called in a loop to print/export all of the results (all of the configurations) of
     * a particular benchmark run.
     */
    template <typename elementType>
    void exportResultsClosedLoop(bool report_standalone, Profiler* profiler, std::vector<int> cpus, Results &results, size_t array_length, int32_t block_size, int32_t server_control_period, int32_t client_control_period, int32_t control_gain, int initial_nop, std::string format, FILE* file, std::ofstream* raw_file){
        #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1 || WRITE_CSV == 1
            if(report_standalone)
            {
                if(!profiler->cpuTopology.empty()){
                    std::vector<int> sockets;
                    std::vector<int> cores;
                    std::vector<int> dies;
                    std::vector<int> threads;

                    getGranularityIndexsOfInterest(cpus, profiler, sockets, cores, dies, threads);

                    #if PRINT_FULL_STATS == 1
                        results.print_statistics(sockets, dies, cores, threads, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        print_results_fifoless_standalone(results);
                    #endif
                }else{
                    #if PRINT_FULL_STATS == 1
                        results.print_statistics(0, 0, 0, cpu_a, STIM_LEN);
                    #endif

                    #if PRINT_STATS == 1
                        print_results_fifoless_standalone(results);
                    #endif
                }
            }
            else
            {
                print_results_closed_loop(results, array_length, block_size, server_control_period, client_control_period, control_gain, initial_nop, format, file, raw_file);
            }
        #endif
    }

    /**
     * Prints (and writes to the summary CSV file) a table of the results for a closed loop benchmark.
     * 
     * Also populates the raw CSV file with results from the benchmark
     * 
     * Accompishes this by repeatedly calling the exportResults function
     */
    template <typename elementType>
    void printWriteClosedLoop2CoreResults(bool report_standalone, Profiler* profiler, std::vector<int> cpus, std::string title, std::vector<Results> results_vec, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, std::vector<int32_t> server_control_periods, std::vector<int32_t> client_control_periods, std::vector<int32_t> control_gains, std::vector<int> initial_nops, std::string format, FILE* file, std::ofstream* raw_file){
    for(int i = 0; i<array_lengths.size(); i++)
    {
        size_t array_length = array_lengths[i];
        for(int j = 0; j<block_lengths.size(); j++)
        {
            int32_t block_length = block_lengths[j];
            for(int k = 0; k<server_control_periods.size(); k++)
            {
                int32_t server_control_period = server_control_periods[k];
                for(int n = 0; n<client_control_periods.size(); n++)
                {
                    int32_t client_control_period = client_control_periods[n];
                    for(int m = 0; m<control_gains.size(); m++)
                    {
                        int32_t control_gain = control_gains[m];
                        for(int l = 0; l<initial_nops.size(); l++){
                            int initial_nop = initial_nops[l];

                            int idx = l + 
                                    m*initial_nops.size() + 
                                    n*initial_nops.size()*control_gains.size() +
                                    k*initial_nops.size()*control_gains.size()*client_control_periods.size() + 
                                    j*initial_nops.size()*control_gains.size()*client_control_periods.size()*server_control_periods.size() +
                                    i*initial_nops.size()*control_gains.size()*client_control_periods.size()*server_control_periods.size()*block_lengths.size();

                            //In this version, a row is created for each configuration (>2 dimensions)

                            //Print/Write individual results
                            printTitleClosedLoopPoint(report_standalone, title, array_length, block_length, server_control_period, client_control_period, control_gain, initial_nop);
                            exportResultsClosedLoop<elementType>(report_standalone, profiler, cpus, results_vec[idx], array_length, block_length, server_control_period, client_control_period, control_gain, initial_nop, format, file, raw_file);
                        }
                    }
                }
            }
        }
    }

        //Print the newline
        #if WRITE_CSV == 1
        fprintf(file, "\n");
        fflush(file);
        #endif

        #if PRINT_TITLE == 1
        tableFooter();
        #endif
    }

#endif

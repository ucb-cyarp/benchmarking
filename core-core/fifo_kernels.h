#ifndef _H_FIFO_KERNELS
    #define _H_FIFO_KERNELS

    #include "cpucounters.h"
    #include "results.h"
    #include "pcm_helper.h"

    #include "kernel_server_runner.h"

    #include "bandwidth_circular_fifo_kernel.h"
    #include "bandwidth_circular_fifo_blocked_kernel.h"
    #include "bandwidth_circular_fifo_read_limit_kernel.h"

    #include "print_results.h"

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

                        print_results(results, sizeof(*shared_array_loc), STIM_LEN); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
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
            printf("Array Length: %lu int32_t Elements, Block Length Transaction: %d\n", array_length, block_length);
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
                        print_results_blocked_fifo(results, sizeof(*shared_array_loc), STIM_LEN, block_length);
                        #endif

                #else
                        #if PRINT_FULL_STATS
                        results->print_statistics(0, cpu_a, STIM_LEN);
                        #endif

                        print_results_blocked_fifo(results, sizeof(*shared_array_loc), STIM_LEN, block_length); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
                #endif
            }
            else
            {
                print_results_blocked_fifo(results, sizeof(*shared_array_loc), STIM_LEN, array_length, block_length, format, file, raw_file); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
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

                    Results* latency_fifo_kernel_results = run_bandwidth_fifo_blocked_kernel(pcm, cpu_a, cpu_b, array_length, block_length, false, format, file, raw_file);

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

    Results* run_bandwidth_fifo_read_limit_kernel(PCM* pcm, int cpu_a, int cpu_b, size_t array_length, int32_t max_elements_per_transaction, bool report_standalone=true, std::string format = "", FILE* file=NULL, std::ofstream* raw_file=NULL)
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
        int32_t* shared_array_loc = new int32_t[array_length];
        int32_t* shared_write_id_loc = new int32_t;
        int32_t* shared_read_id_loc = new int32_t;

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

        Results* results = execute_client_server_kernel(pcm, bandwidth_circular_fifo_read_limit_server_kernel, bandwidth_circular_fifo_read_limit_client_kernel, bandwidth_circular_fifo_read_limit_kernel_reset, args, args, args, cpu_a, cpu_b);

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
                print_results(results, sizeof(*shared_array_loc), STIM_LEN, array_length, max_elements_per_transaction, format, file, raw_file); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
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
    void run_bandwidth_fifo_read_limit_kernel(PCM* pcm, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, std::vector<int32_t> max_elements_per_transaction, FILE* file = NULL, std::ofstream* raw_file=NULL)
    {
        int32_t data_col_width = 10;

        //Print header
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
            printf("\n        %27lu", array_length);
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

                    Results* latency_fifo_kernel_results = run_bandwidth_fifo_read_limit_kernel(pcm, cpu_a, cpu_b, array_length, max_elements, false, format, file, raw_file);

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
        for(int i = 0; i<max_elements_per_transaction.size(); i++)
        {
            for(int j = 0; j<data_col_width; j++)
            {
                printf("=");
            }
        }
        printf("\n");
    }
#endif
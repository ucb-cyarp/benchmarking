#ifndef _H_ARRAY_KERNELS
    #define _H_ARRAY_KERNELS

    #include "cpucounters.h"
    #include "results.h"
    #include "pcm_helper.h"

    #include "kernel_server_runner.h"

    #include "latency_single_array_kernel.h"
    #include "latency_dual_array_kernel.h"
    #include "latency_flow_ctrl_kernel.h"
    #include "latency_flow_ctrl_blocked_read_kernel.h"

    #include "print_results.h"

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

                        print_results(results, sizeof(*shared_loc_a)*array_length, STIM_LEN);
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

                        print_results(results, sizeof(*shared_array_loc)*array_length, STIM_LEN/2); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
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

                        print_results(results, sizeof(*shared_array_loc)*array_length, STIM_LEN/2); //Div by 2 is because the counter increments for each direction of the FIFO transaction (transmit and ack)
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
#endif
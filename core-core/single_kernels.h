#ifndef _H_SINGLE_KERNELS
    #define _H_SINGLE_KERNELS

    #include "cpucounters.h"
    #include "results.h"
    #include "pcm_helper.h"

    #include "kernel_server_runner.h"
    
    #include "latency_single_kernel.h"
    #include "latency_dual_kernel.h"
    
    #include "print_results.h"

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
#endif
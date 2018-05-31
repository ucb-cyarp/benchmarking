#ifndef _H_KERNEL_SERVER_RUNNER
    #define _H_KERNEL_SERVER_RUNNER

    #include "results.h"
    #include "cpucounters.h"
    #include "pcm_helper.h"

    #include "intrin_bench_default_defines.h"

    class KernelExeWrapperArgs
    {
        public:
            PCM* pcm;
            void* (*kernel_fun)(void*);
            void* kernel_arg;
            int cpu_num;
    };

    class SimultaniousResults
    {
        public:
            Results* results_a;
            Results* results_b;
    };

    //Returns a pointer to a Results object
    //Returns NULL if freq change event occured
    void* kernel_exe_wrapper(void *arg)
    {
        KernelExeWrapperArgs* args = (KernelExeWrapperArgs*) arg;

        //Get args from object
        PCM* pcm = args->pcm;
        void* (*kernel_fun)(void*) = args->kernel_fun;
        void* kernel_arg = args->kernel_arg;
        int cpu_num = args->cpu_num;

#if USE_PCM == 1
        int sockets = pcm->getNumSockets();
        int cores = pcm->getNumCores();
#else
        int sockets = 1;
        int cores = 1;
#endif

        Results* results = new Results(sockets, cores);

        //Allocate measurement arrays
        std::chrono::duration<double, std::ratio<1, 1000>> durations[1]; //TODO: setting to 1 trial here.  Trials handled outside kernel_server_wrapper

#if USE_PCM == 1
        //Allocate counter states
        ServerUncorePowerState* startPowerState = new ServerUncorePowerState[sockets];
        ServerUncorePowerState* endPowerState = new ServerUncorePowerState[sockets];
        std::vector<CoreCounterState> startCstates, endCstates;
        std::vector<SocketCounterState> startSktstate, endSktstate;
        SystemCounterState startSstate, endSstate;
#endif

        //TODO: ONLY do 1 trial here (trial 0), trials handled outside
        int trial = 0;

#if USE_PCM == 1
        //Get CPU Core/Socket/Power States
        for (int i = 0; i < sockets; i++)
            startPowerState[i] = pcm->getServerUncorePowerState(i);
        pcm->getAllCounterStates(startSstate, startSktstate, startCstates);
#endif

        //Start Timer
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        clock_t start_clock = clock();
        uint64_t start_rdtsc = _rdtsc();

        //Run Kernel
        kernel_fun(kernel_arg);

        //Stop Timer 
        uint64_t stop_rdtsc = _rdtsc();
        clock_t stop_clock = clock();
        std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();

#if USE_PCM == 1
        //Get CPU Core/Socket/Power States
        pcm->getAllCounterStates(endSstate, endSktstate, endCstates);
        for (int i = 0; i < sockets; i++)
            endPowerState[i] = pcm->getServerUncorePowerState(i);
#endif
        
        TrialResult* trial_result = new TrialResult(sockets, cores, trial);

        //Report Time
        calculate_durations(trial_result->duration, trial_result->duration_clock, trial_result->duration_rdtsc, start, stop, start_clock, stop_clock, start_rdtsc, stop_rdtsc);
        
#if USE_PCM == 1
        //Report Freq, Power
        calc_freq_and_power(pcm, trial_result->avgCPUFreq, trial_result->avgActiveCPUFreq, trial_result->energyCPUUsed, trial_result->energyDRAMUsed,
        startCstates, endCstates, startPowerState, endPowerState);
#else
        trial_result->avgCPUFreq[0] = 0;
        trial_result->avgActiveCPUFreq[0] = 0;
        trial_result->energyCPUUsed[0] = 0;
        trial_result->energyDRAMUsed[0] = 0;
#endif

#if USE_PCM == 1
        //Report Temp
        calc_temp(pcm, trial_result->startPackageThermalHeadroom, trial_result->endPackageThermalHeadroom, startPowerState, startPowerState);
#else
        trial_result->startPackageThermalHeadroom[0] = -1;
        trial_result->endPackageThermalHeadroom[0] = -1;
#endif

        #if PRINT_TRIALS == 1
            trial_result->print_trial();
        #endif 

#if USE_PCM == 1
        //Limit check to socket of interest (single socket for now)
        std::vector<int> sockets_of_interest;
        sockets_of_interest.push_back(pcm->getSocketId(cpu_num));

        bool freq_change_events_occured = check_any_freq_changes(pcm, startPowerState, endPowerState, sockets_of_interest);
        //Proceed if no freq changes occured
        if(freq_change_events_occured == false)
        {
            results->add_trial(trial_result);
        }
        else
        {
            //Cleanup and return null since we had a freq change event
            delete[] startPowerState;
            delete[] endPowerState;
            free(results);

            return NULL;
        }
#else
        //Not checking for freq change event.  Increment
        results->add_trial(trial_result);
#endif
            
#if USE_PCM == 1
        delete[] startPowerState;
        delete[] endPowerState;
#endif
        
        return (void*) results;
    }


    Results* execute_kernel(PCM* pcm, void* (*kernel_fun)(void*), void* (*kernel_reset)(void*), void* arg_a, void* arg_b, void* reset_arg, int cpu_a, int cpu_b)
    {
        #if USE_PCM == 1
            int sockets = pcm->getNumSockets();
            int cores = pcm->getNumCores();
        #else
            int sockets = 1;
            int cores = 1;
        #endif

        Results* results = new Results(sockets, cores);

        int trial = 0;
        int discard_count=0;
        while(trial<TRIALS)
        {
            //Reset before creating threads (and before timer started)
            kernel_reset(reset_arg);

            //=====Create a thread for the server and client on the specified cores=====

            cpu_set_t cpuset_a, cpuset_b;
            pthread_t thread_a, thread_b;
            pthread_attr_t attr_a, attr_b;
            void *res_a, *res_b;

            int status;

            //Create pthread attributes
            status = pthread_attr_init(&attr_a);
            if(status != 0)
            {
                printf("Could not create pthread attributes ... exiting\n");
                exit(1);
            }

            status = pthread_attr_init(&attr_b);
            if(status != 0)
            {
                printf("Could not create pthread attributes ... exiting\n");
                exit(1);
            }

            //Set CPU affinity
            CPU_ZERO(&cpuset_a);
            CPU_SET(cpu_a, &cpuset_a);
            status = pthread_attr_setaffinity_np(&attr_a, sizeof(cpu_set_t), &cpuset_a);
            if(status != 0)
            {
                printf("Could not set thread core affinity ... exiting\n");
                exit(1);
            }

            CPU_ZERO(&cpuset_b);
            CPU_SET(cpu_b, &cpuset_b);
            status = pthread_attr_setaffinity_np(&attr_b, sizeof(cpu_set_t), &cpuset_b);
            if(status != 0)
            {
                printf("Could not set thread core affinity ... exiting\n");
                exit(1);
            }

            //Create threads.  Create thread b (client) before thread a (server) which performs measurments
            // - Start Thread B
            status = pthread_create(&thread_b, &attr_b, kernel_fun, arg_b);
            if(status != 0)
            {
                printf("Could not create b thread ... exiting\n");
                errno = status;
                perror(NULL);
                exit(1);
            }

            // status = pthread_detach(thread_b);
            // if(status != 0)
            // {
            //     printf("Could not detach thread ... exiting\n");
            //     exit(1);
            // }

            // - Start Thread A
            KernelExeWrapperArgs* server_args = new KernelExeWrapperArgs();
            server_args->pcm = pcm;
            server_args->kernel_fun = kernel_fun;
            server_args->kernel_arg = arg_a;
            server_args->cpu_num = cpu_a;

            status = pthread_create(&thread_a, &attr_a, kernel_exe_wrapper, server_args);
            if(status != 0)
            {
                printf("Could not create a thread ... exiting\n");
                errno = status;
                perror(NULL);
                exit(1);
            }

            //Wait for server thread (measuring thread) to finish
            status = pthread_join(thread_a, &res_a);
            if(status != 0)
            {
                printf("Could not join a thread ... exiting\n");
                errno = status;
                perror(NULL);
                exit(1);
            }

            //Wait for client thread to finish
            status = pthread_join(thread_b, &res_b);
            if(status != 0)
            {
                printf("Could not join thread ... exiting\n");
                exit(1);
            }

            //Parse results
            if(res_a != NULL)
            {
                //Decode the results from the server thread
                Results* result_obj = (Results*) res_a;

                //Copy trial into results
                for(size_t i = 0; i<result_obj->trial_results.size(); i++)
                {
                    results->add_trial((result_obj->trial_results)[i]);
                }

                trial++;
            }
            else
            {
                discard_count++;
                #if PRINT_FREQ_CHANGE_EVENT == 1
                    printf("Frequency Change Event Occured Durring Kernel Run ... Discarding Run\n");
                #endif

                if(discard_count >= MAX_DISCARD-1)
                {
                    #if PRINT_FREQ_CHANGE_EVENT == 1
                        printf("Max Discards Reached ... Exiting\n");
                    #endif
                    exit(1);
                }
            }

            //Delete temportary results
            free(res_a);
            free(res_b);

            delete server_args;
        }

        return results;
    }

    SimultaniousResults* execute_kernel(PCM* pcm, void* (*kernel_fun)(void*), void* (*kernel_reset)(void*), void* arg_a, void* arg_b, void* arg_c, void* arg_d, void* reset_arg_1, void* reset_arg_2, int cpu_a, int cpu_b, int cpu_c, int cpu_d)
    {
        #if USE_PCM == 1
            int sockets = pcm->getNumSockets();
            int cores = pcm->getNumCores();
        #else
            int sockets = 1;
            int cores = 1;
        #endif

        Results* results_a = new Results(sockets, cores);
        Results* results_c = new Results(sockets, cores);

        SimultaniousResults* simultanious_results = new SimultaniousResults();

        int trial = 0;
        int discard_count=0;
        while(trial<TRIALS)
        {
            //Reset before creating threads (and before timer started)
            kernel_reset(reset_arg_1);
            kernel_reset(reset_arg_2);

            //=====Create a thread for the server and client on the specified cores=====

            cpu_set_t cpuset_a, cpuset_b, cpuset_c, cpuset_d;
            pthread_t thread_a, thread_b, thread_c, thread_d;
            pthread_attr_t attr_a, attr_b, attr_c, attr_d;
            void *res_a, *res_b, *res_c, *res_d;

            int status;

            //Create pthread attributes
            status = pthread_attr_init(&attr_a);
            if(status != 0)
            {
                printf("Could not create pthread attributes ... exiting\n");
                exit(1);
            }

            status = pthread_attr_init(&attr_b);
            if(status != 0)
            {
                printf("Could not create pthread attributes ... exiting\n");
                exit(1);
            }

            status = pthread_attr_init(&attr_c);
            if(status != 0)
            {
                printf("Could not create pthread attributes ... exiting\n");
                exit(1);
            }

            status = pthread_attr_init(&attr_d);
            if(status != 0)
            {
                printf("Could not create pthread attributes ... exiting\n");
                exit(1);
            }

            //Set CPU affinity
            CPU_ZERO(&cpuset_a);
            CPU_SET(cpu_a, &cpuset_a);
            status = pthread_attr_setaffinity_np(&attr_a, sizeof(cpu_set_t), &cpuset_a);
            if(status != 0)
            {
                printf("Could not set thread core affinity ... exiting\n");
                exit(1);
            }

            CPU_ZERO(&cpuset_b);
            CPU_SET(cpu_b, &cpuset_b);
            status = pthread_attr_setaffinity_np(&attr_b, sizeof(cpu_set_t), &cpuset_b);
            if(status != 0)
            {
                printf("Could not set thread core affinity ... exiting\n");
                exit(1);
            }

            CPU_ZERO(&cpuset_c);
            CPU_SET(cpu_c, &cpuset_c);
            status = pthread_attr_setaffinity_np(&attr_c, sizeof(cpu_set_t), &cpuset_c);
            if(status != 0)
            {
                printf("Could not set thread core affinity ... exiting\n");
                exit(1);
            }

            CPU_ZERO(&cpuset_d);
            CPU_SET(cpu_d, &cpuset_d);
            status = pthread_attr_setaffinity_np(&attr_d, sizeof(cpu_set_t), &cpuset_d);
            if(status != 0)
            {
                printf("Could not set thread core affinity ... exiting\n");
                exit(1);
            }

            //Create threads.  Create thread b and d (client) before thread a and c (server) which performs measurments
            // - Start Thread B & D
            status = pthread_create(&thread_b, &attr_b, kernel_fun, arg_b);
            if(status != 0)
            {
                printf("Could not create b thread ... exiting\n");
                errno = status;
                perror(NULL);
                exit(1);
            }

            status = pthread_create(&thread_d, &attr_d, kernel_fun, arg_d); //both use arg_b
            if(status != 0)
            {
                printf("Could not create d thread ... exiting\n");
                errno = status;
                perror(NULL);
                exit(1);
            }

            // - Start Thread A & C
            KernelExeWrapperArgs* server_args_a = new KernelExeWrapperArgs();
            server_args_a->pcm = pcm;
            server_args_a->kernel_fun = kernel_fun;
            server_args_a->kernel_arg = arg_a;
            server_args_a->cpu_num = cpu_a;

            status = pthread_create(&thread_a, &attr_a, kernel_exe_wrapper, server_args_a);
            if(status != 0)
            {
                printf("Could not create a thread ... exiting\n");
                errno = status;
                perror(NULL);
                exit(1);
            }

            KernelExeWrapperArgs* server_args_c = new KernelExeWrapperArgs();
            server_args_c->pcm = pcm;
            server_args_c->kernel_fun = kernel_fun;
            server_args_c->kernel_arg = arg_c;
            server_args_c->cpu_num = cpu_c;

            status = pthread_create(&thread_c, &attr_c, kernel_exe_wrapper, server_args_c);
            if(status != 0)
            {
                printf("Could not create a thread ... exiting\n");
                errno = status;
                perror(NULL);
                exit(1);
            }

            //Wait for server threads (measuring thread) to finish
            status = pthread_join(thread_a, &res_a);
            if(status != 0)
            {
                printf("Could not join a thread ... exiting\n");
                errno = status;
                perror(NULL);
                exit(1);
            }

            status = pthread_join(thread_c, &res_c);
            if(status != 0)
            {
                printf("Could not join a thread ... exiting\n");
                errno = status;
                perror(NULL);
                exit(1);
            }

            //Wait for client thread to finish
            status = pthread_join(thread_b, &res_b);
            if(status != 0)
            {
                printf("Could not join thread ... exiting\n");
                exit(1);
            }

            status = pthread_join(thread_d, &res_d);
            if(status != 0)
            {
                printf("Could not join thread ... exiting\n");
                exit(1);
            }

            //Parse results
            if(res_a != NULL && res_c != NULL) //Check that both parallel threads did not expeirnece a frequency change.
            {
                //Decode the results from the server thread
                Results* result_obj_a = (Results*) res_a;
                Results* result_obj_c = (Results*) res_c;

                //Copy trial into results
                for(size_t i = 0; i<result_obj_a->trial_results.size(); i++)
                {
                    results_a->add_trial((result_obj_a->trial_results)[i]);
                }

                //Copy trial into results
                for(size_t i = 0; i<result_obj_c->trial_results.size(); i++)
                {
                    results_c->add_trial((result_obj_c->trial_results)[i]);
                }

                trial++;
            }
            else
            {
                discard_count++;
                #if PRINT_FREQ_CHANGE_EVENT == 1
                    printf("Frequency Change Event Occured Durring Kernel Run ... Discarding Run\n");
                #endif

                if(discard_count >= MAX_DISCARD-1)
                {
                    #if PRINT_FREQ_CHANGE_EVENT == 1
                        printf("Max Discards Reached ... Exiting\n");
                    #endif
                    exit(1);
                }
            }

            //Delete temportary results
            free(res_a);
            free(res_b);
            free(res_c);
            free(res_d);

            delete server_args_a;
            delete server_args_c;
        }

        simultanious_results->results_a = results_a;
        simultanious_results->results_b = results_c;

        return simultanious_results;
    }

    SimultaniousResults* execute_kernel_fanin_server_measure(PCM* pcm, void* (*srv_kernel_fun)(void*), void* (*cli_kernel_fun)(void*), void* (*kernel_reset)(void*), void* arg_srv_a, void* arg_srv_b, void* arg_cli_c, void* reset_arg_1, int cpu_srv_a, int cpu_srv_b, int cpu_cli_c)
    {
        #if USE_PCM == 1
            int sockets = pcm->getNumSockets();
            int cores = pcm->getNumCores();
        #else
            int sockets = 1;
            int cores = 1;
        #endif

        Results* results_a = new Results(sockets, cores);
        Results* results_b = new Results(sockets, cores);

        SimultaniousResults* simultanious_results = new SimultaniousResults();

        int trial = 0;
        int discard_count=0;
        while(trial<TRIALS)
        {
            //Reset before creating threads (and before timer started)
            kernel_reset(reset_arg_1);

            //=====Create a thread for the server and client on the specified cores=====

            cpu_set_t cpuset_srv_a, cpuset_srv_b, cpuset_cli_c;
            pthread_t thread_srv_a, thread_srv_b, thread_cli_c;
            pthread_attr_t attr_srv_a, attr_srv_b, attr_cli_c;
            void *res_srv_a, *res_srv_b, *res_cli_c;

            int status;

            //Create pthread attributes
            status = pthread_attr_init(&attr_srv_a);
            if(status != 0)
            {
                printf("Could not create pthread attributes ... exiting\n");
                exit(1);
            }

            status = pthread_attr_init(&attr_srv_b);
            if(status != 0)
            {
                printf("Could not create pthread attributes ... exiting\n");
                exit(1);
            }

            status = pthread_attr_init(&attr_cli_c);
            if(status != 0)
            {
                printf("Could not create pthread attributes ... exiting\n");
                exit(1);
            }

            //Set CPU affinity
            CPU_ZERO(&cpuset_srv_a);
            CPU_SET(cpu_srv_a, &cpuset_srv_a);
            status = pthread_attr_setaffinity_np(&attr_srv_a, sizeof(cpu_set_t), &cpuset_srv_a);
            if(status != 0)
            {
                printf("Could not set thread core affinity ... exiting\n");
                exit(1);
            }

            CPU_ZERO(&cpuset_srv_b);
            CPU_SET(cpu_srv_b, &cpuset_srv_b);
            status = pthread_attr_setaffinity_np(&attr_srv_b, sizeof(cpu_set_t), &cpuset_srv_b);
            if(status != 0)
            {
                printf("Could not set thread core affinity ... exiting\n");
                exit(1);
            }

            CPU_ZERO(&cpuset_cli_c);
            CPU_SET(cpu_cli_c, &cpuset_cli_c);
            status = pthread_attr_setaffinity_np(&attr_cli_c, sizeof(cpu_set_t), &cpuset_cli_c);
            if(status != 0)
            {
                printf("Could not set thread core affinity ... exiting\n");
                exit(1);
            }


            //Create threads.  Create thread c (client) before thread a and b (server) which perform measurments
            // - Start Thread c
            status = pthread_create(&thread_cli_c, &attr_cli_c, cli_kernel_fun, arg_cli_c);
            if(status != 0)
            {
                printf("Could not create c thread ... exiting\n");
                errno = status;
                perror(NULL);
                exit(1);
            }

            // - Start Thread A & B
            KernelExeWrapperArgs* server_args_a = new KernelExeWrapperArgs();
            server_args_a->pcm = pcm;
            server_args_a->kernel_fun = srv_kernel_fun;
            server_args_a->kernel_arg = arg_srv_a;
            server_args_a->cpu_num = cpu_srv_a;

            status = pthread_create(&thread_srv_a, &attr_srv_a, kernel_exe_wrapper, server_args_a);
            if(status != 0)
            {
                printf("Could not create a thread ... exiting\n");
                errno = status;
                perror(NULL);
                exit(1);
            }

            KernelExeWrapperArgs* server_args_b = new KernelExeWrapperArgs();
            server_args_b->pcm = pcm;
            server_args_b->kernel_fun = srv_kernel_fun;
            server_args_b->kernel_arg = arg_srv_b;
            server_args_b->cpu_num = cpu_srv_b;

            status = pthread_create(&thread_srv_b, &attr_srv_b, kernel_exe_wrapper, server_args_b);
            if(status != 0)
            {
                printf("Could not create b thread ... exiting\n");
                errno = status;
                perror(NULL);
                exit(1);
            }

            //Wait for server threads (measuring thread) to finish
            status = pthread_join(thread_srv_a, &res_srv_a);
            if(status != 0)
            {
                printf("Could not join a thread ... exiting\n");
                errno = status;
                perror(NULL);
                exit(1);
            }

            status = pthread_join(thread_srv_b, &res_srv_b);
            if(status != 0)
            {
                printf("Could not join a thread ... exiting\n");
                errno = status;
                perror(NULL);
                exit(1);
            }

            //Wait for client thread to finish
            status = pthread_join(thread_cli_c, &res_cli_c);
            if(status != 0)
            {
                printf("Could not join thread ... exiting\n");
                exit(1);
            }

            //Parse results
            if(res_srv_a != NULL && res_srv_b != NULL) //Check that both parallel threads did not expeirnece a frequency change.
            {
                //Decode the results from the server thread
                Results* result_obj_a = (Results*) res_srv_a;
                Results* result_obj_b = (Results*) res_srv_b;

                //Copy trial into results
                for(size_t i = 0; i<result_obj_a->trial_results.size(); i++)
                {
                    results_a->add_trial((result_obj_a->trial_results)[i]);
                }

                //Copy trial into results
                for(size_t i = 0; i<result_obj_b->trial_results.size(); i++)
                {
                    results_b->add_trial((result_obj_b->trial_results)[i]);
                }

                trial++;
            }
            else
            {
                discard_count++;
                #if PRINT_FREQ_CHANGE_EVENT == 1
                    printf("Frequency Change Event Occured Durring Kernel Run ... Discarding Run\n");
                #endif

                if(discard_count >= MAX_DISCARD-1)
                {
                    #if PRINT_FREQ_CHANGE_EVENT == 1
                        printf("Max Discards Reached ... Exiting\n");
                    #endif
                    exit(1);
                }
            }

            //Delete temportary results
            free(res_srv_a);
            free(res_srv_b);
            free(res_cli_c);

            delete server_args_a;
            delete server_args_b;
        }

        simultanious_results->results_a = results_a;
        simultanious_results->results_b = results_b;

        return simultanious_results;
    }

    //TODO: Note, this version is different from the other in that the server thread starts before the client.  This may allow the server to execute a transaction before the client starts.
    //This can potentially be addressed by changing the initial state of the tests and the arguments passed to the kernels
    SimultaniousResults* execute_kernel_fanout_client_measure(PCM* pcm, void* (*srv_kernel_fun)(void*), void* (*cli_kernel_fun)(void*), void* (*kernel_reset)(void*), void* arg_srv_a, void* arg_cli_b, void* arg_cli_c, void* reset_arg_1, int cpu_srv_a, int cpu_cli_b, int cpu_cli_c)
    {
        #if USE_PCM == 1
            int sockets = pcm->getNumSockets();
            int cores = pcm->getNumCores();
        #else
            int sockets = 1;
            int cores = 1;
        #endif

        Results* results_b = new Results(sockets, cores);
        Results* results_c = new Results(sockets, cores);

        SimultaniousResults* simultanious_results = new SimultaniousResults();

        int trial = 0;
        int discard_count=0;
        while(trial<TRIALS)
        {
            //Reset before creating threads (and before timer started)
            kernel_reset(reset_arg_1);

            //=====Create a thread for the server and client on the specified cores=====

            cpu_set_t cpuset_srv_a, cpuset_cli_b, cpuset_cli_c;
            pthread_t thread_srv_a, thread_cli_b, thread_cli_c;
            pthread_attr_t attr_srv_a, attr_cli_b, attr_cli_c;
            void *res_srv_a, *res_cli_b, *res_cli_c;

            int status;

            //Create pthread attributes
            status = pthread_attr_init(&attr_srv_a);
            if(status != 0)
            {
                printf("Could not create pthread attributes ... exiting\n");
                exit(1);
            }

            status = pthread_attr_init(&attr_cli_b);
            if(status != 0)
            {
                printf("Could not create pthread attributes ... exiting\n");
                exit(1);
            }

            status = pthread_attr_init(&attr_cli_c);
            if(status != 0)
            {
                printf("Could not create pthread attributes ... exiting\n");
                exit(1);
            }

            //Set CPU affinity
            CPU_ZERO(&cpuset_srv_a);
            CPU_SET(cpu_srv_a, &cpuset_srv_a);
            status = pthread_attr_setaffinity_np(&attr_srv_a, sizeof(cpu_set_t), &cpuset_srv_a);
            if(status != 0)
            {
                printf("Could not set thread core affinity ... exiting\n");
                exit(1);
            }

            CPU_ZERO(&cpuset_cli_b);
            CPU_SET(cpu_cli_b, &cpuset_cli_b);
            status = pthread_attr_setaffinity_np(&attr_cli_b, sizeof(cpu_set_t), &cpuset_cli_b);
            if(status != 0)
            {
                printf("Could not set thread core affinity ... exiting\n");
                exit(1);
            }

            CPU_ZERO(&cpuset_cli_c);
            CPU_SET(cpu_cli_c, &cpuset_cli_c);
            status = pthread_attr_setaffinity_np(&attr_cli_c, sizeof(cpu_set_t), &cpuset_cli_c);
            if(status != 0)
            {
                printf("Could not set thread core affinity ... exiting\n");
                exit(1);
            }

            //Create threads.  Create thread a (server) before thread b and c (client) which performs measurments
            // - Start Thread A
            status = pthread_create(&thread_srv_a, &attr_srv_a, srv_kernel_fun, arg_srv_a);
            if(status != 0)
            {
                printf("Could not create a thread ... exiting\n");
                errno = status;
                perror(NULL);
                exit(1);
            }

            // - Start Thread B & C (client)
            KernelExeWrapperArgs* measure_args_b = new KernelExeWrapperArgs();
            measure_args_b->pcm = pcm;
            measure_args_b->kernel_fun = cli_kernel_fun;
            measure_args_b->kernel_arg = arg_cli_b;
            measure_args_b->cpu_num = cpu_cli_b;

            status = pthread_create(&thread_cli_b, &attr_cli_b, kernel_exe_wrapper, measure_args_b);
            if(status != 0)
            {
                printf("Could not create b thread ... exiting\n");
                errno = status;
                perror(NULL);
                exit(1);
            }

            KernelExeWrapperArgs* measure_args_c = new KernelExeWrapperArgs();
            measure_args_c->pcm = pcm;
            measure_args_c->kernel_fun = cli_kernel_fun;
            measure_args_c->kernel_arg = arg_cli_c;
            measure_args_c->cpu_num = cpu_cli_c;

            status = pthread_create(&thread_cli_c, &attr_cli_c, kernel_exe_wrapper, measure_args_c);
            if(status != 0)
            {
                printf("Could not create c thread ... exiting\n");
                errno = status;
                perror(NULL);
                exit(1);
            }

            //Wait for client threads (measuring thread) to finish
            status = pthread_join(thread_cli_b, &res_cli_b);
            if(status != 0)
            {
                printf("Could not join b thread ... exiting\n");
                errno = status;
                perror(NULL);
                exit(1);
            }

            status = pthread_join(thread_cli_c, &res_cli_c);
            if(status != 0)
            {
                printf("Could not join c thread ... exiting\n");
                errno = status;
                perror(NULL);
                exit(1);
            }

            //Wait for server thread to finish
            status = pthread_join(thread_srv_a, &res_srv_a);
            if(status != 0)
            {
                printf("Could not join a thread ... exiting\n");
                exit(1);
            }

            //Parse results
            if(res_cli_b != NULL && res_cli_c != NULL) //Check that both parallel threads did not expeirnece a frequency change.
            {
                //Decode the results from the server thread
                Results* result_obj_b = (Results*) res_cli_b;
                Results* result_obj_c = (Results*) res_cli_c;

                //Copy trial into results
                for(size_t i = 0; i<result_obj_b->trial_results.size(); i++)
                {
                    results_b->add_trial((result_obj_b->trial_results)[i]);
                }

                //Copy trial into results
                for(size_t i = 0; i<result_obj_c->trial_results.size(); i++)
                {
                    results_c->add_trial((result_obj_c->trial_results)[i]);
                }

                trial++;
            }
            else
            {
                discard_count++;
                #if PRINT_FREQ_CHANGE_EVENT == 1
                    printf("Frequency Change Event Occured Durring Kernel Run ... Discarding Run\n");
                #endif

                if(discard_count >= MAX_DISCARD-1)
                {
                    #if PRINT_FREQ_CHANGE_EVENT == 1
                        printf("Max Discards Reached ... Exiting\n");
                    #endif
                    exit(1);
                }
            }

            //Delete temportary results
            free(res_srv_a);
            free(res_cli_b);
            free(res_cli_c);

            delete measure_args_b;
            delete measure_args_c;
        }

        simultanious_results->results_a = results_b;
        simultanious_results->results_b = results_c;

        return simultanious_results;
    }

    Results* execute_client_server_kernel(PCM* pcm, void* (*kernel_server)(void*), void* (*kernel_client)(void*), void* (*kernel_reset)(void*), void* arg_server, void* arg_client, void* reset_arg, int cpu_a, int cpu_b)
    {
        #if USE_PCM == 1
            int sockets = pcm->getNumSockets();
            int cores = pcm->getNumCores();
        #else
            int sockets = 1;
            int cores = 1;
        #endif

        Results* results = new Results(sockets, cores);

        int trial = 0;
        int discard_count=0;
        while(trial<TRIALS)
        {
            //Reset before creating threads (and before timer started)
            kernel_reset(reset_arg);

            //=====Create a thread for the server and client on the specified cores=====

            cpu_set_t cpuset_a, cpuset_b;
            pthread_t thread_a, thread_b;
            pthread_attr_t attr_a, attr_b;
            void *res_a, *res_b;

            int status;

            //Create pthread attributes
            status = pthread_attr_init(&attr_a);
            if(status != 0)
            {
                printf("Could not create pthread attributes ... exiting\n");
                exit(1);
            }

            status = pthread_attr_init(&attr_b);
            if(status != 0)
            {
                printf("Could not create pthread attributes ... exiting\n");
                exit(1);
            }

            //Set CPU affinity
            CPU_ZERO(&cpuset_a);
            CPU_SET(cpu_a, &cpuset_a);
            status = pthread_attr_setaffinity_np(&attr_a, sizeof(cpu_set_t), &cpuset_a);
            if(status != 0)
            {
                printf("Could not set thread core affinity ... exiting\n");
                exit(1);
            }

            CPU_ZERO(&cpuset_b);
            CPU_SET(cpu_b, &cpuset_b);
            status = pthread_attr_setaffinity_np(&attr_b, sizeof(cpu_set_t), &cpuset_b);
            if(status != 0)
            {
                printf("Could not set thread core affinity ... exiting\n");
                exit(1);
            }

            //Create threads.  Create thread b (client) before thread a (server) which performs measurments
            // - Start Thread B
            status = pthread_create(&thread_b, &attr_b, kernel_client, arg_client);
            if(status != 0)
            {
                printf("Could not create b thread ... exiting\n");
                errno = status;
                perror(NULL);
                exit(1);
            }

            // status = pthread_detach(thread_b);
            // if(status != 0)
            // {
            //     printf("Could not detach thread ... exiting\n");
            //     exit(1);
            // }

            // - Start Thread A
            KernelExeWrapperArgs* server_args = new KernelExeWrapperArgs();
            server_args->pcm = pcm;
            server_args->kernel_fun = kernel_server;
            server_args->kernel_arg = arg_server;
            server_args->cpu_num = cpu_a;

            status = pthread_create(&thread_a, &attr_a, kernel_exe_wrapper, server_args);
            if(status != 0)
            {
                printf("Could not create a thread ... exiting\n");
                errno = status;
                perror(NULL);
                exit(1);
            }

            //Wait for server thread (measuring thread) to finish
            status = pthread_join(thread_a, &res_a);
            if(status != 0)
            {
                printf("Could not join a thread ... exiting\n");
                errno = status;
                perror(NULL);
                exit(1);
            }

            //Wait for client thread to finish
            status = pthread_join(thread_b, &res_b);
            if(status != 0)
            {
                printf("Could not join thread ... exiting\n");
                exit(1);
            }

            //Parse results
            if(res_a != NULL)
            {
                //Decode the results from the server thread
                Results* result_obj = (Results*) res_a;

                //Copy trial into results
                for(size_t i = 0; i<result_obj->trial_results.size(); i++)
                {
                    results->add_trial((result_obj->trial_results)[i]);
                }

                trial++;
            }
            else
            {
                discard_count++;
                #if PRINT_FREQ_CHANGE_EVENT == 1
                    printf("Frequency Change Event Occured Durring Kernel Run ... Discarding Run\n");
                #endif

                if(discard_count >= MAX_DISCARD-1)
                {
                    #if PRINT_FREQ_CHANGE_EVENT == 1
                        printf("Max Discards Reached ... Exiting\n");
                    #endif
                    exit(1);
                }
            }

            //Delete temportary results
            free(res_a);
            free(res_b);

            delete server_args;
        }

        return results;
    }

    SimultaniousResults* execute_client_server_kernel(PCM* pcm, void* (*kernel_server)(void*), void* (*kernel_client)(void*), void* (*kernel_reset)(void*), void* arg_server_1, void* arg_server_2, void* arg_client_1, void* arg_client_2, void* reset_arg_1, void* reset_arg_2, int cpu_a, int cpu_b, int cpu_c, int cpu_d)
    {
        #if USE_PCM == 1
            int sockets = pcm->getNumSockets();
            int cores = pcm->getNumCores();
        #else
            int sockets = 1;
            int cores = 1;
        #endif

        Results* results_a = new Results(sockets, cores);
        Results* results_c = new Results(sockets, cores);

        SimultaniousResults* simultanious_results = new SimultaniousResults();

        int trial = 0;
        int discard_count=0;
        while(trial<TRIALS)
        {
            //Reset before creating threads (and before timer started)
            kernel_reset(reset_arg_1);
            kernel_reset(reset_arg_2);

            //=====Create a thread for the server and client on the specified cores=====

            cpu_set_t cpuset_a, cpuset_b, cpuset_c, cpuset_d;
            pthread_t thread_a, thread_b, thread_c, thread_d;
            pthread_attr_t attr_a, attr_b, attr_c, attr_d;
            void *res_a, *res_b, *res_c, *res_d;

            int status;

            //Create pthread attributes
            status = pthread_attr_init(&attr_a);
            if(status != 0)
            {
                printf("Could not create pthread attributes ... exiting\n");
                exit(1);
            }

            status = pthread_attr_init(&attr_b);
            if(status != 0)
            {
                printf("Could not create pthread attributes ... exiting\n");
                exit(1);
            }

            status = pthread_attr_init(&attr_c);
            if(status != 0)
            {
                printf("Could not create pthread attributes ... exiting\n");
                exit(1);
            }

            status = pthread_attr_init(&attr_d);
            if(status != 0)
            {
                printf("Could not create pthread attributes ... exiting\n");
                exit(1);
            }

            //Set CPU affinity
            CPU_ZERO(&cpuset_a);
            CPU_SET(cpu_a, &cpuset_a);
            status = pthread_attr_setaffinity_np(&attr_a, sizeof(cpu_set_t), &cpuset_a);
            if(status != 0)
            {
                printf("Could not set thread core affinity ... exiting\n");
                exit(1);
            }

            CPU_ZERO(&cpuset_b);
            CPU_SET(cpu_b, &cpuset_b);
            status = pthread_attr_setaffinity_np(&attr_b, sizeof(cpu_set_t), &cpuset_b);
            if(status != 0)
            {
                printf("Could not set thread core affinity ... exiting\n");
                exit(1);
            }

            CPU_ZERO(&cpuset_c);
            CPU_SET(cpu_c, &cpuset_c);
            status = pthread_attr_setaffinity_np(&attr_c, sizeof(cpu_set_t), &cpuset_c);
            if(status != 0)
            {
                printf("Could not set thread core affinity ... exiting\n");
                exit(1);
            }

            CPU_ZERO(&cpuset_d);
            CPU_SET(cpu_d, &cpuset_d);
            status = pthread_attr_setaffinity_np(&attr_d, sizeof(cpu_set_t), &cpuset_d);
            if(status != 0)
            {
                printf("Could not set thread core affinity ... exiting\n");
                exit(1);
            }

            //Create threads.  Create thread b and d (client) before thread a and c (server) which performs measurments
            // - Start Thread B & D
            status = pthread_create(&thread_b, &attr_b, kernel_client, arg_client_1);
            if(status != 0)
            {
                printf("Could not create b thread ... exiting\n");
                errno = status;
                perror(NULL);
                exit(1);
            }

            status = pthread_create(&thread_d, &attr_d, kernel_client, arg_client_2);
            if(status != 0)
            {
                printf("Could not create d thread ... exiting\n");
                errno = status;
                perror(NULL);
                exit(1);
            }

            // - Start Thread A & C
            KernelExeWrapperArgs* server_args_a = new KernelExeWrapperArgs();
            server_args_a->pcm = pcm;
            server_args_a->kernel_fun = kernel_server;
            server_args_a->kernel_arg = arg_server_1;
            server_args_a->cpu_num = cpu_a;

            status = pthread_create(&thread_a, &attr_a, kernel_exe_wrapper, server_args_a);
            if(status != 0)
            {
                printf("Could not create a thread ... exiting\n");
                errno = status;
                perror(NULL);
                exit(1);
            }

            KernelExeWrapperArgs* server_args_c = new KernelExeWrapperArgs();
            server_args_c->pcm = pcm;
            server_args_c->kernel_fun = kernel_server;
            server_args_c->kernel_arg = arg_server_2;
            server_args_c->cpu_num = cpu_c;

            status = pthread_create(&thread_c, &attr_c, kernel_exe_wrapper, server_args_c);
            if(status != 0)
            {
                printf("Could not create a thread ... exiting\n");
                errno = status;
                perror(NULL);
                exit(1);
            }

            //Wait for server threads (measuring threads) to finish
            status = pthread_join(thread_a, &res_a);
            if(status != 0)
            {
                printf("Could not join a thread ... exiting\n");
                errno = status;
                perror(NULL);
                exit(1);
            }

            status = pthread_join(thread_c, &res_c);
            if(status != 0)
            {
                printf("Could not join a thread ... exiting\n");
                errno = status;
                perror(NULL);
                exit(1);
            }

            //Wait for client threads to finish
            status = pthread_join(thread_b, &res_b);
            if(status != 0)
            {
                printf("Could not join thread ... exiting\n");
                exit(1);
            }

            status = pthread_join(thread_d, &res_d);
            if(status != 0)
            {
                printf("Could not join thread ... exiting\n");
                exit(1);
            }

            //Parse results
            if(res_a != NULL && res_c != NULL) //Check that both parallel threads did not expeirnece a frequency change.
            {
                //Decode the results from the server thread
                Results* result_obj_a = (Results*) res_a;
                Results* result_obj_c = (Results*) res_c;

                //Copy trial into results
                for(size_t i = 0; i<result_obj_a->trial_results.size(); i++)
                {
                    results_a->add_trial((result_obj_a->trial_results)[i]);
                }

                //Copy trial into results
                for(size_t i = 0; i<result_obj_c->trial_results.size(); i++)
                {
                    results_c->add_trial((result_obj_c->trial_results)[i]);
                }

                trial++;
            }
            else
            {
                discard_count++;
                #if PRINT_FREQ_CHANGE_EVENT == 1
                    printf("Frequency Change Event Occured Durring Kernel Run ... Discarding Run\n");
                #endif

                if(discard_count >= MAX_DISCARD-1)
                {
                    #if PRINT_FREQ_CHANGE_EVENT == 1
                        printf("Max Discards Reached ... Exiting\n");
                    #endif
                    exit(1);
                }
            }

            //Delete temportary results
            free(res_a);
            free(res_b);
            free(res_c);
            free(res_d);

            delete server_args_a;
            delete server_args_c;
        }

        simultanious_results->results_a = results_a;
        simultanious_results->results_b = results_c;

        return simultanious_results;
    }

#endif
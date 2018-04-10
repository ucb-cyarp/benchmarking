#ifndef _H_KERNEL_SERVER_RUNNER
    #define _H_KERNEL_SERVER_RUNNER

    #include "results.h"
    #include "cpucounters.h"
    #include "pcm_helper.h"

    #include "intrin_bench_default_defines.h"

    class KernelServerWrapperArgs
    {
        public:
            PCM* pcm;
            void* (*kernel_fun)(void*);
            void* kernel_arg;
            int cpu_num;
    };

    //Returns a pointer to a Results object
    //Returns NULL if freq change event occured
    void* kernel_server_wrapper(void *arg)
    {
        KernelServerWrapperArgs* args = (KernelServerWrapperArgs*) arg;

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

            status = pthread_attr_init(&attr_a);
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
            KernelServerWrapperArgs* server_args = new KernelServerWrapperArgs();
            server_args->pcm = pcm;
            server_args->kernel_fun = kernel_fun;
            server_args->kernel_arg = arg_a;
            server_args->cpu_num = cpu_a;

            status = pthread_create(&thread_a, &attr_a, kernel_server_wrapper, server_args);
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

            // //Wait for client thread to finish
            // status = pthread_join(thread_b, &res_b);
            // if(status != 0)
            // {
            //     printf("Could not join thread ... exiting\n");
            //     exit(1);
            // }

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
            // free(res_b);
        }

        return results;
    }

#endif
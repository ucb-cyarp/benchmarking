#ifndef _H_KERNEL_RUNNER
    #define _H_KERNEL_RUNNER

    #include "results.h"
    #include "cpucounters.h"
    #include "pcm_helper.h"

    Results* zero_arg_kernel(PCM* pcm, void (*kernel_fun)(), int cpu_num, const char* title)
    {
        #if PRINT_HEADER == 1
            printf("%s\n", title);
        #endif

        int sockets = pcm->getNumSockets();
        int cores = pcm->getNumCores();

        Results* results = new Results(sockets, cores);

        //Allocate measurement arrays
        std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];

        //Allocate counter states
        ServerUncorePowerState* startPowerState = new ServerUncorePowerState[sockets];
        ServerUncorePowerState* endPowerState = new ServerUncorePowerState[sockets];
        std::vector<CoreCounterState> startCstates, endCstates;
        std::vector<SocketCounterState> startSktstate, endSktstate;
        SystemCounterState startSstate, endSstate;

        int trial = 0;
        int discard_count = 0;
        while(trial<TRIALS)
        {
            //Get CPU Core/Socket/Power States
            for (int i = 0; i < sockets; i++)
                startPowerState[i] = pcm->getServerUncorePowerState(i);
            pcm->getAllCounterStates(startSstate, startSktstate, startCstates);

            //Start Timer
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            clock_t start_clock = clock();
            uint64_t start_rdtsc = _rdtsc();

            //Run Kernel
            kernel_fun();

            //Stop Timer 
            uint64_t stop_rdtsc = _rdtsc();
            clock_t stop_clock = clock();
            std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();

            //Get CPU Core/Socket/Power States
            pcm->getAllCounterStates(endSstate, endSktstate, endCstates);
            for (int i = 0; i < sockets; i++)
                endPowerState[i] = pcm->getServerUncorePowerState(i);
            
            TrialResult* trial_result = new TrialResult(sockets, cores, trial);

            //Report Time
            calculate_durations(trial_result->duration, trial_result->duration_clock, trial_result->duration_rdtsc, start, stop, start_clock, stop_clock, start_rdtsc, stop_rdtsc);
            
            //Report Freq, Power
            calc_freq_and_power(pcm, trial_result->avgCPUFreq, trial_result->avgActiveCPUFreq, trial_result->energyCPUUsed, trial_result->energyDRAMUsed,
            startCstates, endCstates, startPowerState, endPowerState);

            #if PRINT_TRIALS == 1
                trial_result->print_trial();
            #endif 

            //Limit check to socket of interest (single socket for now)
            std::vector<int> sockets_of_interest;
            sockets_of_interest.push_back(pcm->getSocketId(cpu_num));

            bool freq_change_events_occured = check_any_freq_changes(pcm, startPowerState, endPowerState, sockets_of_interest);
            //Proceed if no freq changes occured
            if(freq_change_events_occured == false)
            {
                trial++;
                results->add_trial(trial_result);
                discard_count=0;
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
            
        }

        #if PRINT_STATS == 1
            results->print_statistics(pcm->getSocketId(cpu_num), cpu_num);
        #endif

        delete startPowerState;
        delete endPowerState;
        
        return results;
    }

    template <typename VecType, typename KernelType>
    Results* load_store_one_arg_kernel(PCM* pcm, void (*kernel_fun)(VecType*), int cpu_num, const char* title)
    {
        #if PRINT_HEADER == 1
            printf("%s\n", title);
        #endif

        int sockets = pcm->getNumSockets();
        int cores = pcm->getNumCores();

        Results* results = new Results(sockets, cores);

        //Allocate measurement arrays
        std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];

        //Allocate counter states
        ServerUncorePowerState* startPowerState = new ServerUncorePowerState[sockets];
        ServerUncorePowerState* endPowerState = new ServerUncorePowerState[sockets];
        std::vector<CoreCounterState> startCstates, endCstates;
        std::vector<SocketCounterState> startSktstate, endSktstate;
        SystemCounterState startSstate, endSstate;

        int trial = 0;
        int discard_count = 0;
        while(trial<TRIALS)
        {
            //Allocate the arrays to operate over
            //These arrays need to be aligned to the vector register size of 256a
            void* a = _mm_malloc (STIM_LEN*sizeof(KernelType), sizeof(VecType));

            VecType* a_vec = (VecType * ) a;

            //Get CPU Core/Socket/Power States
            for (int i = 0; i < sockets; i++)
                startPowerState[i] = pcm->getServerUncorePowerState(i);
            pcm->getAllCounterStates(startSstate, startSktstate, startCstates);

            //Start Timer
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            clock_t start_clock = clock();
            uint64_t start_rdtsc = _rdtsc();

            //Run Kernel
            kernel_fun(a_vec);

            //Stop Timer 
            uint64_t stop_rdtsc = _rdtsc();
            clock_t stop_clock = clock();
            std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();

            //Get CPU Core/Socket/Power States
            pcm->getAllCounterStates(endSstate, endSktstate, endCstates);
            for (int i = 0; i < sockets; i++)
                endPowerState[i] = pcm->getServerUncorePowerState(i);
            
            TrialResult* trial_result = new TrialResult(sockets, cores, trial);

            //Report Time
            calculate_durations(trial_result->duration, trial_result->duration_clock, trial_result->duration_rdtsc, start, stop, start_clock, stop_clock, start_rdtsc, stop_rdtsc);
            
            //Report Freq, Power
            calc_freq_and_power(pcm, trial_result->avgCPUFreq, trial_result->avgActiveCPUFreq, trial_result->energyCPUUsed, trial_result->energyDRAMUsed,
            startCstates, endCstates, startPowerState, endPowerState);

            #if PRINT_TRIALS == 1
                trial_result->print_trial();
            #endif 

            //Clean up
            _mm_free(a);

            //Limit check to socket of interest (single socket for now)
            std::vector<int> sockets_of_interest;
            sockets_of_interest.push_back(pcm->getSocketId(cpu_num));

            bool freq_change_events_occured = check_any_freq_changes(pcm, startPowerState, endPowerState, sockets_of_interest);
            //Proceed if no freq changes occured
            if(freq_change_events_occured == false)
            {
                trial++;
                results->add_trial(trial_result);
                discard_count=0;
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
            
        }

        #if PRINT_STATS == 1
            results->print_statistics(pcm->getSocketId(cpu_num), cpu_num);
        #endif

        delete startPowerState;
        delete endPowerState;
        
        return results;
    }

    template <typename VecType, typename KernelType>
    Results* load_store_two_arg_kernel(PCM* pcm, void (*kernel_fun)(VecType*, VecType*), int cpu_num, const char* title)
    {
        #if PRINT_HEADER == 1
            printf("%s\n", title);
        #endif

        int sockets = pcm->getNumSockets();
        int cores = pcm->getNumCores();

        Results* results = new Results(sockets, cores);

        //Allocate measurement arrays
        std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];

        //Allocate counter states
        ServerUncorePowerState* startPowerState = new ServerUncorePowerState[sockets];
        ServerUncorePowerState* endPowerState = new ServerUncorePowerState[sockets];
        std::vector<CoreCounterState> startCstates, endCstates;
        std::vector<SocketCounterState> startSktstate, endSktstate;
        SystemCounterState startSstate, endSstate;

        int trial = 0;
        int discard_count = 0;
        while(trial<TRIALS)
        {
            //Allocate the arrays to operate over
            //These arrays need to be aligned to the vector register size of 256a
            void* a = _mm_malloc (STIM_LEN*sizeof(KernelType), sizeof(VecType));
            void* b = _mm_malloc (STIM_LEN*sizeof(KernelType), sizeof(VecType));

            VecType* a_vec = (VecType * ) a;
            VecType* b_vec = (VecType * ) b;

            //Get CPU Core/Socket/Power States
            for (int i = 0; i < sockets; i++)
                startPowerState[i] = pcm->getServerUncorePowerState(i);
            pcm->getAllCounterStates(startSstate, startSktstate, startCstates);

            //Start Timer
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            clock_t start_clock = clock();
            uint64_t start_rdtsc = _rdtsc();

            //Run Kernel
            kernel_fun(a_vec, b_vec);

            //Stop Timer 
            uint64_t stop_rdtsc = _rdtsc();
            clock_t stop_clock = clock();
            std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();

            //Get CPU Core/Socket/Power States
            pcm->getAllCounterStates(endSstate, endSktstate, endCstates);
            for (int i = 0; i < sockets; i++)
                endPowerState[i] = pcm->getServerUncorePowerState(i);
            
            TrialResult* trial_result = new TrialResult(sockets, cores, trial);

            //Report Time
            calculate_durations(trial_result->duration, trial_result->duration_clock, trial_result->duration_rdtsc, start, stop, start_clock, stop_clock, start_rdtsc, stop_rdtsc);
            
            //Report Freq, Power
            calc_freq_and_power(pcm, trial_result->avgCPUFreq, trial_result->avgActiveCPUFreq, trial_result->energyCPUUsed, trial_result->energyDRAMUsed,
            startCstates, endCstates, startPowerState, endPowerState);

            #if PRINT_TRIALS == 1
                trial_result->print_trial();
            #endif 

            //Clean up
            _mm_free(a);
            _mm_free(b);

            //Limit check to socket of interest (single socket for now)
            std::vector<int> sockets_of_interest;
            sockets_of_interest.push_back(pcm->getSocketId(cpu_num));

            bool freq_change_events_occured = check_any_freq_changes(pcm, startPowerState, endPowerState, sockets_of_interest);
            //Proceed if no freq changes occured
            if(freq_change_events_occured == false)
            {
                trial++;
                results->add_trial(trial_result);
                discard_count=0;
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
            
        }

        #if PRINT_STATS == 1
            results->print_statistics(pcm->getSocketId(cpu_num), cpu_num);
        #endif

        delete startPowerState;
        delete endPowerState;
        
        return results;
    }

    template <typename VecType, typename KernelType>
    Results* load_store_three_arg_kernel(PCM* pcm, void (*kernel_fun)(VecType*, VecType*, VecType*), int cpu_num, const char* title)
    {
        #if PRINT_HEADER == 1
            printf("%s\n", title);
        #endif

        int sockets = pcm->getNumSockets();
        int cores = pcm->getNumCores();

        Results* results = new Results(sockets, cores);

        //Allocate measurement arrays
        std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];

        //Allocate counter states
        ServerUncorePowerState* startPowerState = new ServerUncorePowerState[sockets];
        ServerUncorePowerState* endPowerState = new ServerUncorePowerState[sockets];
        std::vector<CoreCounterState> startCstates, endCstates;
        std::vector<SocketCounterState> startSktstate, endSktstate;
        SystemCounterState startSstate, endSstate;

        int trial = 0;
        int discard_count = 0;
        while(trial<TRIALS)
        {
            //Allocate the arrays to operate over
            //These arrays need to be aligned to the vector register size of 256a
            void* a = _mm_malloc (STIM_LEN*sizeof(KernelType), sizeof(VecType));
            void* b = _mm_malloc (STIM_LEN*sizeof(KernelType), sizeof(VecType));
            void* c = _mm_malloc (STIM_LEN*sizeof(KernelType), sizeof(VecType));

            VecType* a_vec = (VecType * ) a;
            VecType* b_vec = (VecType * ) b;
            VecType* c_vec = (VecType * ) c;

            //Get CPU Core/Socket/Power States
            for (int i = 0; i < sockets; i++)
                startPowerState[i] = pcm->getServerUncorePowerState(i);
            pcm->getAllCounterStates(startSstate, startSktstate, startCstates);

            //Start Timer
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            clock_t start_clock = clock();
            uint64_t start_rdtsc = _rdtsc();

            //Run Kernel
            kernel_fun(a_vec, b_vec, c_vec);

            //Stop Timer 
            uint64_t stop_rdtsc = _rdtsc();
            clock_t stop_clock = clock();
            std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();

            //Get CPU Core/Socket/Power States
            pcm->getAllCounterStates(endSstate, endSktstate, endCstates);
            for (int i = 0; i < sockets; i++)
                endPowerState[i] = pcm->getServerUncorePowerState(i);
            
            TrialResult* trial_result = new TrialResult(sockets, cores, trial);

            //Report Time
            calculate_durations(trial_result->duration, trial_result->duration_clock, trial_result->duration_rdtsc, start, stop, start_clock, stop_clock, start_rdtsc, stop_rdtsc);
            
            //Report Freq, Power
            calc_freq_and_power(pcm, trial_result->avgCPUFreq, trial_result->avgActiveCPUFreq, trial_result->energyCPUUsed, trial_result->energyDRAMUsed,
            startCstates, endCstates, startPowerState, endPowerState);

            #if PRINT_TRIALS == 1
                trial_result->print_trial();
            #endif 

            //Clean up
            _mm_free(a);
            _mm_free(b);
            _mm_free(c);

            //Limit check to socket of interest (single socket for now)
            std::vector<int> sockets_of_interest;
            sockets_of_interest.push_back(pcm->getSocketId(cpu_num));

            bool freq_change_events_occured = check_any_freq_changes(pcm, startPowerState, endPowerState, sockets_of_interest);
            //Proceed if no freq changes occured
            if(freq_change_events_occured == false)
            {
                trial++;
                results->add_trial(trial_result);
                discard_count=0;
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
            
        }

        #if PRINT_STATS == 1
            results->print_statistics(pcm->getSocketId(cpu_num), cpu_num);
        #endif

        delete startPowerState;
        delete endPowerState;
        
        return results;
    }

    template <typename VecType, typename KernelType>
    Results* load_store_four_arg_kernel(PCM* pcm, void (*kernel_fun)(VecType*, VecType*, VecType*, VecType*), int cpu_num, const char* title)
    {
        #if PRINT_HEADER == 1
            printf("%s\n", title);
        #endif

        int sockets = pcm->getNumSockets();
        int cores = pcm->getNumCores();

        Results* results = new Results(sockets, cores);

        //Allocate measurement arrays
        std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];

        //Allocate counter states
        ServerUncorePowerState* startPowerState = new ServerUncorePowerState[sockets];
        ServerUncorePowerState* endPowerState = new ServerUncorePowerState[sockets];
        std::vector<CoreCounterState> startCstates, endCstates;
        std::vector<SocketCounterState> startSktstate, endSktstate;
        SystemCounterState startSstate, endSstate;

        int trial = 0;
        int discard_count = 0;
        while(trial<TRIALS)
        {
            //Allocate the arrays to operate over
            //These arrays need to be aligned to the vector register size of 256a
            void* a = _mm_malloc (STIM_LEN*sizeof(KernelType), sizeof(VecType));
            void* b = _mm_malloc (STIM_LEN*sizeof(KernelType), sizeof(VecType));
            void* c = _mm_malloc (STIM_LEN*sizeof(KernelType), sizeof(VecType));
            void* d = _mm_malloc (STIM_LEN*sizeof(KernelType), sizeof(VecType));

            VecType* a_vec = (VecType * ) a;
            VecType* b_vec = (VecType * ) b;
            VecType* c_vec = (VecType * ) c;
            VecType* d_vec = (VecType * ) d;

            //Get CPU Core/Socket/Power States
            for (int i = 0; i < sockets; i++)
                startPowerState[i] = pcm->getServerUncorePowerState(i);
            pcm->getAllCounterStates(startSstate, startSktstate, startCstates);

            //Start Timer
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            clock_t start_clock = clock();
            uint64_t start_rdtsc = _rdtsc();

            //Run Kernel
            kernel_fun(a_vec, b_vec, c_vec, d_vec);

            //Stop Timer 
            uint64_t stop_rdtsc = _rdtsc();
            clock_t stop_clock = clock();
            std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();

            //Get CPU Core/Socket/Power States
            pcm->getAllCounterStates(endSstate, endSktstate, endCstates);
            for (int i = 0; i < sockets; i++)
                endPowerState[i] = pcm->getServerUncorePowerState(i);
            
            TrialResult* trial_result = new TrialResult(sockets, cores, trial);

            //Report Time
            calculate_durations(trial_result->duration, trial_result->duration_clock, trial_result->duration_rdtsc, start, stop, start_clock, stop_clock, start_rdtsc, stop_rdtsc);
            
            //Report Freq, Power
            calc_freq_and_power(pcm, trial_result->avgCPUFreq, trial_result->avgActiveCPUFreq, trial_result->energyCPUUsed, trial_result->energyDRAMUsed,
            startCstates, endCstates, startPowerState, endPowerState);

            #if PRINT_TRIALS == 1
                trial_result->print_trial();
            #endif 

            //Clean up
            _mm_free(a);
            _mm_free(b);
            _mm_free(c);
            _mm_free(d);

            //Limit check to socket of interest (single socket for now)
            std::vector<int> sockets_of_interest;
            sockets_of_interest.push_back(pcm->getSocketId(cpu_num));

            bool freq_change_events_occured = check_any_freq_changes(pcm, startPowerState, endPowerState, sockets_of_interest);
            //Proceed if no freq changes occured
            if(freq_change_events_occured == false)
            {
                trial++;
                results->add_trial(trial_result);
                discard_count=0;
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
            
        }

        #if PRINT_STATS == 1
            results->print_statistics(pcm->getSocketId(cpu_num), cpu_num);
        #endif

        delete startPowerState;
        delete endPowerState;
        
        return results;
    }

    template <typename KernelType>
    Results* no_vec_three_arg_kernel(PCM* pcm, void (*kernel_fun)(KernelType*, KernelType*, KernelType*), int cpu_num, const char* title)
    {
        #if PRINT_HEADER == 1
            printf("%s\n", title);
        #endif

        int sockets = pcm->getNumSockets();
        int cores = pcm->getNumCores();

        Results* results = new Results(sockets, cores);

        //Allocate measurement arrays
        std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];

        //Allocate counter states
        ServerUncorePowerState* startPowerState = new ServerUncorePowerState[sockets];
        ServerUncorePowerState* endPowerState = new ServerUncorePowerState[sockets];
        std::vector<CoreCounterState> startCstates, endCstates;
        std::vector<SocketCounterState> startSktstate, endSktstate;
        SystemCounterState startSstate, endSstate;

        int trial = 0;
        int discard_count = 0;
        while(trial<TRIALS)
        {
            //Allocate the arrays to operate over
            //These arrays need to be aligned to the vector register size of 256a
            void* a = malloc (STIM_LEN*sizeof(KernelType));
            void* b = malloc (STIM_LEN*sizeof(KernelType));
            void* c = malloc (STIM_LEN*sizeof(KernelType));

            KernelType* a_vec = (KernelType * ) a;
            KernelType* b_vec = (KernelType * ) b;
            KernelType* c_vec = (KernelType * ) c;

            //Get CPU Core/Socket/Power States
            for (int i = 0; i < sockets; i++)
                startPowerState[i] = pcm->getServerUncorePowerState(i);
            pcm->getAllCounterStates(startSstate, startSktstate, startCstates);

            //Start Timer
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            clock_t start_clock = clock();
            uint64_t start_rdtsc = _rdtsc();

            //Run Kernel
            kernel_fun(a_vec, b_vec, c_vec);

            //Stop Timer 
            uint64_t stop_rdtsc = _rdtsc();
            clock_t stop_clock = clock();
            std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();

            //Get CPU Core/Socket/Power States
            pcm->getAllCounterStates(endSstate, endSktstate, endCstates);
            for (int i = 0; i < sockets; i++)
                endPowerState[i] = pcm->getServerUncorePowerState(i);
            
            TrialResult* trial_result = new TrialResult(sockets, cores, trial);

            //Report Time
            calculate_durations(trial_result->duration, trial_result->duration_clock, trial_result->duration_rdtsc, start, stop, start_clock, stop_clock, start_rdtsc, stop_rdtsc);
            
            //Report Freq, Power
            calc_freq_and_power(pcm, trial_result->avgCPUFreq, trial_result->avgActiveCPUFreq, trial_result->energyCPUUsed, trial_result->energyDRAMUsed,
            startCstates, endCstates, startPowerState, endPowerState);

            #if PRINT_TRIALS == 1
                trial_result->print_trial();
            #endif 

            //Clean up
            free(a);
            free(b);
            free(c);

            //Limit check to socket of interest (single socket for now)
            std::vector<int> sockets_of_interest;
            sockets_of_interest.push_back(pcm->getSocketId(cpu_num));

            bool freq_change_events_occured = check_any_freq_changes(pcm, startPowerState, endPowerState, sockets_of_interest);
            //Proceed if no freq changes occured
            if(freq_change_events_occured == false)
            {
                trial++;
                results->add_trial(trial_result);
                discard_count=0;
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
            
        }

        #if PRINT_STATS == 1
            results->print_statistics(pcm->getSocketId(cpu_num), cpu_num);
        #endif

        delete startPowerState;
        delete endPowerState;
        
        return results;
    }

#endif
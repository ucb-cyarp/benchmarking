#ifndef _H_KERNEL_RUNNER
    #define _H_KERNEL_RUNNER

    #include "statistics.h"
    #include "depends/pcm/cpucounters.h"

    /**
     * Check for frequency changed events.  Assumes PMU counter 1 was set to track frequency change events.
     */
    bool check_any_freq_changes(PCM* pcm, ServerUncorePowerState* startStates, ServerUncorePowerState* endStates, std::vector<int> sockets_of_interest, const char* print_prefix = "Warning: ")
    {
        bool freq_changed = false;

        for(int i = 0; i < sockets_of_interest.size(); i++)
        {
            int freq_change_events = getPCUCounter(1, startStates[sockets_of_interest[i]], endStates[sockets_of_interest[i]]);
            if(freq_change_events > 0)
            {
                freq_changed = true;
                #ifdef PRINT_FREQ_CHANGE_EVENT
                    printf("%sSocket %d experienced %d clock frequency change events!\n", print_prefix, i, freq_change_events);
                #else
                    break; //No need to check other sockets, frequency changed
                #endif
            }
        }

        return freq_changed;
    }

    /**
     * Calculate durations (for different clocks) for a given trial
     * 
     */
    void calculate_durations(std::chrono::duration<double, std::ratio<1, 1000>>& durations,
    double& durations_clock,
    double& durations_rdtsc,
    std::chrono::high_resolution_clock::time_point start,
    std::chrono::high_resolution_clock::time_point stop,
    clock_t start_clock,
    clock_t stop_clock,
    uint64_t start_rdtsc,
    uint64_t stop_rdtsc)
    {
        durations = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(stop-start);
        durations_clock = 1000.0 * (stop_clock - start_clock) / CLOCKS_PER_SEC;
        durations_rdtsc =  (stop_rdtsc - start_rdtsc);
    }

    void calc_freq_and_power(PCM* pcm, int trial, double avgCPUFreq[][TRIALS], double avgActiveCPUFreq[][TRIALS], double energyCPUUsed[][TRIALS], double energyDRAMUsed[][TRIALS],
    std::vector<CoreCounterState>& startCstates, std::vector<CoreCounterState>& endCstates, ServerUncorePowerState* startPowerState, ServerUncorePowerState* endPowerState)
    {
        int cores = pcm->getNumCores();
        int sockets = pcm->getNumSockets();

        for(int i = 0; i<cores; i++)
        {  
            avgCPUFreq[i][trial] = getAverageFrequency(startCstates[i], endCstates[i]);
            avgActiveCPUFreq[i][trial] = getActiveAverageFrequency(startCstates[i], endCstates[i]);
        }
        for(int i = 0; i<sockets; i++)
        {
            energyCPUUsed[i][trial] = getConsumedJoules(startPowerState[i], endPowerState[i]);
            energyDRAMUsed[i][trial] = getDRAMConsumedJoules(startPowerState[i], endPowerState[i]);
        }
    }

    void print_trial(PCM* pcm, int trial, std::chrono::duration<double, std::ratio<1, 1000>> durations[], double durations_clock[], double durations_rdtsc[], 
    double avgCPUFreq[][TRIALS], double avgActiveCPUFreq[][TRIALS], double energyCPUUsed[][TRIALS], double energyDRAMUsed[][TRIALS])
    {
        int sockets = pcm->getNumSockets();
        int cores = pcm->getNumCores();

        printf("Trial %6d: Duration: %f, Duration (Clk): %f, Duration (rdtsc): %f", trial, durations[trial].count(), durations_clock[trial], durations_rdtsc[trial]);
        for(int i = 0; i<sockets; i++)
        {
                printf("\nEnergyCPUUsed[%d]: %8.4f, EnergyDRAMUsed[%d]: %8.4f ", i, energyCPUUsed[i][trial], i, energyDRAMUsed[i][trial]);
        }

        for(int i = 0; i<cores; i++)
        {
                printf("\nAvgCPUFreq[%d]: %15.4f, AvgActiveCPUFreq[%d]: %15.4f", i, avgCPUFreq[i][trial], i, avgActiveCPUFreq[i][trial]);
        }
        printf("\n");
    }

    void zero_arg_kernel(PCM* pcm, void (*kernel_fun)(), int cpu_num, const char* title)
    {
        #if PRINT_HEADER == 1
            printf("%s\n", title);
        #endif

        //Allocate timer arrays
        std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];
        double durations_clock[TRIALS];
        double durations_rdtsc[TRIALS];

        for(int trial = 0; trial<TRIALS; trial++)
        {
            //Start Timer
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            clock_t start_clock = clock();
            uint64_t start_rdtsc = _rdtsc();

            kernel_fun();

            //Stop Timer and Report Time
            uint64_t stop_rdtsc = _rdtsc();
            clock_t stop_clock = clock();
            std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();

            durations[trial] = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(stop-start);
            durations_clock[trial] = 1000.0 * (stop_clock - start_clock) / CLOCKS_PER_SEC;
            durations_rdtsc[trial] =  (stop_rdtsc - start_rdtsc);

            #if PRINT_TRIALS == 1
                printf("Trial %6d: %f, %f, %f\n", trial, durations[trial].count(), durations_clock[trial], durations_rdtsc[trial]);
            #endif 
        }

        #if PRINT_STATS == 1
        statistics(durations, durations_clock);
        #endif
    }

    template <typename VecType, typename KernelType>
    void load_store_one_arg_kernel(PCM* pcm, void (*kernel_fun)(VecType*), int cpu_num, const char* title)
    {
        #if PRINT_HEADER == 1
            printf("%s\n", title);
        #endif

        //Allocate timer arrays
        std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];
        double durations_clock[TRIALS];
        double durations_rdtsc[TRIALS];

        for(int trial = 0; trial<TRIALS; trial++)
        {
            //Allocate the arrays to operate over
            //These arrays need to be aligned to the vector register size of 256a
            void* a = _mm_malloc (STIM_LEN*sizeof(KernelType), sizeof(VecType));

            VecType* a_vec = (VecType * ) a;

            //Start Timer
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            clock_t start_clock = clock();
            uint64_t start_rdtsc = _rdtsc();

            kernel_fun(a_vec);

            //Stop Timer and Report Time
            uint64_t stop_rdtsc = _rdtsc();
            clock_t stop_clock = clock();
            std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();

            durations[trial] = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(stop-start);
            durations_clock[trial] = 1000.0 * (stop_clock - start_clock) / CLOCKS_PER_SEC;
            durations_rdtsc[trial] =  (stop_rdtsc - start_rdtsc);

            #if PRINT_TRIALS == 1
                printf("Trial %6d: %f, %f, %f\n", trial, durations[trial].count(), durations_clock[trial], durations_rdtsc[trial]);
            #endif 

            //Clean up
            _mm_free(a);
        }

        #if PRINT_STATS == 1
        statistics(durations, durations_clock);
        #endif
    }

    template <typename VecType, typename KernelType>
    void load_store_two_arg_kernel(PCM* pcm, void (*kernel_fun)(VecType*, VecType*), int cpu_num, const char* title)
    {
        #if PRINT_HEADER == 1
            printf("%s\n", title);
        #endif

        int sockets = pcm->getNumSockets();
        int cores = pcm->getNumCores();

        //Allocate measurement arrays
        std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];
        double durations_clock[TRIALS];
        double durations_rdtsc[TRIALS];
        double avgCPUFreq[cores][TRIALS];
        double avgActiveCPUFreq[cores][TRIALS];
        double energyCPUUsed[sockets][TRIALS];
        double energyDRAMUsed[sockets][TRIALS];

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
            
            //Report Time
            calculate_durations(durations[trial], durations_clock[trial], durations_rdtsc[trial], start, stop, start_clock, stop_clock, start_rdtsc, stop_rdtsc);
            
            //Report Freq, Power
            calc_freq_and_power(pcm, trial, avgCPUFreq, avgActiveCPUFreq, energyCPUUsed, energyDRAMUsed,
            startCstates, endCstates, startPowerState, endPowerState);

            #if PRINT_TRIALS == 1
                print_trial(pcm, trial, durations, durations_clock, durations_rdtsc, avgCPUFreq, avgActiveCPUFreq, energyCPUUsed, energyDRAMUsed);
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
                discard_count=0;
            }
            
            else
            {
                discard_count++;
                #ifdef PRINT_FREQ_CHANGE_EVENT
                    printf("Frequency Change Event Occured Durring Kernel Run ... Discarding Run\n");
                #endif

                if(discard_count >= MAX_DISCARD-1)
                {
                    #ifdef PRINT_FREQ_CHANGE_EVENT
                        printf("Max Discards Reached ... Exiting\n");
                    #endif
                    exit(1);
                }
            }
            
        }

        #if PRINT_STATS == 1
        statistics(durations, durations_clock);
        #endif
    }

    template <typename VecType, typename KernelType>
    void load_store_three_arg_kernel(PCM* pcm, void (*kernel_fun)(VecType*, VecType*, VecType*), int cpu_num, const char* title)
    {
        #if PRINT_HEADER == 1
            printf("%s\n", title);
        #endif

        //Allocate timer arrays
        std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];
        double durations_clock[TRIALS];
        double durations_rdtsc[TRIALS];

        for(int trial = 0; trial<TRIALS; trial++)
        {
            //Allocate the arrays to operate over
            //These arrays need to be aligned to the vector register size of 256a
            void* a = _mm_malloc (STIM_LEN*sizeof(KernelType), sizeof(VecType));
            void* b = _mm_malloc (STIM_LEN*sizeof(KernelType), sizeof(VecType));
            void* c = _mm_malloc (STIM_LEN*sizeof(KernelType), sizeof(VecType));

            VecType* a_vec = (VecType * ) a;
            VecType* b_vec = (VecType * ) b;
            VecType* c_vec = (VecType * ) c;

            //Start Timer
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            clock_t start_clock = clock();
            uint64_t start_rdtsc = _rdtsc();

            kernel_fun(a_vec, b_vec, c_vec);

            //Stop Timer and Report Time
            uint64_t stop_rdtsc = _rdtsc();
            clock_t stop_clock = clock();
            std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();

            durations[trial] = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(stop-start);
            durations_clock[trial] = 1000.0 * (stop_clock - start_clock) / CLOCKS_PER_SEC;
            durations_rdtsc[trial] =  (stop_rdtsc - start_rdtsc);

            #if PRINT_TRIALS == 1
                printf("Trial %6d: %f, %f, %f\n", trial, durations[trial].count(), durations_clock[trial], durations_rdtsc[trial]);
            #endif 

            //Clean up
            _mm_free(a);
            _mm_free(b);
            _mm_free(c);
        }

        #if PRINT_STATS == 1
        statistics(durations, durations_clock);
        #endif
    }

    template <typename VecType, typename KernelType>
    void load_store_four_arg_kernel(PCM* pcm, void (*kernel_fun)(VecType*, VecType*, VecType*, VecType*), int cpu_num, const char* title)
    {
        #if PRINT_HEADER == 1
            printf("%s\n", title);
        #endif

        //Allocate timer arrays
        std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];
        double durations_clock[TRIALS];
        double durations_rdtsc[TRIALS];

        for(int trial = 0; trial<TRIALS; trial++)
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

            //Start Timer
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            clock_t start_clock = clock();
            uint64_t start_rdtsc = _rdtsc();

            kernel_fun(a_vec, b_vec, c_vec, d_vec);

            //Stop Timer and Report Time
            uint64_t stop_rdtsc = _rdtsc();
            clock_t stop_clock = clock();
            std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();

            durations[trial] = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(stop-start);
            durations_clock[trial] = 1000.0 * (stop_clock - start_clock) / CLOCKS_PER_SEC;
            durations_rdtsc[trial] =  (stop_rdtsc - start_rdtsc);

            #if PRINT_TRIALS == 1
                printf("Trial %6d: %f, %f, %f\n", trial, durations[trial].count(), durations_clock[trial], durations_rdtsc[trial]);
            #endif 

            //Clean up
            _mm_free(a);
            _mm_free(b);
            _mm_free(c);
            _mm_free(d);
        }

        #if PRINT_STATS == 1
        statistics(durations, durations_clock);
        #endif
    }

    template <typename KernelType>
    void no_vec_three_arg_kernel(PCM* pcm, void (*kernel_fun)(KernelType*, KernelType*, KernelType*), int cpu_num, const char* title)
    {
        #if PRINT_HEADER == 1
            printf("%s\n", title);
        #endif

            //Allocate timer arrays
            std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];
            double durations_clock[TRIALS];
            double durations_rdtsc[TRIALS];

            for(int trial = 0; trial<TRIALS; trial++)
            {
                //Allocate the arrays to operate over
                //These arrays need to be aligned to the vector register size of 256a
                void* a = malloc (STIM_LEN*sizeof(KernelType));
                void* b = malloc (STIM_LEN*sizeof(KernelType));
                void* c = malloc (STIM_LEN*sizeof(KernelType));

                KernelType* a_arr = (KernelType * ) a;
                KernelType* b_arr = (KernelType * ) b;
                KernelType* c_arr = (KernelType * ) c;

                //Start Timer
                std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
                clock_t start_clock = clock();
                uint64_t start_rdtsc = _rdtsc();

                kernel_fun(a_arr, b_arr, c_arr);

                //Stop Timer and Report Time
                uint64_t stop_rdtsc = _rdtsc();
                clock_t stop_clock = clock();
                std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();

                durations[trial] = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(stop-start);
                durations_clock[trial] = 1000.0 * (stop_clock - start_clock) / CLOCKS_PER_SEC;
                durations_rdtsc[trial] =  (stop_rdtsc - start_rdtsc);

                #if PRINT_TRIALS == 1
                    printf("Trial %6d: %f, %f, %f\n", trial, durations[trial].count(), durations_clock[trial], durations_rdtsc[trial]);
                #endif 

                //Clean up
                free(a);
                free(b);
                free(c);
            }

            #if PRINT_STATS == 1
            statistics(durations, durations_clock);
            #endif
        }

#endif
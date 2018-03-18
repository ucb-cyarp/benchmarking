#ifndef _H_KERNEL_RUNNER
    #define _H_KERNEL_RUNNER

    #include "statistics.h"
    #include "depends/pcm/cpucounters.h"

    void zero_arg_kernel(PCM* pcm, void (*kernel_fun)(), const char* title)
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
                printf("Trial %6d: %f, %f, %f\n", trial, durations[trial], durations_clock[trial], durations_rdtsc[trial]);
            #endif 
        }

        #if PRINT_STATS == 1
        statistics(durations, durations_clock);
        #endif
    }

    template <typename VecType, typename KernelType>
    void load_store_one_arg_kernel(PCM* pcm, void (*kernel_fun)(VecType*), const char* title)
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
                printf("Trial %6d: %f, %f, %f\n", trial, durations[trial], durations_clock[trial], durations_rdtsc[trial]);
            #endif 

            //Clean up
            _mm_free(a);
        }

        #if PRINT_STATS == 1
        statistics(durations, durations_clock);
        #endif
    }

    template <typename VecType, typename KernelType>
    void load_store_two_arg_kernel(PCM* pcm, void (*kernel_fun)(VecType*, VecType*), const char* title)
    {
        #if PRINT_HEADER == 1
            printf("%s\n", title);
        #endif

        int sockets = m->getNumSockets();

        //Allocate timer arrays
        std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];
        double durations_clock[TRIALS];
        double durations_rdtsc[TRIALS];
        double avgCPUFreq[sockets][TRIALS];
        double avgActiveCPUFreq[sockets][TRIALS];
        double energyCPUUsed[sockets][TRIALS];
        double energyDRAMUsed[sockets][TRIALS];

        ServerUncorePowerState* startPowerState = new ServerUncorePowerState[sockets];

        int trial = 0
        while(trial<TRIALS)
        {
            //Allocate the arrays to operate over
            //These arrays need to be aligned to the vector register size of 256a
            void* a = _mm_malloc (STIM_LEN*sizeof(KernelType), sizeof(VecType));
            void* b = _mm_malloc (STIM_LEN*sizeof(KernelType), sizeof(VecType));

            VecType* a_vec = (VecType * ) a;
            VecType* b_vec = (VecType * ) b;

            //Start Timer
            std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
            clock_t start_clock = clock();
            uint64_t start_rdtsc = _rdtsc();

            //Get Power States
            for (i = 0; i < sockets; i++)
                startPowerState[i] = m->getServerUncorePowerState(i);

            //Run Kernel
            kernel_fun(a_vec, b_vec);

            //Get Power States

            //Stop Timer and Report Time
            uint64_t stop_rdtsc = _rdtsc();
            clock_t stop_clock = clock();
            std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();

            durations[trial] = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(stop-start);
            durations_clock[trial] = 1000.0 * (stop_clock - start_clock) / CLOCKS_PER_SEC;
            durations_rdtsc[trial] =  (stop_rdtsc - start_rdtsc);

            #if PRINT_TRIALS == 1
                printf("Trial %6d: %f, %f, %f\n", trial, durations[trial], durations_clock[trial], durations_rdtsc[trial]);
            #endif 

            //Clean up
            _mm_free(a);
            _mm_free(b);

            //TODO: If no frequency change event occured
            trial++
            //Else, throw out result and try again
        }

        #if PRINT_STATS == 1
        statistics(durations, durations_clock);
        #endif
    }

    template <typename VecType, typename KernelType>
    void load_store_three_arg_kernel(PCM* pcm, void (*kernel_fun)(VecType*, VecType*, VecType*), const char* title)
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
                printf("Trial %6d: %f, %f, %f\n", trial, durations[trial], durations_clock[trial], durations_rdtsc[trial]);
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
    void load_store_four_arg_kernel(PCM* pcm, void (*kernel_fun)(VecType*, VecType*, VecType*, VecType*), const char* title)
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
                printf("Trial %6d: %f, %f, %f\n", trial, durations[trial], durations_clock[trial], durations_rdtsc[trial]);
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
    void no_vec_three_arg_kernel(PCM* pcm, void (*kernel_fun)(KernelType*, KernelType*, KernelType*), const char* title)
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
                    printf("Trial %6d: %f, %f, %f\n", trial, durations[trial], durations_clock[trial], durations_rdtsc[trial]);
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
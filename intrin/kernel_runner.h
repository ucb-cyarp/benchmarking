#ifndef _H_KERNEL_RUNNER
    #define _H_KERNEL_RUNNER

    #include "results.h"

    //Define some helper functions to be inlined

    //Check for frequency change events.  Discards sample if freq change event occured.  If not, adds result to results.  Prepares for next run in either case
    inline bool processTrialAndPrepareForNextHelper(Profiler* profiler, Results &results, TrialResult &trial_result, int &trial, int &discard_count){
            bool freq_change_events_occured = profiler->detectsFreqChange() ? profiler->checkFreqChanged() : false;
            //Proceed if no freq changes occured
            if(!freq_change_events_occured)
            {
                trial++;
                results.add_trial_set_trialInd(trial_result);
                discard_count=0;
            }
            else
            {
                discard_count++;
                #if PRINT_FREQ_CHANGE_EVENT == 1
                    printf("Discarding Run\n");
                #endif

                if(discard_count >= MAX_DISCARD-1)
                {
                    #if PRINT_FREQ_CHANGE_EVENT == 1
                        printf("Max Discards Reached ... Exiting\n");
                    #endif
                    exit(1);
                }
            }

            profiler->interTrialReset();

            return freq_change_events_occured;
    }

    inline void printTrial(TrialResult &trial_result){
        #if PRINT_TRIALS_DONE == 1
        printf("---- Finished Trial %d ----\n", trial_result.trial);
        #endif
        #if PRINT_TRIALS == 1
            trial_result.print_trial();
        #endif 
    }

    inline void printStats(Profiler* profiler, Results* results, int cpu_num){
        #if PRINT_STATS == 1
        int socket = profiler->cpuTopology[cpu_num].socket;
        int die = profiler->cpuTopology[cpu_num].die;
        int core = profiler->cpuTopology[cpu_num].core;

        if(profiler->cpuTopology.empty()){
            results->print_statistics(STIM_LEN);
        }else{
            results->print_statistics(socket, die, core, cpu_num, STIM_LEN);
        }
        #endif
    }

    inline void printHeader(const char* title){
        #if PRINT_HEADER == 1
            printf("%s\n", title);
        #endif
    }

    inline TrialResult computeTrialResultAndSetTrialNum(Profiler* profiler, Results* results){
        TrialResult trial_result = profiler->computeTrialResult();
        trial_result.trial = results->trial_results.size();
        return trial_result;
    }
    

    inline Results* zero_arg_kernel(Profiler* profiler, void (*kernel_fun)(), int cpu_num, const char* title)
    {
        printHeader(title);

        Results* results = new Results();

        int trial = 0;
        int discard_count = 0;

        while(trial<TRIALS)
        {
            profiler->trialSetup();
            profiler->startTrial();

            //Run Kernel
            kernel_fun();

            profiler->endTrial();
            
            TrialResult trial_result = computeTrialResultAndSetTrialNum(profiler, results);
            printTrial(trial_result);
            processTrialAndPrepareForNextHelper(profiler, *results, trial_result, trial, discard_count);
        }

        printStats(profiler, results, cpu_num);
        
        return results;
    }

    template <typename VecType, typename KernelType>
    Results* load_store_one_arg_kernel(Profiler* profiler, void (*kernel_fun)(VecType*), int cpu_num, const char* title)
    {
        printHeader(title);

        Results* results = new Results();

        int trial = 0;
        int discard_count = 0;

        while(trial<TRIALS)
        {
            //Allocate the arrays to operate over
            //These arrays need to be aligned to the vector register size of 256a
            void* a = _mm_malloc (STIM_LEN*sizeof(KernelType), sizeof(VecType));

            VecType* a_vec = (VecType * ) a;

            profiler->trialSetup();
            profiler->startTrial();

            //Run Kernel
            kernel_fun(a_vec);

            profiler->endTrial();
            
            TrialResult trial_result = computeTrialResultAndSetTrialNum(profiler, results);
            printTrial(trial_result);
            processTrialAndPrepareForNextHelper(profiler, *results, trial_result, trial, discard_count);

            _mm_free(a);
        }

        printStats(profiler, results, cpu_num);
        
        return results;
    }

    template <typename VecType, typename KernelType>
    Results* load_store_two_arg_kernel(Profiler* profiler, void (*kernel_fun)(VecType*, VecType*), int cpu_num, const char* title)
    {
        printHeader(title);

        Results* results = new Results();

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

            profiler->trialSetup();
            profiler->startTrial();

            //Run Kernel
            kernel_fun(a_vec, b_vec);

            profiler->endTrial();
            
            TrialResult trial_result = computeTrialResultAndSetTrialNum(profiler, results);
            printTrial(trial_result);
            processTrialAndPrepareForNextHelper(profiler, *results, trial_result, trial, discard_count);

            _mm_free(a);
            _mm_free(b);
        }

        printStats(profiler, results, cpu_num);
        
        return results;
    }

    template <typename VecType, typename KernelType>
    Results* load_store_three_arg_kernel(Profiler* profiler, void (*kernel_fun)(VecType*, VecType*, VecType*), int cpu_num, const char* title)
    {
        printHeader(title);

        Results* results = new Results();

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

            profiler->trialSetup();
            profiler->startTrial();

            //Run Kernel
            kernel_fun(a_vec, b_vec, c_vec);

            profiler->endTrial();
            
            TrialResult trial_result = computeTrialResultAndSetTrialNum(profiler, results);
            printTrial(trial_result);
            processTrialAndPrepareForNextHelper(profiler, *results, trial_result, trial, discard_count);

            _mm_free(a);
            _mm_free(b);
            _mm_free(c);
        }

        printStats(profiler, results, cpu_num);
        
        return results;
    }

    template <typename VecType, typename KernelType>
    Results* load_store_four_arg_kernel(Profiler* profiler, void (*kernel_fun)(VecType*, VecType*, VecType*, VecType*), int cpu_num, const char* title)
    {
        printHeader(title);

        Results* results = new Results();

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

            profiler->trialSetup();
            profiler->startTrial();

            //Run Kernel
            kernel_fun(a_vec, b_vec, c_vec, d_vec);

            profiler->endTrial();
            
            TrialResult trial_result = computeTrialResultAndSetTrialNum(profiler, results);
            printTrial(trial_result);
            processTrialAndPrepareForNextHelper(profiler, *results, trial_result, trial, discard_count);

            _mm_free(a);
            _mm_free(b);
            _mm_free(c);
            _mm_free(d);
        }

        printStats(profiler, results, cpu_num);
        
        return results;
    }

    template <typename KernelType>
    Results* no_vec_three_arg_kernel(Profiler* profiler, void (*kernel_fun)(KernelType*, KernelType*, KernelType*), int cpu_num, const char* title)
    {
        printHeader(title);

        Results* results = new Results();

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

            profiler->trialSetup();
            profiler->startTrial();

            //Run Kernel
            kernel_fun(a_vec, b_vec, c_vec);

            profiler->endTrial();
            
            TrialResult trial_result = computeTrialResultAndSetTrialNum(profiler, results);
            printTrial(trial_result);
            processTrialAndPrepareForNextHelper(profiler, *results, trial_result, trial, discard_count);

            _mm_free(a);
            _mm_free(b);
            _mm_free(c);
        }

        printStats(profiler, results, cpu_num);
        
        return results;
    }

    template <typename VecInType, typename VecOutType, typename AlignType>
    Results* load_store_two_arg_init_kernel(Profiler* profiler, void (*kernel_fun)(VecInType*, VecOutType*), int cpu_num, const char* title)
    {
        printHeader(title);

        Results* results = new Results();

        int trial = 0;
        int discard_count = 0;

        while(trial<TRIALS)
        {
            //Allocate the arrays to operate over
            //These arrays need to be aligned to the vector register size of 256a
            void* in_mem  = _mm_malloc (STIM_LEN*sizeof(VecInType),  sizeof(AlignType));
            void* out_mem = _mm_malloc (STIM_LEN*sizeof(VecOutType), sizeof(AlignType));

            VecInType* in_vec =   (VecInType * ) in_mem;
            VecOutType* out_vec = (VecOutType * ) out_mem;

            //Initialize input
            for(unsigned long initCounter = 0; initCounter < STIM_LEN; initCounter++){
                void* in_item = (void*) (in_vec + initCounter);
                initInput(in_item, initCounter);
            }

            profiler->trialSetup();
            profiler->startTrial();

            //Run Kernel
            kernel_fun(in_vec, out_vec);

            profiler->endTrial();
            
            TrialResult trial_result = computeTrialResultAndSetTrialNum(profiler, results);
            printTrial(trial_result);
            processTrialAndPrepareForNextHelper(profiler, *results, trial_result, trial, discard_count);

            _mm_free(in_mem);
            _mm_free(out_mem);
        }

        printStats(profiler, results, cpu_num);
        
        return results;
    }

    //In this template, a array of void pointers is passed to the kernel for the input and output.
    //The function need to internally perform the casting
    //A vector of input type sizes (in bytes) and output type sizes (in bytes) are supplied to 
    template <typename AlignType>
    Results* load_store_arb_init_kernel(Profiler* profiler, void (*kernel_fun)(void**, void**), std::vector<int> inSizes, std::vector<int> outSizes, std::vector<int> numInElements, std::vector<int> numOutElements, int cpu_num, const char* title)
    {
        printHeader(title);

        Results* results = new Results();

        int trial = 0;
        int discard_count = 0;

        while(trial<TRIALS)
        {
            //Allocate the arrays to operate over
            //Allocate the input arrays
            void* input_arrays[] = new (void*)[inSizes.size()];
            void* output_arrays[] = new (void*)[outSizes.size()];

            for(unsigned long i = 0; i<inSizes.size(); i++){
                void* in_mem  = _mm_malloc(numInElements[i]*inSizes[i], sizeof(AlignType));
                input_arrays[i] = in_mem;
            }

            for(unsigned long i = 0; i<outSizes.size(); i++){
                void* out_mem  = _mm_malloc(numOutElements[i]*outSizes[i], sizeof(AlignType));
                output_arrays[i] = out_mem;
            }

            //Initialize input
            for(unsigned long initCounter = 0; initCounter < numInElements[0]; initCounter++){
                initInput(input_arrays, initCounter);
            }

            profiler->trialSetup();
            profiler->startTrial();

            //Run Kernel
            kernel_fun(input_arrays, output_arrays);

            profiler->endTrial();
            
            TrialResult trial_result = computeTrialResultAndSetTrialNum(profiler, results);
            printTrial(trial_result);
            processTrialAndPrepareForNextHelper(profiler, *results, trial_result, trial, discard_count);

            for(unsigned long i = 0; i<inSizes.size(); i++){
                _mm_free(input_arrays[i]);
            }

            for(unsigned long i = 0; i<outSizes.size(); i++){
                _mm_free(output_arrays[i]);
            }

            delete[] input_arrays;
            delete[] output_arrays;
        }

        printStats(profiler, results, cpu_num);
        
        return results;
    }

#endif
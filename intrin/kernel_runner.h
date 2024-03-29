#ifndef _H_KERNEL_RUNNER
    #define _H_KERNEL_RUNNER

    #include "results.h"
    #include "kernel_runner_helpers.h"
    #include "mallocHelpers.h"

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
            processTrialAndPrepareForNextHelper(profiler, *results, trial_result, trial, discard_count, {});
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
            void* a = _mm_malloc_core (STIM_LEN*sizeof(KernelType), sizeof(VecType), cpu_num);

            VecType* a_vec = (VecType * ) a;

            profiler->trialSetup();
            profiler->startTrial();

            //Run Kernel
            kernel_fun(a_vec);

            profiler->endTrial();
            
            TrialResult trial_result = computeTrialResultAndSetTrialNum(profiler, results);
            printTrial(trial_result);
            processTrialAndPrepareForNextHelper(profiler, *results, trial_result, trial, discard_count, {});

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
            void* a = _mm_malloc_core (STIM_LEN*sizeof(KernelType), sizeof(VecType), cpu_num);
            void* b = _mm_malloc_core (STIM_LEN*sizeof(KernelType), sizeof(VecType), cpu_num);

            VecType* a_vec = (VecType * ) a;
            VecType* b_vec = (VecType * ) b;

            profiler->trialSetup();
            profiler->startTrial();

            //Run Kernel
            kernel_fun(a_vec, b_vec);

            profiler->endTrial();
            
            TrialResult trial_result = computeTrialResultAndSetTrialNum(profiler, results);
            printTrial(trial_result);
            processTrialAndPrepareForNextHelper(profiler, *results, trial_result, trial, discard_count, {});

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
            void* a = _mm_malloc_core (STIM_LEN*sizeof(KernelType), sizeof(VecType), cpu_num);
            void* b = _mm_malloc_core (STIM_LEN*sizeof(KernelType), sizeof(VecType), cpu_num);
            void* c = _mm_malloc_core (STIM_LEN*sizeof(KernelType), sizeof(VecType), cpu_num);

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
            processTrialAndPrepareForNextHelper(profiler, *results, trial_result, trial, discard_count, {});

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
            void* a = _mm_malloc_core (STIM_LEN*sizeof(KernelType), sizeof(VecType), cpu_num);
            void* b = _mm_malloc_core (STIM_LEN*sizeof(KernelType), sizeof(VecType), cpu_num);
            void* c = _mm_malloc_core (STIM_LEN*sizeof(KernelType), sizeof(VecType), cpu_num);
            void* d = _mm_malloc_core (STIM_LEN*sizeof(KernelType), sizeof(VecType), cpu_num);

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
            processTrialAndPrepareForNextHelper(profiler, *results, trial_result, trial, discard_count, {});

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
            void* a = malloc_core (STIM_LEN*sizeof(KernelType), cpu_num);
            void* b = malloc_core (STIM_LEN*sizeof(KernelType), cpu_num);
            void* c = malloc_core (STIM_LEN*sizeof(KernelType), cpu_num);

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
            processTrialAndPrepareForNextHelper(profiler, *results, trial_result, trial, discard_count, {});

            free(a);
            free(b);
            free(c);
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
            void* in_mem  = _mm_malloc_core (STIM_LEN*sizeof(VecInType),  sizeof(AlignType), cpu_num);
            void* out_mem = _mm_malloc_core (STIM_LEN*sizeof(VecOutType), sizeof(AlignType), cpu_num);

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
            processTrialAndPrepareForNextHelper(profiler, *results, trial_result, trial, discard_count, {});

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

        typedef void* voidArr;

        Results* results = new Results();

        int trial = 0;
        int discard_count = 0;

        while(trial<TRIALS)
        {
            //Allocate the arrays to operate over
            //Allocate the input arrays
            void** input_arrays = new voidArr[inSizes.size()];
            void** output_arrays = new voidArr[outSizes.size()];

            for(unsigned long i = 0; i<inSizes.size(); i++){
                void* in_mem  = _mm_malloc_core(numInElements[i]*inSizes[i], sizeof(AlignType), cpu_num);
                input_arrays[i] = in_mem;
            }

            for(unsigned long i = 0; i<outSizes.size(); i++){
                void* out_mem  = _mm_malloc_core(numOutElements[i]*outSizes[i], sizeof(AlignType), cpu_num);
                output_arrays[i] = out_mem;
            }

            //Initialize input
            void* input_arrays_cast = (void*) input_arrays;
            for(unsigned long initCounter = 0; initCounter < numInElements[0]; initCounter++){
                initInput(input_arrays_cast, initCounter);
            }

            profiler->trialSetup();
            profiler->startTrial();

            //Run Kernel
            kernel_fun(input_arrays, output_arrays);

            profiler->endTrial();
            
            TrialResult trial_result = computeTrialResultAndSetTrialNum(profiler, results);
            printTrial(trial_result);
            processTrialAndPrepareForNextHelper(profiler, *results, trial_result, trial, discard_count, {});

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
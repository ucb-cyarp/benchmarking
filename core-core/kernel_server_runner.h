#ifndef _H_KERNEL_SERVER_RUNNER
    #define _H_KERNEL_SERVER_RUNNER

    #include <stdint.h>
    #include <atomic>
    #include "results.h"
    #include "profiler.h"
    #include "kernel_runner_helpers.h"
    #include "generic_profiler.h"

    #include "intrin_bench_default_defines_and_imports_cpp.h"

    void* noCleanupFctn(void*);

    class KernelExePrimaryWrapperArgs //The instance of the kernel that performs the measurement
    {
        public:
            Profiler* profiler;
            void* (*kernel_fun)(void*);
            void* (*reset_fun)(void*);
            void* (*cleanup_fun)(void*);
            void* kernel_args;
            void* reset_args;
            int num_args; //The number of kernel and reset args
            size_t kernel_arg_size; //The size per kernel arg
            size_t reset_arg_size; //The size per reset arg
            int cpu_num;
            std::vector<std::atomic_flag*> start_new_trial_signals; //A vector of flags used to signal to other threads that a new trial should be started
            std::vector<std::atomic_flag*> restart_trial_signals; //A vector of flags used to signal other threads that a trial should be re-started
            std::vector<std::atomic_flag*> done_signals; //A vector of flags used to indicate that other threads are finished (and that the reset can occur)
            int num_trials;
            //For multiple interconnected primaries (ie. fanin/fanout)
            bool interconnected_primaries = false; //If true, this is one of several interconnected primaries (measuring kernels_)
            bool interconnected_master = false; //If true, this is the master primary
            std::vector<std::atomic_flag*> start_new_trial_signals_to_master;
            std::vector<std::atomic_flag*> restart_trial_signals_to_master;
            std::vector<std::atomic_flag*> start_new_trial_signals_from_master;
            std::vector<std::atomic_flag*> restart_trial_signals_from_master;
            std::vector<std::shared_ptr<BenchmarkSpecificResult>*> benchmarkSpecificResultPtrs;
    };

    class KernelExeSecondaryWrapperArgs
    {
        public:
            void* (*kernel_fun)(void*);
            void* kernel_args;
            int num_args; //The number of kernel and reset args
            size_t kernel_arg_size; //The size per kernel arg
            std::atomic_flag* start_new_trial_signal;
            std::atomic_flag* restart_trial_signal;
            std::vector<std::atomic_flag*> done_signals;
            std::shared_ptr<BenchmarkSpecificResult> *benchmarkSpecificResultPtr;
            int num_trials;
    };

    class SimultaniousResults
    {
        public:
            Results results_a;
            Results results_b;
    };

    //Returns a vector of results std::vector<Results>
    void* kernel_exe_primary_wrapper(void *arg);

    //Returns NULL
    void* kernel_exe_secondary_wrapper(void *arg);

    /*
     * Executes a kernel with multiple experiments and multiple trials per experiment.
     * 
     * The configurations for the experiments are provided by the arg_server, arg_client, and reset_arg arguments.
     * The length of these arrays must match and must be equivalent to num_args
     * 
     * Note that this function does not directly allocate memory.  If allocating/freeing memory between trials is desired
     * it should be conducted in the provided reset function.  Double pointers can be passed to the kernels to be used.
     * 
     * Note, in re-factoring, this function replaced the execute_kernel function.
     * The execute_kernel functionality is achieved by passing the same function as both the kernel_server and kernel_client
     */
    template <typename ServerArgType, typename ClientArgType, typename ResetArgType>
    std::vector<Results> execute_client_server_kernel(Profiler* profiler, void* (*kernel_server)(void*), void* (*kernel_client)(void*), void* (*kernel_reset)(void*), void* (*kernel_cleanup)(void*), ServerArgType* arg_server, ClientArgType* arg_client, ResetArgType* reset_arg, int cpu_a, int cpu_b, int num_args)
    {
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

        #if USE_SCHED_FIFO==1
            status=  pthread_attr_setinheritsched(&attr_a, PTHREAD_EXPLICIT_SCHED);
            if(status != 0)
            {
                printf("Could not set pthread explicit schedule attribute ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setinheritsched(&attr_b, PTHREAD_EXPLICIT_SCHED);
            if(status != 0)
            {
                printf("Could not set pthread explicit schedule attribute ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setschedpolicy(&attr_a, SCHED_FIFO);
            if(status != 0)
            {
                printf("Could not set pthread schedule policy to SCHED_FIFO ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setschedpolicy(&attr_b, SCHED_FIFO);
            if(status != 0)
            {
                printf("Could not set pthread schedule policy to SCHED_FIFO ... exiting\n");
                exit(1);
            }

            struct sched_param threadParams;
            threadParams.sched_priority = sched_get_priority_max(SCHED_FIFO);

            status=  pthread_attr_setschedparam(&attr_a, &threadParams);
            if(status != 0)
            {
                printf("Could not set pthread schedule parameter ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setschedparam(&attr_b, &threadParams);
            if(status != 0)
            {
                printf("Could not set pthread schedule parameter ... exiting\n");
                exit(1);
            }
        #endif

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

        //Create the syncronization mechanisms shared between the client and server
        std::atomic_flag* start_new_trial_signal = new std::atomic_flag;
        std::atomic_flag* restart_trial_signal = new std::atomic_flag;
        std::atomic_flag* done_signal = new std::atomic_flag;

        //Init these syncronization mechanisms before starting threads
        //These flags are active low so are initialized to true
        std::atomic_flag_test_and_set_explicit(start_new_trial_signal, std::memory_order_acq_rel);
        std::atomic_flag_test_and_set_explicit(restart_trial_signal, std::memory_order_acq_rel);
        std::atomic_flag_test_and_set_explicit(done_signal, std::memory_order_acq_rel);

        std::shared_ptr<BenchmarkSpecificResult> *benchmarkSpecificResultPtrClient = new std::shared_ptr<BenchmarkSpecificResult>(nullptr);

        // --- Create threads ---
        //Create threads.  Create thread b (client) before thread a (server) which performs measurments

        // - Start Thread B
        KernelExeSecondaryWrapperArgs* client_args = new KernelExeSecondaryWrapperArgs;
        client_args->kernel_fun = kernel_client;
        client_args->kernel_args = arg_client;
        client_args->num_args = num_args;
        client_args->kernel_arg_size = sizeof(ClientArgType);
        client_args->start_new_trial_signal = start_new_trial_signal;
        client_args->restart_trial_signal = restart_trial_signal;
        client_args->done_signals.push_back(done_signal);
        client_args->num_trials = TRIALS;
        client_args->benchmarkSpecificResultPtr = benchmarkSpecificResultPtrClient;

        status = pthread_create(&thread_b, &attr_b, kernel_exe_secondary_wrapper, client_args);
        if(status != 0)
        {
            printf("Could not create b thread ... exiting\n");
            errno = status;
            perror(NULL);
            exit(1);
        }

        // - Start Thread A
        KernelExePrimaryWrapperArgs* server_args = new KernelExePrimaryWrapperArgs();
        server_args->profiler = profiler;
        server_args->kernel_fun = kernel_server;
        server_args->reset_fun = kernel_reset;
        server_args->cleanup_fun = kernel_cleanup;
        server_args->kernel_args = arg_server;
        server_args->reset_args = reset_arg;
        server_args->num_args = num_args;
        server_args->kernel_arg_size = sizeof(ServerArgType);
        server_args->reset_arg_size = sizeof(ResetArgType);
        server_args->cpu_num = cpu_a;
        server_args->start_new_trial_signals.push_back(start_new_trial_signal);
        server_args->restart_trial_signals.push_back(restart_trial_signal);
        server_args->done_signals.push_back(done_signal);
        server_args->num_trials = TRIALS;
        server_args->benchmarkSpecificResultPtrs.push_back(benchmarkSpecificResultPtrClient);

        status = pthread_create(&thread_a, &attr_a, kernel_exe_primary_wrapper, server_args);
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

        //Copy the results for returning
        std::vector<Results> resultVecCpy;
        std::vector<Results> *resultVec = (std::vector<Results>*) res_a;
        resultVecCpy = *resultVec;

        //Delete temportary results
        delete resultVec;
        free(res_b);

        //Delete thread parameters
        delete start_new_trial_signal;
        delete restart_trial_signal;
        delete done_signal;

        delete client_args;
        delete server_args;

        delete benchmarkSpecificResultPtrClient;

        return resultVecCpy;
    }

    /**
     * NOTE: This function replaced the execute_kernel function.  To get the same functionality as execute_kernel
     * supply the same argument for both kernel_server and kernel_client.
     * 
     * NOTE: The order of arguments has changed.  They are now arranged in pairs server_1, client_1, server_2, client_2.
     * This actually matches the order of the CPU assignments which are
     * server_1 = cpu_a
     * client_1 = cpu_b
     * server_2 = cpu_c
     * client_2 = cpu_d
     */
    template <typename ServerArgType, typename ClientArgType, typename ResetArgType>
    std::vector<SimultaniousResults> execute_client_server_kernel(Profiler* profiler, void* (*kernel_server)(void*), void* (*kernel_client)(void*), void* (*kernel_reset)(void*), void* (*kernel_cleanup)(void*), ServerArgType* arg_server_1, ClientArgType* arg_client_1, ServerArgType* arg_server_2, ClientArgType* arg_client_2, ResetArgType* reset_arg_1, ResetArgType* reset_arg_2, int cpu_a, int cpu_b, int cpu_c, int cpu_d, int num_args)
    {
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

        #if USE_SCHED_FIFO==1
            status=  pthread_attr_setinheritsched(&attr_a, PTHREAD_EXPLICIT_SCHED);
            if(status != 0)
            {
                printf("Could not set pthread explicit schedule attribute ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setinheritsched(&attr_b, PTHREAD_EXPLICIT_SCHED);
            if(status != 0)
            {
                printf("Could not set pthread explicit schedule attribute ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setinheritsched(&attr_c, PTHREAD_EXPLICIT_SCHED);
            if(status != 0)
            {
                printf("Could not set pthread explicit schedule attribute ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setinheritsched(&attr_d, PTHREAD_EXPLICIT_SCHED);
            if(status != 0)
            {
                printf("Could not set pthread explicit schedule attribute ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setschedpolicy(&attr_a, SCHED_FIFO);
            if(status != 0)
            {
                printf("Could not set pthread schedule policy to SCHED_FIFO ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setschedpolicy(&attr_b, SCHED_FIFO);
            if(status != 0)
            {
                printf("Could not set pthread schedule policy to SCHED_FIFO ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setschedpolicy(&attr_c, SCHED_FIFO);
            if(status != 0)
            {
                printf("Could not set pthread schedule policy to SCHED_FIFO ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setschedpolicy(&attr_d, SCHED_FIFO);
            if(status != 0)
            {
                printf("Could not set pthread schedule policy to SCHED_FIFO ... exiting\n");
                exit(1);
            }

            struct sched_param threadParams;
            threadParams.sched_priority = sched_get_priority_max(SCHED_FIFO);

            status=  pthread_attr_setschedparam(&attr_a, &threadParams);
            if(status != 0)
            {
                printf("Could not set pthread schedule parameter ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setschedparam(&attr_b, &threadParams);
            if(status != 0)
            {
                printf("Could not set pthread schedule parameter ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setschedparam(&attr_c, &threadParams);
            if(status != 0)
            {
                printf("Could not set pthread schedule parameter ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setschedparam(&attr_d, &threadParams);
            if(status != 0)
            {
                printf("Could not set pthread schedule parameter ... exiting\n");
                exit(1);
            }
        #endif

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

        //Create the syncronization mechanisms shared between the client and server
        std::atomic_flag* start_new_trial_signal_b = new std::atomic_flag;
        std::atomic_flag* start_new_trial_signal_d = new std::atomic_flag;
        std::atomic_flag* restart_trial_signal_b = new std::atomic_flag;
        std::atomic_flag* restart_trial_signal_d = new std::atomic_flag;
        std::atomic_flag* done_signal_b = new std::atomic_flag;
        std::atomic_flag* done_signal_d = new std::atomic_flag;

        //Init these syncronization mechanisms before starting threads
        //These flags are active low so are initialized to true
        std::atomic_flag_test_and_set_explicit(start_new_trial_signal_b, std::memory_order_acq_rel);
        std::atomic_flag_test_and_set_explicit(restart_trial_signal_b, std::memory_order_acq_rel);
        std::atomic_flag_test_and_set_explicit(done_signal_b, std::memory_order_acq_rel);
        std::atomic_flag_test_and_set_explicit(start_new_trial_signal_d, std::memory_order_acq_rel);
        std::atomic_flag_test_and_set_explicit(restart_trial_signal_d, std::memory_order_acq_rel);
        std::atomic_flag_test_and_set_explicit(done_signal_d, std::memory_order_acq_rel);

        std::shared_ptr<BenchmarkSpecificResult> *benchmarkSpecificResultPtrClient_b = new std::shared_ptr<BenchmarkSpecificResult>(nullptr);
        std::shared_ptr<BenchmarkSpecificResult> *benchmarkSpecificResultPtrClient_d = new std::shared_ptr<BenchmarkSpecificResult>(nullptr);

        // --- Create threads ---
        //Create threads.  Create thread b and d (clients) before thread a and c (servers) which performs measurments

        // - Start Threads B & D
        KernelExeSecondaryWrapperArgs* client_args_1 = new KernelExeSecondaryWrapperArgs;
        client_args_1->kernel_fun = kernel_client;
        client_args_1->kernel_args = arg_client_1;
        client_args_1->num_args = num_args;
        client_args_1->kernel_arg_size = sizeof(ClientArgType);
        client_args_1->start_new_trial_signal = start_new_trial_signal_b;
        client_args_1->restart_trial_signal = restart_trial_signal_b;
        client_args_1->done_signals.push_back(done_signal_b);
        client_args_1->num_trials = TRIALS;
        client_args_1->benchmarkSpecificResultPtr = benchmarkSpecificResultPtrClient_b;

        KernelExeSecondaryWrapperArgs* client_args_2 = new KernelExeSecondaryWrapperArgs;
        client_args_2->kernel_fun = kernel_client;
        client_args_2->kernel_args = arg_client_2;
        client_args_2->num_args = num_args;
        client_args_2->kernel_arg_size = sizeof(ClientArgType);
        client_args_2->start_new_trial_signal = start_new_trial_signal_d;
        client_args_2->restart_trial_signal = restart_trial_signal_d;
        client_args_2->done_signals.push_back(done_signal_d);
        client_args_2->num_trials = TRIALS;
        client_args_2->benchmarkSpecificResultPtr = benchmarkSpecificResultPtrClient_d;

        status = pthread_create(&thread_b, &attr_b, kernel_exe_secondary_wrapper, client_args_1);
        if(status != 0)
        {
            printf("Could not create b thread ... exiting\n");
            errno = status;
            perror(NULL);
            exit(1);
        }

        status = pthread_create(&thread_d, &attr_d, kernel_exe_secondary_wrapper, client_args_2);
        if(status != 0)
        {
            printf("Could not create d thread ... exiting\n");
            errno = status;
            perror(NULL);
            exit(1);
        }

        // - Start Threads A & C
        KernelExePrimaryWrapperArgs* server_args_1 = new KernelExePrimaryWrapperArgs();
        server_args_1->profiler = profiler;
        server_args_1->kernel_fun = kernel_server;
        server_args_1->reset_fun = kernel_reset;
        server_args_1->cleanup_fun = kernel_cleanup;
        server_args_1->kernel_args = arg_server_1;
        server_args_1->reset_args = reset_arg_1;
        server_args_1->num_args = num_args;
        server_args_1->kernel_arg_size = sizeof(ServerArgType);
        server_args_1->reset_arg_size = sizeof(ResetArgType);
        server_args_1->cpu_num = cpu_a;
        server_args_1->start_new_trial_signals.push_back(start_new_trial_signal_b);
        server_args_1->restart_trial_signals.push_back(restart_trial_signal_b);
        server_args_1->done_signals.push_back(done_signal_b);
        server_args_1->num_trials = TRIALS;
        server_args_1->benchmarkSpecificResultPtrs.push_back(benchmarkSpecificResultPtrClient_b);

        //Change to a generic profiler for server c if the profiler does not support multiple instances (ex. AMDuProf)
        Profiler* cProfiler;
        if(profiler->supportsMultipleInstances()){
            cProfiler = profiler->clone();
            cProfiler->init();
        }else{
            cProfiler = new GenericProfiler();
        }

        KernelExePrimaryWrapperArgs* server_args_2 = new KernelExePrimaryWrapperArgs();
        server_args_2->profiler = cProfiler;
        server_args_2->kernel_fun = kernel_server;
        server_args_2->reset_fun = kernel_reset;
        server_args_2->cleanup_fun = kernel_cleanup;
        server_args_2->kernel_args = arg_server_2;
        server_args_2->reset_args = reset_arg_2;
        server_args_2->num_args = num_args;
        server_args_2->kernel_arg_size = sizeof(ServerArgType);
        server_args_2->reset_arg_size = sizeof(ResetArgType);
        server_args_2->cpu_num = cpu_c;
        server_args_2->start_new_trial_signals.push_back(start_new_trial_signal_d);
        server_args_2->restart_trial_signals.push_back(restart_trial_signal_d);
        server_args_2->done_signals.push_back(done_signal_d);
        server_args_2->num_trials = TRIALS;
        server_args_2->benchmarkSpecificResultPtrs.push_back(benchmarkSpecificResultPtrClient_d);

        status = pthread_create(&thread_a, &attr_a, kernel_exe_primary_wrapper, server_args_1);
        if(status != 0)
        {
            printf("Could not create a thread ... exiting\n");
            errno = status;
            perror(NULL);
            exit(1);
        }

        status = pthread_create(&thread_c, &attr_c, kernel_exe_primary_wrapper, server_args_2);
        if(status != 0)
        {
            printf("Could not create c thread ... exiting\n");
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

        std::vector<SimultaniousResults> simultaniousResultVec;

        std::vector<Results>* result_vec_a = (std::vector<Results>*) res_a;
        std::vector<Results>* result_vec_c = (std::vector<Results>*) res_c;

        //Copy the results for returning
        if(result_vec_a->size() != result_vec_c->size()){
            std::cerr << "Error: Different number of results returned from simultanious runs" << std::endl;
            exit(1);
        }

        for(int i = 0; i<result_vec_a->size(); i++){
            SimultaniousResults simultaniousResults;
            simultaniousResults.results_a = (*result_vec_a)[i];
            simultaniousResults.results_b = (*result_vec_c)[i];
            simultaniousResultVec.push_back(simultaniousResults);
        }

        //Delete temportary results
        delete result_vec_a;
        free(res_b);
        delete result_vec_c;
        free(res_d);

        //Cleanup
        delete start_new_trial_signal_b;
        delete start_new_trial_signal_d;
        delete restart_trial_signal_b;
        delete restart_trial_signal_d;
        delete done_signal_b;
        delete done_signal_d;

        delete server_args_1;
        delete server_args_2;
        delete client_args_1;
        delete client_args_2;

        delete cProfiler;

        delete benchmarkSpecificResultPtrClient_b;
        delete benchmarkSpecificResultPtrClient_d;

        return simultaniousResultVec;
    }

    /*
     * Note that srv_a will be selected as the master measurement thread which will control both srv_b and cli_c
     * Both srv_a and srv_b get feedback from cli_c
     * cli_c gets control from srv_a
     * 
     * Also, note that only the master performs the inter-trial reset
     */
    template <typename ServerArgType, typename ClientArgType, typename ResetArgType>
    std::vector<SimultaniousResults> execute_kernel_fanin_server_measure(Profiler* profiler, void* (*srv_kernel_fun)(void*), void* (*cli_kernel_fun)(void*), void* (*kernel_reset)(void*), void* (*kernel_cleanup)(void*), ServerArgType* arg_srv_a, ServerArgType* arg_srv_b, ClientArgType* arg_cli_c, ResetArgType* reset_arg, int cpu_srv_a, int cpu_srv_b, int cpu_cli_c, int num_args)
    {
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

        #if USE_SCHED_FIFO==1
            status=  pthread_attr_setinheritsched(&attr_srv_a, PTHREAD_EXPLICIT_SCHED);
            if(status != 0)
            {
                printf("Could not set pthread explicit schedule attribute ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setinheritsched(&attr_srv_b, PTHREAD_EXPLICIT_SCHED);
            if(status != 0)
            {
                printf("Could not set pthread explicit schedule attribute ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setinheritsched(&attr_cli_c, PTHREAD_EXPLICIT_SCHED);
            if(status != 0)
            {
                printf("Could not set pthread explicit schedule attribute ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setschedpolicy(&attr_srv_a, SCHED_FIFO);
            if(status != 0)
            {
                printf("Could not set pthread schedule policy to SCHED_FIFO ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setschedpolicy(&attr_srv_b, SCHED_FIFO);
            if(status != 0)
            {
                printf("Could not set pthread schedule policy to SCHED_FIFO ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setschedpolicy(&attr_cli_c, SCHED_FIFO);
            if(status != 0)
            {
                printf("Could not set pthread schedule policy to SCHED_FIFO ... exiting\n");
                exit(1);
            }

            struct sched_param threadParams;
            threadParams.sched_priority = sched_get_priority_max(SCHED_FIFO);

            status=  pthread_attr_setschedparam(&attr_srv_a, &threadParams);
            if(status != 0)
            {
                printf("Could not set pthread schedule parameter ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setschedparam(&attr_srv_b, &threadParams);
            if(status != 0)
            {
                printf("Could not set pthread schedule parameter ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setschedparam(&attr_cli_c, &threadParams);
            if(status != 0)
            {
                printf("Could not set pthread schedule parameter ... exiting\n");
                exit(1);
            }
        #endif

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

        //Create the syncronization mechanisms shared between the clients and servers
        //Note that there are interdependent masters which requires additional syncronization flags

        //Create the syncronization mechanisms shared between the client and server
        std::atomic_flag* start_new_trial_signal_client = new std::atomic_flag;
        std::atomic_flag* restart_trial_signal_client = new std::atomic_flag;
        std::atomic_flag* done_signal_client_1 = new std::atomic_flag;
        std::atomic_flag* done_signal_client_2 = new std::atomic_flag;

        std::atomic_flag* start_new_trial_from_master = new std::atomic_flag;
        std::atomic_flag* restart_trial_from_master = new std::atomic_flag;
        std::atomic_flag* start_new_trial_to_master = new std::atomic_flag;
        std::atomic_flag* restart_trial_to_master = new std::atomic_flag;

        //Init these syncronization mechanisms before starting threads
        //These flags are active low so are initialized to true
        std::atomic_flag_test_and_set_explicit(start_new_trial_signal_client, std::memory_order_acq_rel);
        std::atomic_flag_test_and_set_explicit(restart_trial_signal_client, std::memory_order_acq_rel);
        std::atomic_flag_test_and_set_explicit(done_signal_client_1, std::memory_order_acq_rel);
        std::atomic_flag_test_and_set_explicit(done_signal_client_2, std::memory_order_acq_rel);

        std::atomic_flag_test_and_set_explicit(start_new_trial_from_master, std::memory_order_acq_rel);
        std::atomic_flag_test_and_set_explicit(restart_trial_from_master, std::memory_order_acq_rel);
        std::atomic_flag_test_and_set_explicit(start_new_trial_to_master, std::memory_order_acq_rel);
        std::atomic_flag_test_and_set_explicit(restart_trial_to_master, std::memory_order_acq_rel);

        std::shared_ptr<BenchmarkSpecificResult> *benchmarkSpecificResultPtrClient = new std::shared_ptr<BenchmarkSpecificResult>(nullptr);

        // --- Create threads ---
        //Create threads.  Create thread c (client) before thread a and b (server) which perform measurments

        // - Start Thread c
        KernelExeSecondaryWrapperArgs* client_args = new KernelExeSecondaryWrapperArgs;
        client_args->kernel_fun = cli_kernel_fun;
        client_args->kernel_args = arg_cli_c;
        client_args->num_args = num_args;
        client_args->kernel_arg_size = sizeof(ClientArgType);
        client_args->start_new_trial_signal = start_new_trial_signal_client;
        client_args->restart_trial_signal = restart_trial_signal_client;
        client_args->done_signals.push_back(done_signal_client_1);
        client_args->done_signals.push_back(done_signal_client_2);
        client_args->num_trials = TRIALS;
        client_args->benchmarkSpecificResultPtr = benchmarkSpecificResultPtrClient;

        status = pthread_create(&thread_cli_c, &attr_cli_c, kernel_exe_secondary_wrapper, client_args);
        if(status != 0)
        {
            printf("Could not create c thread ... exiting\n");
            errno = status;
            perror(NULL);
            exit(1);
        }

        // - Start Thread A & B

        //Note: This is the master
        KernelExePrimaryWrapperArgs* server_args_a = new KernelExePrimaryWrapperArgs();
        server_args_a->profiler = profiler;
        server_args_a->kernel_fun = srv_kernel_fun;
        server_args_a->reset_fun = kernel_reset;
        server_args_a->cleanup_fun = kernel_cleanup;
        server_args_a->kernel_args = arg_srv_a;
        server_args_a->reset_args = reset_arg;
        server_args_a->num_args = num_args;
        server_args_a->kernel_arg_size = sizeof(ServerArgType);
        server_args_a->reset_arg_size = sizeof(ResetArgType);
        server_args_a->cpu_num = cpu_srv_a;
        //This server gets control of the client
        server_args_a->start_new_trial_signals.push_back(start_new_trial_signal_client);
        server_args_a->restart_trial_signals.push_back(restart_trial_signal_client);
        server_args_a->done_signals.push_back(done_signal_client_1);
        server_args_a->num_trials = TRIALS;
        //Need to setup parameters for interdependent primaries
        server_args_a->interconnected_primaries = true;
        server_args_a->interconnected_master = true;
        server_args_a->start_new_trial_signals_to_master.push_back(start_new_trial_to_master);
        server_args_a->restart_trial_signals_to_master.push_back(restart_trial_to_master);
        server_args_a->start_new_trial_signals_from_master.push_back(start_new_trial_from_master);
        server_args_a->restart_trial_signals_from_master.push_back(restart_trial_from_master);
        server_args_a->benchmarkSpecificResultPtrs.push_back(benchmarkSpecificResultPtrClient); //Only server A gets the benchmark specific result from the client

        //Change to a generic profiler for server b if the profiler does not support multiple instances (ex. AMDuProf)
        //If not, still need another profiler object so that each profiler maintains seperate state
        Profiler* bProfiler;
        if(profiler->supportsMultipleInstances()){
            bProfiler = profiler->clone();
            bProfiler->init();
        }else{
            bProfiler = new GenericProfiler();
        }

        //NOTE: This is the slave process
        KernelExePrimaryWrapperArgs* server_args_b = new KernelExePrimaryWrapperArgs();
        server_args_b->profiler = bProfiler;
        server_args_b->kernel_fun = srv_kernel_fun;
        server_args_b->reset_fun = kernel_reset;
        server_args_b->cleanup_fun = kernel_cleanup;
        server_args_b->kernel_args = arg_srv_b;
        server_args_b->reset_args = reset_arg;
        server_args_b->num_args = num_args;
        server_args_b->kernel_arg_size = sizeof(ServerArgType);
        server_args_b->reset_arg_size = sizeof(ResetArgType);
        server_args_b->cpu_num = cpu_srv_b;
        //This server gets feedback from the client but does not send control
        server_args_b->done_signals.push_back(done_signal_client_2);
        server_args_b->num_trials = TRIALS;
        //Need to setup parameters for interdependent primaries
        server_args_b->interconnected_primaries = true;
        server_args_b->interconnected_master = false;
        server_args_b->start_new_trial_signals_to_master.push_back(start_new_trial_to_master);
        server_args_b->restart_trial_signals_to_master.push_back(restart_trial_to_master);
        server_args_b->start_new_trial_signals_from_master.push_back(start_new_trial_from_master);
        server_args_b->restart_trial_signals_from_master.push_back(restart_trial_from_master);

        status = pthread_create(&thread_srv_a, &attr_srv_a, kernel_exe_primary_wrapper, server_args_a);
        if(status != 0)
        {
            printf("Could not create a thread ... exiting\n");
            errno = status;
            perror(NULL);
            exit(1);
        }

        status = pthread_create(&thread_srv_b, &attr_srv_b, kernel_exe_primary_wrapper, server_args_b);
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

        //Copy results
        std::vector<SimultaniousResults> simultaniousResultVec;

        std::vector<Results>* result_vec_a = (std::vector<Results>*) res_srv_a;
        std::vector<Results>* result_vec_b = (std::vector<Results>*) res_srv_b;

        //Copy the results for returning
        if(result_vec_a->size() != result_vec_b->size()){
            std::cerr << "Error: Different number of results returned from simultanious runs" << std::endl;
            exit(1);
        }

        for(int i = 0; i<result_vec_a->size(); i++){
            SimultaniousResults simultaniousResults;
            simultaniousResults.results_a = (*result_vec_a)[i];
            simultaniousResults.results_b = (*result_vec_b)[i];
            simultaniousResultVec.push_back(simultaniousResults);
        }

        //Delete temportary results
        delete result_vec_a;
        delete result_vec_b;
        free(res_cli_c);

        delete start_new_trial_signal_client;
        delete restart_trial_signal_client;
        delete done_signal_client_1;
        delete done_signal_client_2;

        delete start_new_trial_from_master;
        delete restart_trial_from_master;
        delete start_new_trial_to_master;
        delete restart_trial_to_master;

        delete server_args_a;
        delete server_args_b;
        delete client_args;

        delete bProfiler;

        delete benchmarkSpecificResultPtrClient;

        return simultaniousResultVec;
    }

    //Note, this version is different from the other in that the server thread starts before the client.  This may allow the server to execute a transaction before the client starts.
    //Note that cli_b is a master
    //srv_a gets control from cli_b
    //cli_b and cli_c get feedback from srv_a
    template <typename ServerArgType, typename ClientArgType, typename ResetArgType>
    std::vector<SimultaniousResults> execute_kernel_fanout_client_measure(Profiler* profiler, void* (*srv_kernel_fun)(void*), void* (*cli_kernel_fun)(void*), void* (*kernel_reset)(void*), void* (*kernel_cleanup)(void*), ServerArgType* arg_srv_a, ClientArgType* arg_cli_b, ClientArgType* arg_cli_c, ResetArgType* reset_arg, int cpu_srv_a, int cpu_cli_b, int cpu_cli_c, int num_args)
    {
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

        #if USE_SCHED_FIFO==1
            status=  pthread_attr_setinheritsched(&attr_srv_a, PTHREAD_EXPLICIT_SCHED);
            if(status != 0)
            {
                printf("Could not set pthread explicit schedule attribute ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setinheritsched(&attr_cli_b, PTHREAD_EXPLICIT_SCHED);
            if(status != 0)
            {
                printf("Could not set pthread explicit schedule attribute ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setinheritsched(&attr_cli_c, PTHREAD_EXPLICIT_SCHED);
            if(status != 0)
            {
                printf("Could not set pthread explicit schedule attribute ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setschedpolicy(&attr_srv_a, SCHED_FIFO);
            if(status != 0)
            {
                printf("Could not set pthread schedule policy to SCHED_FIFO ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setschedpolicy(&attr_cli_b, SCHED_FIFO);
            if(status != 0)
            {
                printf("Could not set pthread schedule policy to SCHED_FIFO ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setschedpolicy(&attr_cli_c, SCHED_FIFO);
            if(status != 0)
            {
                printf("Could not set pthread schedule policy to SCHED_FIFO ... exiting\n");
                exit(1);
            }

            struct sched_param threadParams;
            threadParams.sched_priority = sched_get_priority_max(SCHED_FIFO);

            status=  pthread_attr_setschedparam(&attr_srv_a, &threadParams);
            if(status != 0)
            {
                printf("Could not set pthread schedule parameter ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setschedparam(&attr_cli_b, &threadParams);
            if(status != 0)
            {
                printf("Could not set pthread schedule parameter ... exiting\n");
                exit(1);
            }

            status=  pthread_attr_setschedparam(&attr_cli_c, &threadParams);
            if(status != 0)
            {
                printf("Could not set pthread schedule parameter ... exiting\n");
                exit(1);
            }
        #endif

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

        //Create the syncronization mechanisms shared between the clients and servers
        //Note that there are interdependent masters which requires additional syncronization flags

        //Create the syncronization mechanisms shared between the client and server
        std::atomic_flag* start_new_trial_signal_client = new std::atomic_flag;
        std::atomic_flag* restart_trial_signal_client = new std::atomic_flag;
        std::atomic_flag* done_signal_client_1 = new std::atomic_flag;
        std::atomic_flag* done_signal_client_2 = new std::atomic_flag;

        std::atomic_flag* start_new_trial_from_master = new std::atomic_flag;
        std::atomic_flag* restart_trial_from_master = new std::atomic_flag;
        std::atomic_flag* start_new_trial_to_master = new std::atomic_flag;
        std::atomic_flag* restart_trial_to_master = new std::atomic_flag;

        std::shared_ptr<BenchmarkSpecificResult> *benchmarkSpecificResultPtrServer = new std::shared_ptr<BenchmarkSpecificResult>(nullptr);

        //Init these syncronization mechanisms before starting threads
        //These flags are active low so are initialized to true
        std::atomic_flag_test_and_set_explicit(start_new_trial_signal_client, std::memory_order_acq_rel);
        std::atomic_flag_test_and_set_explicit(restart_trial_signal_client, std::memory_order_acq_rel);
        std::atomic_flag_test_and_set_explicit(done_signal_client_1, std::memory_order_acq_rel);
        std::atomic_flag_test_and_set_explicit(done_signal_client_2, std::memory_order_acq_rel);

        std::atomic_flag_test_and_set_explicit(start_new_trial_from_master, std::memory_order_acq_rel);
        std::atomic_flag_test_and_set_explicit(restart_trial_from_master, std::memory_order_acq_rel);
        std::atomic_flag_test_and_set_explicit(start_new_trial_to_master, std::memory_order_acq_rel);
        std::atomic_flag_test_and_set_explicit(restart_trial_to_master, std::memory_order_acq_rel);

        // --- Create threads ---
        //Create threads.  Create thread a (server) before thread b and c (client) which performs measurments

        // - Start Thread A
        KernelExeSecondaryWrapperArgs* server_args = new KernelExeSecondaryWrapperArgs;
        server_args->kernel_fun = srv_kernel_fun;
        server_args->kernel_args = arg_srv_a;
        server_args->num_args = num_args;
        server_args->kernel_arg_size = sizeof(ServerArgType);
        server_args->start_new_trial_signal = start_new_trial_signal_client;
        server_args->restart_trial_signal = restart_trial_signal_client;
        server_args->done_signals.push_back(done_signal_client_1);
        server_args->done_signals.push_back(done_signal_client_2);
        server_args->num_trials = TRIALS;
        server_args->benchmarkSpecificResultPtr = benchmarkSpecificResultPtrServer;

        status = pthread_create(&thread_srv_a, &attr_srv_a, kernel_exe_secondary_wrapper, server_args);
        if(status != 0)
        {
            printf("Could not create a thread ... exiting\n");
            errno = status;
            perror(NULL);
            exit(1);
        }

        // - Start Thread B & C (client)
        //Note: This is the master
        KernelExePrimaryWrapperArgs* measure_args_b = new KernelExePrimaryWrapperArgs();
        measure_args_b->profiler = profiler;
        measure_args_b->kernel_fun = cli_kernel_fun;
        measure_args_b->reset_fun = kernel_reset;
        measure_args_b->cleanup_fun = kernel_cleanup;
        measure_args_b->kernel_args = arg_cli_b;
        measure_args_b->reset_args = reset_arg;
        measure_args_b->num_args = num_args;
        measure_args_b->kernel_arg_size = sizeof(ClientArgType);
        measure_args_b->reset_arg_size = sizeof(ResetArgType);
        measure_args_b->cpu_num = cpu_cli_b;
        //This gets control of the secondary
        measure_args_b->start_new_trial_signals.push_back(start_new_trial_signal_client);
        measure_args_b->restart_trial_signals.push_back(restart_trial_signal_client);
        measure_args_b->done_signals.push_back(done_signal_client_1);
        measure_args_b->num_trials = TRIALS;
        //Need to setup parameters for interdependent primaries
        measure_args_b->interconnected_primaries = true;
        measure_args_b->interconnected_master = true;
        measure_args_b->start_new_trial_signals_to_master.push_back(start_new_trial_to_master);
        measure_args_b->restart_trial_signals_to_master.push_back(restart_trial_to_master);
        measure_args_b->start_new_trial_signals_from_master.push_back(start_new_trial_from_master);
        measure_args_b->restart_trial_signals_from_master.push_back(restart_trial_from_master);
        measure_args_b->benchmarkSpecificResultPtrs.push_back(benchmarkSpecificResultPtrServer); //Only server b gets the benchmarkSpecificResults from the server

        //Change to a generic profiler for primary b if the profiler does not support multiple instances (ex. AMDuProf)
        Profiler* cProfiler;
        if(profiler->supportsMultipleInstances()){
            cProfiler = profiler->clone();
            cProfiler->init();
        }else{
            cProfiler = new GenericProfiler();
        }

        //NOTE: This is the slave process
        KernelExePrimaryWrapperArgs* measure_args_c = new KernelExePrimaryWrapperArgs();
        measure_args_c->profiler = cProfiler;
        measure_args_c->kernel_fun = cli_kernel_fun;
        measure_args_c->reset_fun = kernel_reset;
        measure_args_c->cleanup_fun = kernel_cleanup;
        measure_args_c->kernel_args = arg_cli_c;
        measure_args_c->reset_args = reset_arg;
        measure_args_c->num_args = num_args;
        measure_args_c->kernel_arg_size = sizeof(ClientArgType);
        measure_args_c->reset_arg_size = sizeof(ResetArgType);
        measure_args_c->cpu_num = cpu_cli_c;
        //This gets feedback from the secondary but does not send control
        measure_args_c->done_signals.push_back(done_signal_client_2);
        measure_args_c->num_trials = TRIALS;
        //Need to setup parameters for interdependent primaries
        measure_args_c->interconnected_primaries = true;
        measure_args_c->interconnected_master = false;
        measure_args_c->start_new_trial_signals_to_master.push_back(start_new_trial_to_master);
        measure_args_c->restart_trial_signals_to_master.push_back(restart_trial_to_master);
        measure_args_c->start_new_trial_signals_from_master.push_back(start_new_trial_from_master);
        measure_args_c->restart_trial_signals_from_master.push_back(restart_trial_from_master);

        status = pthread_create(&thread_cli_b, &attr_cli_b, kernel_exe_primary_wrapper, measure_args_b);
        if(status != 0)
        {
            printf("Could not create b thread ... exiting\n");
            errno = status;
            perror(NULL);
            exit(1);
        }

        status = pthread_create(&thread_cli_c, &attr_cli_c, kernel_exe_primary_wrapper, measure_args_c);
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

        //Copy results
        std::vector<SimultaniousResults> simultaniousResultVec;

        std::vector<Results>* result_vec_b = (std::vector<Results>*) res_cli_b;
        std::vector<Results>* result_vec_c = (std::vector<Results>*) res_cli_c;

        //Copy the results for returning
        if(result_vec_b->size() != result_vec_c->size()){
            std::cerr << "Error: Different number of results returned from simultanious runs" << std::endl;
            exit(1);
        }

        for(int i = 0; i<result_vec_b->size(); i++){
            SimultaniousResults simultaniousResults;
            simultaniousResults.results_a = (*result_vec_b)[i];
            simultaniousResults.results_b = (*result_vec_c)[i];
            simultaniousResultVec.push_back(simultaniousResults);
        }

        //Delete temportary results
        free(res_srv_a);
        delete result_vec_b;
        delete result_vec_c;

        delete start_new_trial_signal_client;
        delete restart_trial_signal_client;
        delete done_signal_client_1;
        delete done_signal_client_2;

        delete start_new_trial_from_master;
        delete restart_trial_from_master;
        delete start_new_trial_to_master;
        delete restart_trial_to_master;

        delete measure_args_b;
        delete measure_args_c;
        delete server_args;

        delete cProfiler;

        delete benchmarkSpecificResultPtrServer;

        return simultaniousResultVec;
    }

#endif
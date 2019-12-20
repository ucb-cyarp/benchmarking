#ifndef _H_SINGLE_KERNELS
    #define _H_SINGLE_KERNELS

    #include "results.h"
    #include "profiler.h"

    #include "kernel_server_runner.h"
    
    #include "latency_single_kernel.h"
    #include "latency_dual_kernel.h"
    
    #include "print_results.h"

    #include <cstdlib>

    #include "mallocHelpers.h"

    #include <atomic>
    #include <ctime>

    #include "reporting_helpers.h"

    Results* run_latency_single_kernel(Profiler* profiler, int cpu_a, int cpu_b)
    {
        //=====Test 1=====
        #if PRINT_TITLE == 1
            printf("\n");
            time_t currentTime = time(NULL);
            struct tm * localT;
            localT = localtime(&currentTime);
            printf("Single Memory Location | Start Time: %s", asctime(localT));
        #endif

        //Initialize
        size_t amountToAlloc = sizeof(std::atomic_int32_t);
        if(amountToAlloc % CACHE_LINE_SIZE != 0){
            amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
        }
        std::atomic_int32_t* shared_loc = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);
        std::atomic_init(shared_loc, 0);
        if(!std::atomic_is_lock_free(shared_loc)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }

        LatencySingleKernelArgs* arg_a_array = new LatencySingleKernelArgs[1];
        LatencySingleKernelArgs* arg_b_array = new LatencySingleKernelArgs[1];
        LatencySingleKernelResetArgs* reset_arg_array = new LatencySingleKernelResetArgs[1];

        arg_a_array[0].init_counter = -1; //(server)
        arg_a_array[0].shared_ptr = shared_loc;

        LatencySingleKernelArgs arg_b;
        arg_b_array[0].init_counter = 0; //(client)
        arg_b_array[0].shared_ptr = shared_loc;

        reset_arg_array[0].shared_ptr = shared_loc; //Argument for reset function is shared location

        //This kernel has a single configuraton
        //The client and server kernel are the same
        std::vector<Results> results_vector = execute_client_server_kernel(profiler, latency_single_kernel, latency_single_kernel, latency_single_kernel_reset, arg_a_array, arg_b_array, reset_arg_array, cpu_a, cpu_b, 1);
        if(results_vector.size() != 1){
            std::cerr << "Error: Unexpected number of results" << std::endl;
            exit(1);
        }

        //Copy result (for legacy result memory handling)
        Results* results = new Results;
        *results = results_vector[0];

        exportResultsSingle2Core<std::atomic_int32_t>(profiler, cpu_a, cpu_b, *results);

        //Clean Up
        free(shared_loc);

        delete[] arg_a_array;
        delete[] arg_b_array;
        delete[] reset_arg_array;

        return results;
    }

    Results* run_latency_dual_kernel(Profiler* profiler, int cpu_a, int cpu_b)
    {
        #if PRINT_TITLE == 1
            printf("\n");
            time_t currentTime = time(NULL);
            struct tm * localT;
            localT = localtime(&currentTime);
            printf("Dual Memory Locations | Start Time: %s", asctime(localT));
        #endif

        //Initialize
        size_t amountToAlloc = sizeof(std::atomic_int32_t);
        if(amountToAlloc % CACHE_LINE_SIZE != 0){
            amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
        }
        std::atomic_int32_t* shared_loc_a = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);
        std::atomic_int32_t* shared_loc_b = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_b);
        std::atomic_init(shared_loc_a, 0);
        if(!std::atomic_is_lock_free(shared_loc_a)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }
        std::atomic_init(shared_loc_b, 0);
        if(!std::atomic_is_lock_free(shared_loc_b)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }

        //Init to 0
        *shared_loc_a = 0;
        *shared_loc_b = 0;

        LatencyDualKernelArgs* arg_a_array = new LatencyDualKernelArgs[1];
        LatencyDualKernelArgs* arg_b_array = new LatencyDualKernelArgs[1];
        LatencyDualKernelResetArgs* reset_arg_array = new LatencyDualKernelResetArgs[1];

        arg_a_array[0].init_counter = -1; //(server)
        arg_a_array[0].my_shared_ptr = shared_loc_a;
        arg_a_array[0].other_shared_ptr = shared_loc_b;

        arg_b_array[0].init_counter = 0; //(client)
        arg_b_array[0].my_shared_ptr = shared_loc_b; //Swapped from above
        arg_b_array[0].other_shared_ptr = shared_loc_a;

        reset_arg_array[0].shared_ptr_a = shared_loc_a;
        reset_arg_array[0].shared_ptr_b = shared_loc_b;
        
        //This kernel has a single configuraton
        //The client and server kernel are the same
        std::vector<Results> results_vector = execute_client_server_kernel(profiler, latency_dual_kernel, latency_dual_kernel, latency_dual_kernel_reset, arg_a_array, arg_b_array, reset_arg_array, cpu_a, cpu_b, 1);

        //Copy result (for legacy result memory handling)
        Results* results = new Results;
        *results = results_vector[0];

        exportResultsSingle2Core<std::atomic_int32_t>(profiler, cpu_a, cpu_b, *results);

        //Clean Up
        free(shared_loc_a);
        free(shared_loc_b);

        delete[] arg_a_array;
        delete[] arg_b_array;
        delete[] reset_arg_array;

        return results;
    }
#endif

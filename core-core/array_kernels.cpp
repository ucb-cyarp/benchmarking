#include "array_kernels.h"

//==== Single Array ====
void run_latency_single_array_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, FILE* file, std::ofstream* raw_file)
{
    //==== Print header ====
    std::string format = tableHeaderArray1Stream("Single Memory Location - Array", file, raw_file);

    //==== Allocate Array For the Largest Experiment ====
    size_t max_length = *std::max_element(array_lengths.begin(), array_lengths.end());

    size_t amountToAlloc = max_length*sizeof(std::atomic_int32_t);
    if(amountToAlloc % CACHE_LINE_SIZE != 0){
        amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
    }
    std::atomic_int32_t* shared_loc = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);

    //Init to 0
    for(size_t i = 0; i < max_length; i++)
    {
        std::atomic_int32_t* shared_loc_constructed = new (shared_loc+i) std::atomic_int32_t();
        std::atomic_init(shared_loc+i, 0);
        if(!std::atomic_is_lock_free(shared_loc+i)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }
    }

    //==== Create Configurations for each experiment ====
    int num_experiments = array_lengths.size();

    LatencySingleArrayKernelArgs* arg_a = new LatencySingleArrayKernelArgs[num_experiments];
    LatencySingleArrayKernelArgs* arg_b = new LatencySingleArrayKernelArgs[num_experiments];
    LatencySingleArrayKernelResetArgs* reset_arg = new LatencySingleArrayKernelResetArgs[num_experiments];

    for(int i = 0; i<num_experiments; i++)
    {
        size_t array_length = array_lengths[i];
        arg_a[i].init_counter = -1; //(server)
        arg_a[i].shared_ptr = shared_loc;
        arg_a[i].length = array_length;

        arg_b[i].init_counter = 0; //(client)
        arg_b[i].shared_ptr = shared_loc;
        arg_b[i].length = array_length;

        reset_arg[i].shared_ptr = shared_loc;
        reset_arg[i].length = array_length;
    }

    //==== Run The Experiments ====
    std::vector<Results> results_vec = execute_client_server_kernel(profiler, latency_single_array_kernel, latency_single_array_kernel, latency_single_array_kernel_reset, noCleanupFctn, arg_a, arg_b, reset_arg, cpu_a, cpu_b, num_experiments);

    //==== Process Results ====
    if(results_vec.size() != num_experiments){
        std::cerr << "Error: The number of results did not match the number of experiments" << std::endl;
    }

    for(int i = 0; i<num_experiments; i++)
    {
        printTitleArray(false, "Single Memory Location - Array", array_lengths[i]);
        exportResultsArray2Core<std::atomic_int32_t>(false, profiler, cpu_a, cpu_b, results_vec[i], array_lengths[i], format, file, raw_file, false);
    }

    #if WRITE_CSV == 1
        fflush(file);
    #endif

    tableFooter();

    //==== Cleanup ====
    for(size_t i = 0; i < max_length; i++)
    {
        (shared_loc+i)->~atomic();
    }
    free(shared_loc);
    delete[] arg_a;
    delete[] arg_b;
    delete[] reset_arg;
}

void run_latency_single_array_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, int cpu_d, std::vector<size_t> array_lengths, FILE* file_a, FILE* file_b, std::ofstream* raw_file_a, std::ofstream* raw_file_b)
{
    //==== Print header ====
    std::string format = tableHeaderArray2Streams("Single Memory Location - Simultanious - Array", file_a, file_b, raw_file_a, raw_file_b, true);

    //==== Allocate Array For the Largest Experiment ====
    size_t max_length = *std::max_element(array_lengths.begin(), array_lengths.end());

    //Initialize
    size_t amountToAlloc = max_length*sizeof(std::atomic_int32_t);
    if(amountToAlloc % CACHE_LINE_SIZE != 0){
        amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
    }
    std::atomic_int32_t* shared_loc_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);
    std::atomic_int32_t* shared_loc_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_b);
    

    //Init to 0
    for(size_t i = 0; i < max_length; i++)
    {
        std::atomic_int32_t* shared_loc_1_constructed = new (shared_loc_1+i) std::atomic_int32_t();
        std::atomic_init(shared_loc_1+i, 0);
        if(!std::atomic_is_lock_free(shared_loc_1+i)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }

        std::atomic_int32_t* shared_loc_2_constructed = new (shared_loc_2+i) std::atomic_int32_t();
        std::atomic_init(shared_loc_2+i, 0);
        if(!std::atomic_is_lock_free(shared_loc_2+i)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }
    }

    //==== Create Configurations for each experiment ====
    int num_experiments = array_lengths.size();

    LatencySingleArrayKernelArgs* arg_a = new LatencySingleArrayKernelArgs[num_experiments];
    LatencySingleArrayKernelArgs* arg_b = new LatencySingleArrayKernelArgs[num_experiments];
    LatencySingleArrayKernelArgs* arg_c = new LatencySingleArrayKernelArgs[num_experiments];
    LatencySingleArrayKernelArgs* arg_d = new LatencySingleArrayKernelArgs[num_experiments];
    LatencySingleArrayKernelResetArgs* reset_arg_1 = new LatencySingleArrayKernelResetArgs[num_experiments];
    LatencySingleArrayKernelResetArgs* reset_arg_2 = new LatencySingleArrayKernelResetArgs[num_experiments];

    for(int i = 0; i<num_experiments; i++)
    {
        size_t array_length = array_lengths[i];
        
        arg_a[i].init_counter = -1; //(server)
        arg_a[i].shared_ptr = shared_loc_1;
        arg_a[i].length = array_length;

        arg_b[i].init_counter = 0; //(client)
        arg_b[i].shared_ptr = shared_loc_1;
        arg_b[i].length = array_length;

        arg_c[i].init_counter = -1; //(server)
        arg_c[i].shared_ptr = shared_loc_2;
        arg_c[i].length = array_length;

        arg_d[i].init_counter = 0; //(client)
        arg_d[i].shared_ptr = shared_loc_2;
        arg_d[i].length = array_length;

        reset_arg_1[i].shared_ptr = shared_loc_1;
        reset_arg_1[i].length = array_length;

        reset_arg_2[i].shared_ptr = shared_loc_2;
        reset_arg_2[i].length = array_length;
    }

    //==== Run The Experiments ====
    std::vector<SimultaniousResults> results_vec = execute_client_server_kernel(profiler, latency_single_array_kernel, latency_single_array_kernel, latency_single_array_kernel_reset, noCleanupFctn, arg_a, arg_b, arg_c, arg_d, reset_arg_1, reset_arg_2, cpu_a, cpu_b, cpu_c, cpu_d, num_experiments);

    //==== Process Results ====
    if(results_vec.size() != num_experiments){
        std::cerr << "Error: The number of results did not match the number of experiments" << std::endl;
    }

    for(int i = 0; i<num_experiments; i++)
    {
        printTitleArray(false, "Single Memory Location - Simultanious", array_lengths[i]);
        exportResultsArray4Core<std::atomic_int32_t>(false, profiler, cpu_a, cpu_b, cpu_c, cpu_d, results_vec[i], array_lengths[i], format, file_a, file_b, raw_file_a, raw_file_b, false);
    }

    #if WRITE_CSV == 1
        fflush(file_a);
        fflush(file_b);
    #endif

    tableFooter();

    //==== Cleanup ====
    for(size_t i = 0; i < max_length; i++)
    {
        (shared_loc_1+i)->~atomic();
        (shared_loc_2+i)->~atomic();
    }
    free(shared_loc_1);
    free(shared_loc_2);
    delete[] arg_a;
    delete[] arg_b;
    delete[] arg_c;
    delete[] arg_d;
    delete[] reset_arg_1;
    delete[] reset_arg_2;
}

/**
 * Note: This version tests fanin & fanout.  Since the single array kernel operates by
 * "ping pong-ing" between the client and server threads, each cycle contains a fan-out
 * and a fan-in.  The reported result (since it is reported as 1 way) averages the effect
 * of fanout and fanin
 */
void run_latency_single_array_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, std::vector<size_t> array_lengths, FILE* file_a, FILE* file_b, std::ofstream* raw_file_a, std::ofstream* raw_file_b)
{
    //==== Print header ====
    std::string format = tableHeaderArray2Streams("Single Memory Location - Fan-in/Fan-out - Array", file_a, file_b, raw_file_a, raw_file_b, true);

    //==== Allocate Array For the Largest Experiment ====
    size_t max_length = *std::max_element(array_lengths.begin(), array_lengths.end());

    //Initialize
    size_t amountToAlloc = max_length*sizeof(std::atomic_int32_t);
    if(amountToAlloc % CACHE_LINE_SIZE != 0){
        amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
    }
    std::atomic_int32_t* shared_loc_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);
    std::atomic_int32_t* shared_loc_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_b);

    //Init to 0
    for(size_t i = 0; i < max_length; i++)
    {
        std::atomic_int32_t* shared_loc_1_constructed = new (shared_loc_1+i) std::atomic_int32_t();
        std::atomic_init(shared_loc_1+i, 0);
        if(!std::atomic_is_lock_free(shared_loc_1+i)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }

        std::atomic_int32_t* shared_loc_2_constructed = new (shared_loc_2+i) std::atomic_int32_t();
        std::atomic_init(shared_loc_2+i, 0);
        if(!std::atomic_is_lock_free(shared_loc_2+i)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }
    }

    //==== Create Configurations for each experiment ====
    int num_experiments = array_lengths.size();

    LatencySingleArrayKernelArgs* arg_a = new LatencySingleArrayKernelArgs[num_experiments];
    LatencySingleArrayKernelArgs* arg_b = new LatencySingleArrayKernelArgs[num_experiments];
    LatencySingleArrayJoinKernelArgs* arg_c = new LatencySingleArrayJoinKernelArgs[num_experiments];
    LatencySingleArrayJoinKernelResetArgs* reset_arg_1 = new LatencySingleArrayJoinKernelResetArgs[num_experiments];

    for(int i = 0; i<num_experiments; i++)
    {
        size_t array_length = array_lengths[i];
        
        //Construct 2 servers and 1 client.  Servers will measure.
        //Both fan-in and fan-out is measured 
        arg_a[i].init_counter = -1; //(server)
        arg_a[i].shared_ptr = shared_loc_1;
        arg_a[i].length = array_length;

        arg_b[i].init_counter = -1; //(server)
        arg_b[i].shared_ptr = shared_loc_2;
        arg_b[i].length = array_length;

        arg_c[i].init_counter_a = 0; //(client)
        arg_c[i].init_counter_b = 0; //(client)
        arg_c[i].shared_ptr_a = shared_loc_1;
        arg_c[i].shared_ptr_b = shared_loc_2;
        arg_c[i].length_a = array_length;
        arg_c[i].length_b = array_length;

        reset_arg_1[i].shared_ptr_a = shared_loc_1;
        reset_arg_1[i].shared_ptr_b = shared_loc_2;
        reset_arg_1[i].length_a = array_length;
        reset_arg_1[i].length_b = array_length;
    }

    //==== Run The Experiments ====
    std::vector<SimultaniousResults> results_vec = execute_kernel_fanin_server_measure(profiler, latency_single_array_kernel, latency_single_array_join_kernel, latency_single_array_join_kernel_reset, noCleanupFctn, arg_a, arg_b, arg_c, reset_arg_1, cpu_a, cpu_b, cpu_c, num_experiments);

    //==== Process Results ====
    if(results_vec.size() != num_experiments){
        std::cerr << "Error: The number of results did not match the number of experiments" << std::endl;
    }

    for(int i = 0; i<num_experiments; i++)
    {
        printTitleArray(false, "Single Memory Location - Fan-in/Fan-out - Array", array_lengths[i]);
        exportResultsArray3CoreFanInFanOut<std::atomic_int32_t>(false, profiler, cpu_a, cpu_b, cpu_c, results_vec[i], array_lengths[i], format, file_a, file_b, raw_file_a, raw_file_b, false);
    }

    #if WRITE_CSV == 1
        fflush(file_a);
        fflush(file_b);
    #endif

    tableFooter();

    //==== Cleanup ====
    for(size_t i = 0; i < max_length; i++)
    {
        (shared_loc_1+i)->~atomic();
        (shared_loc_2+i)->~atomic();
    }
    free(shared_loc_1);
    free(shared_loc_2);
    delete[] arg_a;
    delete[] arg_b;
    delete[] arg_c;
    delete[] reset_arg_1;
}

//==== Dual array ====
void run_latency_dual_array_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, FILE* file, std::ofstream* raw_file)
{
    //==== Print header ====
    std::string format = tableHeaderArray1Stream("Dual Memory Location - Array", file, raw_file);

    //==== Allocate Array For the Largest Experiment ====
    size_t max_length = *std::max_element(array_lengths.begin(), array_lengths.end());

    //Initialize
    size_t amountToAlloc = max_length*sizeof(std::atomic_int32_t);
    if(amountToAlloc % CACHE_LINE_SIZE != 0){
        amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
    }
    std::atomic_int32_t* shared_loc_a = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);
    std::atomic_int32_t* shared_loc_b = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_b);

    //Init to 0
    for(size_t i = 0; i < max_length; i++)
    {
        std::atomic_int32_t* shared_loc_a_constructed = new (shared_loc_a+i) std::atomic_int32_t();
        std::atomic_init(shared_loc_a+i, 0);
        if(!std::atomic_is_lock_free(shared_loc_a+i)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }

        std::atomic_int32_t* shared_loc_b_constructed = new (shared_loc_b+i) std::atomic_int32_t();
        std::atomic_init(shared_loc_b+i, 0);
        if(!std::atomic_is_lock_free(shared_loc_b+i)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }
    }

    //==== Create Configurations for each experiment ====
    int num_experiments = array_lengths.size();

    LatencyDualArrayKernelArgs* arg_a = new LatencyDualArrayKernelArgs[num_experiments];
    LatencyDualArrayKernelArgs* arg_b = new LatencyDualArrayKernelArgs[num_experiments];
    LatencyDualArrayKernelResetArgs* reset_arg = new LatencyDualArrayKernelResetArgs[num_experiments];

    for(int i = 0; i<num_experiments; i++)
    {
        size_t array_length = array_lengths[i];
        
        arg_a[i].init_counter = -1; //(server)
        arg_a[i].my_shared_ptr = shared_loc_a;
        arg_a[i].other_shared_ptr = shared_loc_b;
        arg_a[i].length = array_length;

        arg_b[i].init_counter = 0; //(client)
        arg_b[i].my_shared_ptr = shared_loc_b; //Swapped from server
        arg_b[i].other_shared_ptr = shared_loc_a;
        arg_b[i].length = array_length;

        reset_arg[i].shared_ptr_a = shared_loc_a;
        reset_arg[i].shared_ptr_b = shared_loc_b;
        reset_arg[i].length = array_length;
    }

    //==== Run The Experiments ====
    std::vector<Results> results_vec = execute_client_server_kernel(profiler, latency_dual_array_kernel, latency_dual_array_kernel, latency_dual_array_kernel_reset, noCleanupFctn, arg_a, arg_b, reset_arg, cpu_a, cpu_b, num_experiments);

    //==== Process Results ====
    if(results_vec.size() != num_experiments){
        std::cerr << "Error: The number of results did not match the number of experiments" << std::endl;
    }

    for(int i = 0; i<num_experiments; i++)
    {
        printTitleArray(false, "Dual Memory Location - Array", array_lengths[i]);
        exportResultsArray2Core<std::atomic_int32_t>(false, profiler, cpu_a, cpu_b, results_vec[i], array_lengths[i], format, file, raw_file, false);
    }

    #if WRITE_CSV == 1
        fflush(file);
    #endif

    tableFooter();

    //==== Cleanup ====
    for(size_t i = 0; i < max_length; i++)
    {
        (shared_loc_a+i)->~atomic();
        (shared_loc_b+i)->~atomic();
    }
    free(shared_loc_a);
    free(shared_loc_b);
    delete[] arg_a;
    delete[] arg_b;
    delete[] reset_arg;
}

void run_latency_dual_array_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, int cpu_d, std::vector<size_t> array_lengths, FILE* file_a, FILE* file_b, std::ofstream* raw_file_a, std::ofstream* raw_file_b)
{
    //==== Print header ====
    std::string format = tableHeaderArray2Streams("Dual Memory Location - Simultanious - Array", file_a, file_b, raw_file_a, raw_file_b, true);

    //==== Allocate Array For the Largest Experiment ====
    size_t max_length = *std::max_element(array_lengths.begin(), array_lengths.end());

    //Initialize
    size_t amountToAlloc = max_length*sizeof(std::atomic_int32_t);
    if(amountToAlloc % CACHE_LINE_SIZE != 0){
        amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
    }
    std::atomic_int32_t* shared_loc_a_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);
    std::atomic_int32_t* shared_loc_b_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_b);

    std::atomic_int32_t* shared_loc_a_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_c);
    std::atomic_int32_t* shared_loc_b_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_d);

    //Init to 0
    for(size_t i = 0; i < max_length; i++)
    {
        std::atomic_int32_t* shared_loc_a_1_constructed = new (shared_loc_a_1+i) std::atomic_int32_t();
        std::atomic_init(shared_loc_a_1+i, 0);
        if(!std::atomic_is_lock_free(shared_loc_a_1+i)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }

        std::atomic_int32_t* shared_loc_b_1_constructed = new (shared_loc_b_1+i) std::atomic_int32_t();
        std::atomic_init(shared_loc_b_1+i, 0);
        if(!std::atomic_is_lock_free(shared_loc_b_1+i)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }

        std::atomic_int32_t* shared_loc_a_2_constructed = new (shared_loc_a_2+i) std::atomic_int32_t();
        std::atomic_init(shared_loc_a_2+i, 0);
        if(!std::atomic_is_lock_free(shared_loc_a_2+i)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }

        std::atomic_int32_t* shared_loc_b_2_constructed = new (shared_loc_b_2+i) std::atomic_int32_t();
        std::atomic_init(shared_loc_b_2+i, 0);
        if(!std::atomic_is_lock_free(shared_loc_b_2+i)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }
    }

    //==== Create Configurations for each experiment ====
    int num_experiments = array_lengths.size();

    LatencyDualArrayKernelArgs* arg_a = new LatencyDualArrayKernelArgs[num_experiments];
    LatencyDualArrayKernelArgs* arg_b = new LatencyDualArrayKernelArgs[num_experiments];
    LatencyDualArrayKernelArgs* arg_c = new LatencyDualArrayKernelArgs[num_experiments];
    LatencyDualArrayKernelArgs* arg_d = new LatencyDualArrayKernelArgs[num_experiments];
    LatencyDualArrayKernelResetArgs* reset_arg_1 = new LatencyDualArrayKernelResetArgs[num_experiments];
    LatencyDualArrayKernelResetArgs* reset_arg_2 = new LatencyDualArrayKernelResetArgs[num_experiments];

    for(int i = 0; i<num_experiments; i++)
    {
        size_t array_length = array_lengths[i];

        arg_a[i].init_counter = -1; //(server)
        arg_a[i].my_shared_ptr = shared_loc_a_1;
        arg_a[i].other_shared_ptr = shared_loc_b_1;
        arg_a[i].length = array_length;

        arg_b[i].init_counter = 0; //(client)
        arg_b[i].my_shared_ptr = shared_loc_b_1; //Swapped from server
        arg_b[i].other_shared_ptr = shared_loc_a_1;
        arg_b[i].length = array_length;

        arg_c[i].init_counter = -1; //(server)
        arg_c[i].my_shared_ptr = shared_loc_a_2;
        arg_c[i].other_shared_ptr = shared_loc_b_2;
        arg_c[i].length = array_length;

        arg_d[i].init_counter = 0; //(client)
        arg_d[i].my_shared_ptr = shared_loc_b_2; //Swapped from server
        arg_d[i].other_shared_ptr = shared_loc_a_2;
        arg_d[i].length = array_length;

        reset_arg_1[i].shared_ptr_a = shared_loc_a_1;
        reset_arg_1[i].shared_ptr_b = shared_loc_b_1;
        reset_arg_1[i].length = array_length;

        reset_arg_2[i].shared_ptr_a = shared_loc_a_2;
        reset_arg_2[i].shared_ptr_b = shared_loc_b_2;
        reset_arg_2[i].length = array_length;
    }

    //==== Run The Experiments ====
    std::vector<SimultaniousResults> results_vec = execute_client_server_kernel(profiler, latency_dual_array_kernel, latency_dual_array_kernel, latency_dual_array_kernel_reset, noCleanupFctn, arg_a, arg_b, arg_c, arg_d, reset_arg_1, reset_arg_2, cpu_a, cpu_b, cpu_c, cpu_d, num_experiments);

    //==== Process Results ====
    if(results_vec.size() != num_experiments){
        std::cerr << "Error: The number of results did not match the number of experiments" << std::endl;
    }

    for(int i = 0; i<num_experiments; i++)
    {
        printTitleArray(false, "Dual Memory Location - Simultanious - Array", array_lengths[i]);
        exportResultsArray4Core<std::atomic_int32_t>(false, profiler, cpu_a, cpu_b, cpu_c, cpu_d, results_vec[i], array_lengths[i], format, file_a, file_b, raw_file_a, raw_file_b, false);
    }

    #if WRITE_CSV == 1
        fflush(file_a);
        fflush(file_b);
    #endif

    tableFooter();

    //==== Cleanup ====
    for(size_t i = 0; i < max_length; i++)
    {
        (shared_loc_a_1+i)->~atomic();
        (shared_loc_b_1+i)->~atomic();
        (shared_loc_a_2+i)->~atomic();
        (shared_loc_b_2+i)->~atomic();
    }
    free(shared_loc_a_1);
    free(shared_loc_b_1);
    free(shared_loc_a_2);
    free(shared_loc_b_2);
    delete[] arg_a;
    delete[] arg_b;
    delete[] arg_c;
    delete[] arg_d;
    delete[] reset_arg_1;
    delete[] reset_arg_2;
}

void run_latency_dual_array_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, std::vector<size_t> array_lengths, FILE* file_a, FILE* file_b, std::ofstream* raw_file_a, std::ofstream* raw_file_b)
{
    //==== Print header ====
    std::string format = tableHeaderArray2Streams("Dual Memory Location - Fan-in/Fan-out - Array", file_a, file_b, raw_file_a, raw_file_b, true);

    //==== Allocate Array For the Largest Experiment ====
    size_t max_length = *std::max_element(array_lengths.begin(), array_lengths.end());

    //Initialize
    size_t amountToAlloc = max_length*sizeof(std::atomic_int32_t);
    if(amountToAlloc % CACHE_LINE_SIZE != 0){
        amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
    }
    std::atomic_int32_t* shared_loc_a_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);
    std::atomic_int32_t* shared_loc_b_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_c);
    

    std::atomic_int32_t* shared_loc_a_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_b);
    std::atomic_int32_t* shared_loc_b_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_c);

    //Init to 0
    for(size_t i = 0; i < max_length; i++)
    {
        std::atomic_int32_t* shared_loc_a_1_constructed = new (shared_loc_a_1+i) std::atomic_int32_t();
        std::atomic_init(shared_loc_a_1+i, 0);
        if(!std::atomic_is_lock_free(shared_loc_a_1+i)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }

        std::atomic_int32_t* shared_loc_b_1_constructed = new (shared_loc_b_1+i) std::atomic_int32_t();
        std::atomic_init(shared_loc_b_1+i, 0);
        if(!std::atomic_is_lock_free(shared_loc_b_1+i)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }

        std::atomic_int32_t* shared_loc_a_2_constructed = new (shared_loc_a_2+i) std::atomic_int32_t();
        std::atomic_init(shared_loc_a_2+i, 0);
        if(!std::atomic_is_lock_free(shared_loc_a_2+i)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }

        std::atomic_int32_t* shared_loc_b_2_constructed = new (shared_loc_b_2+i) std::atomic_int32_t();
        std::atomic_init(shared_loc_b_2+i, 0);
        if(!std::atomic_is_lock_free(shared_loc_b_2+i)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }
    }

    //==== Create Configurations for each experiment ====
    int num_experiments = array_lengths.size();

    LatencyDualArrayKernelArgs* arg_a = new LatencyDualArrayKernelArgs[num_experiments];
    LatencyDualArrayKernelArgs* arg_b = new LatencyDualArrayKernelArgs[num_experiments];
    LatencyDualArrayJoinKernelArgs* arg_c = new LatencyDualArrayJoinKernelArgs[num_experiments];
    LatencyDualArrayJoinKernelResetArgs* reset_arg_1 = new LatencyDualArrayJoinKernelResetArgs[num_experiments];
    
    for(int i = 0; i<num_experiments; i++)
    {
        size_t array_length = array_lengths[i];
        
        //2 Servers, 1 Client - Servers will measure
        //SERVERS
        arg_a[i].init_counter = -1; //(server)
        arg_a[i].my_shared_ptr = shared_loc_a_1;
        arg_a[i].other_shared_ptr = shared_loc_b_1;
        arg_a[i].length = array_length;

        arg_b[i].init_counter = -1; //(server)
        arg_b[i].my_shared_ptr = shared_loc_a_2; //Swapped from server
        arg_b[i].other_shared_ptr = shared_loc_b_2;
        arg_b[i].length = array_length;

        //CLIENT
        arg_c[i].init_counter_a = 0; //(client)
        arg_c[i].my_shared_ptr_a = shared_loc_b_1; //Swapped from server
        arg_c[i].other_shared_ptr_a = shared_loc_a_1;
        arg_c[i].length_a = array_length;
        arg_c[i].init_counter_b = 0; //(client)
        arg_c[i].my_shared_ptr_b = shared_loc_b_2; //Swapped from server
        arg_c[i].other_shared_ptr_b = shared_loc_a_2;
        arg_c[i].length_b = array_length;

        reset_arg_1[i].shared_ptr_a = shared_loc_a_1;
        reset_arg_1[i].shared_ptr_b = shared_loc_b_1;
        reset_arg_1[i].shared_ptr_c = shared_loc_a_2;
        reset_arg_1[i].shared_ptr_d = shared_loc_b_2;
        reset_arg_1[i].length_ab = array_length;
        reset_arg_1[i].length_cd = array_length;
    }

    //==== Run The Experiments ====
    std::vector<SimultaniousResults> results_vec = execute_kernel_fanin_server_measure(profiler, latency_dual_array_kernel, latency_dual_array_join_kernel, latency_dual_array_join_kernel_reset, noCleanupFctn, arg_a, arg_b, arg_c, reset_arg_1, cpu_a, cpu_b, cpu_c, num_experiments);

    //==== Process Results ====
    if(results_vec.size() != num_experiments){
        std::cerr << "Error: The number of results did not match the number of experiments" << std::endl;
    }

    for(int i = 0; i<num_experiments; i++)
    {
        printTitleArray(false, "Dual Memory Location - Fan-in/Fan-out - Array", array_lengths[i]);
        exportResultsArray3CoreFanInFanOut<std::atomic_int32_t>(false, profiler, cpu_a, cpu_b, cpu_c, results_vec[i], array_lengths[i], format, file_a, file_b, raw_file_a, raw_file_b, false);
    }

    #if WRITE_CSV == 1
        fflush(file_a);
        fflush(file_b);
    #endif

    tableFooter();

    //==== Cleanup ====
    for(size_t i = 0; i < max_length; i++)
    {
        (shared_loc_a_1+i)->~atomic();
        (shared_loc_b_1+i)->~atomic();
        (shared_loc_a_2+i)->~atomic();
        (shared_loc_b_2+i)->~atomic();
    }
    free(shared_loc_a_1);
    free(shared_loc_b_1);
    free(shared_loc_a_2);
    free(shared_loc_b_2);
    delete[] arg_a;
    delete[] arg_b;
    delete[] arg_c;
    delete[] reset_arg_1;
}

void run_latency_flow_ctrl_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, FILE* file, std::ofstream* raw_file)
{
    //==== Print header ====
    std::string format = tableHeaderArray1Stream("Flow Control - Array", file, raw_file);

    //==== Allocate Array For the Largest Experiment ====
    size_t max_length = *std::max_element(array_lengths.begin(), array_lengths.end());

    //Initialize
    size_t amountToAlloc = max_length*sizeof(std::atomic_int32_t);
    if(amountToAlloc % CACHE_LINE_SIZE != 0){
        amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
    }
    std::atomic_int32_t* shared_array_loc = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);

    size_t amountToAllocCursors = sizeof(std::atomic_int32_t);
    if(amountToAllocCursors % CACHE_LINE_SIZE != 0){
        amountToAllocCursors += (CACHE_LINE_SIZE - (amountToAllocCursors % CACHE_LINE_SIZE));
    }
    std::atomic_int32_t* shared_ack_loc = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_b);
    std::atomic_int32_t* shared_ack_loc_constructed = new (shared_ack_loc) std::atomic_int32_t();
    std::atomic_init(shared_ack_loc, 0);
    if(!std::atomic_is_lock_free(shared_ack_loc)){
        printf("Atomic is not lock free and was expected to be");
        exit(1);
    }

    //Init to 0
    for(size_t i = 0; i < max_length; i++)
    {
        std::atomic_int32_t* shared_array_loc_constructed = new (shared_array_loc+i) std::atomic_int32_t();
        std::atomic_init(shared_array_loc+i, 0);
        if(!std::atomic_is_lock_free(shared_array_loc+i)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }
    }

    //==== Create Configurations for each experiment ====
    int num_experiments = array_lengths.size();

    LatencyFlowCtrlKernelArgs* args = new LatencyFlowCtrlKernelArgs[num_experiments];

    for(int i = 0; i<num_experiments; i++)
    {
        size_t array_length = array_lengths[i];

        args[i].array_shared_ptr = shared_array_loc;
        args[i].ack_shared_ptr = shared_ack_loc;
        args[i].length = array_length;
    }

    //==== Run The Experiments ====
    std::vector<Results> results_vec = execute_client_server_kernel(profiler, latency_flow_ctrl_server_kernel, latency_flow_ctrl_client_kernel, latency_flow_ctrl_kernel_reset, noCleanupFctn, args, args, args, cpu_a, cpu_b, num_experiments);

    //==== Process Results ====
    if(results_vec.size() != num_experiments){
        std::cerr << "Error: The number of results did not match the number of experiments" << std::endl;
    }

    for(int i = 0; i<num_experiments; i++)
    {
        printTitleArray(false, "Flow Control - Array", array_lengths[i]);
        exportResultsArray2Core<std::atomic_int32_t>(false, profiler, cpu_a, cpu_b, results_vec[i], array_lengths[i], format, file, raw_file, true);
    }

    #if WRITE_CSV == 1
        fflush(file);
    #endif

    tableFooter();

    //==== Cleanup ====
    shared_ack_loc_constructed->~atomic();
    for(size_t i = 0; i < max_length; i++)
    {
        (shared_array_loc+i)->~atomic();
    }
    free(shared_array_loc);
    free(shared_ack_loc);
    delete[] args;
}

void run_latency_flow_ctrl_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, int cpu_d, std::vector<size_t> array_lengths, FILE* file_a, FILE* file_b, std::ofstream* raw_file_a, std::ofstream* raw_file_b)
{
    //==== Print header ====
    std::string format = tableHeaderArray2Streams("Flow Control - Simultanious - Array", file_a, file_b, raw_file_a, raw_file_b, true);

    //==== Allocate Array For the Largest Experiment ====
    size_t max_length = *std::max_element(array_lengths.begin(), array_lengths.end());

    //Initialize
    size_t amountToAlloc = max_length*sizeof(std::atomic_int32_t);
    if(amountToAlloc % CACHE_LINE_SIZE != 0){
        amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
    }
    std::atomic_int32_t* shared_array_loc_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);
    std::atomic_int32_t* shared_array_loc_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_c);

    size_t amountToAllocCursors = sizeof(std::atomic_int32_t);
    if(amountToAllocCursors % CACHE_LINE_SIZE != 0){
        amountToAllocCursors += (CACHE_LINE_SIZE - (amountToAllocCursors % CACHE_LINE_SIZE));
    }
    std::atomic_int32_t* shared_ack_loc_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_b);
    std::atomic_int32_t* shared_ack_loc_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_d);
    std::atomic_int32_t* shared_ack_loc_1_constructed = new (shared_ack_loc_1) std::atomic_int32_t();
    std::atomic_init(shared_ack_loc_1, 0);
    if(!std::atomic_is_lock_free(shared_ack_loc_1)){
        printf("Atomic is not lock free and was expected to be");
        exit(1);
    }
    std::atomic_int32_t* shared_ack_loc_2_constructed = new (shared_ack_loc_2) std::atomic_int32_t();
    std::atomic_init(shared_ack_loc_2, 0);
    if(!std::atomic_is_lock_free(shared_ack_loc_2)){
        printf("Atomic is not lock free and was expected to be");
        exit(1);
    }

    //Init to 0
    for(size_t i = 0; i < max_length; i++)
    {
        std::atomic_int32_t* shared_array_loc_1_constructed = new (shared_array_loc_1+i) std::atomic_int32_t();
        std::atomic_init(shared_array_loc_1+i, 0);
        if(!std::atomic_is_lock_free(shared_array_loc_1+i)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }
        std::atomic_int32_t* shared_array_loc_2_constructed = new (shared_array_loc_2+i) std::atomic_int32_t();
        std::atomic_init(shared_array_loc_2+i, 0);
        if(!std::atomic_is_lock_free(shared_array_loc_2+i)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }
    }

    //==== Create Configurations for each experiment ====
    int num_experiments = array_lengths.size();

    LatencyFlowCtrlKernelArgs* args_1 = new LatencyFlowCtrlKernelArgs[num_experiments];
    LatencyFlowCtrlKernelArgs* args_2 = new LatencyFlowCtrlKernelArgs[num_experiments];

    for(int i = 0; i<num_experiments; i++)
    {
        size_t array_length = array_lengths[i];

        args_1[i].array_shared_ptr = shared_array_loc_1;
        args_1[i].ack_shared_ptr = shared_ack_loc_1;
        args_1[i].length = array_length;

        args_2[i].array_shared_ptr = shared_array_loc_2;
        args_2[i].ack_shared_ptr = shared_ack_loc_2;
        args_2[i].length = array_length;
    }

    //==== Run The Experiments ====
    std::vector<SimultaniousResults> results_vec = execute_client_server_kernel(profiler, latency_flow_ctrl_server_kernel, latency_flow_ctrl_client_kernel, latency_flow_ctrl_kernel_reset, noCleanupFctn, args_1, args_1, args_2, args_2, args_1, args_2, cpu_a, cpu_b, cpu_c, cpu_d, num_experiments);

    //==== Process Results ====
    if(results_vec.size() != num_experiments){
        std::cerr << "Error: The number of results did not match the number of experiments" << std::endl;
    }

    for(int i = 0; i<num_experiments; i++)
    {
        printTitleArray(false, "Flow Control - Simultanious - Array", array_lengths[i]);
        exportResultsArray4Core<std::atomic_int32_t>(false, profiler, cpu_a, cpu_b, cpu_c, cpu_d, results_vec[i], array_lengths[i], format, file_a, file_b, raw_file_a, raw_file_b, true);
    }

    #if WRITE_CSV == 1
        fflush(file_a);
        fflush(file_b);
    #endif

    tableFooter();

    //==== Cleanup ====
    shared_ack_loc_1->~atomic();
    shared_ack_loc_2->~atomic();
    for(size_t i = 0; i < max_length; i++)
    {
        (shared_array_loc_1+i)->~atomic();
        (shared_array_loc_2+i)->~atomic();
    }
    free(shared_array_loc_1);
    free(shared_array_loc_2);
    free(shared_ack_loc_1);
    free(shared_ack_loc_2);
    delete[] args_1;
    delete[] args_2;
}

/**
 * This version tests for fan-in when the comunication mechanism uses the flow control model.
 * 2 servers feed to 1 client.  The servers conduct the measurment
 */
void run_latency_flow_ctrl_fanin_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, std::vector<size_t> array_lengths, FILE* file_a, FILE* file_b, std::ofstream* raw_file_a, std::ofstream* raw_file_b)
{
    //==== Print header ====
    std::string format = tableHeaderArray2Streams("Flow Control - Fan-in - Array", file_a, file_b, raw_file_a, raw_file_b, false);

    //==== Allocate Array For the Largest Experiment ====
    size_t max_length = *std::max_element(array_lengths.begin(), array_lengths.end());

    //Initialize
    size_t amountToAlloc = max_length*sizeof(std::atomic_int32_t);
    if(amountToAlloc % CACHE_LINE_SIZE != 0){
        amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
    }
    std::atomic_int32_t* shared_array_loc_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);
    std::atomic_int32_t* shared_array_loc_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_b);

    size_t amountToAllocCursors = sizeof(std::atomic_int32_t);
    if(amountToAllocCursors % CACHE_LINE_SIZE != 0){
        amountToAllocCursors += (CACHE_LINE_SIZE - (amountToAllocCursors % CACHE_LINE_SIZE));
    }
    std::atomic_int32_t* shared_ack_loc_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_c);
    std::atomic_int32_t* shared_ack_loc_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_c);
    std::atomic_int32_t* shared_ack_loc_1_constructed = new (shared_ack_loc_1) std::atomic_int32_t();
    std::atomic_init(shared_ack_loc_1, 0);
    if(!std::atomic_is_lock_free(shared_ack_loc_1)){
        printf("Atomic is not lock free and was expected to be");
        exit(1);
    }
    std::atomic_int32_t* shared_ack_loc_2_constructed = new (shared_ack_loc_2) std::atomic_int32_t();
    std::atomic_init(shared_ack_loc_2, 0);
    if(!std::atomic_is_lock_free(shared_ack_loc_2)){
        printf("Atomic is not lock free and was expected to be");
        exit(1);
    }

    //Init to 0
    for(size_t i = 0; i < max_length; i++)
    {
        std::atomic_int32_t* shared_array_loc_1_constructed = new (shared_array_loc_1+i) std::atomic_int32_t();
        std::atomic_init(shared_array_loc_1+i, 0);
        if(!std::atomic_is_lock_free(shared_array_loc_1+i)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }
        std::atomic_int32_t* shared_array_loc_2_constructed = new (shared_array_loc_2+i) std::atomic_int32_t();
        std::atomic_init(shared_array_loc_2+i, 0);
        if(!std::atomic_is_lock_free(shared_array_loc_2+i)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }
    }

    //==== Create Configurations for each experiment ====
    int num_experiments = array_lengths.size();
    
    LatencyFlowCtrlKernelArgs* args_a = new LatencyFlowCtrlKernelArgs[num_experiments];
    LatencyFlowCtrlKernelArgs* args_b = new LatencyFlowCtrlKernelArgs[num_experiments];
    LatencyFlowCtrlJoinKernelArgs* args_c = new LatencyFlowCtrlJoinKernelArgs[num_experiments];

    for(int i = 0; i<num_experiments; i++)
    {
        size_t array_length = array_lengths[i];
        
        //2 Servers
        args_a[i].array_shared_ptr = shared_array_loc_1;
        args_a[i].ack_shared_ptr = shared_ack_loc_1;
        args_a[i].length = array_length;

        args_b[i].array_shared_ptr = shared_array_loc_2;
        args_b[i].ack_shared_ptr = shared_ack_loc_2;
        args_b[i].length = array_length;

        //1 Client
        args_c[i].array_shared_ptr_a = shared_array_loc_1;
        args_c[i].ack_shared_ptr_a = shared_ack_loc_1;
        args_c[i].length_a = array_length;

        args_c[i].array_shared_ptr_b = shared_array_loc_2;
        args_c[i].ack_shared_ptr_b = shared_ack_loc_2;
        args_c[i].length_b = array_length;
    }

    //==== Run The Experiments ====
    std::vector<SimultaniousResults> results_vec = execute_kernel_fanin_server_measure(profiler, latency_flow_ctrl_server_kernel, latency_flow_ctrl_client_join_kernel, latency_flow_ctrl_join_kernel_reset, noCleanupFctn, args_a, args_b, args_c, args_c, cpu_a, cpu_b, cpu_c, num_experiments); //Reset args are same as client join args

    //==== Process Results ====
    if(results_vec.size() != num_experiments){
        std::cerr << "Error: The number of results did not match the number of experiments" << std::endl;
    }

    for(int i = 0; i<num_experiments; i++)
    {
        printTitleArray(false, "Flow Control - Fan-in - Array", array_lengths[i]);
        exportResultsArray3CoreFanInFanOut<std::atomic_int32_t>(false, profiler, cpu_a, cpu_b, cpu_c, results_vec[i], array_lengths[i], format, file_a, file_b, raw_file_a, raw_file_b, true);
    }

    #if WRITE_CSV == 1
        fflush(file_a);
        fflush(file_b);
    #endif

    tableFooter();

    //==== Cleanup ====
    shared_ack_loc_1->~atomic();
    shared_ack_loc_2->~atomic();
    for(size_t i = 0; i < max_length; i++)
    {
        (shared_array_loc_1+i)->~atomic();
        (shared_array_loc_2+i)->~atomic();
    }
    free(shared_array_loc_1);
    free(shared_array_loc_2);
    free(shared_ack_loc_1);
    free(shared_ack_loc_2);
    delete[] args_a;
    delete[] args_b;
    delete[] args_c;
}

/**
 * This version tests for fan-out when the comunication mechanism uses the flow control model.
 * 1 server feed to 2 clients.  The clients conduct the measurment
 * 
 * TODO: Small inaccuracy may occur on the first transaction due to servers starting first
 */
void run_latency_flow_ctrl_fanout_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, std::vector<size_t> array_lengths, FILE* file_a, FILE* file_b, std::ofstream* raw_file_a, std::ofstream* raw_file_b)
{
    //==== Print header ====
    std::string format = tableHeaderArray2Streams("Flow Control - Fan-out - Array", file_a, file_b, raw_file_a, raw_file_b, false);

    //==== Allocate Array For the Largest Experiment ====
    size_t max_length = *std::max_element(array_lengths.begin(), array_lengths.end());

    //Initialize
    size_t amountToAlloc = max_length*sizeof(std::atomic_int32_t);
    if(amountToAlloc % CACHE_LINE_SIZE != 0){
        amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
    }
    std::atomic_int32_t* shared_array_loc_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);
    std::atomic_int32_t* shared_array_loc_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);

    size_t amountToAllocCursors = sizeof(std::atomic_int32_t);
    if(amountToAllocCursors % CACHE_LINE_SIZE != 0){
        amountToAllocCursors += (CACHE_LINE_SIZE - (amountToAllocCursors % CACHE_LINE_SIZE));
    }
    std::atomic_int32_t* shared_ack_loc_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_b);
    std::atomic_int32_t* shared_ack_loc_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_c);
    std::atomic_int32_t* shared_ack_loc_1_constructed = new (shared_ack_loc_1) std::atomic_int32_t();
    std::atomic_init(shared_ack_loc_1, 0);
    if(!std::atomic_is_lock_free(shared_ack_loc_1)){
        printf("Atomic is not lock free and was expected to be");
        exit(1);
    }
    std::atomic_int32_t* shared_ack_loc_2_constructed = new (shared_ack_loc_2) std::atomic_int32_t();
    std::atomic_init(shared_ack_loc_2, 0);
    if(!std::atomic_is_lock_free(shared_ack_loc_2)){
        printf("Atomic is not lock free and was expected to be");
        exit(1);
    }

    //Init to 0
    for(size_t i = 0; i < max_length; i++)
    {
        std::atomic_int32_t* shared_array_loc_1_constructed = new (shared_array_loc_1+i) std::atomic_int32_t();
        std::atomic_init(shared_array_loc_1+i, 0);
        if(!std::atomic_is_lock_free(shared_array_loc_1+i)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }
        std::atomic_int32_t* shared_array_loc_2_constructed = new (shared_array_loc_2+i) std::atomic_int32_t();
        std::atomic_init(shared_array_loc_2+i, 0);
        if(!std::atomic_is_lock_free(shared_array_loc_2+i)){
            printf("Atomic is not lock free and was expected to be");
            exit(1);
        }
    }

    //==== Create Configurations for each experiment ====
    int num_experiments = array_lengths.size();

    LatencyFlowCtrlJoinKernelArgs* args_a = new LatencyFlowCtrlJoinKernelArgs[num_experiments];
    LatencyFlowCtrlKernelArgs* args_b = new LatencyFlowCtrlKernelArgs[num_experiments];
    LatencyFlowCtrlKernelArgs* args_c = new LatencyFlowCtrlKernelArgs[num_experiments];

    for(int i = 0; i<num_experiments; i++)
    {
        size_t array_length = array_lengths[i];
        
        //1 Server
        args_a[i].array_shared_ptr_a = shared_array_loc_1;
        args_a[i].ack_shared_ptr_a = shared_ack_loc_1;
        args_a[i].length_a = array_length;

        args_a[i].array_shared_ptr_b = shared_array_loc_2;
        args_a[i].ack_shared_ptr_b = shared_ack_loc_2;
        args_a[i].length_b = array_length;

        //2 Clients
        args_b[i].array_shared_ptr = shared_array_loc_1;
        args_b[i].ack_shared_ptr = shared_ack_loc_1;
        args_b[i].length = array_length;
        
        args_c[i].array_shared_ptr = shared_array_loc_2;
        args_c[i].ack_shared_ptr = shared_ack_loc_2;
        args_c[i].length = array_length;
    }

    //==== Run The Experiments ====
    std::vector<SimultaniousResults> results_vec = execute_kernel_fanout_client_measure(profiler, latency_flow_ctrl_server_join_kernel, latency_flow_ctrl_client_kernel, latency_flow_ctrl_join_kernel_reset, noCleanupFctn, args_a, args_b, args_c, args_a, cpu_a, cpu_b, cpu_c, num_experiments); //Reset args are same as server join args

    //==== Process Results ====
    if(results_vec.size() != num_experiments){
        std::cerr << "Error: The number of results did not match the number of experiments" << std::endl;
    }

    for(int i = 0; i<num_experiments; i++)
    {
        printTitleArray(false, "Flow Control - Fan-out - Array", array_lengths[i]);
        exportResultsArray3CoreFanOut<std::atomic_int32_t>(false, profiler, cpu_a, cpu_b, cpu_c, results_vec[i], array_lengths[i], format, file_a, file_b, raw_file_a, raw_file_b, true);
    }

    #if WRITE_CSV == 1
            fflush(file_a);
            fflush(file_b);
    #endif

    tableFooter();

    //==== Cleanup ====
    shared_ack_loc_1->~atomic();
    shared_ack_loc_2->~atomic();
    for(size_t i = 0; i < max_length; i++)
    {
        (shared_array_loc_1+i)->~atomic();
        (shared_array_loc_2+i)->~atomic();
    }
    free(shared_array_loc_1);
    free(shared_array_loc_2);
    free(shared_ack_loc_1);
    free(shared_ack_loc_2);
    delete[] args_a;
    delete[] args_b;
    delete[] args_c;
}

void run_latency_flow_ctrl_blocked_read_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, FILE* file, std::ofstream* raw_file)
{
    //==== Print header ====
    std::string format = tableHeaderArray1Stream("Flow Control Blocked Read - Array", file, raw_file);

    //==== Allocate Array For the Largest Experiment ====
    size_t max_length = *std::max_element(array_lengths.begin(), array_lengths.end());

    //Initialize
    size_t amountToAlloc = max_length*sizeof(int32_t);
    if(amountToAlloc % CACHE_LINE_SIZE != 0){
        amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
    }
    int32_t* shared_array_loc = (int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);

    size_t amountToAllocCursors = sizeof(std::atomic_int32_t);
    if(amountToAllocCursors % CACHE_LINE_SIZE != 0){
        amountToAllocCursors += (CACHE_LINE_SIZE - (amountToAllocCursors % CACHE_LINE_SIZE));
    }
    std::atomic_int32_t* shared_ack_loc = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_b);
    std::atomic_int32_t* shared_valid_loc = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_a);
    std::atomic_int32_t* shared_ack_loc_constructed = new (shared_ack_loc) std::atomic_int32_t();
    std::atomic_init(shared_ack_loc, 0);
    if(!std::atomic_is_lock_free(shared_ack_loc)){
        printf("Atomic is not lock free and was expected to be");
        exit(1);
    }
    std::atomic_int32_t* shared_valid_loc_constructed = new (shared_valid_loc) std::atomic_int32_t();
    std::atomic_init(shared_valid_loc, 0);
    if(!std::atomic_is_lock_free(shared_valid_loc)){
        printf("Atomic is not lock free and was expected to be");
        exit(1);
    }

    //Init to 0
    for(size_t i = 0; i < max_length; i++)
    {
        shared_array_loc[i] = 0;
    }

    //==== Create Configurations for each experiment ====
    int num_experiments = array_lengths.size();

    LatencyFlowCtrlBlockedReadKernelArgs* args = new LatencyFlowCtrlBlockedReadKernelArgs[num_experiments];

    for(int i = 0; i<num_experiments; i++)
    {
        size_t array_length = array_lengths[i];

        args[i].array_shared_ptr = shared_array_loc;
        args[i].ack_shared_ptr = shared_ack_loc;
        args[i].valid_shared_ptr = shared_valid_loc;
        args[i].length = array_length;
    }

    //==== Run The Experiments ====
    std::vector<Results> results_vec = execute_client_server_kernel(profiler, latency_flow_ctrl_blocked_read_server_kernel, latency_flow_ctrl_blocked_read_client_kernel, latency_flow_ctrl_blocked_read_kernel_reset, noCleanupFctn, args, args, args, cpu_a, cpu_b, num_experiments);

    //==== Process Results ====
    if(results_vec.size() != num_experiments){
        std::cerr << "Error: The number of results did not match the number of experiments" << std::endl;
    }

    for(int i = 0; i<num_experiments; i++)
    {
        printTitleArray(false, "Flow Control Blocked Read - Array", array_lengths[i]);
        exportResultsArray2Core<int32_t>(false, profiler, cpu_a, cpu_b, results_vec[i], array_lengths[i], format, file, raw_file, true);
    }

    #if WRITE_CSV == 1
        fflush(file);
    #endif

    tableFooter();

    //==== Cleanup ====
    shared_ack_loc->~atomic();
    shared_valid_loc->~atomic();
    free(shared_array_loc);
    free(shared_ack_loc);
    free(shared_valid_loc);
    delete[] args;
}

void run_latency_flow_ctrl_blocked_read_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, int cpu_d, std::vector<size_t> array_lengths, FILE* file_a, FILE* file_b, std::ofstream* raw_file_a, std::ofstream* raw_file_b)
{
    //==== Print header ====
    std::string format = tableHeaderArray2Streams("Flow Control Blocked Read - Simultanious - Array", file_a, file_b, raw_file_a, raw_file_b, true);

    //==== Allocate Array For the Largest Experiment ====
    size_t max_length = *std::max_element(array_lengths.begin(), array_lengths.end());

    //Initialize
    size_t amountToAlloc = max_length*sizeof(int32_t);
    if(amountToAlloc % CACHE_LINE_SIZE != 0){
        amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
    }
    int32_t* shared_array_loc_1 = (int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);
    int32_t* shared_array_loc_2 = (int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_c);

    size_t amountToAllocCursors = sizeof(std::atomic_int32_t);
    if(amountToAllocCursors % CACHE_LINE_SIZE != 0){
        amountToAllocCursors += (CACHE_LINE_SIZE - (amountToAllocCursors % CACHE_LINE_SIZE));
    }
    std::atomic_int32_t* shared_ack_loc_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_b);
    std::atomic_int32_t* shared_valid_loc_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_a);
    std::atomic_int32_t* shared_ack_loc_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_d);
    std::atomic_int32_t* shared_valid_loc_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_c);
    std::atomic_int32_t* shared_ack_loc_1_constructed = new (shared_ack_loc_1) std::atomic_int32_t();
    std::atomic_init(shared_ack_loc_1, 0);
    if(!std::atomic_is_lock_free(shared_ack_loc_1)){
        printf("Atomic is not lock free and was expected to be");
        exit(1);
    }
    std::atomic_int32_t* shared_valid_loc_1_constructed = new (shared_valid_loc_1) std::atomic_int32_t();
    std::atomic_init(shared_valid_loc_1, 0);
    if(!std::atomic_is_lock_free(shared_valid_loc_1)){
        printf("Atomic is not lock free and was expected to be");
        exit(1);
    }
    std::atomic_int32_t* shared_ack_loc_2_constructed = new (shared_ack_loc_2) std::atomic_int32_t();
    std::atomic_init(shared_ack_loc_2, 0);
    if(!std::atomic_is_lock_free(shared_ack_loc_2)){
        printf("Atomic is not lock free and was expected to be");
        exit(1);
    }
    std::atomic_int32_t* shared_valid_loc_2_constructed = new (shared_valid_loc_2) std::atomic_int32_t();
    std::atomic_init(shared_valid_loc_2, 0);
    if(!std::atomic_is_lock_free(shared_valid_loc_2)){
        printf("Atomic is not lock free and was expected to be");
        exit(1);
    }

    //Init to 0
    for(size_t i = 0; i < max_length; i++)
    {
        shared_array_loc_1[i] = 0;
        shared_array_loc_2[i] = 0;
    }

    //==== Create Configurations for each experiment ====
    int num_experiments = array_lengths.size();

    LatencyFlowCtrlBlockedReadKernelArgs* args_1 = new LatencyFlowCtrlBlockedReadKernelArgs[num_experiments];
    LatencyFlowCtrlBlockedReadKernelArgs* args_2 = new LatencyFlowCtrlBlockedReadKernelArgs[num_experiments];

    for(int i = 0; i<num_experiments; i++)
    {
        size_t array_length = array_lengths[i];

        args_1[i].array_shared_ptr = shared_array_loc_1;
        args_1[i].ack_shared_ptr = shared_ack_loc_1;
        args_1[i].valid_shared_ptr = shared_valid_loc_1;
        args_1[i].length = array_length;

        args_2[i].array_shared_ptr = shared_array_loc_2;
        args_2[i].ack_shared_ptr = shared_ack_loc_2;
        args_2[i].valid_shared_ptr = shared_valid_loc_2;
        args_2[i].length = array_length;
    }

    //==== Run The Experiments ====
    std::vector<SimultaniousResults> results_vec = execute_client_server_kernel(profiler, latency_flow_ctrl_blocked_read_server_kernel, latency_flow_ctrl_blocked_read_client_kernel, latency_flow_ctrl_blocked_read_kernel_reset, noCleanupFctn, args_1, args_1, args_2, args_2, args_1, args_2, cpu_a, cpu_b, cpu_c, cpu_d, num_experiments);

    //==== Process Results ====
    if(results_vec.size() != num_experiments){
        std::cerr << "Error: The number of results did not match the number of experiments" << std::endl;
    }

    for(int i = 0; i<num_experiments; i++)
    {
        printTitleArray(false, "Flow Control Blocked Read - Simultanious - Array", array_lengths[i]);
        exportResultsArray4Core<int32_t>(false, profiler, cpu_a, cpu_b, cpu_c, cpu_d, results_vec[i], array_lengths[i], format, file_a, file_b, raw_file_a, raw_file_b, true);
    }

    #if WRITE_CSV == 1
        fflush(file_a);
        fflush(file_b);
    #endif

    tableFooter();

    //==== Cleanup ====
    shared_ack_loc_1->~atomic();
    shared_valid_loc_1->~atomic();
    shared_ack_loc_2->~atomic();
    shared_valid_loc_2->~atomic();
    free(shared_array_loc_1);
    free(shared_array_loc_2);
    free(shared_ack_loc_1);
    free(shared_valid_loc_1);
    free(shared_ack_loc_2);
    free(shared_valid_loc_2);
    delete[] args_1;
    delete[] args_2;
}

void run_latency_flow_ctrl_blocked_read_fanin_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, std::vector<size_t> array_lengths, FILE* file_a, FILE* file_b, std::ofstream* raw_file_a, std::ofstream* raw_file_b)
{
    //==== Print header ====
    std::string format = tableHeaderArray2Streams("Flow Control Blocked Read - Fan-in - Array", file_a, file_b, raw_file_a, raw_file_b, false);

    //==== Allocate Array For the Largest Experiment ====
    size_t max_length = *std::max_element(array_lengths.begin(), array_lengths.end());

    //Initialize
    size_t amountToAlloc = max_length*sizeof(int32_t);
    if(amountToAlloc % CACHE_LINE_SIZE != 0){
        amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
    }
    int32_t* shared_array_loc_1 = (int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);
    int32_t* shared_array_loc_2 = (int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_b);

    size_t amountToAllocCursors = sizeof(std::atomic_int32_t);
    if(amountToAllocCursors % CACHE_LINE_SIZE != 0){
        amountToAllocCursors += (CACHE_LINE_SIZE - (amountToAllocCursors % CACHE_LINE_SIZE));
    }
    std::atomic_int32_t* shared_ack_loc_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_c);
    std::atomic_int32_t* shared_valid_loc_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_a);
    std::atomic_int32_t* shared_ack_loc_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_c);
    std::atomic_int32_t* shared_valid_loc_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_b);
    std::atomic_int32_t* shared_ack_loc_1_constructed = new (shared_ack_loc_1) std::atomic_int32_t();
    std::atomic_init(shared_ack_loc_1, 0);
    if(!std::atomic_is_lock_free(shared_ack_loc_1)){
        printf("Atomic is not lock free and was expected to be");
        exit(1);
    }
    std::atomic_int32_t* shared_valid_loc_1_constructed = new (shared_valid_loc_1) std::atomic_int32_t();
    std::atomic_init(shared_valid_loc_1, 0);
    if(!std::atomic_is_lock_free(shared_valid_loc_1)){
        printf("Atomic is not lock free and was expected to be");
        exit(1);
    }
    std::atomic_int32_t* shared_ack_loc_2_constructed = new (shared_ack_loc_2) std::atomic_int32_t();
    std::atomic_init(shared_ack_loc_2, 0);
    if(!std::atomic_is_lock_free(shared_ack_loc_2)){
        printf("Atomic is not lock free and was expected to be");
        exit(1);
    }
    std::atomic_int32_t* shared_valid_loc_2_constructed = new (shared_valid_loc_2) std::atomic_int32_t();
    std::atomic_init(shared_valid_loc_2, 0);
    if(!std::atomic_is_lock_free(shared_valid_loc_2)){
        printf("Atomic is not lock free and was expected to be");
        exit(1);
    }

    //Init to 0
    for(size_t i = 0; i < max_length; i++)
    {
        shared_array_loc_1[i] = 0;
        shared_array_loc_2[i] = 0;
    }

    //==== Create Configurations for each experiment ====
    int num_experiments = array_lengths.size();

    LatencyFlowCtrlBlockedReadKernelArgs* srv_args_1 = new LatencyFlowCtrlBlockedReadKernelArgs[num_experiments];
    LatencyFlowCtrlBlockedReadKernelArgs* srv_args_2 = new LatencyFlowCtrlBlockedReadKernelArgs[num_experiments];
    LatencyFlowCtrlBlockedReadJoinKernelArgs* cli_args_1 = new LatencyFlowCtrlBlockedReadJoinKernelArgs[num_experiments];

    for(int i = 0; i<num_experiments; i++)
    {
        size_t array_length = array_lengths[i];

        //Server Args
        srv_args_1[i].array_shared_ptr = shared_array_loc_1;
        srv_args_1[i].ack_shared_ptr = shared_ack_loc_1;
        srv_args_1[i].valid_shared_ptr = shared_valid_loc_1;
        srv_args_1[i].length = array_length;
        
        srv_args_2[i].array_shared_ptr = shared_array_loc_2;
        srv_args_2[i].ack_shared_ptr = shared_ack_loc_2;
        srv_args_2[i].valid_shared_ptr = shared_valid_loc_2;
        srv_args_2[i].length = array_length;

        //Client Arg
        cli_args_1[i].array_shared_ptr_a = shared_array_loc_1;
        cli_args_1[i].ack_shared_ptr_a = shared_ack_loc_1;
        cli_args_1[i].valid_shared_ptr_a = shared_valid_loc_1;
        cli_args_1[i].length_a = array_length;
        cli_args_1[i].array_shared_ptr_b = shared_array_loc_2;
        cli_args_1[i].ack_shared_ptr_b = shared_ack_loc_2;
        cli_args_1[i].valid_shared_ptr_b = shared_valid_loc_2;
        cli_args_1[i].length_b = array_length;
    }

    //==== Run The Experiments ====
    std::vector<SimultaniousResults> results_vec = execute_kernel_fanin_server_measure(profiler, latency_flow_ctrl_blocked_read_server_kernel, latency_flow_ctrl_blocked_read_client_join_kernel, latency_flow_ctrl_blocked_read_join_kernel_reset, noCleanupFctn, srv_args_1, srv_args_2, cli_args_1, cli_args_1, cpu_a, cpu_b, cpu_c, num_experiments); //Reset arg is cli args

    //==== Process Results ====
    if(results_vec.size() != num_experiments){
        std::cerr << "Error: The number of results did not match the number of experiments" << std::endl;
    }

    for(int i = 0; i<num_experiments; i++)
    {
        printTitleArray(false, "Flow Control Blocked Read - Fan-in - Array", array_lengths[i]);
        exportResultsArray3CoreFanInFanOut<int32_t>(false, profiler, cpu_a, cpu_b, cpu_c, results_vec[i], array_lengths[i], format, file_a, file_b, raw_file_a, raw_file_b, true);
    }

    #if WRITE_CSV == 1
        fflush(file_a);
        fflush(file_b);
    #endif

    tableFooter();

    //==== Cleanup ====
    shared_ack_loc_1->~atomic();
    shared_ack_loc_2->~atomic();
    shared_valid_loc_1->~atomic();
    shared_valid_loc_2->~atomic();
    free(shared_array_loc_1);
    free(shared_array_loc_2);
    free(shared_ack_loc_1);
    free(shared_ack_loc_2);
    free(shared_valid_loc_1);
    free(shared_valid_loc_2);
    delete[] srv_args_1;
    delete[] srv_args_2;
    delete[] cli_args_1;
}

void run_latency_flow_ctrl_blocked_read_fanout_kernel(Profiler* profiler, int cpu_a, int cpu_b, int cpu_c, std::vector<size_t> array_lengths, FILE* file_a, FILE* file_b, std::ofstream* raw_file_a, std::ofstream* raw_file_b)
{
    //==== Print header ====
    std::string format = tableHeaderArray2Streams("Flow Control Blocked Read - Fan-out - Array", file_a, file_b, raw_file_a, raw_file_b, false);

    //==== Allocate Array For the Largest Experiment ====
    size_t max_length = *std::max_element(array_lengths.begin(), array_lengths.end());

    //Initialize
    size_t amountToAlloc = max_length*sizeof(int32_t);
    if(amountToAlloc % CACHE_LINE_SIZE != 0){
        amountToAlloc += (CACHE_LINE_SIZE - (amountToAlloc % CACHE_LINE_SIZE));
    }
    int32_t* shared_array_loc_1 = (int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);
    int32_t* shared_array_loc_2 = (int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAlloc, cpu_a);

    size_t amountToAllocCursors = sizeof(std::atomic_int32_t);
    if(amountToAllocCursors % CACHE_LINE_SIZE != 0){
        amountToAllocCursors += (CACHE_LINE_SIZE - (amountToAllocCursors % CACHE_LINE_SIZE));
    }
    std::atomic_int32_t* shared_ack_loc_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_b);
    std::atomic_int32_t* shared_valid_loc_1 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_a);
    std::atomic_int32_t* shared_ack_loc_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_c);
    std::atomic_int32_t* shared_valid_loc_2 = (std::atomic_int32_t*) aligned_alloc_core(CACHE_LINE_SIZE, amountToAllocCursors, cpu_a);
    std::atomic_int32_t* shared_ack_loc_1_constructed = new (shared_ack_loc_1) std::atomic_int32_t();
    std::atomic_init(shared_ack_loc_1, 0);
    if(!std::atomic_is_lock_free(shared_ack_loc_1)){
        printf("Atomic is not lock free and was expected to be");
        exit(1);
    }
    std::atomic_int32_t* shared_valid_loc_1_constructed = new (shared_valid_loc_1) std::atomic_int32_t();
    std::atomic_init(shared_valid_loc_1, 0);
    if(!std::atomic_is_lock_free(shared_valid_loc_1)){
        printf("Atomic is not lock free and was expected to be");
        exit(1);
    }
    std::atomic_int32_t* shared_ack_loc_2_constructed = new (shared_ack_loc_2) std::atomic_int32_t();
    std::atomic_init(shared_ack_loc_2, 0);
    if(!std::atomic_is_lock_free(shared_ack_loc_2)){
        printf("Atomic is not lock free and was expected to be");
        exit(1);
    }
    std::atomic_int32_t* shared_valid_loc_2_constructed = new (shared_valid_loc_2) std::atomic_int32_t();
    std::atomic_init(shared_valid_loc_2, 0);
    if(!std::atomic_is_lock_free(shared_valid_loc_2)){
        printf("Atomic is not lock free and was expected to be");
        exit(1);
    }

    //Init to 0
    for(size_t i = 0; i < max_length; i++)
    {
        shared_array_loc_1[i] = 0;
        shared_array_loc_2[i] = 0;
    }

    //==== Create Configurations for each experiment ====
    int num_experiments = array_lengths.size();

    LatencyFlowCtrlBlockedReadJoinKernelArgs* srv_args_1 = new LatencyFlowCtrlBlockedReadJoinKernelArgs[num_experiments];
    LatencyFlowCtrlBlockedReadKernelArgs* cli_args_1 = new LatencyFlowCtrlBlockedReadKernelArgs[num_experiments];
    LatencyFlowCtrlBlockedReadKernelArgs* cli_args_2 = new LatencyFlowCtrlBlockedReadKernelArgs[num_experiments];

    for(int i = 0; i<num_experiments; i++)
    {
        size_t array_length = array_lengths[i];

        //Server Arg
        srv_args_1[i].array_shared_ptr_a = shared_array_loc_1;
        srv_args_1[i].ack_shared_ptr_a = shared_ack_loc_1;
        srv_args_1[i].valid_shared_ptr_a = shared_valid_loc_1;
        srv_args_1[i].length_a = array_length;
        srv_args_1[i].array_shared_ptr_b = shared_array_loc_2;
        srv_args_1[i].ack_shared_ptr_b = shared_ack_loc_2;
        srv_args_1[i].valid_shared_ptr_b = shared_valid_loc_2;
        srv_args_1[i].length_b = array_length;

        //Client Args
        cli_args_1[i].array_shared_ptr = shared_array_loc_1;
        cli_args_1[i].ack_shared_ptr = shared_ack_loc_1;
        cli_args_1[i].valid_shared_ptr = shared_valid_loc_1;
        cli_args_1[i].length = array_length;

        cli_args_2[i].array_shared_ptr = shared_array_loc_2;
        cli_args_2[i].ack_shared_ptr = shared_ack_loc_2;
        cli_args_2[i].valid_shared_ptr = shared_valid_loc_2;
        cli_args_2[i].length = array_length;
    }

    //==== Run The Experiments ====
    std::vector<SimultaniousResults> results_vec = execute_kernel_fanout_client_measure(profiler, latency_flow_ctrl_blocked_read_server_join_kernel, latency_flow_ctrl_blocked_read_client_kernel, latency_flow_ctrl_blocked_read_join_kernel_reset, noCleanupFctn, srv_args_1, cli_args_1, cli_args_2, srv_args_1, cpu_a, cpu_b, cpu_c, num_experiments); //Reset arg is srv args

    //==== Process Results ====
    if(results_vec.size() != num_experiments){
        std::cerr << "Error: The number of results did not match the number of experiments" << std::endl;
    }

    for(int i = 0; i<num_experiments; i++)
    {
        printTitleArray(false, "Flow Control Blocked Read - Fan-Out - Array", array_lengths[i]);
        exportResultsArray3CoreFanOut<int32_t>(false, profiler, cpu_a, cpu_b, cpu_c, results_vec[i], array_lengths[i], format, file_a, file_b, raw_file_a, raw_file_b, true);
    }

    #if WRITE_CSV == 1
        fflush(file_a);
        fflush(file_b);
    #endif

    tableFooter();

    //==== Cleanup ====
    shared_ack_loc_1->~atomic();
    shared_ack_loc_2->~atomic();
    shared_valid_loc_1->~atomic();
    shared_valid_loc_2->~atomic();
    free(shared_array_loc_1);
    free(shared_array_loc_2);
    free(shared_ack_loc_1);
    free(shared_ack_loc_2);
    free(shared_valid_loc_1);
    free(shared_valid_loc_2);
    delete[] srv_args_1;
    delete[] cli_args_1;
    delete[] cli_args_2;
}
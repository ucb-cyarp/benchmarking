#include "fifo_kernels.h"

//MAKE A 2D Table
void run_bandwidth_fifo_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, std::vector<int32_t> max_writes_per_transaction, FILE* file, std::ofstream* raw_file)
{
    int32_t data_col_width = 10;

    //==== Print Title ====
    printTitleFIFO("FIFO - Array", max_writes_per_transaction.size(), data_col_width);

    //==== Print/Write Headers ====
    std::string format = printAndWriteHeadersFIFO("Max Trans. Len", "Max Writes Per Transaction", max_writes_per_transaction, data_col_width, file, raw_file);

    //==== Allocate Array For the Largest Experiment ====
    size_t max_array_length = *std::max_element(array_lengths.begin(), array_lengths.end());
    int32_t* shared_array_loc;
    std::atomic_int32_t* shared_write_id_loc;
    std::atomic_int32_t* shared_read_id_loc;

    fifoAllocate(&shared_array_loc, &shared_write_id_loc, &shared_read_id_loc, max_array_length, cpu_a, cpu_b);

    //==== Create Configurations for each experiment ====
    int num_experiments = countNumberOfFIFOExperiments(array_lengths, max_writes_per_transaction);

    BandwidthCircularFifoKernelArgs* args = new BandwidthCircularFifoKernelArgs[num_experiments];

    int arg_idx = 0;
    for(int i = 0; i<array_lengths.size(); i++)
    {
        size_t array_length = array_lengths[i];
        for(int j = 0; j<max_writes_per_transaction.size(); j++)
        {
            int32_t max_writes = max_writes_per_transaction[j];
            if(max_writes <= array_length) //Check if we should bother running this case
            {
                args[arg_idx].array_shared_ptr = shared_array_loc;
                args[arg_idx].write_pos_shared_ptr = shared_write_id_loc;
                args[arg_idx].read_pos_shared_ptr = shared_read_id_loc;
                args[arg_idx].length = array_length;
                args[arg_idx].max_write_per_transaction = max_writes;
                
                arg_idx++;
            }
        }
    }

    //==== Run The Experiments ====
    std::vector<Results> results_vec = execute_client_server_kernel(profiler, bandwidth_circular_fifo_server_kernel, bandwidth_circular_fifo_client_kernel, bandwidth_circular_fifo_kernel_reset, noCleanupFctn, args, args, args, cpu_a, cpu_b, num_experiments);

    //==== Process Results ====
    if(results_vec.size() != num_experiments){
        std::cerr << "Error: The number of results did not match the number of experiments" << std::endl;
    }

    printWriteFIFOTable2Core<int32_t>(false, profiler, cpu_a, cpu_b, "FIFO - Array", "Max Write Per Transaction", results_vec, array_lengths, max_writes_per_transaction, data_col_width, format, file, raw_file);

    //==== Cleanup ====
    shared_write_id_loc->~atomic();
    shared_read_id_loc->~atomic();
    free(shared_array_loc);
    free(shared_write_id_loc);
    free(shared_read_id_loc);
    delete[] args;
}

//MAKE A 2D Table
void run_bandwidth_fifo_blocked_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, FILE* file, std::ofstream* raw_file)
{
    int32_t data_col_width = 10;

    //==== Print Title ====
    printTitleFIFO("FIFO - Array - Blocked Transfers", block_lengths.size(), data_col_width);

    //==== Print/Write Headers ====
    std::string format = printAndWriteHeadersFIFO("Block Length", "Block Length", block_lengths, data_col_width, file, raw_file);

    //==== Allocate Array For the Largest Experiment ====
    size_t max_array_length = *std::max_element(array_lengths.begin(), array_lengths.end());
    int32_t* shared_array_loc;
    std::atomic_int32_t* shared_write_id_loc;
    std::atomic_int32_t* shared_read_id_loc;

    fifoAllocate(&shared_array_loc, &shared_write_id_loc, &shared_read_id_loc, max_array_length, cpu_a, cpu_b);

    //==== Create Configurations for each experiment ====
    int num_experiments = countNumberOfFIFOExperiments(array_lengths, block_lengths);

    BandwidthCircularFifoBlockedKernelArgs* args = new BandwidthCircularFifoBlockedKernelArgs[num_experiments];

    int arg_idx = 0;
    for(int i = 0; i<array_lengths.size(); i++)
    {
        size_t array_length = array_lengths[i];
        for(int j = 0; j<block_lengths.size(); j++)
        {
            int32_t block_length = block_lengths[j];
            if(block_length <= array_length) //Check if we should bother running this case
            {
                args[arg_idx].array_shared_ptr = shared_array_loc;
                args[arg_idx].write_pos_shared_ptr = shared_write_id_loc;
                args[arg_idx].read_pos_shared_ptr = shared_read_id_loc;
                args[arg_idx].length = array_length;
                args[arg_idx].block_length = block_length;

                arg_idx++;
            }
        }
    }

    //==== Run The Experiments ====
    std::vector<Results> results_vec = execute_client_server_kernel(profiler, bandwidth_circular_fifo_blocked_server_kernel, bandwidth_circular_fifo_blocked_client_kernel, bandwidth_circular_fifo_blocked_kernel_reset, noCleanupFctn, args, args, args, cpu_a, cpu_b, num_experiments);

    //==== Process Results ====
    if(results_vec.size() != num_experiments){
        std::cerr << "Error: The number of results did not match the number of experiments" << std::endl;
    }

    printWriteBlockedFIFOTable2Core<int32_t>(false, profiler, cpu_a, cpu_b, "FIFO - Array - Blocked Transfers", "Block Length Transaction", results_vec, array_lengths, block_lengths, data_col_width, format, file, raw_file);
    
    //==== Cleanup ====
    shared_write_id_loc->~atomic();
    shared_read_id_loc->~atomic();
    free(shared_array_loc);
    free(shared_write_id_loc);
    free(shared_read_id_loc);
    delete[] args;
}

//MAKE A 2D Table
void run_bandwidth_fifo_read_limit_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, std::vector<int32_t> max_elements_per_transaction, FILE* file, std::ofstream* raw_file)
{
    int32_t data_col_width = 10;

    //==== Print Title ====
    printTitleFIFO("FIFO - Array - Read and Write Limited", max_elements_per_transaction.size(), data_col_width);

    //==== Print/Write Headers ====
    std::string format = printAndWriteHeadersFIFO("Max Trans. Len", "Max Reads|Writes Per Transaction", max_elements_per_transaction, data_col_width, file, raw_file);

    //==== Allocate Array For the Largest Experiment ====
    size_t max_array_length = *std::max_element(array_lengths.begin(), array_lengths.end());
    int32_t* shared_array_loc;
    std::atomic_int32_t* shared_write_id_loc;
    std::atomic_int32_t* shared_read_id_loc;

    fifoAllocate(&shared_array_loc, &shared_write_id_loc, &shared_read_id_loc, max_array_length, cpu_a, cpu_b);

    //==== Create Configurations for each experiment ====
    int num_experiments = countNumberOfFIFOExperiments(array_lengths, max_elements_per_transaction);

    BandwidthCircularFifoReadLimitKernelArgs* args = new BandwidthCircularFifoReadLimitKernelArgs[num_experiments];

    int arg_idx = 0;
    for(int i = 0; i<array_lengths.size(); i++)
    {
        size_t array_length = array_lengths[i];
        for(int j = 0; j<max_elements_per_transaction.size(); j++)
        {
            int32_t max_elements = max_elements_per_transaction[j];
            if(max_elements <= array_length) //Check if we should bother running this case
            {
                args[arg_idx].array_shared_ptr = shared_array_loc;
                args[arg_idx].write_pos_shared_ptr = shared_write_id_loc;
                args[arg_idx].read_pos_shared_ptr = shared_read_id_loc;
                args[arg_idx].length = array_length;
                args[arg_idx].max_elements_per_transaction = max_elements;

                arg_idx++;
            }
        }
    }

    //==== Run The Experiments ====
    std::vector<Results> results_vec = execute_client_server_kernel(profiler, bandwidth_circular_fifo_read_limit_server_kernel, bandwidth_circular_fifo_read_limit_client_kernel, bandwidth_circular_fifo_read_limit_kernel_reset, noCleanupFctn, args, args, args, cpu_a, cpu_b, num_experiments);

    //==== Process Results ====
    if(results_vec.size() != num_experiments){
        std::cerr << "Error: The number of results did not match the number of experiments" << std::endl;
    }

    printWriteFIFOTable2Core<int32_t>(false, profiler, cpu_a, cpu_b, "FIFO - Array - Read and Write Limit", "Max Read|Write Per Transaction", results_vec, array_lengths, max_elements_per_transaction, data_col_width, format, file, raw_file);

    //==== Cleanup ====
    shared_write_id_loc->~atomic();
    shared_read_id_loc->~atomic();
    free(shared_array_loc);
    free(shared_write_id_loc);
    free(shared_read_id_loc);
    delete[] args;
}

//MAKE A 2D Table
void run_bandwidth_fifo_blocked_cachedptr_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, FILE* file, std::ofstream* raw_file)
{
    int32_t data_col_width = 10;

    //==== Print Title ====
    printTitleFIFO("FIFO - Array - Blocked Transfers Cached Indexes", block_lengths.size(), data_col_width);

    //==== Print/Write Headers ====
    std::string format = printAndWriteHeadersFIFO("Block Length", "Block Length", block_lengths, data_col_width, file, raw_file);

    //==== Allocate Array For the Largest Experiment ====
    size_t max_array_length = *std::max_element(array_lengths.begin(), array_lengths.end());
    int32_t* shared_array_loc;
    std::atomic_int32_t* shared_write_id_loc;
    std::atomic_int32_t* shared_read_id_loc;

    fifoAllocate(&shared_array_loc, &shared_write_id_loc, &shared_read_id_loc, max_array_length, cpu_a, cpu_b);

    //==== Create Configurations for each experiment ====
    int num_experiments = countNumberOfFIFOExperiments(array_lengths, block_lengths);

    BandwidthCircularFifoBlockedCachedPtrKernelArgs* args = new BandwidthCircularFifoBlockedCachedPtrKernelArgs[num_experiments];

    int arg_idx = 0;
    for(int i = 0; i<array_lengths.size(); i++)
    {
        size_t array_length = array_lengths[i];
        for(int j = 0; j<block_lengths.size(); j++)
        {
            int32_t block_length = block_lengths[j];
            if(block_length <= array_length) //Check if we should bother running this case
            {
                args[arg_idx].array_shared_ptr = shared_array_loc;
                args[arg_idx].write_pos_shared_ptr = shared_write_id_loc;
                args[arg_idx].read_pos_shared_ptr = shared_read_id_loc;
                args[arg_idx].length = array_length;
                args[arg_idx].block_length = block_length;

                arg_idx++;
            }
        }
    }

    //==== Run The Experiments ====
    std::vector<Results> results_vec = execute_client_server_kernel(profiler, bandwidth_circular_fifo_blocked_cachedptr_server_kernel, bandwidth_circular_fifo_blocked_cachedptr_client_kernel, bandwidth_circular_fifo_blocked_cachedptr_kernel_reset, noCleanupFctn, args, args, args, cpu_a, cpu_b, num_experiments);

    //==== Process Results ====
    if(results_vec.size() != num_experiments){
        std::cerr << "Error: The number of results did not match the number of experiments" << std::endl;
    }

    printWriteBlockedFIFOTable2Core<int32_t>(false, profiler, cpu_a, cpu_b, "FIFO - Array - Blocked Transfers Cached Indexes", "Block Length Transaction", results_vec, array_lengths, block_lengths, data_col_width, format, file, raw_file);
    
    //==== Cleanup ====
    shared_write_id_loc->~atomic();
    shared_read_id_loc->~atomic();
    free(shared_array_loc);
    free(shared_write_id_loc);
    free(shared_read_id_loc);
    delete[] args;
}
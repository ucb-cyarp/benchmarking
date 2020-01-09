#include "closed_loop_kernels.h"
#include "closed_loop_buffer.h"
#include "closed_loop_helpers.h"

//MAKE A 2D Table
void run_closed_loop_bang_control_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, std::vector<int32_t> server_control_periods, std::vector<int32_t> client_control_periods, std::vector<int32_t> control_gains, std::vector<int> initial_nops, int alignment, int64_t max_block_transfers, FILE* file, std::ofstream* raw_file)
{
    int32_t data_col_width = 10;

    //==== Print Title ====
    std::string format = tableHeaderClosedLoop("Closed Loop - Bang Controller - One Way", file);

    //==== Allocate Array For the Largest Experiment ====
    std::vector<int32_t*> shared_array_locs;
    std::vector<std::atomic_int32_t*> shared_write_id_locs;
    std::vector<std::atomic_int32_t*> shared_read_id_locs;

    std::vector<int> cpus = {cpu_a, cpu_b};

    std::vector<std::atomic_flag*> ready_flags;
    std::vector<std::atomic_flag*> start_flags;
    std::atomic_flag* stop_flag;

    std::vector<std::atomic_int32_t*> nopsControl;

    closedLoopAllocate<int32_t, std::atomic_int32_t, std::atomic_int32_t, std::atomic_int32_t>(shared_array_locs, shared_write_id_locs, shared_read_id_locs, ready_flags, start_flags, stop_flag, nopsControl, array_lengths, block_lengths, cpus, alignment, false, false);

    //==== Create Configurations for each experiment ====
    int num_experiments = array_lengths.size() * block_lengths.size() * server_control_periods.size() * client_control_periods.size() * control_gains.size();

    ClosedLoopBufferArgs<int32_t, std::atomic_int32_t, std::atomic_int32_t, std::atomic_int32_t>* args = new ClosedLoopBufferArgs<int32_t, std::atomic_int32_t, std::atomic_int32_t, std::atomic_int32_t>[num_experiments];

    for(int i = 0; i<array_lengths.size(); i++)
    {
        size_t array_length = array_lengths[i];
        for(int j = 0; j<block_lengths.size(); j++)
        {
            int32_t block_length = block_lengths[j];
            for(int k = 0; k<server_control_periods.size(); k++)
            {
                int32_t server_control_period = server_control_periods[k];
                for(int n = 0; n<client_control_periods.size(); n++)
                {
                    int32_t client_control_period = client_control_periods[n];
                    for(int m = 0; m<control_gains.size(); m++)
                    {
                        int32_t control_gain = control_gains[m];
                        for(int l = 0; l<initial_nops.size(); l++){
                            int initial_nop = initial_nops[l];

                            int idx = l + 
                                    m*initial_nops.size() + 
                                    n*initial_nops.size()*control_gains.size() +
                                    k*initial_nops.size()*control_gains.size()*client_control_periods.size() + 
                                    j*initial_nops.size()*control_gains.size()*client_control_periods.size()*server_control_periods.size() +
                                    i*initial_nops.size()*control_gains.size()*client_control_periods.size()*server_control_periods.size()*block_lengths.size();

                            args[idx].read_offset_ptr = shared_read_id_locs[0];
                            args[idx].write_offset_ptr = shared_write_id_locs[0];
                            args[idx].array = shared_array_locs[0];
                            args[idx].start_flag = start_flags[0];
                            args[idx].stop_flag = stop_flag;
                            args[idx].ready_flag = ready_flags[0];
                            args[idx].array_length = array_length;
                            args[idx].max_block_transfers = max_block_transfers;
                            args[idx].blockSize = block_length;
                            args[idx].control_check_period = server_control_period;
                            args[idx].control_client_check_period = client_control_period;
                            args[idx].control_gain = control_gain;
                            args[idx].clientNops = nopsControl[0];
                            args[idx].alignment = alignment;
                            args[idx].core_server = cpus[0]; //The server is the secondary but is placed on cpu a to have the typical transfer from cpu_a -> cpu_b
                            args[idx].core_client = cpus[1]; //The client is the primary but is placed on cpu b
                            args[idx].initialNops = initial_nop;
                        }
                    }
                }
            }
        }
    }

    //==== Run The Experiments ====
    //The primary is the client (because it performs the measurment) and the secondary is the server
    std::vector<Results> results_vec = execute_client_server_kernel(profiler, closed_loop_buffer_client<int32_t, std::atomic_int32_t, std::atomic_int32_t, int32_t, int32_t, std::atomic_int32_t, int32_t, INT32_MAX>, 
                                                                    closed_loop_buffer_bang_control_server<int32_t, std::atomic_int32_t, std::atomic_int32_t, int32_t, int32_t, std::atomic_int32_t, int32_t, INT32_MAX>,
                                                                    closed_loop_buffer_reset<int32_t, std::atomic_int32_t, std::atomic_int32_t, std::atomic_int32_t>,
                                                                    closed_loop_buffer_cleanup<int32_t, std::atomic_int32_t, std::atomic_int32_t, std::atomic_int32_t>, 
                                                                    args, args, args, cpus[1], cpus[0], num_experiments);

    //==== Process Results ====
    if(results_vec.size() != num_experiments){
        std::cerr << "Error: The number of results did not match the number of experiments" << std::endl;
    }

    if(results_vec.size() == 0){
        printf("No Experiments!");
        exit(2);
    }

    //==== Write Raw Headers (Done after the experiment is run because we need the benchmark specific results to get the header information) ====
    std::vector<std::shared_ptr<BenchmarkSpecificResult>> implSpecificResultExamples;
    if(results_vec[0].trial_results.size() > 0){
        if(results_vec[0].trial_results[0].benchmarkSpecificResults.size() > 0){
            implSpecificResultExamples = results_vec[0].trial_results[0].benchmarkSpecificResults;
        }
    }
    writeRawHeaderClosedLoop(implSpecificResultExamples, raw_file);

    //Print results
    printWriteClosedLoop2CoreResults<int32_t>(false, profiler, cpus, "Closed Loop (Bang Control) - One Way", results_vec, array_lengths, block_lengths, server_control_periods, client_control_periods, control_gains, initial_nops, format, file, raw_file);

    //==== Cleanup ====
    destructSharedIDs(shared_write_id_locs, shared_read_id_locs, ready_flags, start_flags, stop_flag, nopsControl);

    freeVectorContents(shared_array_locs);
    freeVectorContents(shared_write_id_locs);
    freeVectorContents(shared_read_id_locs);
    freeVectorContents(ready_flags);
    freeVectorContents(start_flags);
    freeVectorContents(nopsControl);
    free(stop_flag);
    delete[] args;
}
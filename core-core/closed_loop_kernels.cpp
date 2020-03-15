#include "closed_loop_kernels.h"
#include "closed_loop_bang_buffer.h"
#include "closed_loop_pi_buffer.h"
#include "closed_loop_helpers.h"

//MAKE A 2D Table
void run_closed_loop_bang_control_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, std::vector<int32_t> server_control_periods, std::vector<int32_t> client_control_periods, std::vector<float> control_gains, std::vector<float> initial_nops, int alignment, int64_t max_block_transfers, FILE* file, std::ofstream* raw_file)
{
    int32_t data_col_width = 10;

    //==== Print Title ====
    std::string format = tableHeaderClosedLoopBang("Closed Loop - Bang Controller - One Way", file);

    //==== Allocate Array For the Largest Experiment ====
    std::vector<int32_t*> shared_array_locs;
    std::vector<int32_t*> local_array_locs;
    std::vector<std::atomic_int32_t*> shared_write_id_locs;
    std::vector<std::atomic_int32_t*> shared_read_id_locs;

    std::vector<int> cpus = {cpu_a, cpu_b};

    std::vector<std::atomic_flag*> ready_flags;
    std::vector<std::atomic_flag*> start_flags;
    std::atomic_flag* stop_flag;

    std::vector<std::atomic<float>*> nopsControl;

    #if CLOSED_LOOP_DISABLE_INTERRUPTS==1
        std::vector<FILE*> interruptReporterFiles;

        for(int i = 0; i<cpus.size(); i++){
            FILE* interruptReporter = fopen("/dev/sir0", "r");
            if(interruptReporter == NULL){
                std::cerr << "Unable to open /dev/sir0.  Check that the sir module is loaded" << std::endl;
                exit(1);
            }

            interruptReporterFiles.push_back(interruptReporter);
        }
    #endif

    closedLoopAllocate<int32_t, std::atomic_int32_t, std::atomic_int32_t, std::atomic<float>>(shared_array_locs, local_array_locs, shared_write_id_locs, shared_read_id_locs, ready_flags, start_flags, stop_flag, nopsControl, array_lengths, block_lengths, cpus, alignment, false, false);

    //==== Create Configurations for each experiment ====
    int num_experiments = array_lengths.size() * block_lengths.size() * server_control_periods.size() * client_control_periods.size() * control_gains.size() * initial_nops.size();

    ClosedLoopBangBufferArgs<int32_t, std::atomic_int32_t, std::atomic_int32_t, std::atomic<float>, float>* args = new ClosedLoopBangBufferArgs<int32_t, std::atomic_int32_t, std::atomic_int32_t, std::atomic<float>, float>[num_experiments];

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
                        float control_gain = control_gains[m];
                        for(int l = 0; l<initial_nops.size(); l++){
                            float initial_nop = initial_nops[l];

                            int idx = l + 
                                    m*initial_nops.size() + 
                                    n*initial_nops.size()*control_gains.size() +
                                    k*initial_nops.size()*control_gains.size()*client_control_periods.size() + 
                                    j*initial_nops.size()*control_gains.size()*client_control_periods.size()*server_control_periods.size() +
                                    i*initial_nops.size()*control_gains.size()*client_control_periods.size()*server_control_periods.size()*block_lengths.size();

                            args[idx].read_offset_ptr = shared_read_id_locs[0];
                            args[idx].write_offset_ptr = shared_write_id_locs[0];
                            args[idx].array = shared_array_locs[0];
                            args[idx].local_array_reader = local_array_locs[0];
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
                            args[idx].core_server = cpus[0];
                            args[idx].core_client = cpus[1];
                            args[idx].initialNops = initial_nop;

                            #if CLOSED_LOOP_DISABLE_INTERRUPTS==1
                                args[idx].writerSirFile = interruptReporterFiles[0];
                                args[idx].readerSirFile = interruptReporterFiles[1];
                            #endif
                        }
                    }
                }
            }
        }
    }

    //==== Run The Experiments ====
    //The primary is the client (because it performs the measurment) and the secondary is the server
    std::vector<Results> results_vec = execute_client_server_kernel(profiler, closed_loop_buffer_bang_control_server<int32_t, std::atomic_int32_t, std::atomic_int32_t, int32_t, int32_t, std::atomic<float>, float, INT32_MAX>, 
                                                                    closed_loop_buffer_float_client<int32_t, std::atomic_int32_t, std::atomic_int32_t, int32_t, int32_t, std::atomic<float>, float, INT32_MAX>,
                                                                    closed_loop_buffer_reset<int32_t, std::atomic_int32_t, std::atomic_int32_t, std::atomic<float>, float>,
                                                                    closed_loop_buffer_cleanup<int32_t, std::atomic_int32_t, std::atomic_int32_t, std::atomic<float>, float>, 
                                                                    args, args, args, cpus[0], cpus[1], num_experiments);

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
    writeRawHeaderClosedLoopBang(implSpecificResultExamples, raw_file);

    //Print results
    for(int i = 0; i<results_vec.size(); i++){
        args[i].printExportCorrespondingResult(results_vec[i], false, "Closed Loop (Bang Control) - One Way", profiler, cpus, format, file, raw_file);
    }

    closeEntry(file, raw_file);

    //==== Cleanup ====
    #if CLOSED_LOOP_DISABLE_INTERRUPTS==1
        for(int i = 0; i<interruptReporterFiles.size(); i++){
            fclose(interruptReporterFiles[i]);
        }
    #endif

    destructSharedIDs(shared_write_id_locs, shared_read_id_locs, ready_flags, start_flags, stop_flag, nopsControl);

    freeVectorContents(shared_array_locs);
    freeVectorContents(local_array_locs);
    freeVectorContents(shared_write_id_locs);
    freeVectorContents(shared_read_id_locs);
    freeVectorContents(ready_flags);
    freeVectorContents(start_flags);
    freeVectorContents(nopsControl);
    free(stop_flag);
    delete[] args;
}

void run_closed_loop_pi_control_period_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, std::vector<int32_t> server_control_periods, std::vector<int32_t> client_control_periods, std::vector<float> control_gains_p, std::vector<float> control_gains_i, std::vector<float> control_gains_base_bang_bang, std::vector<float> initial_nops, int alignment, int64_t max_block_transfers, FILE* file, std::ofstream* raw_file)
{
    int32_t data_col_width = 10;

    //==== Print Title ====
    std::string format = tableHeaderClosedLoopPI("Closed Loop - PI Controller - Rate - One Way", file);

    //==== Allocate Array For the Largest Experiment ====
    std::vector<int32_t*> shared_array_locs;
    std::vector<int32_t*> local_array_locs;
    std::vector<std::atomic_int32_t*> shared_write_id_locs;
    std::vector<std::atomic_int32_t*> shared_read_id_locs;

    std::vector<int> cpus = {cpu_a, cpu_b};

    std::vector<std::atomic_flag*> ready_flags;
    std::vector<std::atomic_flag*> start_flags;
    std::atomic_flag* stop_flag;

    std::vector<std::atomic<float>*> nopsControl;

    #if CLOSED_LOOP_DISABLE_INTERRUPTS==1
        std::vector<FILE*> interruptReporterFiles;

        for(int i = 0; i<cpus.size(); i++){
            FILE* interruptReporter = fopen("/dev/sir0", "r");
            if(interruptReporter == NULL){
                std::cerr << "Unable to open /dev/sir0.  Check that the sir module is loaded" << std::endl;
                exit(1);
            }

            interruptReporterFiles.push_back(interruptReporter);
        }
    #endif

    closedLoopAllocate<int32_t, std::atomic_int32_t, std::atomic_int32_t, std::atomic<float>>(shared_array_locs, local_array_locs, shared_write_id_locs, shared_read_id_locs, ready_flags, start_flags, stop_flag, nopsControl, array_lengths, block_lengths, cpus, alignment, false, false);

    //==== Create Configurations for each experiment ====
    int num_experiments = array_lengths.size() * block_lengths.size() * server_control_periods.size() * client_control_periods.size() * control_gains_p.size() * control_gains_i.size() * control_gains_base_bang_bang.size() * initial_nops.size();

    ClosedLoopPIBufferArgs<int32_t, std::atomic_int32_t, std::atomic_int32_t, std::atomic<float>, float>* args = new ClosedLoopPIBufferArgs<int32_t, std::atomic_int32_t, std::atomic_int32_t, std::atomic<float>, float>[num_experiments];

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
                    for(int m = 0; m<control_gains_p.size(); m++)
                    {
                        float control_gain_p = control_gains_p[m];
                        for(int mm = 0; mm<control_gains_i.size(); mm++){
                            float control_gain_i = control_gains_i[mm];
                            for(int mmm = 0; mmm<control_gains_base_bang_bang.size(); mmm++){
                                float control_gain_base_bang_bang = control_gains_base_bang_bang[mmm];
                                for(int l = 0; l<initial_nops.size(); l++){
                                    float initial_nop = initial_nops[l];

                                    int idx = l + 
                                            mmm*initial_nops.size() + 
                                             mm*initial_nops.size()*control_gains_base_bang_bang.size() + 
                                              m*initial_nops.size()*control_gains_base_bang_bang.size()*control_gains_i.size() + 
                                              n*initial_nops.size()*control_gains_base_bang_bang.size()*control_gains_i.size()*control_gains_p.size() +
                                              k*initial_nops.size()*control_gains_base_bang_bang.size()*control_gains_i.size()*control_gains_p.size()*client_control_periods.size() + 
                                              j*initial_nops.size()*control_gains_base_bang_bang.size()*control_gains_i.size()*control_gains_p.size()*client_control_periods.size()*server_control_periods.size() +
                                              i*initial_nops.size()*control_gains_base_bang_bang.size()*control_gains_i.size()*control_gains_p.size()*client_control_periods.size()*server_control_periods.size()*block_lengths.size();

                                    args[idx].read_offset_ptr = shared_read_id_locs[0];
                                    args[idx].write_offset_ptr = shared_write_id_locs[0];
                                    args[idx].array = shared_array_locs[0];
                                    args[idx].local_array_reader = local_array_locs[0];
                                    args[idx].start_flag = start_flags[0];
                                    args[idx].stop_flag = stop_flag;
                                    args[idx].ready_flag = ready_flags[0];
                                    args[idx].array_length = array_length;
                                    args[idx].max_block_transfers = max_block_transfers;
                                    args[idx].blockSize = block_length;
                                    args[idx].control_check_period = server_control_period;
                                    args[idx].control_client_check_period = client_control_period;
                                    args[idx].control_gain_i = control_gain_i;
                                    args[idx].control_gain_p = control_gain_p;
                                    args[idx].control_gain_base_bang = control_gain_base_bang_bang;
                                    args[idx].clientNops = nopsControl[0];
                                    args[idx].alignment = alignment;
                                    args[idx].core_server = cpus[0];
                                    args[idx].core_client = cpus[1];
                                    args[idx].initialNops = initial_nop;

                                    #if CLOSED_LOOP_DISABLE_INTERRUPTS==1
                                        args[idx].writerSirFile = interruptReporterFiles[0];
                                        args[idx].readerSirFile = interruptReporterFiles[1];
                                    #endif
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    //==== Run The Experiments ====
    //The primary is the client (because it performs the measurment) and the secondary is the server
    std::vector<Results> results_vec = execute_client_server_kernel(profiler, closed_loop_buffer_pi_period_control_server<int32_t, std::atomic_int32_t, std::atomic_int32_t, int32_t, int32_t, std::atomic<float>, float, INT32_MAX>, 
                                                                    closed_loop_buffer_float_client<int32_t, std::atomic_int32_t, std::atomic_int32_t, int32_t, int32_t, std::atomic<float>, float, INT32_MAX>,
                                                                    closed_loop_buffer_reset<int32_t, std::atomic_int32_t, std::atomic_int32_t, std::atomic<float>, float>,
                                                                    closed_loop_buffer_cleanup<int32_t, std::atomic_int32_t, std::atomic_int32_t, std::atomic<float>, float>, 
                                                                    args, args, args, cpus[0], cpus[1], num_experiments);

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
    writeRawHeaderClosedLoopPI(implSpecificResultExamples, raw_file);

   //Print results
    for(int i = 0; i<results_vec.size(); i++){
        args[i].printExportCorrespondingResult(results_vec[i], false, "Closed Loop (PI Control - Rate) - One Way", profiler, cpus, format, file, raw_file);
    }

    closeEntry(file, raw_file);

    //==== Cleanup ====
    #if CLOSED_LOOP_DISABLE_INTERRUPTS==1
        for(int i = 0; i<interruptReporterFiles.size(); i++){
            fclose(interruptReporterFiles[i]);
        }
    #endif

    destructSharedIDs(shared_write_id_locs, shared_read_id_locs, ready_flags, start_flags, stop_flag, nopsControl);

    freeVectorContents(shared_array_locs);
    freeVectorContents(local_array_locs);
    freeVectorContents(shared_write_id_locs);
    freeVectorContents(shared_read_id_locs);
    freeVectorContents(ready_flags);
    freeVectorContents(start_flags);
    freeVectorContents(nopsControl);
    free(stop_flag);
    delete[] args;
}
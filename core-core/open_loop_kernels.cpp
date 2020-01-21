#include "open_loop_kernels.h"
#include "open_loop_buffer.h"
#include "open_loop_helpers.h"
#include "open_loop_fullness_tracker.h"

//MAKE A 2D Table
void run_open_loop_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, std::vector<int32_t> balance_nops, std::vector<int> initial_nops, int alignment, int64_t max_block_transfers, FILE* file, std::ofstream* raw_file)
{
    int32_t data_col_width = 10;

    //==== Print Title ====
    std::string format = tableHeaderOpenLoop("Open Loop - One Way", file);

    //==== Allocate Array For the Largest Experiment ====
    std::vector<int32_t*> shared_array_locs;
    std::vector<std::atomic_int32_t*> shared_write_id_locs;
    std::vector<std::atomic_int32_t*> shared_read_id_locs;

    std::vector<int> cpus = {cpu_a, cpu_b};

    std::vector<std::atomic_flag*> ready_flags;
    std::vector<std::atomic_flag*> start_flags;
    std::atomic_flag* stop_flag;

    openLoopAllocate<int32_t, std::atomic_int32_t, std::atomic_int32_t>(shared_array_locs, shared_write_id_locs, shared_read_id_locs, ready_flags, start_flags, stop_flag, array_lengths, block_lengths, cpus, alignment, false, false);

    //==== Create Configurations for each experiment ====
    int num_experiments = array_lengths.size() * block_lengths.size() * balance_nops.size() * initial_nops.size();

    OpenLoopBufferArgs<int32_t, std::atomic_int32_t, std::atomic_int32_t>* args = new OpenLoopBufferArgs<int32_t, std::atomic_int32_t, std::atomic_int32_t>[num_experiments];

    for(int i = 0; i<array_lengths.size(); i++)
    {
        size_t array_length = array_lengths[i];
        for(int j = 0; j<block_lengths.size(); j++)
        {
            int32_t block_length = block_lengths[j];
            for(int k = 0; k<balance_nops.size(); k++){
                int32_t balance_nop = balance_nops[k];
                for(int l = 0; l<initial_nops.size(); l++){
                    int initial_nop = initial_nops[l];
                    int idx = l+
                              k*initial_nops.size()+
                              j*initial_nops.size()*balance_nops.size()+
                              i*initial_nops.size()*balance_nops.size()*block_lengths.size();

                    args[idx].read_offset_ptr = shared_read_id_locs[0];
                    args[idx].write_offset_ptr = shared_write_id_locs[0];
                    args[idx].array = shared_array_locs[0];
                    args[idx].start_flag = start_flags[0];
                    args[idx].stop_flag = stop_flag;
                    args[idx].ready_flag = ready_flags[0];
                    args[idx].array_length = array_length;
                    args[idx].max_block_transfers = max_block_transfers;
                    args[idx].balancing_nops = balance_nop;
                    args[idx].blockSize = block_length;
                    args[idx].alignment = alignment;
                    args[idx].core_server = cpus[0]; 
                    args[idx].core_client = cpus[1]; 
                    args[idx].initialNOPs = initial_nop;
                }
            }
        }
    }

    //==== Run The Experiments ====
    //The primary is the client (because it performs the measurment) and the secondary is the server
    std::vector<Results> results_vec = execute_client_server_kernel(profiler, open_loop_buffer_server<int32_t, std::atomic_int32_t, std::atomic_int32_t, int32_t, int32_t, INT32_MAX>,
                                                                    open_loop_buffer_client<int32_t, std::atomic_int32_t, std::atomic_int32_t, int32_t, int32_t, INT32_MAX>,
                                                                    open_loop_buffer_reset<int32_t, std::atomic_int32_t, std::atomic_int32_t>,
                                                                    open_loop_buffer_cleanup<int32_t, std::atomic_int32_t, std::atomic_int32_t>, 
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
    writeRawHeaderOpenLoop(implSpecificResultExamples, raw_file);

    //Print results
    for(int i = 0; i<results_vec.size(); i++){
        args[i].printExportCorrespondingResult(results_vec[i], false, "Open Loop - One Way", profiler, cpus, format, file, raw_file);
    }

    closeEntry(file, raw_file);

    //==== Cleanup ====
    destructSharedIDs(shared_write_id_locs, shared_read_id_locs, ready_flags, start_flags, stop_flag);

    freeVectorContents(shared_array_locs);
    freeVectorContents(shared_write_id_locs);
    freeVectorContents(shared_read_id_locs);
    freeVectorContents(ready_flags);
    freeVectorContents(start_flags);
    free(stop_flag);
    delete[] args;
}

//MAKE A 2D Table
void run_open_loop_fullness_tracker_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, std::vector<int32_t> balance_nops, std::vector<int> initial_nops, std::vector<int> checkPeriods, int alignment, int64_t max_block_transfers, int trackerLen, FILE* file, std::ofstream* raw_file)
{
    int32_t data_col_width = 10;

    //==== Print Title ====
    std::string format = tableHeaderOpenLoop("Open Loop - One Way (Fullness Tracker)", file);

    //==== Allocate Array For the Largest Experiment ====
    std::vector<int32_t*> shared_array_locs;
    std::vector<std::atomic_int32_t*> shared_write_id_locs;
    std::vector<std::atomic_int32_t*> shared_read_id_locs;

    std::vector<int> cpus = {cpu_a, cpu_b};

    std::vector<std::atomic_flag*> ready_flags;
    std::vector<std::atomic_flag*> start_flags;
    std::atomic_flag* stop_flag;
    std::vector<int32_t*> startTrackers;
    std::vector<int32_t*> startInterruptTrackers;
    std::vector<int32_t*> endTrackers;
    std::vector<int32_t*> endInterruptTrackers;
    std::vector<FILE*> interruptReporterFiles;

    openLoopFullnessTrackerAllocate<int32_t, std::atomic_int32_t, std::atomic_int32_t>(shared_array_locs, shared_write_id_locs, shared_read_id_locs, ready_flags, start_flags, stop_flag, startTrackers, startInterruptTrackers, endTrackers, endInterruptTrackers, array_lengths, block_lengths, cpus, alignment, false, false, trackerLen, trackerLen);

    for(int i = 0; i<cpus.size(); i++){
        FILE* interruptReporter = fopen("/dev/sir0", "r");
        if(interruptReporter == NULL){
            std::cerr << "Unable to open /dev/sir0.  Check that the sir module is loaded" << std::endl;
            exit(1);
        }

        interruptReporterFiles.push_back(interruptReporter);
    }

    //==== Create Configurations for each experiment ====
    int num_experiments = array_lengths.size() * block_lengths.size() * balance_nops.size() * initial_nops.size() * checkPeriods.size();

    OpenLoopFullnessTrackerBufferArgs<int32_t, std::atomic_int32_t, std::atomic_int32_t>* args = new OpenLoopFullnessTrackerBufferArgs<int32_t, std::atomic_int32_t, std::atomic_int32_t>[num_experiments];

    for(int i = 0; i<array_lengths.size(); i++)
    {
        size_t array_length = array_lengths[i];
        for(int j = 0; j<block_lengths.size(); j++)
        {
            int32_t block_length = block_lengths[j];
            for(int k = 0; k<balance_nops.size(); k++){
                int32_t balance_nop = balance_nops[k];
                for(int l = 0; l<initial_nops.size(); l++){
                    int initial_nop = initial_nops[l];
                    for(int m = 0; m<checkPeriods.size(); m++){
                        int checkPeriod = checkPeriods[m];
                        int idx = m+ 
                                  l*checkPeriods.size()+
                                  k*checkPeriods.size()*initial_nops.size()+
                                  j*checkPeriods.size()*initial_nops.size()*balance_nops.size()+
                                  i*checkPeriods.size()*initial_nops.size()*balance_nops.size()*block_lengths.size();

                        args[idx].read_offset_ptr = shared_read_id_locs[0];
                        args[idx].write_offset_ptr = shared_write_id_locs[0];
                        args[idx].array = shared_array_locs[0];
                        args[idx].start_flag = start_flags[0];
                        args[idx].stop_flag = stop_flag;
                        args[idx].ready_flag = ready_flags[0];
                        args[idx].array_length = array_length;
                        args[idx].max_block_transfers = max_block_transfers;
                        args[idx].balancing_nops = balance_nop;
                        args[idx].blockSize = block_length;
                        args[idx].alignment = alignment;
                        args[idx].core_server = cpus[0]; //Making the writer the server so that it performs the reset and initialization (will have exclusive access to the cache lines it writes)
                        args[idx].core_client = cpus[1]; //The client is secondary even through it does the measurements and error checks.  This will cause the timing measurement to be slightly off as there is a delay for the failure thread to be analyzed
                        args[idx].initialNOPs = initial_nop;
                        args[idx].checkPeriod = checkPeriod;
                        args[idx].startTracker = startTrackers[0];
                        args[idx].startInterruptTracker = startInterruptTrackers[0];
                        args[idx].startTrackerLen = trackerLen;
                        args[idx].endTracker = endTrackers[0];
                        args[idx].endInterruptTracker = endInterruptTrackers[0];
                        args[idx].endTrackerLen = trackerLen;
                        args[idx].readerInterruptReporter = interruptReporterFiles[0];
                        args[idx].writerInterruptReporter = interruptReporterFiles[1];
                    }
                }
            }
        }
    }

    //==== Run The Experiments ====
    //The primary is the writer which also performs the reset.
    //The reader still controls starting the writer thread internally (an additional handshake to the primary/secondary handshake)
    //The reader will read the entries that do not have initial values in them so that they are in the shared state.  This should cause the writer to need to invalidate the lines.
    //The reader will not read the 
    std::vector<Results> results_vec = execute_client_server_kernel(profiler, open_loop_buffer_server<int32_t, std::atomic_int32_t, std::atomic_int32_t, int32_t, int32_t, INT32_MAX>,
                                                                    open_loop_fullness_tracker_buffer_client<int32_t, std::atomic_int32_t, std::atomic_int32_t, int32_t, int32_t, INT32_MAX>,
                                                                    open_loop_fullness_tracker_buffer_reset<int32_t, std::atomic_int32_t, std::atomic_int32_t>,
                                                                    open_loop_buffer_cleanup<int32_t, std::atomic_int32_t, std::atomic_int32_t>, 
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
    writeRawHeaderOpenLoopFullnessTracker(implSpecificResultExamples, raw_file);

    //Print results
    for(int i = 0; i<results_vec.size(); i++){
        args[i].printExportCorrespondingResult(results_vec[i], false, "Open Loop - One Way (Fullness Tracker)", profiler, cpus, format, file, raw_file);
    }

    closeEntry(file, raw_file);

    //==== Cleanup ====
    for(int i = 0; i<interruptReporterFiles.size(); i++){
        fclose(interruptReporterFiles[i]);
    }

    destructSharedIDs(shared_write_id_locs, shared_read_id_locs, ready_flags, start_flags, stop_flag);

    freeVectorContents(startTrackers);
    freeVectorContents(startInterruptTrackers);
    freeVectorContents(endTrackers);
    freeVectorContents(endInterruptTrackers);
    freeVectorContents(shared_array_locs);
    freeVectorContents(shared_write_id_locs);
    freeVectorContents(shared_read_id_locs);
    freeVectorContents(ready_flags);
    freeVectorContents(start_flags);
    free(stop_flag);
    delete[] args;
}
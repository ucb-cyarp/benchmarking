#include "open_loop_kernels.h"
#include "open_loop_buffer.h"
#include "open_loop_helpers.h"
#include "open_loop_fullness_tracker.h"
#include "open_loop_run_past_failure.h"

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
    std::vector<BenchmarkSpecificResult*> implSpecificResultExamples;
    if(results_vec[0].trial_results.size() > 0){
        if(results_vec[0].trial_results[0]->benchmarkSpecificResults.size() > 0){
            implSpecificResultExamples = results_vec[0].trial_results[0]->benchmarkSpecificResults;
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
    std::vector<INTERRUPT_TRACKER_TYPE*> startStdInterruptTrackers;
    std::vector<INTERRUPT_TRACKER_TYPE*> startLocInterruptTrackers;
    std::vector<INTERRUPT_TRACKER_TYPE*> startOtherArchInterruptTrackers;
    std::vector<INTERRUPT_TRACKER_TYPE*> startSoftirqTimerInterruptTrackers;
    std::vector<INTERRUPT_TRACKER_TYPE*> startSoftirqOtherInterruptTrackers;
    std::vector<double*> startTimingTrackers;
    std::vector<int32_t*> endTrackers;
    std::vector<INTERRUPT_TRACKER_TYPE*> endStdInterruptTrackers;
    std::vector<INTERRUPT_TRACKER_TYPE*> endLocInterruptTrackers;
    std::vector<INTERRUPT_TRACKER_TYPE*> endOtherArchInterruptTrackers;
    std::vector<INTERRUPT_TRACKER_TYPE*> endSoftirqTimerInterruptTrackers;
    std::vector<INTERRUPT_TRACKER_TYPE*> endSoftirqOtherInterruptTrackers;
    std::vector<double*> endTimingTrackers;
    #if TRACK_INTERRUPTS>0 || DISABLE_INTERRUPTS>0
        std::vector<FILE*> interruptReporterFiles;
    #endif
    std::vector<std::vector<FifolessBufferFullnessTrackerEndCondition*>> readerResults;
    std::vector<std::vector<FifolessBufferFullnessTrackerEndCondition*>> writerResults;

    int num_experiments = array_lengths.size() * block_lengths.size() * balance_nops.size() * initial_nops.size() * checkPeriods.size();

    openLoopFullnessTrackerAllocate<int32_t, std::atomic_int32_t, std::atomic_int32_t>(shared_array_locs, 
                                                                                       shared_write_id_locs, 
                                                                                       shared_read_id_locs, 
                                                                                       ready_flags, 
                                                                                       start_flags, 
                                                                                       stop_flag, 
                                                                                       startTrackers, 
                                                                                       startStdInterruptTrackers, 
                                                                                       startLocInterruptTrackers, 
                                                                                       startOtherArchInterruptTrackers, 
                                                                                       startSoftirqTimerInterruptTrackers, 
                                                                                       startSoftirqOtherInterruptTrackers, 
                                                                                       startTimingTrackers, 
                                                                                       endTrackers, 
                                                                                       endStdInterruptTrackers, 
                                                                                       endLocInterruptTrackers, 
                                                                                       endOtherArchInterruptTrackers, 
                                                                                       endSoftirqTimerInterruptTrackers, 
                                                                                       endSoftirqOtherInterruptTrackers, 
                                                                                       endTimingTrackers, 
                                                                                       readerResults,
                                                                                       writerResults,
                                                                                       array_lengths, 
                                                                                       block_lengths, 
                                                                                       cpus, 
                                                                                       alignment, 
                                                                                       false, false, 
                                                                                       trackerLen, trackerLen, num_experiments, TRIALS);


    #if TRACK_INTERRUPTS>0 || DISABLE_INTERRUPTS>0
        for(int i = 0; i<cpus.size(); i++){
            FILE* interruptReporter = fopen("/dev/sir0", "r");
            if(interruptReporter == NULL){
                std::cerr << "Unable to open /dev/sir0.  Check that the sir module is loaded" << std::endl;
                exit(1);
            }

            interruptReporterFiles.push_back(interruptReporter);
        }
    #endif

    //==== Create Configurations for each experiment ====
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
                        args[idx].startStdInterruptTracker = startStdInterruptTrackers[0];
                        args[idx].startLocInterruptTracker = startLocInterruptTrackers[0];
                        args[idx].startOtherArchInterruptTracker = startOtherArchInterruptTrackers[0];
                        args[idx].startSoftirqTimerInterruptTracker = startSoftirqTimerInterruptTrackers[0];
                        args[idx].startSoftirqOtherInterruptTracker = startSoftirqOtherInterruptTrackers[0];
                        args[idx].startTimingTracker = startTimingTrackers[0];
                        args[idx].startStdInterruptTrackerWriter = startStdInterruptTrackers[1];
                        args[idx].startLocInterruptTrackerWriter = startLocInterruptTrackers[1];
                        args[idx].startOtherArchInterruptTrackerWriter = startOtherArchInterruptTrackers[1];
                        args[idx].startSoftirqTimerInterruptTrackerWriter = startSoftirqTimerInterruptTrackers[1];
                        args[idx].startSoftirqOtherInterruptTrackerWriter = startSoftirqOtherInterruptTrackers[1];
                        args[idx].startTimingTrackerWriter = startTimingTrackers[1];
                        args[idx].startTrackerLen = trackerLen;
                        args[idx].endTracker = endTrackers[0];
                        args[idx].endStdInterruptTracker = endStdInterruptTrackers[0];
                        args[idx].endLocInterruptTracker = endLocInterruptTrackers[0];
                        args[idx].endOtherArchInterruptTracker = endOtherArchInterruptTrackers[0];
                        args[idx].endSoftirqTimerInterruptTracker = endSoftirqTimerInterruptTrackers[0];
                        args[idx].endSoftirqOtherInterruptTracker = endSoftirqOtherInterruptTrackers[0];
                        args[idx].endTimingTracker = endTimingTrackers[0];
                        args[idx].endStdInterruptTrackerWriter = endStdInterruptTrackers[1];
                        args[idx].endLocInterruptTrackerWriter = endLocInterruptTrackers[1];
                        args[idx].endOtherArchInterruptTrackerWriter = endOtherArchInterruptTrackers[1];
                        args[idx].endSoftirqTimerInterruptTrackerWriter = endSoftirqTimerInterruptTrackers[1];
                        args[idx].endSoftirqOtherInterruptTrackerWriter = endSoftirqOtherInterruptTrackers[1];
                        args[idx].endTimingTrackerWriter = endTimingTrackers[1];
                        args[idx].endTrackerLen = trackerLen;
                        #if TRACK_INTERRUPTS>0 || DISABLE_INTERRUPTS>0
                            args[idx].writerInterruptReporter = interruptReporterFiles[0];
                            args[idx].readerInterruptReporter = interruptReporterFiles[1];
                        #endif
                        args[idx].readerRtn = readerResults[idx]; //This will result in copying the pointer vector but that is OK since we have not started trials yet
                        args[idx].writerRtn = writerResults[idx]; //This will result in copying the pointer vector but that is OK since we have not started trials yet
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
    std::vector<Results> results_vec = execute_client_server_kernel(profiler, open_loop_fullness_tracker_buffer_server<int32_t, std::atomic_int32_t, std::atomic_int32_t, int32_t, int32_t, INT32_MAX>,
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
    std::vector<BenchmarkSpecificResult*> implSpecificResultExamples;
    if(results_vec[0].trial_results.size() > 0){
        if(results_vec[0].trial_results[0]->benchmarkSpecificResults.size() > 0){
            implSpecificResultExamples = results_vec[0].trial_results[0]->benchmarkSpecificResults;
        }
    }
    writeRawHeaderOpenLoopFullnessTracker(implSpecificResultExamples, raw_file);

    //Print results
    for(int i = 0; i<results_vec.size(); i++){
        args[i].printExportCorrespondingResult(results_vec[i], false, "Open Loop - One Way (Fullness Tracker)", profiler, cpus, format, file, raw_file);
    }

    closeEntry(file, raw_file);

    //==== Cleanup ====
    #if TRACK_INTERRUPTS>0 || DISABLE_INTERRUPTS>0
        for(int i = 0; i<interruptReporterFiles.size(); i++){
            fclose(interruptReporterFiles[i]);
        }
    #endif

    destructSharedIDs(shared_write_id_locs, shared_read_id_locs, ready_flags, start_flags, stop_flag);

    freeVectorContents(startTrackers);
    freeVectorContents(startStdInterruptTrackers);
    freeVectorContents(startLocInterruptTrackers);
    freeVectorContents(startOtherArchInterruptTrackers);
    freeVectorContents(startSoftirqTimerInterruptTrackers);
    freeVectorContents(startSoftirqOtherInterruptTrackers);
    freeVectorContents(startTimingTrackers);
    freeVectorContents(endTrackers);
    freeVectorContents(endStdInterruptTrackers);
    freeVectorContents(endLocInterruptTrackers);
    freeVectorContents(endOtherArchInterruptTrackers);
    freeVectorContents(endSoftirqTimerInterruptTrackers);
    freeVectorContents(endSoftirqOtherInterruptTrackers);
    freeVectorContents(endTimingTrackers);
    freeVectorContents(shared_array_locs);
    freeVectorContents(shared_write_id_locs);
    freeVectorContents(shared_read_id_locs);
    freeVectorContents(ready_flags);
    freeVectorContents(start_flags);
    free(stop_flag);
    delete[] args;
    //Note: readerResults and writerResults are freed by smartpointers which are freed when the Result vectors are freed
}

void run_open_loop_run_past_failure_kernel(Profiler* profiler, int cpu_a, int cpu_b, std::vector<size_t> array_lengths, std::vector<int32_t> block_lengths, std::vector<int32_t> balance_nops, std::vector<int> initial_nops, int numUnderOverflowRecords, int alignment, int64_t max_block_transfers, FILE* file, std::ofstream* raw_file)
{
    int32_t data_col_width = 10;

    //==== Print Title ====
    std::string format = tableHeaderOpenLoop("Open Loop - One Way - Run Past Failure", file);

    //==== Allocate Array For the Largest Experiment ====
    std::vector<int32_t*> shared_array_locs;
    std::vector<std::atomic_int32_t*> shared_write_id_locs;
    std::vector<std::atomic_int32_t*> shared_read_id_locs;

    std::vector<int> cpus = {cpu_a, cpu_b};

    std::vector<std::atomic_flag*> ready_flags;
    std::vector<std::atomic_flag*> start_flags;
    std::atomic_flag* stop_flag;

    std::vector<int64_t*> underOverflowIterations;
    std::vector<int16_t*> underOverflowAmounts;
    std::vector<bool*> underOverflowPartials;

    openLoopRunPastFailureAllocate<int32_t, std::atomic_int32_t, std::atomic_int32_t>(shared_array_locs, shared_write_id_locs, shared_read_id_locs, ready_flags, start_flags, stop_flag, underOverflowIterations, underOverflowAmounts, underOverflowPartials, array_lengths, block_lengths, cpus, alignment, false, false, numUnderOverflowRecords);

    //==== Create Configurations for each experiment ====
    int num_experiments = array_lengths.size() * block_lengths.size() * balance_nops.size() * initial_nops.size();

    OpenLoopPastFailureBufferArgs<int32_t, std::atomic_int32_t, std::atomic_int32_t>* args = new OpenLoopPastFailureBufferArgs<int32_t, std::atomic_int32_t, std::atomic_int32_t>[num_experiments];

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

                    args[idx].underOverflowIterations = underOverflowIterations[0];
                    args[idx].underOverflowAmounts = underOverflowAmounts[0];
                    args[idx].underOverflowPartial = underOverflowPartials[0];
                    args[idx].maxUnderOverFlowsRecording = numUnderOverflowRecords;
                }
            }
        }
    }

    //==== Run The Experiments ====
    //The primary is the client (because it performs the measurment) and the secondary is the server
    std::vector<Results> results_vec = execute_client_server_kernel(profiler, open_loop_buffer_server<int32_t, std::atomic_int32_t, std::atomic_int32_t, int32_t, int32_t, INT32_MAX>,
                                                                    open_loop_buffer_past_failure_client<int32_t, std::atomic_int32_t, std::atomic_int32_t, int32_t, int32_t, INT32_MAX>,
                                                                    open_loop_buffer_past_failure_reset<int32_t, std::atomic_int32_t, std::atomic_int32_t>,
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
    std::vector<BenchmarkSpecificResult*> implSpecificResultExamples;
    if(results_vec[0].trial_results.size() > 0){
        if(results_vec[0].trial_results[0]->benchmarkSpecificResults.size() > 0){
            implSpecificResultExamples = results_vec[0].trial_results[0]->benchmarkSpecificResults;
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
    freeVectorContents(underOverflowIterations);
    freeVectorContents(underOverflowAmounts);
    freeVectorContents(underOverflowPartials);
    free(stop_flag);
    delete[] args;
}
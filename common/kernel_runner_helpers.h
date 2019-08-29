#ifndef _H_KERNEL_RUNNER_HELPERS
    #define _H_KERNEL_RUNNER_HELPERS

    #include "profiler.h"
    #include "intrin_bench_default_defines.h"
    #include <vector>

    //Define some helper functions to be inlined
    
    //Check for frequency change events.  Discards sample if freq change event occured.  If not, adds result to results.  Prepares for next run in either case
    inline bool processTrialAndPrepareForNextHelper(Profiler* profiler, Results &results, TrialResult &trial_result, int &trial, int &discard_count, std::vector<int> socketsOfInterest = {}){
            bool freq_change_events_occured = profiler->detectsFreqChange() ? profiler->checkFreqChanged(socketsOfInterest) : false;
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

#endif
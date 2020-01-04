#include "kernel_server_runner.h"
#include <atomic>

void* kernel_exe_primary_wrapper(void *arg)
{
    KernelExePrimaryWrapperArgs* args = (KernelExePrimaryWrapperArgs*) arg;

    //Get args from object
    Profiler* profiler = args->profiler;
    void* (*kernel_fun)(void*) = args->kernel_fun;
    void* (*reset_fun)(void*) = args->reset_fun;
    int numExperiments = args->num_args;
    int numTrials = args->num_trials;

    //Perform config check for interconnected primaries
    if(args->interconnected_primaries){
        if(!args->interconnected_master){
            if(args->start_new_trial_signals_from_master.size() != 1 || 
                args->restart_trial_signals_from_master.size() != 1 ||
                args->start_new_trial_signals_to_master.size() != 1 ||
                args->restart_trial_signals_to_master.size() != 1){
                std::cerr << "Error: slave processes should have exactly one signal to & from the master process" << std::endl;
                exit(1);
            }
        }else{
            if(args->start_new_trial_signals_from_master.size() != args->restart_trial_signals_from_master.size()){
                std::cerr << "Error: start_new_trial_signals_from_master.size() != restart_trial_signals_from_master.size()" << std::endl;
                exit(1);
            }

            if(args->start_new_trial_signals_to_master.size() != args->restart_trial_signals_to_master.size()){
                std::cerr << "Error: start_new_trial_signals_to_master.size() != restart_trial_signals_to_master.size()" << std::endl;
                exit(1);
            }
        }
    }else{
        if(!args->start_new_trial_signals_to_master.empty() || 
            !args->restart_trial_signals_to_master.empty() || 
            !args->start_new_trial_signals_from_master.empty() || 
            !args->restart_trial_signals_from_master.empty()){
            std::cerr << "Error: Interconnected primaries vectors should be empty if interconnected_primaries is false" << std::endl;
            exit(1);
        }
    }

    std::vector<Results> *resultVec = new std::vector<Results>;

    //=== Logic for interconnected primaries ===
    //Wait for initial signal from master
    if(args->interconnected_primaries && !args->interconnected_master){
        bool cont = false;
        while (!cont){
            bool start_new_trial = !(std::atomic_flag_test_and_set_explicit(args->start_new_trial_signals_from_master[0], std::memory_order_acq_rel)); 
            bool restart_trial = !(std::atomic_flag_test_and_set_explicit(args->restart_trial_signals_from_master[0], std::memory_order_acq_rel)); 

            if(restart_trial){
                std::cerr << "Error: First signal recieved by secondary slave should be to start a new trial" << std::endl;
                exit(1);
            }else if(start_new_trial){
                cont = true;
            }//else, continue waiting
        }

        // std::cout << "Primary Slave Got the Initial Signal" << std::endl;
    }
    //=== End for interconnected primaries ===

    for(int experiment = 0; experiment < numExperiments; experiment++){
        void* kernel_arg = (void*) (((char*) args->kernel_args)+experiment*(args->kernel_arg_size));
        void* reset_arg = (void*) (((char*) args->reset_args)+experiment*(args->reset_arg_size));
        int cpu_num = args->cpu_num;

        Results results; //Trials are added to results
        int discard_count = 0;

        bool lastTrialWasDiscarded = false;
        for(int trial = 0; trial < numTrials; ){ //Trial is incremented by processTrialAndPrepareForNextHelper as appropriate
            //Reset the experiment state for new trial
            //Reset if not a member of interconnected primaries or if it is the master interconnected primary
            if(!(args->interconnected_primaries) || (args->interconnected_primaries && args->interconnected_master)){
                reset_fun(reset_arg);
            }

            //=== Logic for interconnected primaries ===
            //Signal to other threads
            if(args->interconnected_primaries && args->interconnected_master){
                std::atomic_thread_fence(std::memory_order_acquire);
                for(int i = 0; i<args->start_new_trial_signals_from_master.size(); i++){
                    if(lastTrialWasDiscarded){
                        std::atomic_flag_clear_explicit(args->restart_trial_signals_from_master[i], std::memory_order_release);
                    }else{
                        std::atomic_flag_clear_explicit(args->start_new_trial_signals_from_master[i], std::memory_order_release);
                    }
                }
            }
            //=== End Logic for interconnected primaries ===

            std::atomic_thread_fence(std::memory_order_acquire);
            if(lastTrialWasDiscarded){
                //Set restart_trial flags (which is actually clearing them because only test_and_set is available)
                for(unsigned long i = 0; i<args->restart_trial_signals.size(); i++){
                    std::atomic_flag_clear_explicit(args->restart_trial_signals[i], std::memory_order_release);
                }
            }else{
                //Set start_new_trial flags (which is actually clearing them because only test_and_set is available)
                for(unsigned long i = 0; i<args->start_new_trial_signals.size(); i++){
                    std::atomic_flag_clear_explicit(args->start_new_trial_signals[i], std::memory_order_release);
                }
            }

            //Start the trial
            profiler->trialSetup();

            //Note that atomic_signal_fence should only effect compiler re-ordering and will not cause CPU fence instructions to be generated (see https://en.cppreference.com/w/c/atomic/atomic_signal_fence)
            std::atomic_signal_fence(std::memory_order_acq_rel); //Place barriers around the calls to the instrimentation functions (getting time, reading MSRs, etc...) to prevent re-ordering.  This should only be nesssasary if link time optimizations are used
            profiler->startTrial();
            std::atomic_signal_fence(std::memory_order_acq_rel);

            //Run Kernel
            void* kernel_result = kernel_fun(kernel_arg);

            std::atomic_signal_fence(std::memory_order_acq_rel); //Place barriers around the calls to the instrimentation functions (getting time, reading MSRs, etc...) to prevent re-ordering.  This should only be nesssasary if link time optimizations are used
            profiler->endTrial();
            std::atomic_signal_fence(std::memory_order_acq_rel);

            //Free the Kernel Result
            free(kernel_result);
            
            //Compute the trial result
            TrialResult trial_result = computeTrialResultAndSetTrialNum(profiler, &results);

            #if PRINT_TRIALS == 1
                trial_result->print_trial();
            #endif 

            //Limit frequency change check to socket of interest (single socket for now)
            std::vector<int> sockets_of_interest;
            sockets_of_interest.push_back(profiler->cpuTopology[cpu_num].socket);

            //This function adds the trial result if no frequency change event occured (if the profiler supports checking)
            //Important: This function increments trial if no frequency change event occured.  It also increments discard_count if a discard occured
            lastTrialWasDiscarded = processTrialAndPrepareForNextHelper(profiler, results, trial_result, trial, discard_count);

            //Wait for client threads to finish before next trial
            for(unsigned long i = 0; i<args->done_signals.size(); i++){
                bool val = true;
                while(val){
                    //Done is signaled by val being false (the other thread calling atomic_flag_clear).
                    //It is set by this call to be true again for the next cycle
                    val = std::atomic_flag_test_and_set_explicit(args->done_signals[i], std::memory_order_acq_rel); 
                }
            }

            //=== Logic for interconnected primaries ===
            if(args->interconnected_primaries){
                if(args->interconnected_master){
                    std::vector<bool> finishedTracker;
                    int numberReturned = 0;
                    bool foundDiscardRemote = false;
                    for(int i = 0; i<args->start_new_trial_signals_to_master.size(); i++){
                        finishedTracker.push_back(false);
                    }

                    //Wait for other primaries to return
                    while(numberReturned<finishedTracker.size()){
                        for(int i = 0; i<finishedTracker.size(); i++){
                            if(!finishedTracker[i]){
                                bool start_new_trial = !(std::atomic_flag_test_and_set_explicit(args->start_new_trial_signals_to_master[i], std::memory_order_acq_rel)); 
                                bool restart_trial = !(std::atomic_flag_test_and_set_explicit(args->restart_trial_signals_to_master[i], std::memory_order_acq_rel)); 
                                if(start_new_trial && restart_trial){
                                    std::cerr << "Error: Kernel Master Wrapper Received Start and Restart Signal Simultaniously" << std::endl;
                                    exit(1);
                                }else if(start_new_trial || restart_trial){
                                    finishedTracker[i] = true;
                                    numberReturned++;

                                    foundDiscardRemote |= restart_trial;
                                }//else, still waiting
                            }//else, we already got a response from this one
                        }
                    }

                    //Determine if a discard should occure (including own results)
                    bool discardShouldOccure = lastTrialWasDiscarded | foundDiscardRemote;

                    //Discard if local decision was not to discard but group decision was to discard
                    if(discardShouldOccure && !lastTrialWasDiscarded){
                        discardLastTrial(results, trial, discard_count);
                    }

                    //Set lastTrialWasDiscarded to group decision so that any secondaries are properly informed of the discard on the next cycle
                    lastTrialWasDiscarded = discardShouldOccure;

                    //Signal decision to other primaries after inter-trial reset (only the master resets)
                    //This occurs in the next loop iteration
                }else{
                    //Signal local decision to master primary
                    if(lastTrialWasDiscarded){
                        std::atomic_flag_clear_explicit(args->restart_trial_signals_to_master[0], std::memory_order_release);
                    }else{
                        std::atomic_flag_clear_explicit(args->start_new_trial_signals_to_master[0], std::memory_order_release);
                    }

                    //Wait for decision from master (guarenteed to have 1 master - checked at start)
                    bool remote_discard_found = false;
                    bool cont = false;
                    while (!cont){
                        bool start_new_trial = !(std::atomic_flag_test_and_set_explicit(args->start_new_trial_signals_from_master[0], std::memory_order_acq_rel)); 
                        bool restart_trial = !(std::atomic_flag_test_and_set_explicit(args->restart_trial_signals_from_master[0], std::memory_order_acq_rel)); 

                        if(start_new_trial && restart_trial){
                            std::cerr << "Error: Recieved both the flag to start a new trial and the flag to restart the last trial" << std::endl;
                            exit(1);
                        }else if(start_new_trial || restart_trial){
                            cont = true;

                            remote_discard_found |= restart_trial;
                        }//else, continue waiting
                    }

                    //Discard if local decision was not to discard but group decision was to discard
                    if(remote_discard_found && !lastTrialWasDiscarded){
                        discardLastTrial(results, trial, discard_count);
                    }

                    //Set lastTrialWasDiscarded to group decision so that any secondaries are properly informed of the discard on the next cycle
                    lastTrialWasDiscarded = remote_discard_found; //Note that the master took what this thread returned into account when deciding if a discard occured
                }
            }
            //=== End Logic for interconnected primaries ===

        }

        resultVec->push_back(results);
    }

    //==== Send the Final Success Flags ====
    //If the master, signal to the other primaries
    if(args->interconnected_primaries && args->interconnected_master){
        std::atomic_thread_fence(std::memory_order_acquire);
        for(int i = 0; i<args->start_new_trial_signals_from_master.size(); i++){
            std::atomic_flag_clear_explicit(args->start_new_trial_signals_from_master[i], std::memory_order_release);
        }
    }

    //Send to the secondaries
    std::atomic_thread_fence(std::memory_order_acquire);
    for(unsigned long i = 0; i<args->start_new_trial_signals.size(); i++){
        std::atomic_flag_clear_explicit(args->start_new_trial_signals[i], std::memory_order_release);
    }

    return (void*) resultVec;
}

void* kernel_exe_secondary_wrapper(void *arg){
    KernelExeSecondaryWrapperArgs* args = (KernelExeSecondaryWrapperArgs*) arg;

    //Get args from object
    void* (*kernel_fun)(void*) = args->kernel_fun;
    int numExperiments = args->num_args;
    int numTrials = args->num_trials;

    //Wait for first trial (should always be signaled by start_new_trial_signal)
    bool execute = false;
    while(!execute){
        //Invert because the flag is effectivly active low (signaled by clearring)
        bool start_new_trial = !(std::atomic_flag_test_and_set_explicit(args->start_new_trial_signal, std::memory_order_acq_rel)); 
        bool restart_trial = !(std::atomic_flag_test_and_set_explicit(args->restart_trial_signal, std::memory_order_acq_rel)); 

        if(restart_trial){
            std::cerr << "Error: Secondary Kernel Wrapper Received Restart Signal at the Start of the Benchmark" << std::endl;
            exit(1);
        }
        execute = start_new_trial;
    }

    for(int experiment = 0; experiment < numExperiments; experiment++){
        void* kernel_arg = (void*) (((char*) args->kernel_args)+experiment*(args->kernel_arg_size));

        for(int trial = 0; trial < numTrials; ){
            //Run the kernel
            kernel_fun(kernel_arg);

            //Set the done flag
            for(int i = 0; i<args->done_signals.size(); i++){
                std::atomic_flag_clear_explicit(args->done_signals[i], std::memory_order_acq_rel);
            }

            //Wait for the start flag which will indicate if trial should be incremented or not
            bool cont = false;
            while (!cont){
                bool start_new_trial = !(std::atomic_flag_test_and_set_explicit(args->start_new_trial_signal, std::memory_order_acq_rel)); 
                bool restart_trial = !(std::atomic_flag_test_and_set_explicit(args->restart_trial_signal, std::memory_order_acq_rel)); 

                if(start_new_trial && restart_trial){
                    std::cerr << "Error: Recieved both the flag to start a new trial and the flag to restart the last trial" << std::endl;
                    exit(1);
                }else if(start_new_trial){
                    //Increment the trial and continue
                    trial++;
                    cont = true;
                }else if(restart_trial){
                    //Do not increment the trial and continue
                    cont = true;
                }//else, wait for flags
            }
        }
    }

    return NULL;
}
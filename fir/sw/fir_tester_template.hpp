/*
 * This is a generic tester which can be used with
 * any FIR filter implementation.  To use this tester, simply create a
 * .cpp file, include the version of the FIR filter you would like to test,
 * define TESTER_HEADER_STR, and finally include this file.
 * 
 * Exersises fir filter by feeding random data into the filter.
 * 
 * This version precomputes the stimulus vector.
 * The timing results should therefore only include the actual
 * FIR filtering.
 * 
 * Compiler Parameters (defined as macros)
 *     - DATATYPE:     Datatape 
 *     - COEF_LEN:     Coefficient Length
 *     - IO_LEN:       Stimulus Vector Length
 *     - STIM_LEN:     Stimulus Length
 *     - TRIALS:       Number of Trial Runs
 *     - RAND_MEAN:    Random Number Mean
 *     - RAND_STDDEV:  Random Number StdDev
 *     - RAND_SEED:    Random Number Generator Seed
 *     - PRINT_TITLE:  1 to Print Title Text
 *     - PRINT_TRIALS: 1 to Print Each Trial
 *     - PRINT_STATS:  1 to Print Statistics
 *     - WRITE_CSV:    1 to Write Results to CSV 
 * 
 * Arguments
 *     - CSV Filename (only used when WRITE_CSV == TRUE)
 */

#include <chrono>
#include <ctime>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <iostream>
#include <cstdint>

#ifndef DATATYPE
    #define DATATYPE double
#endif

#ifndef COEF_LEN
    #define COEF_LEN 60
#endif

//IO_LEN should be a factor of STIM_LEN, otherwise

#ifndef IO_LEN
    #define IO_LEN 20
#endif

#ifndef STIM_LEN
    #define STIM_LEN 1000000
#endif

#ifndef TRIALS
    #define TRIALS 10
#endif

#ifndef RAND_MEAN
    #define RAND_MEAN 100.0
#endif

#ifndef RAND_STDDEV
    #define RAND_STDDEV 1000.0
#endif

#ifndef RAND_SEED
    #define RAND_SEED 510582
#endif

#ifndef PRINT_TITLE
    #define PRINT_TITLE 1
#endif

#ifndef PRINT_TRIALS
    #define PRINT_TRIALS 1
#endif

#ifndef PRINT_STATS
    #define PRINT_STATS 1
#endif

#ifndef WRITE_CSV
    #define WRITE_CSV 0
#endif

#ifndef TESTER_HEADER_STR
    #define TESTER_HEADER_STR "Unknown Test\n"
#endif

struct cli_args{
    int argc;
    char* argv[];
    
    int cpu_number;
};

void* run_benchmark(void* args_ptr)
{
    cli_args* args = (cli_args*) args_ptr;

    int argc = args->argc;
    char *argv[] = args->argv;

    printf("\n");
    printf("****** Platform Information Provided by PCM ******\n");
    PCM* pcm = init_PCM();

    printf("**************************************************\n");
    printf("CPU Brand String: %s\n", pcm->getCPUBrandString().c_str());
    printf("**************************************************\n");

    int* cpu_num_int = (int*) cpu_num;
    int socket = pcm->getSocketId(*cpu_num_int);
    printf("Executing on Core: %3d (Socket: %2d)\n", *cpu_num_int, socket);
    printf("**************************************************\n");
    printf("\n");

    int sockets = pcm->getNumSockets();
    int cores = pcm->getNumCores();
    Results* results = new Results(sockets, cores);

    //Allocate measurement arrays
    std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];

    //Allocate counter states
    ServerUncorePowerState* startPowerState = new ServerUncorePowerState[sockets];
    ServerUncorePowerState* endPowerState = new ServerUncorePowerState[sockets];
    std::vector<CoreCounterState> startCstates, endCstates;
    std::vector<SocketCounterState> startSktstate, endSktstate;
    SystemCounterState startSstate, endSstate;

    //Allocate Arrays on Heap
    DATATYPE *stimulus = new DATATYPE[STIM_LEN];
    DATATYPE *output = new DATATYPE[STIM_LEN];
    DATATYPE *coefs = new DATATYPE[COEF_LEN];
    DATATYPE *init = new DATATYPE[COEF_LEN];

    #if PRINT_TITLE == 1
    printf(TESTER_HEADER_STR);
    printf("COEF_LEN: %d, IO_LEN: %d, STIM_LEN: %d, TRIALS: %d\n", COEF_LEN, IO_LEN, STIM_LEN, TRIALS);
    #endif

    int trial = 0;
    int discard_count = 0;
    while(trial<TRIALS)
    {
        //Construct FIR
        Fir<DATATYPE, COEF_LEN, IO_LEN> fir1(coefs, init);

        //Get CPU Core/Socket/Power States
        for (int i = 0; i < sockets; i++)
            startPowerState[i] = pcm->getServerUncorePowerState(i);
        pcm->getAllCounterStates(startSstate, startSktstate, startCstates);

        //Start Timer
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        clock_t start_clock = clock();
        uint64_t start_rdtsc = _rdtsc();

        //Run Kernel
        for(size_t i = 0; i<(STIM_LEN/IO_LEN); i++)
        {
            fir1.filter(&stimulus[IO_LEN*i], &output[IO_LEN*i]);
        }

        //Stop Timer 
        uint64_t stop_rdtsc = _rdtsc();
        clock_t stop_clock = clock();
        std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();

        //Get CPU Core/Socket/Power States
        pcm->getAllCounterStates(endSstate, endSktstate, endCstates);
        for (int i = 0; i < sockets; i++)
            endPowerState[i] = pcm->getServerUncorePowerState(i);
        
        TrialResult* trial_result = new TrialResult(sockets, cores, trial);

        //Report Time
        calculate_durations(trial_result->duration, trial_result->duration_clock, trial_result->duration_rdtsc, start, stop, start_clock, stop_clock, start_rdtsc, stop_rdtsc);
        
        //Report Freq, Power
        calc_freq_and_power(pcm, trial_result->avgCPUFreq, trial_result->avgActiveCPUFreq, trial_result->energyCPUUsed, trial_result->energyDRAMUsed,
        startCstates, endCstates, startPowerState, endPowerState);

        #if PRINT_TRIALS == 1
            trial_result->print_trial();
        #endif 

        //Limit check to socket of interest (single socket for now)
        std::vector<int> sockets_of_interest;
        sockets_of_interest.push_back(pcm->getSocketId(cpu_num));

        bool freq_change_events_occured = check_any_freq_changes(pcm, startPowerState, endPowerState, sockets_of_interest);
        //Proceed if no freq changes occured
        if(freq_change_events_occured == false)
        {
            trial++;
            results->add_trial(trial_result);
            discard_count=0;
        }
        
        else
        {
            discard_count++;
            #if PRINT_FREQ_CHANGE_EVENT == 1
                printf("Frequency Change Event Occured Durring Kernel Run ... Discarding Run\n");
            #endif

            if(discard_count >= MAX_DISCARD-1)
            {
                #if PRINT_FREQ_CHANGE_EVENT == 1
                    printf("Max Discards Reached ... Exiting\n");
                #endif
                exit(1);
            }
        }
    }

    #if PRINT_STATS == 1
        results->print_statistics(pcm->getSocketId(cpu_num), cpu_num);
    #endif

    #if WRITE_CSV == 1
    //Write CSV File
    if(argc >= 2)
    {
        std::ofstream csv_file;
        //printf("Writing results to %s\n", argv[1]);
        csv_file.open(argv[1]);

        if(csv_file.is_open())
        {
            csv_file << "\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\"" << std::endl;

            results->write_csv(csv_file, socket, core);

            csv_file.close();
        }
        else
        {
            fprintf(stderr, "Unable to open file\n");
        }
    }
    else
    {
        fprintf(stderr, "Need CSV Filename as First CLI Argument\n");
    }
    #endif

    //Cleanup
    delete startPowerState;
    delete endPowerState;

    delete[] stimulus;
    delete[] output;
    delete[] coefs;
    delete[] init;
}

int main(int argc, char *argv[])
{
    //Run these single-threaded benchmarks on CPU 0 (all machines should have CPU 0)

    //Based off of http://man7.org/linux/man-pages/man3/pthread_setaffinity_np.3.html
    //http://man7.org/linux/man-pages/man3/pthread_create.3.html
    //http://man7.org/linux/man-pages/man3/pthread_attr_setaffinity_np.3.html,
    //http://man7.org/linux/man-pages/man3/pthread_join.3.html

    cpu_set_t cpuset;
    pthread_t thread;
    pthread_attr_t attr;
    void *res;

    int status;

    status = pthread_attr_init(&attr);

    if(status != 0)
    {
        printf("Could not create pthread attributes ... exiting\n");
        exit(1);
    }

    int cpu_number = 0;

    CPU_ZERO(&cpuset);
    CPU_SET(cpu_number, &cpuset);

    status = pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);

    if(status != 0)
    {
        printf("Could not set thread core affinity ... exiting\n");
        exit(1);
    }

    cli_args* args = new cli_args;
    args->argc = argc;
    args->argv = argv;
    args->cpu_number = cpu_number;

    status = pthread_create(&thread, &attr, &run_benchmark, args);
    if(status != 0)
    {
        printf("Could not create thread ... exiting\n");
        exit(1);
    }

    status = pthread_join(thread, &res);

    if(status != 0)
    {
        printf("Could not join thread ... exiting\n");
        exit(1);
    }

    free(res);
    delete args;

    return 0;
}

/*
 * Tester for fir1_1
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
#include "fir1_1.hpp"

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

int main(int argc, char *argv[])
{
    //Create Timer Result Array
    std::chrono::duration<double, std::ratio<1, 1000>> durations[TRIALS];
    double durations_clock[TRIALS];



    //Allocate Arrays on Heap
    DATATYPE *stimulus = new DATATYPE[STIM_LEN];
    DATATYPE *output = new DATATYPE[STIM_LEN];
    DATATYPE *coefs = new DATATYPE[COEF_LEN];
    DATATYPE *init = new DATATYPE[COEF_LEN];

    #if PRINT_TITLE == 1
    printf("FIR1_1 Tester\n");
    printf("COEF_LEN: %d, IO_LEN: %d, STIM_LEN: %d, TRIALS: %d\n", COEF_LEN, IO_LEN, STIM_LEN, TRIALS);
    #endif

    //Conduct multiple trials
    for(size_t trial = 0; trial < TRIALS; trial++)
    {
        //Construct Random Coef Array & Init Array
        for(size_t i = 0; i<COEF_LEN; i++)
        {
            //coefs[i] = (DATATYPE) 1;
            init[i] = 0;
        }

        //Construct FIR
        Fir<DATATYPE, COEF_LEN, IO_LEN> fir1(coefs, init);

        //Start Timer and Filter
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        clock_t start_clock = clock();

        for(size_t i = 0; i<(STIM_LEN/IO_LEN); i++)
        {
            fir1.filter(&stimulus[IO_LEN*i], &output[IO_LEN*i]);
        }

        //Stop Timer and Report Time
        clock_t stop_clock = clock();
        std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();
        durations[trial] = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(stop-start);
        durations_clock[trial] = 1000.0 * (stop_clock - start_clock) / CLOCKS_PER_SEC;

        #if PRINT_TRIALS == 1
            printf("Trial %6d: %f, %f\n", trial, durations[trial], durations_clock[trial]);
        #endif 
    }

    #if PRINT_STATS == 1
    //Print Average & StdDev
    double avg_duration = 0;
    double avg_duration_clock = 0;
    for(size_t i = 0; i < TRIALS; i++)
    {
        avg_duration += (double) durations[i].count();
        avg_duration_clock += (double) durations_clock[i];
    }
    avg_duration /= TRIALS;
    avg_duration_clock /= TRIALS;

    double std_dev_duration = 0;
    double std_dev_duration_clock = 0;
    for(size_t i = 0; i < TRIALS; i++)
    {
        double tmp = durations[i].count() - avg_duration;
        double tmp_clock = durations_clock[i] - avg_duration_clock;
        tmp = tmp*tmp;
        tmp_clock = tmp_clock*tmp_clock;

        std_dev_duration += tmp;
        std_dev_duration_clock += tmp_clock;
    }
    std_dev_duration /= (TRIALS - 1);
    std_dev_duration_clock /= (TRIALS - 1);
    std_dev_duration = sqrt(std_dev_duration);
    std_dev_duration_clock = sqrt(std_dev_duration_clock);

    printf("High Res Clock - Sample Mean (ms): %f, Sample Std Dev: %f\n", avg_duration, std_dev_duration);
    printf("         Clock - Sample Mean (ms): %f, Sample Std Dev: %f\n", avg_duration_clock, std_dev_duration_clock);

    printf("High Res Clock - Sample Mean (MS/s): %f\n", STIM_LEN*1.0/(1000.0*avg_duration));
    printf("         Clock - Sample Mean (MS/s): %f\n", STIM_LEN*1.0/(1000.0*avg_duration_clock));
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

            for(size_t i = 0; i < TRIALS; i++)
            {
                csv_file << (double) durations[i].count() << "," << durations_clock[i] << std::endl;
            }

            // if(TRIALS>0)
            // {
            //     csv_file << (double) durations[TRIALS-1].count() << std::endl;
            // }

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
    delete[] stimulus;
    delete[] output;
    delete[] coefs;
    delete[] init;

    return 0;
}
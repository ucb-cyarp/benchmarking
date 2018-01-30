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
#include <random>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <iostream>
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

    //Create Random Number Generator & Distribution
    std::mt19937 rand_gen(RAND_SEED);
    std::normal_distribution<double> distribution(RAND_MEAN, RAND_STDDEV);

    //Allocate Arrays on Heap
    DATATYPE *stimulus = new DATATYPE[STIM_LEN];
    DATATYPE *output = new DATATYPE[STIM_LEN];
    DATATYPE *coefs = new DATATYPE[COEF_LEN];
    DATATYPE *init = new DATATYPE[COEF_LEN];

    #if PRINT_TITLE == 1
    printf("FIR1_1 Tester\n");
    #endif

    //Conduct multiple trials
    for(size_t trial = 0; trial < TRIALS; trial++)
    {
        // DATATYPE stimulus[STIM_LEN];
        // DATATYPE output[STIM_LEN];

        //Construct Random Array
        for(size_t i = 0; i<STIM_LEN; i++)
        {
            stimulus[i] = (DATATYPE) distribution(rand_gen);
        }

        
        //Construct Random Coef Array & Init Array
        DATATYPE coefs[COEF_LEN];
        DATATYPE init[COEF_LEN];
        for(size_t i = 0; i<COEF_LEN; i++)
        {
            coefs[i] = (DATATYPE) distribution(rand_gen);
        }

        //Construct FIR
        Fir<DATATYPE, COEF_LEN, IO_LEN> fir1(coefs, init);

        //Start Timer and Filter
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

        for(size_t i = 0; i<(STIM_LEN/IO_LEN); i++)
        {
            fir1.filter(&stimulus[IO_LEN*i], &output[IO_LEN*i]);
        }

        //Stop Timer and Report Time
        std::chrono::high_resolution_clock::time_point stop = std::chrono::high_resolution_clock::now();
        durations[trial] = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(stop-start);

        #if PRINT_TRIALS == 1
            printf("Trial %6d: %f\n", trial, durations[trial]);
        #endif 
    }

    #if PRINT_STATS == 1
    //Print Average & StdDev
    double avg_duration = 0;
    for(size_t i = 0; i < TRIALS; i++)
    {
        avg_duration += (double) durations[i].count();
    }
    avg_duration /= TRIALS;

    double std_dev_duration = 0;
    for(size_t i = 0; i < TRIALS; i++)
    {
        double tmp = durations[i].count() - avg_duration;
        tmp = tmp*tmp;

        std_dev_duration += tmp;
    }
    std_dev_duration /= (TRIALS - 1);
    std_dev_duration = sqrt(std_dev_duration);

    printf("Sample Mean (ms): %f, Sample Std Dev: %f\n", avg_duration, std_dev_duration);
    #endif

    #if WRITE_CSV == 1
    //Write CSV File
    if(argc >= 2)
    {
        std::ofstream csv_file;
        printf("Writing results to %s\n", argv[1]);
        csv_file.open(argv[1]);

        if(csv_file.is_open())
        {
            for(size_t i = 0; i < TRIALS-1; i++)
            {
                csv_file << (double) durations[i].count() << ", ";
            }

            if(TRIALS>0)
            {
                csv_file << (double) durations[TRIALS-1].count() << std::endl;
            }

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
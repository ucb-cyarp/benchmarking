#include "results.h"
#include "intrin_bench_default_defines.h"
#include <fstream>
#include <iostream>

double average(double* arr, size_t len)
{
    double avg = 0;
    for(size_t i = 0; i < len; i++)
    {
        avg += (double) arr[i];
    }
    avg /= len;

    return avg;
}

double standard_dev(double* arr, size_t len)
{
    double std_dev = 0;
    double avg = average(arr, len);

    for(size_t i = 0; i < TRIALS; i++)
    {
        double tmp = arr[i] - avg;
        tmp = tmp*tmp;

        std_dev += tmp;
    }
    std_dev /= (len - 1);
    std_dev = sqrt(std_dev);

    return std_dev;
}

TrialResult::TrialResult(int _sockets, int _cores, int _trial)
{
    sockets = _sockets;
    cores = _cores;
    trial = _trial;

    avgCPUFreq = new double[cores];
    avgActiveCPUFreq = new double[cores];

    energyCPUUsed = new double[sockets];
    energyDRAMUsed = new double[sockets];

    startPackageThermalHeadroom = new int32_t[sockets];
    endPackageThermalHeadroom = new int32_t[sockets];
}

TrialResult::~TrialResult()
{
    delete[] avgCPUFreq;
    delete[] avgActiveCPUFreq;

    delete[] energyCPUUsed;
    delete[] energyDRAMUsed;

    delete[] startPackageThermalHeadroom;
    delete[] endPackageThermalHeadroom;
}


void TrialResult::print_trial()
    {
        printf("Trial %6d: Duration: %f, Duration (Clk): %f, Duration (rdtsc): %f", trial, duration, duration_clock, duration_rdtsc);
        for(int i = 0; i<sockets; i++)
        {
                printf("\nEnergyCPUUsed[%d]: %8.4f, EnergyDRAMUsed[%d]: %8.4f ", i, energyCPUUsed[i], i, energyDRAMUsed[i]);
                printf("\nStart thermal headroom below TjMax (deg C): %d", startPackageThermalHeadroom[i]);
                printf("\nEnd thermal headroom below TjMax (deg C): %d", endPackageThermalHeadroom[i]);
        }

        for(int i = 0; i<cores; i++)
        {
                printf("\nAvgCPUFreq[%d]: %15.4f, AvgActiveCPUFreq[%d]: %15.4f", i, avgCPUFreq[i], i, avgActiveCPUFreq[i]);
        }
        printf("\n");
    }

Results::Results(int _sockets, int _cores)
{
    sockets = _sockets;
    cores = _cores;
}

Results::~Results()
{
    
}

void Results::add_trial(TrialResult* trial)
{
    trial_results.push_back(trial);
}

double Results::avg_duration()
{
    size_t trials = trial_results.size();
    double result[trials];

    for(size_t i = 0; i<trials; i++)
    {
        result[i] = trial_results[i]->duration;
    }

    return average(result, trials);
}

double Results::avg_duration_clock()
{
    size_t trials = trial_results.size();
    double result[trials];

    for(size_t i = 0; i<trials; i++)
    {
        result[i] = trial_results[i]->duration_clock;
    }

    return average(result, trials);
}

double Results::avg_duration_rdtsc()
{
    size_t trials = trial_results.size();
    double result[trials];

    for(size_t i = 0; i<trials; i++)
    {
        result[i] = trial_results[i]->duration_rdtsc;
    }

    return average(result, trials);
}

double Results::stddev_duration()
{
    size_t trials = trial_results.size();
    double result[trials];

    for(size_t i = 0; i<trials; i++)
    {
        result[i] = trial_results[i]->duration;
    }

    return standard_dev(result, trials);
}

double Results::stddev_duration_clock()
{
    size_t trials = trial_results.size();
    double result[trials];

    for(size_t i = 0; i<trials; i++)
    {
        result[i] = trial_results[i]->duration_clock;
    }

    return standard_dev(result, trials);
}

double Results::stddev_duration_rdtsc()
{
    size_t trials = trial_results.size();
    double result[trials];

    for(size_t i = 0; i<trials; i++)
    {
        result[i] = trial_results[i]->duration_rdtsc;
    }

    return standard_dev(result, trials);
}

double Results::avg_CPUFreq(int core)
{
    size_t trials = trial_results.size();
    double result[trials];

    for(size_t i = 0; i<trials; i++)
    {
        result[i] = trial_results[i]->avgCPUFreq[core];
    }

    return average(result, trials);
}

double Results::avg_ActiveCPUFreq(int core)
{
    size_t trials = trial_results.size();
    double result[trials];

    for(size_t i = 0; i<trials; i++)
    {
        result[i] = trial_results[i]->avgActiveCPUFreq[core];
    }

    return average(result, trials);
}

double Results::stddev_CPUFreq(int core)
{
    size_t trials = trial_results.size();
    double result[trials];

    for(size_t i = 0; i<trials; i++)
    {
        result[i] = trial_results[i]->avgCPUFreq[core];
    }

    return standard_dev(result, trials);
}

double Results::stddev_ActiveCPUFreq(int core)
{
    size_t trials = trial_results.size();
    double result[trials];

    for(size_t i = 0; i<trials; i++)
    {
        result[i] = trial_results[i]->avgActiveCPUFreq[core];
    }

    return standard_dev(result, trials);
}

double Results::avg_CPUPer(int core)
{
    size_t trials = trial_results.size();
    double result[trials];

    for(size_t i = 0; i<trials; i++)
    {
        result[i] = 1/trial_results[i]->avgCPUFreq[core];
    }

    return average(result, trials);
}

double Results::avg_ActiveCPUPer(int core)
{
    size_t trials = trial_results.size();
    double result[trials];

    for(size_t i = 0; i<trials; i++)
    {
        result[i] = 1/trial_results[i]->avgActiveCPUFreq[core];
    }

    return average(result, trials);
}

double Results::stddev_CPUPer(int core)
{
    size_t trials = trial_results.size();
    double result[trials];

    for(size_t i = 0; i<trials; i++)
    {
        result[i] = 1/trial_results[i]->avgCPUFreq[core];
    }

    return standard_dev(result, trials);
}

double Results::stddev_ActiveCPUPer(int core)
{
    size_t trials = trial_results.size();
    double result[trials];

    for(size_t i = 0; i<trials; i++)
    {
        result[i] = 1/trial_results[i]->avgActiveCPUFreq[core];
    }

    return standard_dev(result, trials);
}

double Results::avg_EnergyCPUUsed(int socket)
{
    size_t trials = trial_results.size();
    double result[trials];

    for(size_t i = 0; i<trials; i++)
    {
        result[i] = trial_results[i]->energyCPUUsed[socket];
    }

    return average(result, trials);
}

double Results::avg_EnergyDRAMUsed(int socket)
{
    size_t trials = trial_results.size();
    double result[trials];

    for(size_t i = 0; i<trials; i++)
    {
        result[i] = trial_results[i]->energyDRAMUsed[socket];
    }

    return average(result, trials);
}

double Results::stddev_EnergyCPUUsed(int socket)
{
    size_t trials = trial_results.size();
    double result[trials];

    for(size_t i = 0; i<trials; i++)
    {
        result[i] = trial_results[i]->energyCPUUsed[socket];
    }

    return standard_dev(result, trials);
}

double Results::stddev_EnergyDRAMUsed(int socket)
{
    size_t trials = trial_results.size();
    double result[trials];

    for(size_t i = 0; i<trials; i++)
    {
        result[i] = trial_results[i]->energyDRAMUsed[socket];
    }

    return standard_dev(result, trials);
}

void Results::delete_results()
{
    size_t trials = trial_results.size();

    for(size_t i=0; i<trials; i++)
    {
        delete trial_results[i];
    }
}

void Results::print_statistics(int socket, int core)
{
    double avg_cpu_per_dbl = this->avg_CPUPer(core);
    double stddev_cpu_per_dbl = this->stddev_CPUPer(core);

    double avg_duration_clock_dbl = this->avg_duration_clock();
    double stddev_duration_clock_dbl = this->stddev_duration_clock();

    double avg_cpu_energy_dbl = this->avg_EnergyCPUUsed(socket);
    double stddev_cpu_energy_dbl = this->stddev_EnergyCPUUsed(socket);

    double avg_dram_energy_dbl = this->avg_EnergyDRAMUsed(socket);
    double stddev_dram_energy_dbl = this->stddev_EnergyDRAMUsed(socket);


    printf("         CPU Period Mean (ns): %f, Sample Std Dev: %f\n", avg_cpu_per_dbl*1000000000, stddev_cpu_per_dbl*1000000000);
    printf("         CPU Freq (MHz): %f\n", 1/avg_cpu_per_dbl/1000000);

    // printf("High Res Clock - Sample Mean (ms): %f, Sample Std Dev: %f\n", avg_duration, std_dev_duration);
    printf("         Clock - Sample Mean (ms): %f, Sample Std Dev: %f\n", avg_duration_clock_dbl, stddev_duration_clock_dbl);
    printf("         Clock Normalized to Sample - Sample Mean (ns): %f, Sample Std Dev: %f\n", avg_duration_clock_dbl*1000000/STIM_LEN, stddev_duration_clock_dbl*1000000/STIM_LEN);

    // printf("High Res Clock - Sample Mean (MS/s): %f\n", STIM_LEN*1.0/(1000.0*avg_duration));
    printf("         Clock - Sample Mean (MS/s): %f\n", STIM_LEN*1.0/(1000.0*avg_duration_clock_dbl));

    printf("         CPU Energy Mean (J): %f, Sample Std Dev: %f\n", avg_cpu_energy_dbl, stddev_cpu_energy_dbl);
    printf("         DRAM Energy Mean (J): %f, Sample Std Dev: %f\n", avg_dram_energy_dbl, stddev_dram_energy_dbl);

    printf("         CPU Energy Mean - Normalized to Sample (nJ): %f, Sample Std Dev: %f\n", avg_cpu_energy_dbl*1000000000/STIM_LEN, stddev_cpu_energy_dbl*1000000000/STIM_LEN);
    printf("         DRAM Energy Mean - Normalized to Sample (nJ): %f, Sample Std Dev: %f\n", avg_dram_energy_dbl*1000000000/STIM_LEN, stddev_dram_energy_dbl*1000000000/STIM_LEN);
}

void Results::write_csv(std::ofstream &csv_file, int socket, int core)
{
    //Print Header
    csv_file << "\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\",\"Average CPU Frequency (Hz)\",\"Average Active CPU Frequency (Hz)\",\"Energy CPU Used (J)\",\"Energy DRAM Used (J)\",\"Socket Executed On\",\"Core Executed On\",\"Start thermal headroom below TjMax (deg C)\",\"End thermal headroom below TjMax (deg C)\"" << std::endl;

    size_t trials = trial_results.size();
    for(size_t i = 0; i < trials; i++)
    {
        csv_file <<  trial_results[i]->duration << "," << trial_results[i]->duration_clock << "," << trial_results[i]->duration_rdtsc << "," << trial_results[i]->avgCPUFreq[core] << "," << trial_results[i]->avgActiveCPUFreq[core] << "," << trial_results[i]->energyCPUUsed[socket] << "," <<  trial_results[i]->energyDRAMUsed[socket] << "," << socket << "," << core << "," << trial_results[i]->startPackageThermalHeadroom[socket] << "," << trial_results[i]->endPackageThermalHeadroom[socket] << std::endl;
    }
}

void statistics(std::chrono::duration<double, std::ratio<1, 1000>>* durations, double* durations_clock)
{
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

    // printf("High Res Clock - Sample Mean (ms): %f, Sample Std Dev: %f\n", avg_duration, std_dev_duration);
    printf("         Clock - Sample Mean (ms): %f, Sample Std Dev: %f\n", avg_duration_clock, std_dev_duration_clock);
    printf("         Clock Normalized to Sample - Sample Mean (ns): %f, Sample Std Dev: %f\n", avg_duration_clock*1000000/STIM_LEN, std_dev_duration_clock*1000000/STIM_LEN);

    // printf("High Res Clock - Sample Mean (MS/s): %f\n", STIM_LEN*1.0/(1000.0*avg_duration));
    printf("         Clock - Sample Mean (MS/s): %f\n", STIM_LEN*1.0/(1000.0*avg_duration_clock));
}
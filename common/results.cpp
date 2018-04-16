#include "results.h"
//#include "intrin_bench_default_defines.h"
#include <fstream>
#include <iostream>
#include <cmath>
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

    for(size_t i = 0; i < len; i++)
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

void Results::print_statistics(int socket, int core, int stim_len)
{
    double avg_cpu_per_dbl = this->avg_CPUPer(core);
    double stddev_cpu_per_dbl = this->stddev_CPUPer(core);

    double avg_duration_dbl = this->avg_duration();
    double stddev_duration_dbl = this->stddev_duration();

    double avg_duration_clock_dbl = this->avg_duration_clock();
    double stddev_duration_clock_dbl = this->stddev_duration_clock();

    double avg_cpu_energy_dbl = this->avg_EnergyCPUUsed(socket);
    double stddev_cpu_energy_dbl = this->stddev_EnergyCPUUsed(socket);

    double avg_dram_energy_dbl = this->avg_EnergyDRAMUsed(socket);
    double stddev_dram_energy_dbl = this->stddev_EnergyDRAMUsed(socket);

    printf("         ##### CPU Frequency #####\n");
    printf("             CPU Period Mean (ns): %f, Sample Std Dev: %f\n", avg_cpu_per_dbl*1000000000, stddev_cpu_per_dbl*1000000000);
    printf("             CPU Freq (MHz): %f\n", 1/avg_cpu_per_dbl/1000000);

    // printf("High Res Clock - Sample Mean (ms): %f, Sample Std Dev: %f\n", avg_duration, std_dev_duration);

    printf("         ##### High Resolution Clock - Clock With Smallest Tick on System #####\n");
    printf("             High Resolution Timer - Sample Mean (ms): %f, Sample Std Dev: %f\n", avg_duration_dbl, stddev_duration_dbl);
    printf("             High Resolution Timer Normalized to Sample - Sample Mean (ns): %f, Sample Std Dev: %f\n", avg_duration_dbl*1000000/stim_len, stddev_duration_dbl*1000000/stim_len);
    printf("             High Resolution Timer - Sample Mean (MS/s): %f\n", stim_len*1.0/(1000.0*avg_duration_dbl));

    printf("         ##### clock() Duration - Process CPU Time (May be Different from Wall Clock Time - Cumulative Time For All Threads) #####\n");
    printf("             Clock - Sample Mean (ms): %f, Sample Std Dev: %f\n", avg_duration_clock_dbl, stddev_duration_clock_dbl);
    printf("             *Clock Normalized to Sample - Sample Mean (ns): %f, Sample Std Dev: %f\n", avg_duration_clock_dbl*1000000/stim_len, stddev_duration_clock_dbl*1000000/stim_len);
    printf("             *Clock - Sample Mean (MS/s): %f\n", stim_len*1.0/(1000.0*avg_duration_clock_dbl));

    printf("         ##### System Energy Use #####\n");
    printf("             CPU Energy Mean (J): %f, Sample Std Dev: %f\n", avg_cpu_energy_dbl, stddev_cpu_energy_dbl);
    printf("             DRAM Energy Mean (J): %f, Sample Std Dev: %f\n", avg_dram_energy_dbl, stddev_dram_energy_dbl);
    printf("             CPU Energy Mean - Normalized to Sample (nJ): %f, Sample Std Dev: %f\n", avg_cpu_energy_dbl*1000000000/stim_len, stddev_cpu_energy_dbl*1000000000/stim_len);
    printf("             DRAM Energy Mean - Normalized to Sample (nJ): %f, Sample Std Dev: %f\n", avg_dram_energy_dbl*1000000000/stim_len, stddev_dram_energy_dbl*1000000000/stim_len);
}

void Results::print_statistics(std::vector<int> sockets, std::vector<int> cores, int stim_len)
{
    double avg_duration_dbl = this->avg_duration();
    double stddev_duration_dbl = this->stddev_duration();

    double avg_duration_clock_dbl = this->avg_duration_clock();
    double stddev_duration_clock_dbl = this->stddev_duration_clock();

    printf("         ##### CPU Frequency #####\n");
    for(size_t i = 0; i<cores.size(); i++){
        double avg_cpu_per_dbl = this->avg_CPUPer(cores[i]);
        double stddev_cpu_per_dbl = this->stddev_CPUPer(cores[i]);
        printf("             CPU [%d] Period Mean (ns): %f, Sample Std Dev: %f\n", cores[i], avg_cpu_per_dbl*1000000000, stddev_cpu_per_dbl*1000000000);
        printf("             CPU [%d] Freq (MHz): %f\n", cores[i], 1/avg_cpu_per_dbl/1000000);
    }

    // printf("High Res Clock - Sample Mean (ms): %f, Sample Std Dev: %f\n", avg_duration, std_dev_duration);

    printf("         ##### High Resolution Clock - Clock With Smallest Tick on System #####\n");
    printf("             High Resolution Timer - Sample Mean (ms): %f, Sample Std Dev: %f\n", avg_duration_dbl, stddev_duration_dbl);
    printf("             High Resolution Timer Normalized to Sample - Sample Mean (ns): %f, Sample Std Dev: %f\n", avg_duration_dbl*1000000/stim_len, stddev_duration_dbl*1000000/stim_len);
    printf("             High Resolution Timer - Sample Mean (MS/s): %f\n", stim_len*1.0/(1000.0*avg_duration_dbl));

    printf("         ##### clock() Duration - Process CPU Time (May be Different from Wall Clock Time - Cumulative Time For All Threads) #####\n");
    printf("             Clock - Sample Mean (ms): %f, Sample Std Dev: %f\n", avg_duration_clock_dbl, stddev_duration_clock_dbl);
    printf("             *Clock Normalized to Sample - Sample Mean (ns): %f, Sample Std Dev: %f\n", avg_duration_clock_dbl*1000000/stim_len, stddev_duration_clock_dbl*1000000/stim_len);
    printf("             *Clock - Sample Mean (MS/s): %f\n", stim_len*1.0/(1000.0*avg_duration_clock_dbl));

    printf("         ##### System Energy Use #####\n");
    for(size_t i = 0; i<sockets.size(); i++){
        double avg_cpu_energy_dbl = this->avg_EnergyCPUUsed(sockets[i]);
        double stddev_cpu_energy_dbl = this->stddev_EnergyCPUUsed(sockets[i]);
        double avg_dram_energy_dbl = this->avg_EnergyDRAMUsed(sockets[i]);
        double stddev_dram_energy_dbl = this->stddev_EnergyDRAMUsed(sockets[i]);
        printf("             CPU Skt  [%d] Energy Mean (J): %f, Sample Std Dev: %f\n", sockets[i], avg_cpu_energy_dbl, stddev_cpu_energy_dbl);
        printf("             DRAM Skt [%d] Energy Mean (J): %f, Sample Std Dev: %f\n", sockets[i], avg_dram_energy_dbl, stddev_dram_energy_dbl);
        printf("             CPU Skt  [%d] Energy Mean - Normalized to Sample (nJ): %f, Sample Std Dev: %f\n", sockets[i], avg_cpu_energy_dbl*1000000000/stim_len, stddev_cpu_energy_dbl*1000000000/stim_len);
        printf("             DRAM Skt [%d] Energy Mean - Normalized to Sample (nJ): %f, Sample Std Dev: %f\n", sockets[i], avg_dram_energy_dbl*1000000000/stim_len, stddev_dram_energy_dbl*1000000000/stim_len);
    }
}

void Results::write_csv(std::ofstream &csv_file, int socket, int core)
{
    //Print Header
    csv_file << "\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\",\"Average CPU Frequency (Hz)\",\"Average Active CPU Frequency (Hz)\",\"Energy CPU Used (J)\",\"Energy DRAM Used (J)\",\"Socket Executed On\",\"Core Executed On\",\"Start thermal headroom below TjMax (deg C)\",\"End thermal headroom below TjMax (deg C)\"" << std::endl;

    size_t trials = trial_results.size();
    for(size_t i = 0; i < trials; i++)
    {
        csv_file << std::scientific <<  trial_results[i]->duration << "," << trial_results[i]->duration_clock << "," << trial_results[i]->duration_rdtsc << "," << trial_results[i]->avgCPUFreq[core] << "," << trial_results[i]->avgActiveCPUFreq[core] << "," << trial_results[i]->energyCPUUsed[socket] << "," <<  trial_results[i]->energyDRAMUsed[socket] << "," << socket << "," << core << "," << trial_results[i]->startPackageThermalHeadroom[socket] << "," << trial_results[i]->endPackageThermalHeadroom[socket] << std::endl;
    }
}

void Results::write_csv(std::ofstream &csv_file, int socket, int core, std::string col0_name, int col0_val)
{
    //Print Header
    csv_file << "\"" << col0_name << "\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\",\"Average CPU Frequency (Hz)\",\"Average Active CPU Frequency (Hz)\",\"Energy CPU Used (J)\",\"Energy DRAM Used (J)\",\"Socket Executed On\",\"Core Executed On\",\"Start thermal headroom below TjMax (deg C)\",\"End thermal headroom below TjMax (deg C)\"" << std::endl;

    size_t trials = trial_results.size();
    for(size_t i = 0; i < trials; i++)
    {
        csv_file << col0_val << std::scientific <<  trial_results[i]->duration << "," << trial_results[i]->duration_clock << "," << trial_results[i]->duration_rdtsc << "," << trial_results[i]->avgCPUFreq[core] << "," << trial_results[i]->avgActiveCPUFreq[core] << "," << trial_results[i]->energyCPUUsed[socket] << "," <<  trial_results[i]->energyDRAMUsed[socket] << "," << socket << "," << core << "," << trial_results[i]->startPackageThermalHeadroom[socket] << "," << trial_results[i]->endPackageThermalHeadroom[socket] << std::endl;
    }
}

void Results::write_durations(std::ofstream &csv_file, std::string col0_name, int col0_val, bool include_header)
{
    //Print Header
    if(include_header)
    {
        csv_file << "\"" << col0_name << "\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
    }

    size_t trials = trial_results.size();
    for(size_t i = 0; i < trials; i++)
    {
        csv_file << col0_val << "," << std::scientific <<  trial_results[i]->duration << "," << trial_results[i]->duration_clock << "," << trial_results[i]->duration_rdtsc << std::endl;
    }
}

void Results::write_durations(std::ofstream &csv_file, std::string col0_name, int col0_val, std::string col1_name, int col1_val, bool include_header)
{
    //Print Header
    if(include_header)
    {
        csv_file << "\"" << col0_name << "\",\"" << col1_name << "\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
    }

    size_t trials = trial_results.size();
    for(size_t i = 0; i < trials; i++)
    {
        csv_file << col0_val << "," << col1_val << "," << std::scientific <<  trial_results[i]->duration << "," << trial_results[i]->duration_clock << "," << trial_results[i]->duration_rdtsc << std::endl;
    }
}
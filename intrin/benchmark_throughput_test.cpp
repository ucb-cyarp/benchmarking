/*
 * Benchmark for x86_64/x07/SSE/AVX/AVX2/FMA
 * 
 * Test execution speeds for x86_64/x07/SSE/AVX/AVX2/FMA for different
 * operations and datatypes.
 * 
 * Supports running micro and macro benchmark suites.
 * 
 * Compiler Parameters (defined as macros)
 *     - STIM_LEN:     Stimulus Length
 *     - TRIALS:       Number of Trial Runs
 *     - PRINT_TITLE:  1 to Print Title Text
 *     - PRINT_HEADER: 1 to Print Test Header
 *     - PRINT_TRIALS: 1 to Print Each Trial
 *     - PRINT_STATS:  1 to Print Statistics
 * 
 * Arguments
 *     
 */

#include <pthread.h>
#include <stdio.h>
#include <map>
#include <unistd.h>

#include <chrono>
#include <ctime>
#include <algorithm>

#include "intrin_bench_default_defines.h"

#include "profiler.h"
#include "pcm_profiler.h"

// #include "cpucounters.h"
// #include "pcm_helper.h"

//#include "kernel_runner.h"
#include "results.h"

#include "benchmark_throughput_test.h"

std::string timerType_toString(TimerType type){
    switch(type){
        case TimerType::HRC:
            return "High Resolution Clock";
        case TimerType::CLOCK:
            return "Clock";
        case TimerType::RDTSC:
            return "rdtsc";
        default:
            throw std::runtime_error("Unknown TimerType");
            return "Unknown";
    }
}

void writeTimingMeasurementsToCSV(TimerType timerType, bool frequency, FILE* csv_file, std::map<std::string, std::map<std::string, Results*>*> &kernel_results, std::vector<std::string> &kernels, std::vector<std::string> &types){
    for(size_t i = 0; i<types.size(); i++)
    {
        std::string datatype = types[i];

        std::string descr = frequency ? "Rate (Ms/s)" : "Normalized Execution Time for 1 Sample (ns)";

        //Print Descr:
        if(i == 0)
        {
            fprintf(csv_file, "\"%s - From %s\",\"%s\"", descr.c_str(), timerType_toString(timerType).c_str(), datatype.c_str());
        }
        else
        {
            fprintf(csv_file, ",\"%s\"", datatype.c_str());
        }

        //Print Data
        for(size_t j = 0; j<kernels.size(); j++)
        {
            std::string kernel_name = kernels[j];
            std::map<std::string, std::map<std::string, Results*>*>::iterator result_container_it = kernel_results.find(kernel_name);

            if(result_container_it == kernel_results.end())
            {
                //No such kernel exists.  Print empty values
                fprintf(csv_file, ",,");
            }
            else
            {
                std::map<std::string, Results*>::iterator result_it = result_container_it->second->find(datatype);
                if(result_it == result_container_it->second->end())
                {
                    //No such result exists.  Print empty values
                    fprintf(csv_file, ",,");
                }
                else
                {
                    //Print result
                    double mean_exe_time = 0;
                    switch(timerType){
                        case TimerType::HRC:
                            mean_exe_time = result_it->second->avg_duration();
                            break;
                        case TimerType::CLOCK:
                            mean_exe_time = result_it->second->avg_duration_clock();
                            break;
                        case TimerType::RDTSC:
                            mean_exe_time = result_it->second->avg_duration_rdtsc();
                            break;
                        default:
                            throw std::runtime_error("Unknown TimerType");
                    }

                    if(frequency){
                        //Computing frequency
                        
                        double scaled_mean = STIM_LEN*1.0/(1000.0*mean_exe_time);
                        fprintf(csv_file, ",%e,", scaled_mean);
                    }else{
                        //Computing normalized duration
                        double scaled_mean = mean_exe_time*1000000/STIM_LEN;

                        double stddev_exe_time = 0;
                        switch(timerType){
                            case TimerType::HRC:
                                stddev_exe_time = result_it->second->stddev_duration();
                                break;
                            case TimerType::CLOCK:
                                stddev_exe_time = result_it->second->stddev_duration_clock();
                                break;
                            case TimerType::RDTSC:
                                stddev_exe_time = result_it->second->stddev_duration_rdtsc();
                                break;
                            default:
                                throw std::runtime_error("Unknown TimerType");
                        }

                        double scaled_stddev = stddev_exe_time*1000000/STIM_LEN;

                        fprintf(csv_file, ",%e,%e", scaled_mean, scaled_stddev);
                    }
                }
            }
        }
        fprintf(csv_file, "\n");
    }
}

void writeMeasurementsToCSV(MeasurementType measurementType, HW_Granularity granularity, Unit tgtUnit, FILE* csv_file, Profiler* profiler, std::map<std::string, std::map<std::string, Results*>*> &kernel_results, std::vector<std::string> &kernels, std::vector<std::string> &types, int ind, bool printStdDev){
    for(size_t i = 0; i<types.size(); i++)
    {
        std::string datatype = types[i];

        //Print Descr:
        if(i == 0)
        {
            std::string unitStr = MeasurementHelper::exponentAbrev(tgtUnit.exponent)+MeasurementHelper::BaseUnit_abrev(tgtUnit.baseUnit);
            fprintf(csv_file, "\"%s Normalized to 1 Sample (%s) - %s[%d]:\",\"%s\"", MeasurementHelper::MeasurementType_toString(measurementType).c_str(), unitStr.c_str(), MeasurementHelper::HW_Granularity_toString(granularity).c_str(), ind, datatype.c_str());
        }
        else
        {
            fprintf(csv_file, ",\"%s\"", datatype.c_str());
        }

        //Print Data
        for(size_t j = 0; j<kernels.size(); j++)
        {
            std::string kernel_name = kernels[j];
            std::map<std::string, std::map<std::string, Results*>*>::iterator result_container_it = kernel_results.find(kernel_name);

            if(result_container_it == kernel_results.end())
            {
                //No such kernel exists.  Print empty values
                fprintf(csv_file, ",,");
            }
            else
            {
                std::map<std::string, Results*>::iterator result_it = result_container_it->second->find(datatype);
                if(result_it == result_container_it->second->end())
                {
                    //No such result exists.  Print empty values
                    fprintf(csv_file, ",,");
                }
                else
                {
                    Statistics stats = result_it->second->measurementStats(measurementType, granularity, ind, true); //TODO: treating as lumped for now.  Re-evaluate later
                    if(stats.valid){
                        double mean = stats.avg;
                        double scaled_mean = Unit::scale(stats.unit, tgtUnit, mean)/STIM_LEN;

                        if(printStdDev){
                            double stddev = stats.stdDev;
                            double scaled_stddev = Unit::scale(stats.unit, tgtUnit, stddev)/STIM_LEN;

                            fprintf(csv_file, ",%e,%e", scaled_mean, scaled_stddev);
                        }else{
                            fprintf(csv_file, ",%e,", scaled_mean);
                        }
                    }else{
                        //No such result exists.  Print empty values
                        fprintf(csv_file, ",,");
                    }
                }
            }
        }
        fprintf(csv_file, "\n");
    }
}

void writeMeasurementsToCSV(MeasurementType measurementType, Unit tgtUnit, FILE* csv_file, Profiler* profiler, std::map<std::string, std::map<std::string, Results*>*> &kernel_results, std::vector<std::string> &kernels, std::vector<std::string> &types, int cpu, bool printStdDev, std::vector<HW_Granularity> granularities){
    if(!profiler->cpuTopology.empty()){
        int core = profiler->cpuTopology[cpu].core;
        int die = profiler->cpuTopology[cpu].die;
        int socket = profiler->cpuTopology[cpu].socket;

        MeasurementCapabilities capabilities = profiler->findMeasurementCapabilities();

        for(unsigned long i = 0; i<granularities.size(); i++){
            if(capabilities.measurementCapabilities.find(measurementType) != capabilities.measurementCapabilities.end() && std::find(capabilities.measurementCapabilities[measurementType].begin(), capabilities.measurementCapabilities[measurementType].end(), granularities[i]) != capabilities.measurementCapabilities[measurementType].end()){
                int ind;
                if(granularities[i] == HW_Granularity::SYSTEM){
                    ind = 0;
                }else if(granularities[i] == HW_Granularity::DIE){
                    ind = die;
                }else if(granularities[i] == HW_Granularity::SOCKET){
                    ind = socket;
                }else if(granularities[i] == HW_Granularity::CORE){
                    ind = core;
                }

                writeMeasurementsToCSV(measurementType, granularities[i], tgtUnit, csv_file, profiler, kernel_results, kernels, types, ind, printStdDev);
            }
        }
    }else{
        //For each valid granularity for the measurement, print out each index
        MeasurementCapabilities capabilities = profiler->findMeasurementCapabilities();

        for(unsigned long i = 0; i<granularities.size(); i++){
            if(capabilities.measurementCapabilities.find(measurementType) != capabilities.measurementCapabilities.end() && std::find(capabilities.measurementCapabilities[measurementType].begin(), capabilities.measurementCapabilities[measurementType].end(), granularities[i]) != capabilities.measurementCapabilities[measurementType].end()){
                bool size = 0;

                for(unsigned long j = 0; j<kernels.size(); j++){
                    if(kernel_results.find(kernels[j]) != kernel_results.end()){
                        //Check all of the types to see what the max number of indexes for the granularity are
                        for(unsigned long l = 0; l<types.size(); l++){
                            if(kernel_results[kernels[j]]->find(types[l]) != kernel_results[kernels[j]]->end()){
                                int size_temp = (*kernel_results[kernels[j]])[types[l]]->trial_results[0].measurements[measurementType][granularities[i]].size();
                                size = size_temp>size ? size_temp : size;
                            }
                        }

                        for(unsigned long k = 0; k<size; k++){
                            writeMeasurementsToCSV(measurementType, granularities[i], tgtUnit, csv_file, profiler, kernel_results, kernels, types, k, printStdDev);
                        }
                    }
                }
            }
        }
    }
}

void* run_benchmarks(void* cpu_num)
{
    #if PRINT_TITLE == 1
    std::cout << getBenchSuiteName() << std::endl;
    std::cout << getReportUnitsName() << std::endl;
    #endif

    bool print_topology = false;
    #if PRINT_TITLE == 1
    print_topology = true;
    #endif

    Profiler* profiler = Profiler::ProfilerFactory(USE_PCM);
    profiler->init();

    #if PRINT_TITLE == 1
    printf("\n");
    printf("****** Profiler Used: %s ******\n", profiler->profilerName().c_str());
    #endif

    #if PRINT_TITLE == 1
    printf("**************************************************\n");
    printf("CPU Brand String: %s\n", Profiler::findCPUModelStr().c_str());
    printf("**************************************************\n");
    #endif

    int* cpu_num_int = (int*) cpu_num;

    int socket = 0;
    int die = 0;
    int core = 0;
    int reportFilter = false;
    if(profiler->cpuTopology.empty()){
        std::cerr << "Unable to get CPU Topology from lscpu.  Reported Socket, Die/NUMA, and Core Numbers Will Be Inaccurate." << std::endl;
        std::cerr << "Results will not be filtered accoring to socket, die/NUMA, and core numbers." << std::endl;
    }else{
        socket = profiler->cpuTopology[*cpu_num_int].socket;
        die = profiler->cpuTopology[*cpu_num_int].die;
        core = profiler->cpuTopology[*cpu_num_int].core;
    }

    #if PRINT_TITLE == 1
    #ifdef __APPLE__
    //Apple does not allow core affinity to be explicitally set 
    #else
    printf("Executing on CPU: %3d (Socket: %2d, Die/NUMA: %2d, Core: %3d)\n", *cpu_num_int, socket, die, core);
    #endif
    printf("**************************************************\n");
    printf("\n");
    #endif

    std::map<std::string, std::map<std::string, Results*>*> kernel_results = runBenchSuite(profiler, cpu_num_int);

    std::vector<std::string> types = getVarientsToReport();

    std::vector<std::string> kernels;
    std::vector<std::string> vec_ext;
    getBenchmarksToReport(kernels, vec_ext);

    //Open CSV File to write
    std::string report_file_name = getReportFileName() + ".csv";
    FILE* csv_file;
    csv_file = fopen(report_file_name.c_str(), "w");
    
    //=====Write header=====
    //Number of columns = 2 + Number of Benchmarks*2 (1 for Mean and 1 for StdDev)
    //Print Title + Benchmark Names
    std::string csvBenchTitle = "\"" + getBenchSuiteName() + "\",";
    fprintf(csv_file, "%s", csvBenchTitle.c_str());
    for(std::vector<std::string>::iterator it = kernels.begin(); it != kernels.end(); it++)
    {
        fprintf(csv_file, ",\"%s\",", it->c_str());
    }
    fprintf(csv_file, "\n");

    //Print machine information + Vector Extensions
    fprintf(csv_file, "\"%s\",", profiler->findCPUModelStr().c_str());
    for(std::vector<std::string>::iterator it = vec_ext.begin(); it != vec_ext.end(); it++)
    {
        fprintf(csv_file, ",\"%s\",", it->c_str());
    }
    fprintf(csv_file, "\n");

    //Print Hostname, Mean, StdDev Labels
    char hostname[300];
    int got_host = gethostname(hostname, 300);
    if(got_host != 0)
    {
        sprintf(hostname, "Unavailable");
    }

    //Getting time: Example from http://en.cppreference.com/w/cpp/chrono
    auto wall_clock = std::chrono::system_clock::now();
    std::time_t report_gen_time = std::chrono::system_clock::to_time_t(wall_clock);

    fprintf(csv_file, "\"Host: %s, Stimulus Length: %d, Trials: %d, Report Generated: %s\",", hostname, STIM_LEN, TRIALS, ctime(&report_gen_time));
    for(size_t i = 0; i<kernels.size(); i++)
    {
        fprintf(csv_file, ",\"Mean\",\"Std Dev\"");
    }
    fprintf(csv_file, "\n");

    //======Print table======
    //Print Rate (Ms/s)
    writeTimingMeasurementsToCSV(TimerType::HRC,   true,  csv_file, kernel_results, kernels, types);
    writeTimingMeasurementsToCSV(TimerType::HRC,   false, csv_file, kernel_results, kernels, types);
    writeTimingMeasurementsToCSV(TimerType::CLOCK, true,  csv_file, kernel_results, kernels, types);
    writeTimingMeasurementsToCSV(TimerType::CLOCK, false, csv_file, kernel_results, kernels, types);
    //writeTimingMeasurementsToCSV(TimerType::RDTSC, true,  csv_file, kernel_results, kernels, types);
    //writeTimingMeasurementsToCSV(TimerType::RDTSC, false, csv_file, kernel_results, kernels, types);

    //Print Energy Use Normalized to 1 Sample (nJ)
    writeMeasurementsToCSV(MeasurementType::ENERGY_USED_CPU, Unit(BaseUnit::JOULE, -9), csv_file, profiler, kernel_results, kernels, types, *cpu_num_int, true);

    //Print Clk Frequency (MHz)
    writeMeasurementsToCSV(MeasurementType::AVG_FREQ, Unit(BaseUnit::HERTZ, 6), csv_file, profiler, kernel_results, kernels, types, *cpu_num_int, false);

    //Close report file
    fclose(csv_file);

    //Cleanup
    for (std::map<std::string, std::map<std::string, Results*>*>::iterator it = kernel_results.begin(); it != kernel_results.end(); it++)
    {
        std::map<std::string, Results*>* sub_result = it->second;

        for (std::map<std::string, Results*>::iterator sub_it = sub_result->begin(); sub_it != sub_result->end(); sub_it++)
        {
            delete sub_it->second; //Delete the result which was stored in the heap
        }

        delete it->second;
    }
    
    delete profiler;

    return NULL;
}

int main(int argc, char *argv[])
{
    //Run these single-threaded benchmarks on CPU 0 (all machines should have CPU 0)

    //Based off of http://man7.org/linux/man-pages/man3/pthread_setaffinity_np.3.html
    //http://man7.org/linux/man-pages/man3/pthread_create.3.html
    //http://man7.org/linux/man-pages/man3/pthread_attr_setaffinity_np.3.html,
    //http://man7.org/linux/man-pages/man3/pthread_join.3.html

#ifdef __APPLE__
#else
    cpu_set_t cpuset;
#endif
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

#ifdef __APPLE__
    fprintf(stderr, "Warning: Running on MacOS - Thread Affinity Not Set.  Disregard Core Number\n");
    //It looks like the best we could do would be to request that sets of threads be placed on the same L2 if possible.
    //https://forums.developer.apple.com/thread/44002
#else
    CPU_ZERO(&cpuset);
    CPU_SET(cpu_number, &cpuset);

    status = pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);

    if(status != 0)
    {
        printf("Could not set thread core affinity ... exiting\n");
        exit(1);
    }
#endif

    status = pthread_create(&thread, &attr, &run_benchmarks, &cpu_number);
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

    return 0;
}

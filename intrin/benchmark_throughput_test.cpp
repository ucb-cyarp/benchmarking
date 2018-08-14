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

#include "intrin_bench_default_defines.h"

#include "cpucounters.h"
#include "pcm_helper.h"

//#include "kernel_runner.h"
#include "results.h"

#include "benchmark_throughput_test.h"

void* run_benchmarks(void* cpu_num)
{
    #if PRINT_TITLE == 1
    std::cout << getBenchSuiteName() << std::endl;
    std::cout << getReportUnitsName() << std::endl;
    #endif

#if USE_PCM == 1

    bool print_topology = false;
    #if PRINT_TITLE == 1
    print_topology = true;
    #endif

    #if PRINT_TITLE == 1
    printf("\n");
    printf("****** Platform Information Provided by PCM ******\n");
    #endif

    PCM* pcm = init_PCM(print_topology);

    #if PRINT_TITLE == 1
    printf("**************************************************\n");
    printf("CPU Brand String: %s\n", pcm->getCPUBrandString().c_str());
    printf("**************************************************\n");
    #endif

    int* cpu_num_int = (int*) cpu_num;
    int socket = pcm->getSocketId(*cpu_num_int);

    #if PRINT_TITLE == 1

    #if __APPLE__
    //Apple does not allow core affinity to be explicitally set 
    #else
    printf("Executing on Core: %3d (Socket: %2d)\n", *cpu_num_int, socket);
    #endif
    printf("**************************************************\n");
    printf("\n");
    #endif

#else
    PCM* pcm = NULL;
    int* cpu_num_int = (int*) cpu_num;
    int socket = 0;
#endif


    std::map<std::string, std::map<std::string, Results*>*> kernel_results = runBenchSuite(pcm, cpu_num_int);

    std::vector<std::string> types = getVarientsToReport();

    std::vector<std::string> kernels;
    std::vector<std::string> vec_ext;
    getBenchmarksToReport(kernels, vec_ext);

    //Open CSV File to write
    std::string report_file_name = getReportFileName() + ".csv";
    FILE * csv_file;
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
    fprintf(csv_file, "\"%s\",", pcm->getCPUBrandString().c_str());
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
    for(size_t i = 0; i<types.size(); i++)
    {
        std::string datatype = types[i];

        //Print Descr:
        if(i == 0)
        {
            fprintf(csv_file, "\"Rate (Ms/s)\",\"%s\"", datatype.c_str());
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
                    //Print result (only mean in this case)

                    double mean_exe_time = result_it->second->avg_duration_clock();
                    double scaled_mean = STIM_LEN*1.0/(1000.0*mean_exe_time);
                    fprintf(csv_file, ",%e,", scaled_mean);
                }
            }
        }
        fprintf(csv_file, "\n");
    }

    //Print Execution Time Normalized to 1 Sample
    for(size_t i = 0; i<types.size(); i++)
    {
        std::string datatype = types[i];

        //Print Descr:
        if(i == 0)
        {
            fprintf(csv_file, "\"Normalized Execution Time for 1 Sample (ns)\",\"%s\"", datatype.c_str());
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
                    double mean_exe_time = result_it->second->avg_duration_clock();
                    double scaled_mean = mean_exe_time*1000000/STIM_LEN;

                    double stddev_exe_time = result_it->second->stddev_duration_clock();
                    double scaled_stddev = stddev_exe_time*1000000/STIM_LEN;

                    fprintf(csv_file, ",%e,%e", scaled_mean, scaled_stddev);
                }
            }
        }
        fprintf(csv_file, "\n");
    }

    //Print Energy Use Normalized to 1 Sample
        for(size_t i = 0; i<types.size(); i++)
    {
        std::string datatype = types[i];

        //Print Descr:
        if(i == 0)
        {
            fprintf(csv_file, "\"Normalized CPU Energy Use Time for 1 Sample (nJ)\",\"%s\"", datatype.c_str());
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
                    double mean_cpu_energy = result_it->second->avg_EnergyCPUUsed(socket);
                    double scaled_mean = mean_cpu_energy*1000000000/STIM_LEN;

                    double stddev_cpu_energy = result_it->second->stddev_EnergyCPUUsed(socket);
                    double scaled_stddev = stddev_cpu_energy*1000000000/STIM_LEN;

                    fprintf(csv_file, ",%e,%e", scaled_mean, scaled_stddev);
                }
            }
        }
        fprintf(csv_file, "\n");
    }

    //Print Clk Frequency
    for(size_t i = 0; i<types.size(); i++)
    {
        std::string datatype = types[i];

        //Print Descr:
        if(i == 0)
        {
            fprintf(csv_file, "\"CPU Clk Frequency (MHz)\",\"%s\"", datatype.c_str());
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
                    double avg_cpu_per_dbl = result_it->second->avg_CPUPer(*cpu_num_int);
                    double freq_mean = 1/avg_cpu_per_dbl/1000000;


                    fprintf(csv_file, ",%e,", freq_mean);
                }
            }
        }
        fprintf(csv_file, "\n");
    }

    //Print Clk Per
    for(size_t i = 0; i<types.size(); i++)
    {
        std::string datatype = types[i];

        //Print Descr:
        if(i == 0)
        {
            fprintf(csv_file, "\"CPU Clk Period (ns)\",\"%s\"", datatype.c_str());
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
                    double avg_cpu_per_dbl = result_it->second->avg_CPUPer(*cpu_num_int);
                    double scaled_mean = avg_cpu_per_dbl*1000000000;

                    double stddev_cpu_per_dbl = result_it->second->stddev_CPUPer(*cpu_num_int);
                    double scaled_stddev = stddev_cpu_per_dbl*1000000000;


                    fprintf(csv_file, ",%e,%e", scaled_mean, scaled_stddev);
                }
            }
        }
        fprintf(csv_file, "\n");
    }

    //Close report file
    fclose(csv_file);

    //Cleanup
    for (std::map<std::string, std::map<std::string, Results*>*>::iterator it = kernel_results.begin(); it != kernel_results.end(); it++)
    {
        std::map<std::string, Results*>* sub_result = it->second;

        for (std::map<std::string, Results*>::iterator sub_it = sub_result->begin(); sub_it != sub_result->end(); sub_it++)
        {
            sub_it->second->delete_results();
            delete sub_it->second;
        }

        delete it->second;
    }

    #if PRINT_TITLE == 1
    printf("****** PCM Ceanup ******\n");
    #endif
    //Output from PCM appears when distructor runs

    return NULL;
}

int main(int argc, char *argv[])
{
    //Run these single-threaded benchmarks on CPU 0 (all machines should have CPU 0)

    //Based off of http://man7.org/linux/man-pages/man3/pthread_setaffinity_np.3.html
    //http://man7.org/linux/man-pages/man3/pthread_create.3.html
    //http://man7.org/linux/man-pages/man3/pthread_attr_setaffinity_np.3.html,
    //http://man7.org/linux/man-pages/man3/pthread_join.3.html

#if __APPLE__
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

#if __APPLE__
    fprintf(stderr, "Warning: Running on MacOS - Thread Affinity Not Set.  Disregard Core Number");
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
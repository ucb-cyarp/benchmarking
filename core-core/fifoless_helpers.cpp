#include "fifoless_helpers.h"

void FifolessConfig::printResultsStandalone(Results &results)
{
    double avg_duration_ms = results.avg_duration();
    double stddev_duration_ms = results.stddev_duration();

    printf("        =======================================================\n");
    printf("        Metric                   |     Avg      |    StdDev    \n");
    printf("        =======================================================\n");
    printf("        Duration (ms)            |%14.6e|%14.6e\n", avg_duration_ms, stddev_duration_ms);
    printf("        =======================================================\n");
}

void FifolessConfig::printExportCorrespondingResult(Results &result, bool report_standalone, std::string title, Profiler* profiler, std::vector<int> cpus, std::string resultPrintFormatStr, FILE* file, std::ofstream* raw_file){
    printStandaloneTitle(report_standalone, title); //This can be overridden by subclasses to report more information

    //Report the results
    #if PRINT_STATS == 1 || PRINT_FULL_STATS == 1 || WRITE_CSV == 1
        if(report_standalone)
        {
            if(!profiler->cpuTopology.empty()){
                std::vector<int> sockets;
                std::vector<int> cores;
                std::vector<int> dies;
                std::vector<int> threads;

                getGranularityIndexsOfInterest(cpus, profiler, sockets, cores, dies, threads);

                #if PRINT_FULL_STATS == 1
                    results.print_statistics(sockets, dies, cores, threads, STIM_LEN);
                #endif

                #if PRINT_STATS == 1
                    printResultsStandalone(result);
                #endif
            }else{
                #if PRINT_FULL_STATS == 1
                    results.print_statistics(0, 0, 0, cpu_a, STIM_LEN);
                #endif

                #if PRINT_STATS == 1
                    printResultsStandalone(result);
                #endif
            }
        }
        else
        {
            printExportNonStandaloneResults(result, report_standalone, resultPrintFormatStr, file, raw_file); //This can be overridden by subclasses to report more information
        }
    #endif
}
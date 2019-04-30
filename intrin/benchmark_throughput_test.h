#ifndef BENCHMARK_THROUGHPUT_TEST_H
#define BENCHMARK_THROUGHPUT_TEST_H

#include <string>
#include <map>
#include <vector>
#include "results.h"
#include "cpucounters.h"

//This file is used to describe functions that the benchmark_throughput_test relies on from a test suite
//Test benches need to implement these functions for a benchmark application to be compiled for them.

//The running and reporting are handled by the main benchmark_throughput_test file.

/**
 * Get the bechmark suite name
 */ 
std::string getBenchSuiteName();

/**
 * Get the benchmark report file name
 */ 
std::string getReportFileName();

/**
 * Get the units string for the benchmark suite
 */
std::string getReportUnitsName();

/**
 * Run the benchmark suite.
 * Returns a map of benchmark names to a map of type/varient names to results.
 * 
 * The first mapping gets the results for a particular benchmark (as a map)
 * The second mapping take the map for a benchmark and maps the type/varient name to the result of the benchmark for that type/varient.
 */
std::map<std::string, std::map<std::string, Results*>*> runBenchSuite(Profiler* profiler, int* cpu_num_int);

/**
 * Returns a list of benchmarks in this suite (by name) and the associated ISA extensions to include in the report.  Empty vectors should be passed to this function.
 */
void getBenchmarksToReport(std::vector<std::string> &kernels, std::vector<std::string> &vec_ext);

/**
 * Returns a list of benchmarks varients/types (by name) to include in the report
 */
std::vector<std::string> getVarientsToReport();

/**
 * This function initializes a given memory location which will be used as an input to the benchmark
 * 
 * This function is called repeatedly on the input data before the benchmark is run
 */
void initInput(void* ptr, unsigned long index);


enum class TimerType{
    HRC,
    CLOCK,
    RDTSC
}

std::string timerType_toString(TimerType type);

void writeTimingMeasurementsToCSV(TimerType timerType, bool frequency, FILE* csv_file, std::map<std::string, std::map<std::string, Results*>*> &kernel_results, std::vector<std::string> &datatypes);
void writeMeasurementsToCSV(MeasurementType measurementType, Unit tgtUnit, FILE* csv_file, Profiler* profiler, std::map<std::string, std::map<std::string, Results*>*> &kernel_results, std::vector<std::string> &types, int cpu, bool printStdDev, std::vector<HW_Granularity> granularities = DEFAULT_GRANULARITY_LIST);
void writeMeasurementsToCSV(MeasurementType measurementType, HW_Granularity granularity, Unit tgtUnit, std::string label, FILE* csv_file, Profiler* profiler, std::map<std::string, std::map<std::string, Results*>*> &kernel_results, std::vector<std::string> &types, int ind, bool printStdDev);

#endif
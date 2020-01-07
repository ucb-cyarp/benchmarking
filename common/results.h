#ifndef _H_RESULTS
    #define _H_RESULTS
    //#include "intrin_bench_default_defines_and_imports_cpp.h"

    #include <chrono>
    #include <vector>
    #include <cmath>
    #include <fstream>
    #include <iostream>
    #include <set>
    #include <utility>
    #include <memory>

    #include "measurement.h"

    double avg(double* arr, size_t len);
    double avg(const std::vector<double> arr);
    double std_dev(double* arr, size_t len);

    /**
     * This class allows benchmarks to return their own implementation specific results.
     * For example, if the benchmark is measuring the number of times a certain failure condition
     * occurs, the BenchmarkSpecificResult class would be overwritten for that benchmark and would
     * provide reporting for failures
     * 
     * 
     */
    class BenchmarkSpecificResult{
        public:
            HW_Granularity resultGranularity = HW_Granularity::CORE;
            int granularityIndex; //Ie. if the resultGranularity is CORE, this is the core it responds to
            virtual std::string getGranularityStr();
            
            //These are for individual trials
            //The trial results header should not include the granularity information
            virtual std::string getTrialResultsHeader();
            virtual std::string getTrialResults();
            //The CSV header should include granularity information if appropriate
            //The CSV header should include quotes where appropriate and commas between elements
            virtual std::string getTrialCSVHeader();
            virtual std::string getTrialCSVData();

            //TODO: Provide a function for a summary of multiple trials.  May include avg, stddev, max, min
            //May require casting pointers to the derived class (and checking that cast succeeded)
    };

    class TrialResult
    {
        public:
            double duration; //steady clock duration (in ms)
            double duration_clock; //clock duration (in ms)
            double duration_rdtsc;

            int trial;

            bool sampled;
            /**
             * This is a map of measurement types to maps of hardware granularities to measurement vectors.
             * The outer map provides a container for different measurment types.
             * The second layer of maps exist because a particular measurment type may be measurable at different granulartity levels.
             * For each granularity, there is a vector of measurmenets.  The vector is indexed by the unit that took the measurement
             * The index pertains to HW_Granulatity. With HW_Granulatity CORE, index 0 refers to core 0.
             * Each measurement can contain multiple samples if a sampling methodology was used.
             */
            std::map<MeasurementType, std::map<HW_Granularity, std::vector<Measurement>>> measurements;

            /**
             * These are benchmark (implementation) specific results which may not exist for every benchmark.
             * The are ment to carry data which is of interest to a particular benchmark and is not one of the
             * result types provided by the generic profiler.
             * 
             * Warning: If exporting to CSV or printing multiple trial results, the relative order of these 
             * benchmark_specific_results should be consistent across the different TrialResult objects
             */
            std::vector<std::shared_ptr<BenchmarkSpecificResult>> benchmarkSpecificResults;

            TrialResult();
            TrialResult(bool sampled);

            void print_trial(const std::vector<HW_Granularity> granularityToPrint = DEFAULT_GRANULARITY_LIST, const std::vector<MeasurementType> measurementTypeToPrint = DEFAULT_REPORT_TYPE_LIST);
    };

    struct Statistics{
        double avg;
        double stdDev;
        Unit unit;
        bool valid;
        bool cumulative; ///<Signals if the result is for a cumulative metric which should be normalized to the number of samples when reporting

        Statistics();
        Statistics(double avg, double stdDev, Unit unit, bool cumulative, bool valid);
        Statistics(double avg, double stdDev, Unit unit, bool cumulative);
    };

    class Results
    {
        public:
            std::vector<TrialResult> trial_results;

            Results();

            TrialResult* add_trial_set_trialInd(TrialResult &trial);
            TrialResult* add_trial(TrialResult &trial);
            void remove_last_trial();

            double avg_duration();
            double avg_duration_clock();
            double avg_duration_rdtsc();
            double stddev_duration();
            double stddev_duration_clock();
            double stddev_duration_rdtsc();

            //Use Measurement Capabilities to find these

            Statistics measurementStats(MeasurementType measurmentType, HW_Granularity granularity, int32_t index, bool treatTrialSamplesAsLumped);

            void print_statistics(int socket, int dies, int core, int thread, int stim_len, const std::vector<HW_Granularity> granularityToPrint = DEFAULT_GRANULARITY_LIST, const std::vector<MeasurementType> measurementTypeToPrint = DEFAULT_REPORT_TYPE_LIST);
            void print_statistics(std::vector<int> socket, std::vector<int> dies, std::vector<int> cores, std::vector<int> threads, int stim_len, const std::vector<HW_Granularity> granularityToPrint = DEFAULT_GRANULARITY_LIST, const std::vector<MeasurementType> measurementTypeToPrint = DEFAULT_REPORT_TYPE_LIST);
            void print_statistics(std::set<int> socket, std::set<int> dies, std::set<int> cores, std::set<int> threads, int stim_len, const std::vector<HW_Granularity> granularityToPrint = DEFAULT_GRANULARITY_LIST, const std::vector<MeasurementType> measurementTypeToPrint = DEFAULT_REPORT_TYPE_LIST);
            void print_statistics(int stim_len, const std::vector<HW_Granularity> granularityToPrint = DEFAULT_GRANULARITY_LIST, const std::vector<MeasurementType> measurementTypeToPrint = DEFAULT_REPORT_TYPE_LIST);
            void write_csv(std::ofstream &csv_file, int socket, int core, int thread, const std::vector<HW_Granularity> granularityToPrint = DEFAULT_GRANULARITY_LIST, const std::vector<MeasurementType> measurementTypeToPrint = DEFAULT_REPORT_TYPE_LIST);
            void write_csv(std::ofstream &csv_file, int socket, int core, int thread, std::string col0_name, int col0_val, const std::vector<HW_Granularity> granularityToPrint = DEFAULT_GRANULARITY_LIST, const std::vector<MeasurementType> measurementTypeToPrint = DEFAULT_REPORT_TYPE_LIST);
            void write_durations(std::ofstream &csv_file, std::vector<std::string> col_names, std::vector<std::string> col_vals, bool include_header);
            void write_durations_and_benchmark_specific_results(std::ofstream &csv_file, std::vector<std::string> col_names, std::vector<std::string> col_vals, bool include_header);

            std::map<MeasurementType, std::map<HW_Granularity, std::map<int, Unit>>> measurementsAvailUnion(const std::vector<HW_Granularity> granularityToInspect = DEFAULT_GRANULARITY_LIST, const std::vector<MeasurementType> measurementTypeToInspect = DEFAULT_REPORT_TYPE_LIST);
    };

    void statistics(std::chrono::duration<double, std::ratio<1, 1000>>* durations, double* durations_clock);
#endif
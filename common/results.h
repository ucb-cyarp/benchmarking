#ifndef _H_RESULTS
    #define _H_RESULTS
    //#include "intrin_bench_default_defines.h"

    #include <chrono>
    #include <vector>
    #include <cmath>
    #include <fstream>
    #include <iostream>
    #include <set>
    #include <utility>

    #include "measurement.h"

    double avg(double* arr, size_t len);
    double avg(const std::vector<double> arr);
    double std_dev(double* arr, size_t len);

    class TrialResult
    {
        public:
            double duration;
            double duration_clock;
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

            TrialResult();

            void print_trial(const std::vector<HW_Granularity> granularityToPrint = DEFAULT_GRANULARITY_LIST, const std::vector<MeasurementType> measurementTypeToPrint = DEFAULT_REPORT_TYPE_LIST);
    };

    struct Statistics{
        double avg;
        double stdDev;
        Unit unit;
        bool valid;

        Statistics();
        Statistics(double avg, double stdDev, Unit unit, bool valid);
        Statistics(double avg, double stdDev, Unit unit);
    };

    class Results
    {
        public:
            std::vector<TrialResult> trial_results;

            Results();

            TrialResult* add_trial(TrialResult &trial);

            double avg_duration();
            double avg_duration_clock();
            double avg_duration_rdtsc();
            double stddev_duration();
            double stddev_duration_clock();
            double stddev_duration_rdtsc();

            //Use Measurement Capabilities to find these

            Statistics measurementStats(MeasurementType measurmentType, HW_Granularity granularity, int32_t index, bool treatTrialSamplesAsLumped);

            void print_statistics(int socket, int dies, int core, int stim_len, const std::vector<HW_Granularity> granularityToPrint = DEFAULT_GRANULARITY_LIST, const std::vector<MeasurementType> measurementTypeToPrint = DEFAULT_REPORT_TYPE_LIST);
            void print_statistics(std::vector<int> socket, std::vector<int> dies, std::vector<int> cores, int stim_len, const std::vector<HW_Granularity> granularityToPrint = DEFAULT_GRANULARITY_LIST, const std::vector<MeasurementType> measurementTypeToPrint = DEFAULT_REPORT_TYPE_LIST);
            void print_statistics(std::set<int> socket, std::set<int> dies, std::set<int> cores, int stim_len, const std::vector<HW_Granularity> granularityToPrint = DEFAULT_GRANULARITY_LIST, const std::vector<MeasurementType> measurementTypeToPrint = DEFAULT_REPORT_TYPE_LIST);
            void print_statistics(int stim_len, const std::vector<HW_Granularity> granularityToPrint, const std::vector<MeasurementType> measurementTypeToPrint);
            void write_csv(std::ofstream &csv_file, int socket, int core, const std::vector<HW_Granularity> granularityToPrint = DEFAULT_GRANULARITY_LIST, const std::vector<MeasurementType> measurementTypeToPrint = DEFAULT_REPORT_TYPE_LIST);
            void write_csv(std::ofstream &csv_file, int socket, int core, std::string col0_name, int col0_val, const std::vector<HW_Granularity> granularityToPrint = DEFAULT_GRANULARITY_LIST, const std::vector<MeasurementType> measurementTypeToPrint = DEFAULT_REPORT_TYPE_LIST);
            void write_durations(std::ofstream &csv_file, std::string col0_name, int col0_val, bool include_header);
            void write_durations(std::ofstream &csv_file, std::string col0_name, int col0_val, std::string col1_name, int col1_val, bool include_header);

            std::map<MeasurementType, std::map<HW_Granularity, std::map<int, Unit>>> measurementsAvailUnion(const std::vector<HW_Granularity> granularityToInspect = DEFAULT_GRANULARITY_LIST, const std::vector<MeasurementType> measurementTypeToInspect = DEFAULT_REPORT_TYPE_LIST);
    };

    void statistics(std::chrono::duration<double, std::ratio<1, 1000>>* durations, double* durations_clock);
#endif
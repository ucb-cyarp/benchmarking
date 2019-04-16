#ifndef _H_STATISTICS
    #define _H_STATISTICS
    //#include "intrin_bench_default_defines.h"

    #include <chrono>
    #include <vector>
    #include <cmath>
    #include <fstream>
    #include <iostream>
    #include <set>

    #include "measurement.h"
    #include "profiler.h"

    double avg(double* arr, size_t len);
    double std_dev(double* arr, size_t len);

    class TrialResult
    {
        public:
            double duration;
            double duration_clock;
            double duration_rdtsc;

            double* avgCPUFreq;
            double* avgActiveCPUFreq;

            double* energyCPUUsed;
            double* energyDRAMUsed;

            bool sampled;
            std::map<MeasurmentType, std::map<HW_Granularity, std::vector<Measurment>>> measurments;

            TrialResult(int _sockets, int _cores, int _trial);
            ~TrialResult();

            void print_trial();
    };

    class Results
    {
        public:
            int sockets;
            int cores;

            std::vector<TrialResult> trial_results;

            Results(int _sockets, int _cores);
            ~Results();

            void add_trial(TrialResult &trial);

            double avg_duration();
            double avg_duration_clock();
            double avg_duration_rdtsc();
            double stddev_duration();
            double stddev_duration_clock();
            double stddev_duration_rdtsc();

            //Use Measurement Capabilities to find these

            double measurmentAvg(MeasurmentType measurmentType, HW_Granularity granularity, int32_t index, bool treatTrialSamplesAsLumped);
            double measurmentStdDev(MeasurmentType measurmentType, HW_Granularity granularity, int32_t index, bool treatTrialSamplesAsLumped);

            void print_statistics(int socket, int core, int stim_len);
            void print_statistics(std::vector<int> socket, std::vector<int> cores, int stim_len);
            void print_statistics(std::set<int> socket, std::set<int> cores, int stim_len);
            void write_csv(std::ofstream &csv_file, int socket, int core);
            void write_csv(std::ofstream &csv_file, int socket, int core, std::string col0_name, int col0_val);
            void write_durations(std::ofstream &csv_file, std::string col0_name, int col0_val, bool include_header);
            void write_durations(std::ofstream &csv_file, std::string col0_name, int col0_val, std::string col1_name, int col1_val, bool include_header);

            void delete_results();
    };

    void statistics(std::chrono::duration<double, std::ratio<1, 1000>>* durations, double* durations_clock);
#endif
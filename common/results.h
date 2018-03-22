#ifndef _H_STATISTICS
    #define _H_STATISTICS
    #include "intrin_bench_default_defines.h"

    #include <chrono>
    #include <vector>

    double avg(double* arr, size_t len);
    double std_dev(double* arr, size_t len);

    class TrialResult
    {
        public:
            int sockets;
            int cores;
            int trial;

            double duration;
            double duration_clock;
            double duration_rdtsc;

            double* avgCPUFreq;
            double* avgActiveCPUFreq;

            double* energyCPUUsed;
            double* energyDRAMUsed;

            TrialResult(int _sockets, int _cores, int _trial);
            ~TrialResult();

            void print_trial();
    };

    class Results
    {
        public:
            int sockets;
            int cores;

            std::vector<TrialResult*> trial_results;

            Results(int _sockets, int _cores);
            ~Results();

            void add_trial(TrialResult* trial);

            double avg_duration();
            double avg_duration_clock();
            double avg_duration_rdtsc();
            double stddev_duration();
            double stddev_duration_clock();
            double stddev_duration_rdtsc();

            double avg_CPUFreq(int core);
            double avg_ActiveCPUFreq(int core);
            double stddev_CPUFreq(int core);
            double stddev_ActiveCPUFreq(int core);

            double avg_CPUPer(int core);
            double avg_ActiveCPUPer(int core);
            double stddev_CPUPer(int core);
            double stddev_ActiveCPUPer(int core);

            double avg_EnergyCPUUsed(int socket);
            double avg_EnergyDRAMUsed(int socket);
            double stddev_EnergyCPUUsed(int socket);
            double stddev_EnergyDRAMUsed(int socket);

            void print_statistics(int socket, int core);
            void write_csv(std::ofstream &csv_file, int socket, int core);

            void delete_results();
    };

    void statistics(std::chrono::duration<double, std::ratio<1, 1000>>* durations, double* durations_clock);
#endif
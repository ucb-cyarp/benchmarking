#ifndef _H_PCM_HELPER
    #define _H_PCM_HELPER

    #include "cpucounters.h"
    #include <chrono>

    bool check_any_freq_changes(PCM* pcm, ServerUncorePowerState* startStates, ServerUncorePowerState* endStates, std::vector<int> sockets_of_interest, const char* print_prefix = "Warning: ");

    void calculate_durations(double& durations,
    double& durations_clock,
    double& durations_rdtsc,
    std::chrono::high_resolution_clock::time_point start,
    std::chrono::high_resolution_clock::time_point stop,
    clock_t start_clock,
    clock_t stop_clock,
    uint64_t start_rdtsc,
    uint64_t stop_rdtsc);

    void calc_freq_and_power(PCM* pcm, double* avgCPUFreq, double* avgActiveCPUFreq, double* energyCPUUsed, double* energyDRAMUsed,
    std::vector<CoreCounterState>& startCstates, std::vector<CoreCounterState>& endCstates, ServerUncorePowerState* startPowerState, ServerUncorePowerState* endPowerState);

    PCM* init_PCM(bool print_info);
#endif


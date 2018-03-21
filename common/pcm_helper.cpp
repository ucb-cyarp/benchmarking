#include "cpucounters.h"
#include <chrono>

/**
 * Check for frequency changed events.  Assumes PMU counter 1 was set to track frequency change events.
 */
bool check_any_freq_changes(PCM* pcm, ServerUncorePowerState* startStates, ServerUncorePowerState* endStates, std::vector<int> sockets_of_interest, const char* print_prefix = "Warning: ")
{
    bool freq_changed = false;

    for(int i = 0; i < sockets_of_interest.size(); i++)
    {
        int freq_change_events = getPCUCounter(1, startStates[sockets_of_interest[i]], endStates[sockets_of_interest[i]]);
        if(freq_change_events > 0)
        {
            freq_changed = true;
            #if PRINT_FREQ_CHANGE_EVENT == 1
                printf("%sSocket %d experienced %d clock frequency change events!\n", print_prefix, i, freq_change_events);
            #else
                break; //No need to check other sockets, frequency changed
            #endif
        }
    }

    return freq_changed;
}

/**
 * Calculate durations (for different clocks) for a given trial
 * 
 */
void calculate_durations(double& durations,
double& durations_clock,
double& durations_rdtsc,
std::chrono::high_resolution_clock::time_point start,
std::chrono::high_resolution_clock::time_point stop,
clock_t start_clock,
clock_t stop_clock,
uint64_t start_rdtsc,
uint64_t stop_rdtsc)
{
    durations = (std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(stop-start)).count();
    durations_clock = 1000.0 * (stop_clock - start_clock) / CLOCKS_PER_SEC;
    durations_rdtsc =  (stop_rdtsc - start_rdtsc);
}

void calc_freq_and_power(PCM* pcm, double* avgCPUFreq, double* avgActiveCPUFreq, double* energyCPUUsed, double* energyDRAMUsed,
std::vector<CoreCounterState>& startCstates, std::vector<CoreCounterState>& endCstates, ServerUncorePowerState* startPowerState, ServerUncorePowerState* endPowerState)
{
    int cores = pcm->getNumCores();
    int sockets = pcm->getNumSockets();

    for(int i = 0; i<cores; i++)
    {  
        avgCPUFreq[i] = getAverageFrequency(startCstates[i], endCstates[i]);
        avgActiveCPUFreq[i] = getActiveAverageFrequency(startCstates[i], endCstates[i]);
    }
    for(int i = 0; i<sockets; i++)
    {
        energyCPUUsed[i] = getConsumedJoules(startPowerState[i], endPowerState[i]);
        energyDRAMUsed[i] = getDRAMConsumedJoules(startPowerState[i], endPowerState[i]);
    }
}

//Initialize PCM
//Based off opcm/pcm example pcm-power.cpp
PCM* init_PCM()
{
    set_signal_handlers();

    PCM * m = PCM::getInstance();
    //m->allowMultipleInstances();
    m->disableJKTWorkaround();

    //Configure PCM
    const int cpu_model = m->getCPUModel();
    if (!(m->hasPCICFGUncore()))
    {
        std::cerr << "Unsupported processor model (" << cpu_model << ")." << std::endl;
        exit(1);
    }

    printf("**************************************************\n");
    printf("Resetting PMU\n");
    m->resetPMU();

    int default_freq_band[3] = { 12, 20, 40 };
    
    int imc_profile = -1; //Do not gather DRAM statistics (for now)
    int pcu_profile = 5; //Get frequency Change statistics
    int* freq_band = default_freq_band;

    //Configure PCM PCU Monitoring
    if (PCM::Success != m->programServerUncorePowerMetrics(imc_profile, pcu_profile, freq_band))
    {
        #ifdef _MSC_VER
        std::cerr << "You must have signed msr.sys driver in your current directory and have administrator rights to run this program" << std::endl;
        #elif defined(__linux__)
        std::cerr << "You need to be root and loaded 'msr' Linux kernel module to execute the program. You may load the 'msr' module with 'modprobe msr'. \n";
        #endif
        exit(EXIT_FAILURE);
    }

    if (m->program() != PCM::Success) exit(EXIT_FAILURE);

    m->setBlocked(true);

    return m;
}
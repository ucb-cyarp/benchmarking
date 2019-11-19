#include "pcm_profiler.h"

PCM_Profiler::PCM_Profiler(){
    startPowerState = nullptr;
    endPowerState = nullptr;
}

PCM_Profiler::~PCM_Profiler(){
    if(startPowerState){
        delete[] startPowerState;
    }
    if(endPowerState){
        delete[] endPowerState;
    }

    //Deleting PCM appears to cause issues.  Examples do not delete it
    //delete pcm;
}

MeasurementCapabilities PCM_Profiler::findMeasurementCapabilities(){
    MeasurementCapabilities capabilities;
    //For now, does not attempt to collect DRAM statistics
    //Modify init function to change profile to collect DRAM statistics and uncomment capability line below to collect it

    //TODO: Could be expanded in future Intel chips.  This is what the PCM driver provides as of 2018

    capabilities.measurementCapabilities[MeasurementType::AVG_FREQ] = {HW_Granularity::CORE};
    capabilities.measurementCapabilities[MeasurementType::AVG_ACTIVE_FREQ] = {HW_Granularity::CORE};
    capabilities.measurementCapabilities[MeasurementType::ENERGY_USED_CPU] = {HW_Granularity::SOCKET};
    //capabilities.measurementCapabilities[MeasurementType::ENERGY_USED_DRAM] = {HW_Granularity::SOCKET};
    capabilities.measurementCapabilities[MeasurementType::THERMAL_HEADROOM_START] = {HW_Granularity::SOCKET};
    capabilities.measurementCapabilities[MeasurementType::THERMAL_HEADROOM_STOP] = {HW_Granularity::SOCKET};

    return capabilities;
};

bool PCM_Profiler::checkProfiler(){
    CPUVendor cpuVendor = findCPUVendor();
    OS os = findOS();

    if(cpuVendor == CPUVendor::INTEL && (os == OS::LINUX || os == OS::MACOS || os == OS::WINDOWS)){
        return true;
    }

    return false;
};

void PCM_Profiler::init()
{
    set_signal_handlers();

    pcm = PCM::getInstance();

    // if(print_info)
    // {
    //     pcm->printSystemTopology();
    //     //Replicate the rest of the topology output
    //     std::cerr << "Nominal core frequency: " << m->getNominalFrequency() << " Hz" << std::endl;
        
    //     std::cerr << "Package thermal spec power: "<< m->getPackageThermalSpecPower() << " Watt; ";
    //     std::cerr << "Package minimum power: "<< m->getPackageMinimumPower() << " Watt; ";
    //     std::cerr << "Package maximum power: "<< m->getPackageMaximumPower() << " Watt; " << std::endl;
    // }

    //m->allowMultipleInstances();
    pcm->disableJKTWorkaround();

    //Configure PCM
    const int cpu_model = pcm->getCPUModel();
    if (!(pcm->hasPCICFGUncore()))
    {
        std::cerr << "Unsupported processor model (" << cpu_model << ")." << std::endl;
        exit(1);
    }

    // if(print_info)
    // {
    //     printf("**************************************************\n");
    //     printf("Resetting PMU\n");
    // }
    pcm->resetPMU();

    int default_freq_band[3] = { 12, 20, 40 };
    
    int imc_profile = -1; //Do not gather DRAM statistics (for now)
    int pcu_profile = 5; //Get frequency Change statistics
    int* freq_band = default_freq_band;

    //Configure PCM PCU Monitoring
    if (PCM::Success != pcm->programServerUncorePowerMetrics(imc_profile, pcu_profile, freq_band))
    {
        #ifdef _MSC_VER
        std::cerr << "You must have signed msr.sys driver in your current directory and have administrator rights to run this program" << std::endl;
        #elif defined(__linux__)
        std::cerr << "You need to be root and loaded 'msr' Linux kernel module to execute the program. You may load the 'msr' module with 'modprobe msr'. \n";
        #endif
        exit(EXIT_FAILURE);
    }

    if (pcm->program() != PCM::Success){
        exit(EXIT_FAILURE);
    }

    pcm->setBlocked(true);

    
}

void PCM_Profiler::trialSetup(){
    int sockets = pcm->getNumSockets();
    startPowerState = new ServerUncorePowerState[sockets];
    endPowerState = new ServerUncorePowerState[sockets];

    startCstates = std::vector<CoreCounterState>();
    endCstates = std::vector<CoreCounterState>();

    startSktstate = std::vector<SocketCounterState>();
    endSktstate = std::vector<SocketCounterState>();

    startSstate = SystemCounterState();
    endSstate = SystemCounterState();
}

void PCM_Profiler::startTrialPowerProfile() {
    //Get CPU Core/Socket/Power States
    int sockets = pcm->getNumSockets();
    for (int i = 0; i < sockets; i++){
        startPowerState[i] = pcm->getServerUncorePowerState(i);
    }
    pcm->getAllCounterStates(startSstate, startSktstate, startCstates);
};

void PCM_Profiler::endTrialPowerProfile() {
    //Get CPU Core/Socket/Power States
    int sockets = pcm->getNumSockets();
    pcm->getAllCounterStates(endSstate, endSktstate, endCstates);
    for (int i = 0; i < sockets; i++){
        endPowerState[i] = pcm->getServerUncorePowerState(i);
    }
};

void PCM_Profiler::interTrialReset() {
    delete[] startPowerState;
    startPowerState = nullptr;
    delete[] endPowerState;
    endPowerState = nullptr;
};

TrialResult PCM_Profiler::computeTrialResult(){
    //At the superclass level, we compute the durations
    TrialResult result = Profiler::computeTrialResult();

    //Compute the trial result for the measurements
    int cores = pcm->getNumCores();
    int sockets = pcm->getNumSockets();

    std::vector<Measurement> avgCPUFreq;
    std::vector<Measurement> avgActiveCPUFreq;

    for(int i = 0; i<cores; i++)
    {  
        avgCPUFreq.push_back(Measurement(i, Unit(BaseUnit::HERTZ, 0), MeasurementCollectionType::INSTANTANEOUS, getAverageFrequency(startCstates[i], endCstates[i])));
        avgActiveCPUFreq.push_back(Measurement(i, Unit(BaseUnit::HERTZ, 0), MeasurementCollectionType::INSTANTANEOUS, getActiveAverageFrequency(startCstates[i], endCstates[i])));
    }

    result.measurements[MeasurementType::AVG_FREQ][HW_Granularity::CORE] = avgCPUFreq;
    result.measurements[MeasurementType::AVG_ACTIVE_FREQ][HW_Granularity::CORE] = avgActiveCPUFreq;

    std::vector<Measurement> energyCPUUsed;
    // std::vector<Measurement> energyDRAMUsed;
    std::vector<Measurement> startPackageThermalHeadroom;
    std::vector<Measurement> endPackageThermalHeadroom;
    for(int i = 0; i<sockets; i++)
    {
        energyCPUUsed.push_back(Measurement(i, Unit(BaseUnit::JOULE, 0), MeasurementCollectionType::CUMULATIVE_WHILE_SAMPLED, getConsumedJoules(startPowerState[i], endPowerState[i]))); //This is a degenerate case of MeasurmentCollectionType::CUMULATIVE_WHILE_SAMPLED since only 1 measurmenent is reported.  INSTANTANEOUS would have also worked 
        // energyDRAMUsed.push_back(Measurement(i, Unit(BaseUnit::JOULE, 0), MeasurmentCollectionType::CUMULATIVE, getDRAMConsumedJoules(startPowerState[i], endPowerState[i])));
        startPackageThermalHeadroom.push_back(Measurement(i, Unit(BaseUnit::DEG_CELSIUS, 0), MeasurementCollectionType::INSTANTANEOUS, startPowerState[i].getPackageThermalHeadroom()));
        endPackageThermalHeadroom.push_back(Measurement(i, Unit(BaseUnit::DEG_CELSIUS, 0), MeasurementCollectionType::INSTANTANEOUS, endPowerState[i].getPackageThermalHeadroom()));
    }

    result.measurements[MeasurementType::ENERGY_USED_CPU][HW_Granularity::SOCKET] = energyCPUUsed;
    // result.measurements[MeasurementType::ENERGY_USED_DRAM][HW_Granularity::SOCKET] = energyDRAMUsed;
    result.measurements[MeasurementType::THERMAL_HEADROOM_START][HW_Granularity::SOCKET] = startPackageThermalHeadroom;
    result.measurements[MeasurementType::THERMAL_HEADROOM_STOP][HW_Granularity::SOCKET] = endPackageThermalHeadroom;

    return result;
};

bool PCM_Profiler::detectsFreqChange(){
    return true;
}

bool PCM_Profiler::checkFreqChanged(std::vector<int> socketsOfInterest)
{
    int limit;
    if(socketsOfInterest.empty()){
        limit = pcm->getNumSockets();
    }else{
        limit = socketsOfInterest.size();
    }
    

    bool freq_changed = false;
    for(int ind = 0; ind < limit; ind++)
    {
        int socket;
        if(socketsOfInterest.empty()){
            socket = ind;
        }else{
            socket = socketsOfInterest[ind];
        }

        int freq_change_events = getPCUCounter(1, startPowerState[socket], endPowerState[socket]);
        if(freq_change_events > 0)
        {
            freq_changed = true;
            #if PRINT_FREQ_CHANGE_EVENT == 1
                printf("Socket %d experienced %d clock frequency change events!\n", socket, freq_change_events);
            #else
                break; //No need to check other sockets, frequency changed
            #endif
        }
    }

    return freq_changed;
}

std::string PCM_Profiler::profilerName(){
    return "OPCM";
}

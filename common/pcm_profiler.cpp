#include "pcm_profiler.h"

PCM_Profiler::PCM_Profiler{
    startPowerState = nullptr;
    endPowerState = nullptr;
}

PCM_Profiler::~PCM_Profiler{
    if(startPowerState){
        delete[] startPowerState;
    }
    if(endPowerState){
        delete[] endPowerState;
    }
}

MeasurmentCapabilities PCM_Profiler::findMeasurementCapabilities(){
    MeasurmentCapabilities capabilities;
    //For now, attempts to collect DRAM statistics but does not check if valid results returned

    //TODO: Could be expanded in future Intel chips.  This is what the PCM driver provides as of 2018

    capabilities.measurmentCapabilities[MeasurmentType::AVG_FREQ] = {HW_Granularity::CORE};
    capabilities.measurmentCapabilities[MeasurmentType::AVG_ACTIVE_FREQ] = {HW_Granularity::CORE};
    capabilities.measurmentCapabilities[MeasurmentType::ENERGY_USED_CPU] = {HW_Granularity::SOCKET};
    capabilities.measurmentCapabilities[MeasurmentType::ENERGY_USED_DRAM] = {HW_Granularity::SOCKET};
};

bool PCM_Profiler::checkProfiler(){
    CPUVendor cpuVendor = findCPUVendor();
    OS os = findOS();

    if(cpuVendor == CPUVendor::INTEL && (os == OS::LINUX || os == OS::MACOS || os = OS::WINDOWS)){
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
    m->disableJKTWorkaround();

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

    int sockets = pcm->getNumSockets();
    ServerUncorePowerState* startPowerState = new ServerUncorePowerState[sockets];
    ServerUncorePowerState* endPowerState = new ServerUncorePowerState[sockets];
}

void PCM_Profiler::startTrialPowerProfile() {
    //Get CPU Core/Socket/Power States
    int sockets = pcm->getNumSockets();
    for (int i = 0; i < sockets; i++)
        startPowerState[i] = pcm->getServerUncorePowerState(i);
    pcm->getAllCounterStates(startSstate, startSktstate, startCstates);
};

void PCM_Profiler::endTrialPowerProfile() {
    //Get CPU Core/Socket/Power States
    pcm->getAllCounterStates(endSstate, endSktstate, endCstates);
    for (int i = 0; i < sockets; i++)
        endPowerState[i] = pcm->getServerUncorePowerState(i);
};

void PCM_Profiler::interTrialReset() {
    //Nothing special required between trials
};

TrialResult PCM_Profiler::computeTrialReult(){

};
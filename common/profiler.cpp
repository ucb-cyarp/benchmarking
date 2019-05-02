#include "profiler.h"
#include <cpuid.h>
#include <x86intrin.h>
#include <cstdio>
#include <regex>

#include "pcm_profiler.h"
#include "generic_profiler.h"

CPUInfo::CPUInfo() : cpu(0), core(0), die(0), socket(0){
}

CPUInfo::CPUInfo(int cpu, int core, int die, int socket) : cpu(cpu), core(core), die(die), socket(socket){
}

Profiler::Profiler(){
    cpuTopology = Profiler::getCPUTopology();
}

Profiler::~Profiler(){
}

Profiler::CPUVendor Profiler::findCPUVendor(){
    unsigned int sig;
    unsigned int count = __get_cpuid_max(0, &sig);

    if(count>0){
        //CPUID Supported
        unsigned int eax, ebx, ecx, edx;
        __get_cpuid(0, &eax, &ebx, &ecx, &edx);

        if(ebx == signature_AMD_ebx && ecx == signature_AMD_ecx && edx == signature_AMD_edx){
            return Profiler::CPUVendor::AMD;
        }else if(ebx == signature_INTEL_ebx && ecx == signature_INTEL_ecx && edx == signature_INTEL_edx){
            return Profiler::CPUVendor::INTEL;
        }
    }

    return Profiler::CPUVendor::UNKNOWN;
}

std::string Profiler::findCPUModelStr(){
    unsigned int modelStr[4*4+1]; //Adding an extra char to force a \0 termination

    unsigned int sig;
    unsigned int count = __get_cpuid_max(0x80000000, &sig);

    if(count >= 0x80000004){
        unsigned int eax, ebx, ecx, edx;

        for(unsigned int i = 0; i<3; i++){
            __get_cpuid(0x80000002+i, &eax, &ebx, &ecx, &edx);
            modelStr[i*4] = eax;
            modelStr[i*4+1] = ebx;
            modelStr[i*4+2] = ecx;
            modelStr[i*4+3] = edx;
        }

        ((char*)modelStr)[4*4*4] = '\0';
    }else{
        return "";
    }

    std::string str((char*) modelStr);
    return str;
}

Profiler::OS Profiler::findOS(){
    //This technically finds the OS this was compiled under

    //Using list from https://sourceforge.net/p/predef/wiki/OperatingSystems/
    #ifdef __APPLE__
    return Profiler::OS::MACOS;
    #elif defined __gnu_linux__
    return Profiler::OS::LINUX;
    #elif defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__ || defined __bsdi__ || defined __DragonFly__
    return Profiler::OS::BSD;
    #elif defined __unix__ || defined __unix
    return Profiler::OS::UNIX;
    #elif defined __WINDOWS__ || defined _WIN16 || defined _WIN32 || defined _WIN64
    return Profiler::OS::WINDOWS;
    #endif

}

std::map<int, CPUInfo> Profiler::getCPUTopology(){
    //Right now, this depends on lscpu being installed

    //Uses the same technique as in common/cpuTopology.py

    FILE* lscpu = popen("lscpu -p=CPU,SOCKET,NODE,CORE", "r");
    if(lscpu != NULL){
        char* buffer = new char[2048];
        std::map<int, CPUInfo> topo;

        while(fgets(buffer, 2047, lscpu) != NULL){
            std::string lscpu_str = std::string(buffer);

            std::smatch matches;
            std::regex csvRegexExpr("[ |\\t]*([0-9]*),([0-9]*),([0-9]*),([0-9]*)[ |\\t|\\r|\\n]*");
            bool fixMatched = std::regex_match(lscpu_str, matches, csvRegexExpr);

            if(fixMatched && matches.size()==5){
                //Header will not parse but following lines should
                //Parse Matches
                //Match0 is the full match

                int cpu = std::stoi(matches[1].str());
                int socket = std::stoi(matches[2].str());
                int numa = std::stoi(matches[3].str());
                int core = std::stoi(matches[4].str());

                CPUInfo cpuInfo(cpu, core, numa, socket);
                topo[cpu] = cpuInfo;
            }
        }

        pclose(lscpu);
        delete[] buffer;
        return topo;
    }else{
        std::cerr << "Warning: Could not get CPU topology.  Make sure lscpu is installed and in the path" << std::endl;
    }
    
    return std::map<int, CPUInfo>();
}

Profiler* Profiler::ProfilerFactory(bool usePerformanceCounters){
    Profiler* profiler;

    if(usePerformanceCounters){
        CPUVendor cpuVendor = Profiler::findCPUVendor();

        if(cpuVendor == CPUVendor::INTEL){
            profiler = new PCM_Profiler();
        }else if(cpuVendor == CPUVendor::AMD){
            //TODO implement
            std::cerr << "Performance Counter Support is not yet Implemented for AMD" << std::endl;
            profiler = new GenericProfiler();
        }else{
            std::cerr << "Performance Counter Support is not Implemented for this Vendor" << std::endl;
            profiler = new GenericProfiler();
        }
    }else{
        profiler = new GenericProfiler();
    }

    return profiler;
}

bool Profiler::isSampling(){
    return false;
}

int64_t Profiler::samplingPeriod(){
    return 0;
}

void Profiler::startTrialTimer(){
    start_hrc = std::chrono::high_resolution_clock::now();
    start_clock = clock();
    start_rdtsc = _rdtsc();
}

void Profiler::endTrialTimer(){
    stop_hrc = std::chrono::high_resolution_clock::now();
    stop_clock = clock();
    stop_rdtsc = _rdtsc();
}

void Profiler::startTrial(){
    //Start power profile first since it may take some time to start
    startTrialPowerProfile();
    startTrialTimer();
}

void Profiler::endTrial(){
    //Stop power profile first since it may take some time to start
    endTrialPowerProfile();
    endTrialTimer();
}

TrialResult Profiler::computeTrialResult(){
    TrialResult result;

    //At the superclass level, only the time for the trial is computed and stored.
    result.duration = (std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(stop_hrc-start_hrc)).count();
    result.duration_clock = 1000.0 * (stop_clock - start_clock) / CLOCKS_PER_SEC;
    result.duration_rdtsc =  (stop_rdtsc - start_rdtsc);

    return result;
}

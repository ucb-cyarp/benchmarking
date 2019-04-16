#include "profiler.h"
#include <cpuid.h>
#include "ia32intrin.h"

Profiler::Profiler(){

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
    }else{
        return Profiler::CPUVendor::UNKNOWN;
    }
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
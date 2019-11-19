#include "generic_profiler.h"

GenericProfiler::GenericProfiler(){
}

GenericProfiler::~GenericProfiler(){
}

MeasurementCapabilities GenericProfiler::findMeasurementCapabilities(){
    MeasurementCapabilities capabilities;
    //Only collects timing information

    return capabilities;
};

bool GenericProfiler::checkProfiler(){
    return true; //Works on all platforms
};

void GenericProfiler::init()
{
    //No init required
}

void GenericProfiler::trialSetup(){
    //No setup required
}

void GenericProfiler::startTrialPowerProfile() {
    //Does not profile power
};

void GenericProfiler::endTrialPowerProfile() {
   //Does not profile power
};

void GenericProfiler::interTrialReset() {
    //Nothing special required between trials
};

TrialResult GenericProfiler::computeTrialResult(){
    //At the superclass level, we compute the durations
    TrialResult result = Profiler::computeTrialResult();

    //Durations are all we compute
    return result;
};

bool GenericProfiler::detectsFreqChange(){
    return false;
}

bool GenericProfiler::checkFreqChanged(std::vector<int> socketsOfInterest)
{
    return false;
}

std::string GenericProfiler::profilerName(){
    return "Generic";
}
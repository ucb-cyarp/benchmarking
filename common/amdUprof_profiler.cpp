#include "amdUprof_profiler.h"

#include <algorithm>

AMDuProfProfiler::AMDuProfProfiler() : initialized(false), amdCounterSize(0), amdCounterDescrs(nullptr), samplingInterval(100){
    
}

AMDuProfProfiler::~AMDuProfProfiler(){

    //If initialized, close AMD uProf
    if(initialized){
        AMDTResult exitState = AMDTPwrProfileClose();
        if(exitState != AMDT_STATUS_OK){
            //Would throw exception except this is a destructor
            std::cerr << "AMD uProf did not exit cleanly" << std::endl;
        }
    }
}

MeasurementCapabilities AMDuProfProfiler::findMeasurementCapabilities(){
    if(!initialized){
        throw std::runtime_error("Attempted to find measurement capabilities before AMD uProf was initialized");
    }

    return capabilities;
};

bool AMDuProfProfiler::checkProfiler(){
    CPUVendor cpuVendor = findCPUVendor();
    OS os = findOS();

    if(cpuVendor == CPUVendor::AMD && (os == OS::LINUX || os == OS::MACOS || os == OS::WINDOWS)){
        return true;
    }

    return false;
};

void AMDuProfProfiler::AMDMeasurmentType2MeasurementType(AMDTPwrCounterDesc amdCounterDesc, MeasurementType &measurementType, bool &foundMeasurementType){
    foundMeasurementType = false;
        
    switch(amdCounterDesc.m_category){
        case AMDT_PWR_CATEGORY_POWER:
            measurementType = MeasurementType::AVG_PWR_CPU;
            foundMeasurementType = true;
            break;
        case AMDT_PWR_CATEGORY_FREQUENCY:
            measurementType = MeasurementType::AVG_FREQ;
            foundMeasurementType = true;
            break;
        case AMDT_PWR_CATEGORY_TEMPERATURE:
            measurementType = MeasurementType::TEMPERATURE;
            foundMeasurementType = true;
            break;
        case AMDT_PWR_CATEGORY_VOLTAGE:
            measurementType = MeasurementType::VOLTAGE_CPU;
            foundMeasurementType = true;
            break;
        case AMDT_PWR_CATEGORY_CURRENT:
            measurementType = MeasurementType::CURRENT_CPU;
            foundMeasurementType = true;
            break;
        case AMDT_PWR_CATEGORY_PSTATE:
            measurementType = MeasurementType::P_STATE;
            foundMeasurementType = true;
            break;
        case AMDT_PWR_CATEGORY_CSTATES_RESIDENCY:
            measurementType = MeasurementType::C_STATE;
            foundMeasurementType = true;
            break;
        case AMDT_PWR_CATEGORY_TIME:
            //TODO: Implement?
            break;
        case AMDT_PWR_CATEGORY_ENERGY:
            measurementType = MeasurementType::ENERGY_USED_CPU;
            foundMeasurementType = true;
            break;
        case AMDT_PWR_CATEGORY_CORRELATED_POWER:
            //TODO: Evaluate if this should be included as well.
            //      Compare to other power catagory
            // measurementType = MeasurementType::AVG_PWR_CPU;
            // foundMeasurementType = true;
            break;
        case AMDT_PWR_CATEGORY_CAC:
            //TODO: Implement?
            break;
        case AMDT_PWR_CATEGORY_CONTROLLER:
            //TODO: Implement?
            break;
        case AMDT_PWR_CATEGORY_DPM:
            //TODO: Implement?
            break;
        case AMDT_PWR_CATEGORY_CNT:
            //TODO: Implement?
            break;
    }
}

void AMDuProfProfiler::AMDDeviceType2HW_Granularity(AMDTPwrCounterDesc amdCounterDesc, HW_Granularity &hwGranularity, bool &foundHwGranularity){
    foundHwGranularity = false;
        
    switch(amdCounterDesc.m_devType){
        case AMDT_PWR_DEVICE_SYSTEM:
            hwGranularity = HW_Granularity::SYSTEM;
            foundHwGranularity = true;
            break;
        case AMDT_PWR_DEVICE_PACKAGE:
            hwGranularity = HW_Granularity::SOCKET;
            foundHwGranularity = true;
            break;
        case AMDT_PWR_DEVICE_CPU_COMPUTE_UNIT:
            //This is an AMD specific concept of groups of cores into CCX units
            //Will not implement now
            //TODO: Reconsider
            break;
        case AMDT_PWR_DEVICE_CPU_CORE:
            //TODO: Check if this overlaps with AMDT_PWR_DEVICE_PHYSICAL_CORE
            hwGranularity = HW_Granularity::CORE;
            foundHwGranularity = true;
            break;
        case AMDT_PWR_DEVICE_DIE:
            hwGranularity = HW_Granularity::DIE;
            foundHwGranularity = true;
            break;
        case AMDT_PWR_DEVICE_PHYSICAL_CORE:
            //TODO: Check if this overlaps with AMDT_PWR_DEVICE_CPU_CORE
            hwGranularity = HW_Granularity::CORE;
            foundHwGranularity = true;
            break;
        case AMDT_PWR_DEVICE_THREAD:
            hwGranularity = HW_Granularity::THREAD;
            foundHwGranularity = true;
            break;
        case AMDT_PWR_DEVICE_INTERNAL_GPU:
            //Not implementing GPU for now
            break;
        case AMDT_PWR_DEVICE_EXTERNAL_GPU:
            //Not implementing GPU for now
            break;
        case AMDT_PWR_DEVICE_SVI2:
            //TODO: Implement?
            break;
        case AMDT_PWR_DEVICE_CNT:
            //TODO: Implement?
            break;
    }
}

MeasurementCollectionType AMDuProfProfiler::AMDTPwrAggregation2MeasurementCollectionType(AMDTPwrAggregation amdPwrAggregation){
    switch (amdPwrAggregation){
        case AMDTPwrAggregation::AMDT_PWR_VALUE_SINGLE:
            return MeasurementCollectionType::INSTANTANEOUS;
        case AMDTPwrAggregation::AMDT_PWR_VALUE_CUMULATIVE:
            return MeasurementCollectionType::CUMULATIVE;
    }
    //TODO: support more
    return MeasurementCollectionType::UNSUPPORTED;
}

void AMDuProfProfiler::AMDTPwrUnit2Unit(AMDTPwrUnit amdPwrUnit, Unit &unit, bool &validUnit){
    validUnit = false;

    switch(amdPwrUnit){
        case AMDTPwrUnit::AMDT_PWR_UNIT_TYPE_COUNT:
            unit = Unit(BaseUnit::UNITLESS, 0);
            validUnit = true;
            break;
        case AMDTPwrUnit::AMDT_PWR_UNIT_TYPE_NUMBER:
            unit = Unit(BaseUnit::UNITLESS, 0);
            validUnit = true;
            break;
        case AMDTPwrUnit::AMDT_PWR_UNIT_TYPE_PERCENT:
            //TOOD: Create percentage unit?
            unit = Unit(BaseUnit::UNITLESS, 0);
            validUnit = true;
            break;
        case AMDTPwrUnit::AMDT_PWR_UNIT_TYPE_RATIO:
            //TOOD: Create ratio unit?
            unit = Unit(BaseUnit::UNITLESS, 0);
            validUnit = true;
            break;
        case AMDTPwrUnit::AMDT_PWR_UNIT_TYPE_MILLI_SECOND:
            unit = Unit(BaseUnit::SECOND, -3);
            validUnit = true;
            break;
        case AMDTPwrUnit::AMDT_PWR_UNIT_TYPE_JOULE:
            unit = Unit(BaseUnit::JOULE, 0);
            validUnit = true;
            break;
        case AMDTPwrUnit::AMDT_PWR_UNIT_TYPE_WATT:
            unit = Unit(BaseUnit::WATT, 0);
            validUnit = true;
            break;
        case AMDTPwrUnit::AMDT_PWR_UNIT_TYPE_VOLT:
            unit = Unit(BaseUnit::VOLT, 0);
            validUnit = true;
            break;
        case AMDTPwrUnit::AMDT_PWR_UNIT_TYPE_MILLI_AMPERE:
            unit = Unit(BaseUnit::AMP, -3);
            validUnit = true;
            break;
        case AMDTPwrUnit::AMDT_PWR_UNIT_TYPE_MEGA_HERTZ:
            unit = Unit(BaseUnit::HERTZ, 6);
            validUnit = true;
            break;
        case AMDTPwrUnit::AMDT_PWR_UNIT_TYPE_CENTIGRADE:
            unit = Unit(BaseUnit::DEG_CELSIUS, 0);
            validUnit = true;
            break;
        case AMDTPwrUnit::AMDT_PWR_UNIT_TYPE_CNT:
            unit = Unit(BaseUnit::UNITLESS, 0);
            validUnit = true;
            break;
    }
}

void AMDuProfProfiler::init()
{
    //Initialize AMD uProf in online mode (allows collection and analysis of data durring profiling)
    AMDTResult result = AMDTPwrProfileInitialize(AMDT_PWR_MODE_TIMELINE_ONLINE);

    if(result != AMDT_STATUS_OK){
        throw std::runtime_error("Unable to initialize AMD uProf");
    }

    //Get the supported counters
    result = AMDTPwrGetSupportedCounters(&amdCounterSize, &amdCounterDescrs);

    std::set<AMDTPwrCategory> found_AMDT_PWR_DEVICE_CPU_CORE;
    std::set<AMDTPwrCategory> found_AMDT_PWR_DEVICE_PHYSICAL_CORE;

    //Itterate through the found counter and build the measurement capabilities
    //Also, assemble a map of counterID -> (MeasurementType, Granularity, Index)
    for(unsigned long i = 0; i<amdCounterSize; i++){
        MeasurementType measurementType;
        bool foundMeasurementType;
        AMDMeasurmentType2MeasurementType(amdCounterDescrs[i], measurementType, foundMeasurementType);
        if(!foundMeasurementType){
            continue; //Do not add this catagory to the Measuremenent capabilities
        }

        HW_Granularity hwGranularity;
        bool foundHwGranularity;
        AMDDeviceType2HW_Granularity(amdCounterDescrs[i], hwGranularity, foundHwGranularity);
        if(!foundHwGranularity){
            continue; //Do not add this catagory/granularity to the Measuremenent capabilities
        }

        Unit measurementUnit;
        bool validMeasurementUnit;
        AMDTPwrUnit2Unit(amdCounterDescrs[i].m_units, measurementUnit, validMeasurementUnit);
        if(!validMeasurementUnit){
            continue; //Do not add this catagory/granularity to the Measuremenent capabilities
        }

        MeasurementCollectionType collectionType = AMDTPwrAggregation2MeasurementCollectionType(amdCounterDescrs[i].m_aggregation);

        if(collectionType == MeasurementCollectionType::UNSUPPORTED){
            //Unsupported type, skip
            continue;
        }

        //TODO: Consider removing
        //Check for overlapping AMD catagories for same measurement type
        if(amdCounterDescrs[i].m_devType == AMDT_PWR_DEVICE_CPU_CORE){
            if(found_AMDT_PWR_DEVICE_PHYSICAL_CORE.find(amdCounterDescrs[i].m_category) != found_AMDT_PWR_DEVICE_PHYSICAL_CORE.end()){
                throw std::runtime_error("Found a measurement type that included both AMDT_PWR_DEVICE_CPU_CORE and AMDT_PWR_DEVICE_PHYSICAL_CORE granularities");
            }else{
                found_AMDT_PWR_DEVICE_CPU_CORE.insert(amdCounterDescrs[i].m_category);
            }
        }
        if(amdCounterDescrs[i].m_devType == AMDT_PWR_DEVICE_PHYSICAL_CORE){
            if(found_AMDT_PWR_DEVICE_CPU_CORE.find(amdCounterDescrs[i].m_category) != found_AMDT_PWR_DEVICE_CPU_CORE.end()){
                throw std::runtime_error("Found a measurement type that included both AMDT_PWR_DEVICE_CPU_CORE and AMDT_PWR_DEVICE_PHYSICAL_CORE granularities");
            }else{
                found_AMDT_PWR_DEVICE_PHYSICAL_CORE.insert(amdCounterDescrs[i].m_category);
            }
        }

        //Add this to the measurement capabilities and the map
        if(std::find(capabilities.measurementCapabilities[measurementType].begin(), capabilities.measurementCapabilities[measurementType].end(), hwGranularity) != capabilities.measurementCapabilities[measurementType].end()){
            capabilities.measurementCapabilities[measurementType].push_back(hwGranularity);
        }

        //Note, it is the device instance name that corresponds to our index
        amdCounterIDMap[amdCounterDescrs[i].m_counterID] = MeasurementCollectionPoint(measurementType, hwGranularity, amdCounterDescrs[i].m_devInstanceId, collectionType, measurementUnit);
    }

    result = AMDTPwrSetTimerSamplingPeriod(samplingInterval);
    if(result != AMDT_STATUS_OK){
        throw std::runtime_error("Unable to set sampling interval of AMD uProf");
    }
}

void AMDuProfProfiler::trialSetup(){
    //No specific trial setup required
}

void AMDuProfProfiler::startTrialPowerProfile() {
    AMDTResult result = AMDTPwrStartProfiling();

    if(result != AMDT_STATUS_OK){
        throw std::runtime_error("Unable to start profiling - AMD uProf");
    }
};

void AMDuProfProfiler::endTrialPowerProfile() {
    AMDTResult result = AMDTPwrProfileClose();

    if(result != AMDT_STATUS_OK){
        throw std::runtime_error("Unable to stop profiling - AMD uProf");
    }
};

void AMDuProfProfiler::interTrialReset() {
    //No specific inter trial reset
};

TrialResult AMDuProfProfiler::computeTrialResult(){
    //Itterate through the counters and see if there is a match in the map.
    //If so, dump the measurements into a result

    //Note that m_systemTime for samples is the start time of the profiling and does not change
    //between samples in the same profiling run (trial)
    //m_elapsedTimeMs contains the elapsed time since the profiling started.  This does change
    //between samples in a profile (trial).  However, this is relative to the start of profiling
    //and is therefore cumulative.

    //Emits a warning if the duration covered by the samples is < some percent of the duration measured
    //by the high resolution timer for the trial.  This will effect the accuracy of the linear interpolation

    //At the superclass level, we compute the durations
    TrialResult result = Profiler::computeTrialResult();

    //REMEMBER, set sampling to true!
    result.sampled = true;

    //Get the samples
    AMDTUInt32 numSamples = 0;
    AMDTPwrSample* samples;
    AMDTResult status = AMDTPwrReadAllEnabledCounters(&numSamples, &samples);

    if(status != AMDT_STATUS_OK){
        throw std::runtime_error("A problem was encountered when fetching samples");
    }

    if(numSamples == 0){
        throw std::runtime_error("The sample period was set such that no samples were corrected for the trial.");
    }else{
        double sampledDurationMS = samples[numSamples-1].m_elapsedTimeMs; //This is the cumulative time since the trial started.
        if (abs(result.duration - sampledDurationMS)/result.duration > SAMPLING_DURATION_WARNING_THRESHOLD){
            std::cerr << "Warning! The chosen Sampling Duration resulted in a samplign period which is < " << (1-SAMPLING_DURATION_WARNING_THRESHOLD) << "of the measured trial duration.  This may result in inaccurate results."  << std::endl;
        }
    }

    //Itterates through the samples
    for(unsigned long sampleInd = 0; sampleInd<numSamples; sampleInd++){
        double sampleTimestamp = samples[sampleInd].m_elapsedTimeMs;

        //Itterate through the counters within that sample
        for(unsigned long counterInd = 0; counterInd<samples[sampleInd].m_numOfCounter; counterInd++){
            if(amdCounterIDMap.find(samples[sampleInd].m_counterValues[counterInd].m_counterID) != amdCounterIDMap.end()){
                //This counter is in our map of counters to collect
                
                MeasurementCollectionPoint counterInfo = amdCounterIDMap[samples[sampleInd].m_counterValues[counterInd].m_counterID];

                //Check if a result already exits for this counter
                if(result.measurements[counterInfo.measurementType][counterInfo.granularity].size() > counterInfo.index){
                    //An entry already exists

                }else{
                    //An entry does not exist yet, create it
                    for(long i = result.measurements[counterInfo.measurementType][counterInfo.granularity].size(); i<=counterInfo.index; i++){
                        Measurement newMeasurement;
                        newMeasurement.index = i;
                        result.measurements[counterInfo.measurementType][counterInfo.granularity].push_back(newMeasurement);
                    }
                }

                Measurement* measurementToModify = &result.measurements[counterInfo.measurementType][counterInfo.granularity][counterInfo.index];

                //The entry may exist but may not have had any data added to it yet (ex. counters out of order)
                if(measurementToModify->measurement.size() == 0){
                    //This measurement has not been initialized yet
                    measurementToModify->deltaTUnit = Unit(BaseUnit::SECOND, -3);
                    
                    measurementToModify->collectionType = counterInfo.collectionType;
                    measurementToModify->unit = counterInfo.unit;
                }

                //Add the data from this counter

                double lastTimestamp = 0;
                
                //Sum up time deltas up to this point to get the last time stamp
                for(unsigned long i = 0; i<measurementToModify->deltaT.size(); i++){
                    lastTimestamp += measurementToModify->deltaT[i];
                }

                //TODO: consider removing
                if(sampleTimestamp - lastTimestamp < 0){
                    //Because the timestamp if cumulative, if the timestamp is less than the sum of the deltas of the preceeding
                    //samples, it means that a sample after this one has already been counted.  Therefore, samples were out of order
                    throw std::runtime_error("Found a sample with a timestep that is out of order");
                }

                measurementToModify->deltaT.push_back(sampleTimestamp - lastTimestamp);
                measurementToModify->measurement.push_back(samples[sampleInd].m_counterValues[counterInd].m_data);
            }
            
        }
    }

    return result;
};

bool AMDuProfProfiler::detectsFreqChange(){
    return false;
}

bool AMDuProfProfiler::checkFreqChanged()
{
    return false;
}

std::string AMDuProfProfiler::profilerName(){
    return "AMD uProf";
}

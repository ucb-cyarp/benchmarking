#include "results.h"
#include "measurement.h"
//#include "intrin_bench_default_defines_and_imports_cpp.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <fstream>
#include <iostream>
#include <set>

double average(double* arr, size_t len)
{
    double avg = 0;
    for(size_t i = 0; i < len; i++)
    {
        avg += (double) arr[i];
    }
    avg /= len;

    return avg;
}

double average(const std::vector<double> arr)
{
    double avg = 0;
    for(size_t i = 0; i < arr.size(); i++)
    {
        avg += (double) arr[i];
    }
    avg /= arr.size();

    return avg;
}

double standard_dev(double* arr, size_t len)
{
    double std_dev = 0;
    double avg = average(arr, len);

    for(size_t i = 0; i < len; i++)
    {
        double tmp = arr[i] - avg;
        tmp = tmp*tmp;

        std_dev += tmp;
    }
    std_dev /= (len - 1);
    std_dev = sqrt(std_dev);

    return std_dev;
}

Statistics::Statistics() : avg(0), stdDev(0), unit(Unit(BaseUnit::UNITLESS, 0)), valid(true){

}
Statistics::Statistics(double avg, double stdDev, Unit unit, bool cumulative, bool valid) : avg(avg), stdDev(stdDev), unit(unit), cumulative(cumulative), valid(valid){

}
Statistics::Statistics(double avg, double stdDev, Unit unit, bool cumulative) : avg(avg), stdDev(stdDev), unit(unit), cumulative(cumulative), valid(true){

}

TrialResult::TrialResult() : sampled(false){}
TrialResult::TrialResult(bool sampled) : sampled(sampled){}

void TrialResult::print_trial(const std::vector<HW_Granularity> granularityToPrint, const std::vector<MeasurementType> measurementTypeToPrint)
{
    Unit timeOrigUnit(BaseUnit::SECOND, -3); //Orig unit is ms
    Unit timeTgtUnit(BaseUnit::SECOND, -3);
    std::string unitStr = MeasurementHelper::exponentAbrev(timeTgtUnit.exponent)+MeasurementHelper::BaseUnit_abrev(timeTgtUnit.baseUnit);
    printf("         ##### System Trial [%d] Statistics - Timing #####\n", trial);
    printf("             Steady Clock Duration (%s): %f\n", unitStr.c_str(), Unit::scale(timeOrigUnit, timeTgtUnit, duration));
    printf("             Clock Duration (%s): %f\n", unitStr.c_str(), Unit::scale(timeOrigUnit, timeTgtUnit, duration_clock));
    //printf("             rdtsc Duration (%s): %f\n", unitStr.c_str(), Unit::scale(timeOrigUnit, timeTgtUnit, duration_rdtsc));

    //Print Statistics for each level of HW granularity
    for(unsigned long i = 0; i<measurementTypeToPrint.size(); i++){
        bool foundMeasurement = false;
        for(unsigned long j = 0; j<granularityToPrint.size(); j++){
            if(measurements.find(measurementTypeToPrint[i]) != measurements.end() && 
            measurements[measurementTypeToPrint[i]].find(granularityToPrint[j]) != measurements[measurementTypeToPrint[i]].end()){
                for(unsigned long k = 0; k<measurements[measurementTypeToPrint[i]][granularityToPrint[j]].size(); k++){
                    if(!foundMeasurement){
                        //Found the first measurement at this granularity, print the section header
                        printf("\n");
                        printf("         ##### %s Trial [%d] Statistics #####\n", MeasurementHelper::MeasurementType_toString(measurementTypeToPrint[i]).c_str(), trial);
                        foundMeasurement = true;
                    }
                    std::string unitStr = MeasurementHelper::exponentAbrev(measurements[measurementTypeToPrint[i]][granularityToPrint[j]][k].unit.exponent)+MeasurementHelper::BaseUnit_abrev(measurements[measurementTypeToPrint[i]][granularityToPrint[j]][k].unit.baseUnit);
                    printf("             %s %s[%2lu] Mean (%s): %f\n", MeasurementHelper::MeasurementType_toString(measurementTypeToPrint[i]).c_str(), MeasurementHelper::HW_Granularity_toString(granularityToPrint[j]).c_str(), k, unitStr.c_str(), average(measurements[measurementTypeToPrint[i]][granularityToPrint[j]][k].measurement));
                }
            }
        }
    }

    //Print benchmark specific results (if any)
    if(benchmarkSpecificResults.size() > 0){
        printf("\n");
        std::string benchmarkSpecificResultsHeader = benchmarkSpecificResults[0]->getTrialResultsHeader();
        printf("         ##### %s Trial [%d] Statistics #####\n", benchmarkSpecificResultsHeader.c_str(), trial);

        for(int i = 0; i<benchmarkSpecificResults.size(); i++){
            printf("             %s [%d] %s\n", MeasurementHelper::HW_Granularity_toString(benchmarkSpecificResults[i]->resultGranularity).c_str(), benchmarkSpecificResults[i]->granularityIndex, benchmarkSpecificResults[i]->getTrialResults().c_str());
        }
    }
}

Results::Results()
{
}

Statistics Results::measurementStats(MeasurementType measurmentType, HW_Granularity granularity, int32_t index, bool treatTrialSamplesAsLumped){
    //Itterate through the measurements to find the ones that apply
    std::vector<Measurement> filteredMeasurements;
    for(unsigned long i = 0; i<trial_results.size(); i++){
        //Itterate through the measurements for this trial at the specified meaurment type and granularity level
        if(trial_results[i].measurements.find(measurmentType) != trial_results[i].measurements.end() && 
        trial_results[i].measurements[measurmentType].find(granularity) != trial_results[i].measurements[measurmentType].end()){
            for(unsigned long j = 0; j<trial_results[i].measurements[measurmentType][granularity].size(); j++){
                if(trial_results[i].measurements[measurmentType][granularity][j].index == index){

                    bool shouldLump = treatTrialSamplesAsLumped && trial_results[i].measurements[measurmentType][granularity][j].measurement.size()>1;
                    bool isCumulativeAndSampled = (trial_results[i].measurements[measurmentType][granularity][j].collectionType == MeasurementCollectionType::CUMULATIVE_DELTA ||
                                                   trial_results[i].measurements[measurmentType][granularity][j].collectionType == MeasurementCollectionType::CUMULATIVE_WHILE_SAMPLED) && trial_results[i].sampled;
                    
                    if(shouldLump || isCumulativeAndSampled){ //Still need to run of the degenerate cumulative case of 1 sample if a sampling profiler was used -- need to interpolate

                        if(trial_results[i].measurements[measurmentType][granularity][j].collectionType == MeasurementCollectionType::CUMULATIVE_WHILE_SAMPLED){
                            //Just take the last measurement in the sample
                            unsigned long numSamples = trial_results[i].measurements[measurmentType][granularity][j].measurement.size();
                            if(numSamples>=1){
                                double duration = 0;
                                for(unsigned long k = 0; k<trial_results[i].measurements[measurmentType][granularity][j].deltaT.size(); k++){
                                    duration += trial_results[i].measurements[measurmentType][granularity][j].deltaT[k];
                                }

                                //Need to scale measurement
                                double sampledVal = trial_results[i].measurements[measurmentType][granularity][j].measurement[numSamples-1];

                                Unit durationUnit(BaseUnit::SECOND, -3);
                                double scaledVal = sampledVal * trial_results[i].duration / Unit::scale(trial_results[i].measurements[measurmentType][granularity][j].deltaTUnit, durationUnit, duration);

                                Measurement cumulativeMeasurement;
                                cumulativeMeasurement.index = trial_results[i].measurements[measurmentType][granularity][j].index;
                                cumulativeMeasurement.unit = trial_results[i].measurements[measurmentType][granularity][j].unit;
                                cumulativeMeasurement.measurement.push_back(scaledVal);
                                cumulativeMeasurement.deltaTUnit = durationUnit;
                                cumulativeMeasurement.deltaT.push_back(scaledVal);
                                cumulativeMeasurement.collectionType = trial_results[i].measurements[measurmentType][granularity][j].collectionType;
                                filteredMeasurements.push_back(cumulativeMeasurement);
                            }
                        }else{
                            //Average the measurements (if instantanious) or sum (if cumulative_delya)
                            double sum = 0;
                            double sampleDuration = 0;
                            for(unsigned long k = 0; k<trial_results[i].measurements[measurmentType][granularity][j].measurement.size(); k++){
                                sum += trial_results[i].measurements[measurmentType][granularity][j].measurement[k];
                                sampleDuration += trial_results[i].measurements[measurmentType][granularity][j].deltaT[k];
                            }

                            double measurementVal=0;
                            if(trial_results[i].measurements[measurmentType][granularity][j].collectionType == MeasurementCollectionType::INSTANTANEOUS){
                                measurementVal = sum/trial_results[i].measurements[measurmentType][granularity][j].measurement.size();
                            }else if(trial_results[i].measurements[measurmentType][granularity][j].collectionType == MeasurementCollectionType::CUMULATIVE_DELTA){
                                //Interpolate cumulative to the reported measurement time
                                //We need to do this because, when sampling, the last moments of the trial may not be captured.
                                //HWhen normalized to the number of samples computed, this may give an optomistic view
                                //Linear interpolation is used to grow the measurment to the value expected for the remaining portion of the run.
                                //NOTE: this is only accurate if the sampling interval is sufficiently short that the interpolation is only for a small
                                //fraction of the trial
                                Unit durationUnit(BaseUnit::SECOND, -3);
                                measurementVal = sum * trial_results[i].duration / Unit::scale(trial_results[i].measurements[measurmentType][granularity][j].deltaTUnit, durationUnit, sampleDuration);
                            }else{
                                throw std::runtime_error("Unknown MeasurementCollectionType");
                            }

                            Measurement lumpedMeasurement;
                            lumpedMeasurement.index = trial_results[i].measurements[measurmentType][granularity][j].index;
                            lumpedMeasurement.unit = trial_results[i].measurements[measurmentType][granularity][j].unit;
                            lumpedMeasurement.deltaTUnit = Unit(BaseUnit::SECOND, -3);
                            lumpedMeasurement.deltaT.push_back(trial_results[i].duration);
                            lumpedMeasurement.measurement.push_back(measurementVal);
                            lumpedMeasurement.collectionType = trial_results[i].measurements[measurmentType][granularity][j].collectionType;
                            filteredMeasurements.push_back(lumpedMeasurement);
                        }
                    }else if(trial_results[i].measurements[measurmentType][granularity][j].collectionType != MeasurementCollectionType::CUMULATIVE_DELTA &&
                    trial_results[i].measurements[measurmentType][granularity][j].collectionType != MeasurementCollectionType::CUMULATIVE_WHILE_SAMPLED &&
                    trial_results[i].measurements[measurmentType][granularity][j].collectionType != MeasurementCollectionType::INSTANTANEOUS){
                        throw std::runtime_error("Unknown MeasurementCollectionType");
                    }
                    else{
                        //This is not supposed to be treated as lumped or is not cumulative and sampled
                        //Pass the measurement, with the full array of values
                        if((trial_results[i].measurements[measurmentType][granularity][j].collectionType == MeasurementCollectionType::CUMULATIVE_DELTA ||
                         trial_results[i].measurements[measurmentType][granularity][j].collectionType == MeasurementCollectionType::CUMULATIVE_WHILE_SAMPLED) &&
                        trial_results[i].measurements[measurmentType][granularity][j].measurement.size()>1){
                            throw std::runtime_error("A measurement was found which was cumulative and had >1 data points but was not marked as being sampled");
                        }else{
                            filteredMeasurements.push_back(trial_results[i].measurements[measurmentType][granularity][j]);
                        }
                    }
                }
            }
        }
    }

    if(filteredMeasurements.size() == 0){
        return Statistics(0, 0, Unit(BaseUnit::UNITLESS, 0), false, false);
    }

    double sum = 0;
    unsigned long count = 0;
    Unit avgUnit;
    //Compute the average
    for(unsigned long i = 0; i<filteredMeasurements.size(); i++){
        if(i == 0){
            //Set the unit based off of the first measurement
            avgUnit = filteredMeasurements[i].unit;
        }

        //Check for sampled
        if(filteredMeasurements[i].measurement.size()>1 && treatTrialSamplesAsLumped){
            throw std::runtime_error("Specified that sampled trials should be treated as lumped but were not properly processed.");
        }

        //Check for unit change
        double scaleFactor = Unit::scaleFactor(filteredMeasurements[i].unit, avgUnit);

        for(unsigned long j = 0; j<filteredMeasurements[i].measurement.size(); j++){
            sum += scaleFactor*(filteredMeasurements[i].measurement[j]);
            count++;
        }
    }

    double avg = sum/count;

    //Compute the stdDev
    double stdDev = 0;
    for(unsigned long i = 0; i<filteredMeasurements.size(); i++){
        //AvgUnit already set

        //Already Checked for sampled
        //Already checked for baseUnit change

        //Check for unit change
        double scaleFactor = Unit::scaleFactor(filteredMeasurements[i].unit, avgUnit);

        for(unsigned long j = 0; j<filteredMeasurements[i].measurement.size(); j++){
            double tmp = scaleFactor*(filteredMeasurements[i].measurement[j]) - avg;
            tmp = tmp*tmp;
            stdDev += tmp;
        }
        stdDev /= (count - 1);
        stdDev = sqrt(stdDev);
    }

    bool isCumulative = false;
    if(filteredMeasurements.size() > 0){
        isCumulative = filteredMeasurements[0].collectionType == MeasurementCollectionType::CUMULATIVE_DELTA || 
                        filteredMeasurements[0].collectionType == MeasurementCollectionType::CUMULATIVE_WHILE_SAMPLED; 
    }

    return Statistics(avg, stdDev, avgUnit, isCumulative);
}

TrialResult* Results::add_trial(TrialResult &trial)
{
    trial_results.push_back(trial);
    return &trial_results[trial_results.size()-1];
}

TrialResult* Results::add_trial_set_trialInd(TrialResult &trial)
{
    trial.trial = trial_results.size();
    trial_results.push_back(trial);
    return &trial_results[trial_results.size()-1];
}

void Results::remove_last_trial(){
    trial_results.pop_back();
}

double Results::avg_duration()
{
    size_t trials = trial_results.size();
    double result[trials];

    for(size_t i = 0; i<trials; i++)
    {
        result[i] = trial_results[i].duration;
    }

    return average(result, trials);
}

double Results::avg_duration_clock()
{
    size_t trials = trial_results.size();
    double result[trials];

    for(size_t i = 0; i<trials; i++)
    {
        result[i] = trial_results[i].duration_clock;
    }

    return average(result, trials);
}

double Results::avg_duration_rdtsc()
{
    size_t trials = trial_results.size();
    double result[trials];

    for(size_t i = 0; i<trials; i++)
    {
        result[i] = trial_results[i].duration_rdtsc;
    }

    return average(result, trials);
}

double Results::stddev_duration()
{
    size_t trials = trial_results.size();
    double result[trials];

    for(size_t i = 0; i<trials; i++)
    {
        result[i] = trial_results[i].duration;
    }

    return standard_dev(result, trials);
}

double Results::stddev_duration_clock()
{
    size_t trials = trial_results.size();
    double result[trials];

    for(size_t i = 0; i<trials; i++)
    {
        result[i] = trial_results[i].duration_clock;
    }

    return standard_dev(result, trials);
}

double Results::stddev_duration_rdtsc()
{
    size_t trials = trial_results.size();
    double result[trials];

    for(size_t i = 0; i<trials; i++)
    {
        result[i] = trial_results[i].duration_rdtsc;
    }

    return standard_dev(result, trials);
}

void Results::print_statistics(int socket, int dies, int core, int thread, int stim_len, const std::vector<HW_Granularity> granularityToPrint, const std::vector<MeasurementType> measurementTypeToPrint)
{
    std::vector<int> socket_vec = {socket};
    std::vector<int> dies_vec = {dies};
    std::vector<int> core_vec = {core};
    std::vector<int> thread_vec = {thread};

    print_statistics(socket_vec, dies_vec, core_vec, thread_vec, stim_len, granularityToPrint, measurementTypeToPrint);
}

void Results::print_statistics(std::vector<int> sockets, std::vector<int> dies, std::vector<int> cores, std::vector<int> threads, int stim_len, const std::vector<HW_Granularity> granularityToPrint, const std::vector<MeasurementType> measurementTypeToPrint)
{
    double avg_duration_dbl = this->avg_duration();
    double stddev_duration_dbl = this->stddev_duration();

    double avg_duration_clock_dbl = this->avg_duration_clock();
    double stddev_duration_clock_dbl = this->stddev_duration_clock();

    // printf("High Res Clock - Sample Mean (ms): %f, Sample Std Dev: %f\n", avg_duration, std_dev_duration);

    printf("         ##### Steady Clock - Monotonically Increasing Clock #####\n");
    printf("             Steady Clock - Sample Mean (ms): %f, Sample Std Dev: %f\n", avg_duration_dbl, stddev_duration_dbl);
    printf("             Steady Clock Normalized to Sample - Sample Mean (ns): %f, Sample Std Dev: %f\n", avg_duration_dbl*1000000/stim_len, stddev_duration_dbl*1000000/stim_len);
    printf("             Steady Clock - Sample Mean (MS/s): %f\n", stim_len*1.0/(1000.0*avg_duration_dbl));

    printf("\n");
    printf("         ##### clock() Duration - Process CPU Time (May be Different from Wall Clock Time - Cumulative Time For All Threads) #####\n");
    printf("             Clock - Sample Mean (ms): %f, Sample Std Dev: %f\n", avg_duration_clock_dbl, stddev_duration_clock_dbl);
    printf("             *Clock Normalized to Sample - Sample Mean (ns): %f, Sample Std Dev: %f\n", avg_duration_clock_dbl*1000000/stim_len, stddev_duration_clock_dbl*1000000/stim_len);
    printf("             *Clock - Sample Mean (MS/s): %f\n", stim_len*1.0/(1000.0*avg_duration_clock_dbl));

    //Print Statistics for each level of HW granularity
    for(unsigned long i = 0; i<measurementTypeToPrint.size(); i++){
        bool foundMeasurement = false;
        for(unsigned long j = 0; j<granularityToPrint.size(); j++){
            std::vector<int> ind_to_search;
            if(granularityToPrint[j] == HW_Granularity::SYSTEM){
                ind_to_search.push_back(0);
            }else if(granularityToPrint[j] == HW_Granularity::DIE){
                ind_to_search = dies;
            }else if(granularityToPrint[j] == HW_Granularity::SOCKET){
                ind_to_search = sockets;
            }else if(granularityToPrint[j] == HW_Granularity::CORE){
                ind_to_search = cores;
            }else if(granularityToPrint[j] == HW_Granularity::THREAD){
                ind_to_search = threads;
            }

            for(unsigned long k = 0; k<ind_to_search.size(); k++){
                Statistics stats = measurementStats(measurementTypeToPrint[i], granularityToPrint[j], ind_to_search[k], true);
                if(!foundMeasurement && stats.valid){
                    //Found the first measurement at this granularity, print the section header
                    printf("\n");
                    printf("         ##### %s Statistics #####\n", MeasurementHelper::MeasurementType_toString(measurementTypeToPrint[i]).c_str());
                    foundMeasurement = true;
                }
                if(stats.valid){
                    std::string unitStr = MeasurementHelper::exponentAbrev(stats.unit.exponent)+MeasurementHelper::BaseUnit_abrev(stats.unit.baseUnit);
                    printf("             %s %s[%2d] Mean (%s): %f, Sample Std Dev: %f\n", MeasurementHelper::MeasurementType_toString(measurementTypeToPrint[i]).c_str(), MeasurementHelper::HW_Granularity_toString(granularityToPrint[j]).c_str(), ind_to_search[k], unitStr.c_str(), stats.avg, stats.stdDev);
                    if(stats.cumulative){
                        Unit normalizeUnit = stats.unit;
                        normalizeUnit.exponent = normalizeUnit.exponent - 6; //TODO, make scale with trial size
                        std::string normalizeUnitStr = MeasurementHelper::exponentAbrev(normalizeUnit.exponent)+MeasurementHelper::BaseUnit_abrev(normalizeUnit.baseUnit);
                        double scaleFactor = Unit::scaleFactor(stats.unit, normalizeUnit);
                        printf("             %s %s[%2lu] Mean - Normalized to Sample (%s): %f, Sample Std Dev: %f\n", MeasurementHelper::MeasurementType_toString(measurementTypeToPrint[i]).c_str(), MeasurementHelper::HW_Granularity_toString(granularityToPrint[j]).c_str(), k, normalizeUnitStr.c_str(), scaleFactor*stats.avg/stim_len, scaleFactor*stats.stdDev/stim_len);
                    }
                }
            }
        }
    }
    //printf("\n");
}

void Results::print_statistics(int stim_len, const std::vector<HW_Granularity> granularityToPrint, const std::vector<MeasurementType> measurementTypeToPrint)
{
    double avg_duration_dbl = this->avg_duration();
    double stddev_duration_dbl = this->stddev_duration();

    double avg_duration_clock_dbl = this->avg_duration_clock();
    double stddev_duration_clock_dbl = this->stddev_duration_clock();

    // printf("High Res Clock - Sample Mean (ms): %f, Sample Std Dev: %f\n", avg_duration, std_dev_duration);

    printf("         ##### Steady Clock - Monotonically Increasing Clock #####\n");
    printf("             Steady Clock - Sample Mean (ms): %f, Sample Std Dev: %f\n", avg_duration_dbl, stddev_duration_dbl);
    printf("             Steady Clock Normalized to Sample - Sample Mean (ns): %f, Sample Std Dev: %f\n", avg_duration_dbl*1000000/stim_len, stddev_duration_dbl*1000000/stim_len);
    printf("             Steady Clock - Sample Mean (MS/s): %f\n", stim_len*1.0/(1000.0*avg_duration_dbl));

    printf("\n");
    printf("         ##### clock() Duration - Process CPU Time (May be Different from Wall Clock Time - Cumulative Time For All Threads) #####\n");
    printf("             Clock - Sample Mean (ms): %f, Sample Std Dev: %f\n", avg_duration_clock_dbl, stddev_duration_clock_dbl);
    printf("             *Clock Normalized to Sample - Sample Mean (ns): %f, Sample Std Dev: %f\n", avg_duration_clock_dbl*1000000/stim_len, stddev_duration_clock_dbl*1000000/stim_len);
    printf("             *Clock - Sample Mean (MS/s): %f\n", stim_len*1.0/(1000.0*avg_duration_clock_dbl));

    //Print Statistics for each level of HW granularity
    for(unsigned long i = 0; i<measurementTypeToPrint.size(); i++){
        bool foundMeasurement = false;
        for(unsigned long j = 0; j<granularityToPrint.size(); j++){
            bool searching = true;
            for(unsigned long k = 0; searching; k++){
                Statistics stats = measurementStats(measurementTypeToPrint[i], granularityToPrint[j], k, true);
                searching = stats.valid;
                if(!foundMeasurement && stats.valid){
                    //Found the first measurement at this granularity, print the section header
                    printf("\n");
                    printf("         ##### %s Statistics #####\n", MeasurementHelper::MeasurementType_toString(measurementTypeToPrint[i]).c_str());
                    foundMeasurement = true;
                }
                if(stats.valid){
                    std::string unitStr = MeasurementHelper::exponentAbrev(stats.unit.exponent)+MeasurementHelper::BaseUnit_abrev(stats.unit.baseUnit);
                    printf("             %s %s[%2lu] Mean (%s): %f, Sample Std Dev: %f\n", MeasurementHelper::MeasurementType_toString(measurementTypeToPrint[i]).c_str(), MeasurementHelper::HW_Granularity_toString(granularityToPrint[j]).c_str(), k, unitStr.c_str(), stats.avg, stats.stdDev);
                    if(stats.cumulative){
                        Unit normalizeUnit = stats.unit;
                        normalizeUnit.exponent = normalizeUnit.exponent - 6; //TODO, make scale with trial size
                        std::string normalizeUnitStr = MeasurementHelper::exponentAbrev(normalizeUnit.exponent)+MeasurementHelper::BaseUnit_abrev(normalizeUnit.baseUnit);
                        double scaleFactor = Unit::scaleFactor(stats.unit, normalizeUnit);
                        printf("             %s %s[%2lu] Mean - Normalized to Sample (%s): %f, Sample Std Dev: %f\n", MeasurementHelper::MeasurementType_toString(measurementTypeToPrint[i]).c_str(), MeasurementHelper::HW_Granularity_toString(granularityToPrint[j]).c_str(), k, normalizeUnitStr.c_str(), scaleFactor*stats.avg/stim_len, scaleFactor*stats.stdDev/stim_len);
                    }
                }
            }
        }
    }
    //printf("\n");
}

void Results::print_statistics(std::set<int> sockets, std::set<int> dies, std::set<int> cores, std::set<int> threads, int stim_len, const std::vector<HW_Granularity> granularityToPrint, const std::vector<MeasurementType> measurementTypeToPrint)
{
    std::vector<int> socket_vec;
    for(auto it = sockets.begin(); it != sockets.end(); it++){
        socket_vec.push_back(*it);
    }

    std::vector<int> dies_vec;
    for(auto it = dies.begin(); it != dies.end(); it++){
        dies_vec.push_back(*it);
    }

    std::vector<int> core_vec;
    for(auto it = cores.begin(); it != cores.end(); it++){
        core_vec.push_back(*it);
    }

    std::vector<int> thread_vec;
    for(auto it = threads.begin(); it != threads.end(); it++){
        thread_vec.push_back(*it);
    }

    print_statistics(socket_vec, dies_vec, core_vec, thread_vec, stim_len, granularityToPrint, measurementTypeToPrint);
}

void Results::write_csv(std::ofstream &csv_file, int socket, int core, int thread, const std::vector<HW_Granularity> granularityToPrint, const std::vector<MeasurementType> measurementTypeToPrint)
{
    write_csv(csv_file, socket, core, thread, "", 0, granularityToPrint, measurementTypeToPrint);
}

void Results::write_csv(std::ofstream &csv_file, int socket, int core, int thread, std::string col0_name, int col0_val, const std::vector<HW_Granularity> granularityToPrint, const std::vector<MeasurementType> measurementTypeToPrint)
{
    std::map<MeasurementType, std::map<HW_Granularity, std::map<int, Unit>>> avail = measurementsAvailUnion(granularityToPrint, measurementTypeToPrint); 

    //Print Header
    csv_file << std::string(col0_name.empty() ? "" : "\"" + col0_name + "\",") << "\"Steady Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"";

    size_t trials = trial_results.size();

    if(trials>0){
        for(int i = 0; i<trial_results[0].benchmarkSpecificResults.size(); i++){
            csv_file << "," << trial_results[0].benchmarkSpecificResults[i]->getTrialCSVHeader();
        }
    }

    for(unsigned long i = 0; i<measurementTypeToPrint.size(); i++){
        if(avail.find(measurementTypeToPrint[i]) != avail.end()){
            //Found the measurement
            for(unsigned long j = 0; j<granularityToPrint.size(); j++){
                if(avail[measurementTypeToPrint[i]].find(granularityToPrint[j]) != avail[measurementTypeToPrint[i]].end()){
                    int indexes = avail[measurementTypeToPrint[i]][granularityToPrint[j]].size();
                    for(unsigned long k = 0; k<indexes; k++){
                        csv_file << ",\"" << MeasurementHelper::MeasurementType_toString(measurementTypeToPrint[i]);
                        csv_file << " - " << MeasurementHelper::HW_Granularity_toString(granularityToPrint[j]);
                        csv_file << "[" << k << "] (";
                        csv_file << MeasurementHelper::exponentAbrev(avail[measurementTypeToPrint[i]][granularityToPrint[j]][k].exponent);
                        csv_file << MeasurementHelper::BaseUnit_abrev(avail[measurementTypeToPrint[i]][granularityToPrint[j]][k].baseUnit);
                        csv_file << ")\"";
                    }
                }
            }
        }
    }

    csv_file << std::endl;

    for(size_t trial = 0; trial < trials; trial++)
    {
        if(!col0_name.empty()){
            csv_file << col0_val;
        }
        csv_file << std::scientific << trial_results[trial].duration << "," << trial_results[trial].duration_clock << "," << trial_results[trial].duration_rdtsc;

        for(int i = 0; i<trial_results[trial].benchmarkSpecificResults.size(); i++){
            csv_file << "," << trial_results[trial].benchmarkSpecificResults[i]->getTrialCSVData();
        }
        
        for(unsigned long i = 0; i<measurementTypeToPrint.size(); i++){
            if(avail.find(measurementTypeToPrint[i]) != avail.end()){
                //Found the measurement
                for(unsigned long j = 0; j<granularityToPrint.size(); j++){
                    if(avail[measurementTypeToPrint[i]].find(granularityToPrint[j]) != avail[measurementTypeToPrint[i]].end()){
                        int indexes = avail[measurementTypeToPrint[i]][granularityToPrint[j]].size();
                        for(unsigned long k = 0; k<indexes; k++){
                            if(trial_results[trial].measurements.find(measurementTypeToPrint[i]) != trial_results[trial].measurements.end() && 
                            trial_results[trial].measurements[measurementTypeToPrint[i]].find(granularityToPrint[j]) != trial_results[trial].measurements[measurementTypeToPrint[i]].end()){
                                csv_file << "," << average(trial_results[trial].measurements[measurementTypeToPrint[i]][granularityToPrint[j]][k].measurement);
                            }else{
                                csv_file << ",";
                            }
                        }
                    }
                }
            }
        }

        csv_file << std::endl;
    }
}

std::map<MeasurementType, std::map<HW_Granularity, std::map<int, Unit>>> Results::measurementsAvailUnion(const std::vector<HW_Granularity> granularityToInspect, const std::vector<MeasurementType> measurementTypeToInspect){
    std::map<MeasurementType, std::map<HW_Granularity, std::map<int, Unit>>> avail;
    
    for(unsigned long i = 0; i<trial_results.size(); i++){
        for(unsigned long j = 0; j<measurementTypeToInspect.size(); j++){
            if(trial_results[i].measurements.find(measurementTypeToInspect[j]) != trial_results[i].measurements.end()){
                //Found the measurement
                for(unsigned long k = 0; k<granularityToInspect.size(); k++){
                    //Found granularity
                    if(trial_results[i].measurements[measurementTypeToInspect[j]].find(granularityToInspect[k]) != trial_results[i].measurements[measurementTypeToInspect[j]].end()){
                        for(unsigned long l = 0; l<trial_results[i].measurements[measurementTypeToInspect[j]][granularityToInspect[k]].size(); l++){
                            if(avail[measurementTypeToInspect[j]][granularityToInspect[k]].find(l) == avail[measurementTypeToInspect[j]][granularityToInspect[k]].end()){
                                avail[measurementTypeToInspect[j]][granularityToInspect[k]][l] = trial_results[i].measurements[measurementTypeToInspect[j]][granularityToInspect[k]][l].unit;
                            }/*else{
                                //check unit 
                                Unit origUnit = avail[measurementTypeToInspect[j]][granularityToInspect[k]][l];
                                if(origUnit != trial_results[i].measurements[measurementTypeToInspect[j]][granularityToInspect[k]][l].unit){
                                    throw std::runtime_error("Inconsistent Units Across Trials");
                                }
                            }*/
                        }
                    }
                }
            }
        }
    }

    return avail;
}

void Results::write_durations(std::ofstream &csv_file, std::string col0_name, int col0_val, bool include_header)
{
    //Print Header
    if(include_header)
    {
        csv_file << "\"" << col0_name << "\",\"Steady Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
    }

    size_t trials = trial_results.size();
    for(size_t i = 0; i < trials; i++)
    {
        csv_file << col0_val << "," << std::scientific <<  trial_results[i].duration << "," << trial_results[i].duration_clock << "," << trial_results[i].duration_rdtsc << std::endl;
    }
}

void Results::write_durations(std::ofstream &csv_file, std::string col0_name, int col0_val, std::string col1_name, int col1_val, bool include_header)
{
    //Print Header
    if(include_header)
    {
        csv_file << "\"" << col0_name << "\",\"" << col1_name << "\",\"Steady Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
    }

    size_t trials = trial_results.size();
    for(size_t i = 0; i < trials; i++)
    {
        csv_file << col0_val << "," << col1_val << "," << std::scientific <<  trial_results[i].duration << "," << trial_results[i].duration_clock << "," << trial_results[i].duration_rdtsc << std::endl;
    }
}

std::string BenchmarkSpecificResult::getTrialResultsHeader(){
    return "";
}

std::string BenchmarkSpecificResult::getTrialResults(){
    return "";
}

std::string BenchmarkSpecificResult::getTrialCSVHeader(){
    return "";
}

std::string BenchmarkSpecificResult::getTrialCSVData(){
    return "";
}
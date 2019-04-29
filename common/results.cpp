#include "results.h"
#include "measurement.h"
//#include "intrin_bench_default_defines.h"
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
Statistics::Statistics(double avg, double stdDev, Unit unit, bool valid) : avg(avg), stdDev(stdDev), unit(unit), valid(valid){

}
Statistics::Statistics(double avg, double stdDev, Unit unit) : avg(avg), stdDev(stdDev), unit(unit), valid(true){

}

TrialResult::TrialResult(){}

void TrialResult::print_trial(const std::vector<HW_Granularity> granularityToPrint, const std::vector<MeasurementType> measurementTypeToPrint)
{
    //Print Statistics for each level of HW granularity
    for(unsigned long i = 0; i<measurementTypeToPrint.size(); i++){
        bool foundMeasurement = false;
        for(unsigned long j = 0; j<granularityToPrint.size(); j++){
            bool searching = true;
            for(unsigned long k = 0; searching; k++){
                if(measurements.find(measurementTypeToPrint[i]) != measurements.end() && 
                    measurements[measurementTypeToPrint[i]].find(granularityToPrint[j]) != measurements[measurementTypeToPrint[i]].end()){

                    if(!foundMeasurement){
                        //Found the first measurement at this granularity, print the section header
                        printf("\n");
                        printf("         ##### %s Statistics #####\n", MeasurementHelper::MeasurementType_toString(measurementTypeToPrint[i]).c_str());
                        foundMeasurement = true;
                    }
                    std::string unitStr = MeasurementHelper::exponentAbrev(measurements[measurementTypeToPrint[i]][granularityToPrint[j]][k].unit.exponent)+MeasurementHelper::BaseUnit_abrev(measurements[measurementTypeToPrint[i]][granularityToPrint[j]][k].unit.baseUnit);
                    printf("             %s %s[%2lu] Mean (%s): %f\n", MeasurementHelper::MeasurementType_toString(measurementTypeToPrint[j]).c_str(), MeasurementHelper::HW_Granularity_toString(granularityToPrint[i]).c_str(), k, unitStr.c_str(), average(measurements[measurementTypeToPrint[i]][granularityToPrint[j]][k].measurement));
                }
            }
        }
    }
}

Results::Results()
{
}

Statistics Results::measurementStats(MeasurementType measurmentType, HW_Granularity granularity, int32_t index, bool treatTrialSamplesAsLumped){
    //Itterate through the measurements to find the ones that apply
    std::vector<Measurement> lumpedMeasurements;
    std::vector<Measurement*> filteredMeasurements;
    for(unsigned long i = 0; i<trial_results.size(); i++){
        //Itterate through the measurements for this trial at the specified meaurment type and granularity level
        if(trial_results[i].measurements.find(measurmentType) != trial_results[i].measurements.end() && 
        trial_results[i].measurements[measurmentType].find(granularity) != trial_results[i].measurements[measurmentType].end()){
            for(unsigned long j = 0; j<trial_results[i].measurements[measurmentType][granularity].size(); j++){
                if(trial_results[i].measurements[measurmentType][granularity][j].index == index){
                    if(treatTrialSamplesAsLumped && trial_results[i].measurements[measurmentType][granularity][j].measurement.size()>1){
                        //Average the measurements
                        double sum = 0;
                        for(unsigned long k = 0; k<trial_results[i].measurements[measurmentType][granularity][j].measurement.size(); k++){
                            sum += trial_results[i].measurements[measurmentType][granularity][j].measurement[k];
                        }

                        double avg = sum/trial_results[i].measurements[measurmentType][granularity][j].measurement.size();

                        Measurement lumpedMeasurement;
                        lumpedMeasurement.index = trial_results[i].measurements[measurmentType][granularity][j].index;
                        lumpedMeasurement.unit = trial_results[i].measurements[measurmentType][granularity][j].unit;
                        lumpedMeasurement.measurement.push_back(avg);
                        lumpedMeasurements.push_back(lumpedMeasurement);
                        filteredMeasurements.push_back(&lumpedMeasurements[lumpedMeasurements.size()-1]);
                    }else{
                        filteredMeasurements.push_back(&trial_results[i].measurements[measurmentType][granularity][j]);
                    }
                }
            }
        }
    }

    if(filteredMeasurements.size() == 0){
        return Statistics(0, 0, Unit(BaseUnit::UNITLESS, 0), false);
    }

    double sum = 0;
    unsigned long count = 0;
    Unit avgUnit;
    //Compute the average
    for(unsigned long i = 0; i<filteredMeasurements.size(); i++){
        if(i == 0){
            //Set the unit based off of the first measurement
            avgUnit = filteredMeasurements[i]->unit;
        }

        //Check for sampled
        if(filteredMeasurements[i]->measurement.size()>1 && treatTrialSamplesAsLumped){
            throw std::runtime_error("Specified that sampled trials should be treated as lumped but were not properly processed.");
        }

        //Check for unit change
        if(avgUnit.baseUnit != filteredMeasurements[i]->unit.baseUnit){
            throw std::runtime_error("Incompatible unit found when averaging across trials");
        }
        bool needsScaling = avgUnit.exponent != filteredMeasurements[i]->unit.exponent;
        double scaleFactor = needsScaling ? pow(10.0, filteredMeasurements[i]->unit.exponent - avgUnit.exponent) : 1;

        for(unsigned long j = 0; j<filteredMeasurements[i]->measurement.size(); j++){
            sum += scaleFactor*(filteredMeasurements[i]->measurement[j]);
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
        bool needsScaling = avgUnit.exponent != filteredMeasurements[i]->unit.exponent;
        double scaleFactor = needsScaling ? pow(10.0, filteredMeasurements[i]->unit.exponent - avgUnit.exponent) : 1;

        for(unsigned long j = 0; j<filteredMeasurements[i]->measurement.size(); j++){
            double tmp = scaleFactor*(filteredMeasurements[i]->measurement[j]) - avg;
            tmp = tmp*tmp;
            stdDev += tmp;
        }
        stdDev /= (count - 1);
        stdDev = sqrt(stdDev);
    }

    return Statistics(avg, stdDev, avgUnit);
}

TrialResult* Results::add_trial(TrialResult &trial)
{
    trial_results.push_back(trial);
    return &trial_results[trial_results.size()-1];
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

void Results::print_statistics(int socket, int dies, int core, int stim_len, const std::vector<HW_Granularity> granularityToPrint, const std::vector<MeasurementType> measurementTypeToPrint)
{
    std::vector<int> socket_vec = {socket};
    std::vector<int> dies_vec = {dies};
    std::vector<int> core_vec = {core};

    print_statistics(socket_vec, dies_vec, core_vec, stim_len, granularityToPrint, measurementTypeToPrint);
}

void Results::print_statistics(std::vector<int> sockets, std::vector<int> dies, std::vector<int> cores, int stim_len, const std::vector<HW_Granularity> granularityToPrint, const std::vector<MeasurementType> measurementTypeToPrint)
{
    double avg_duration_dbl = this->avg_duration();
    double stddev_duration_dbl = this->stddev_duration();

    double avg_duration_clock_dbl = this->avg_duration_clock();
    double stddev_duration_clock_dbl = this->stddev_duration_clock();

    // printf("High Res Clock - Sample Mean (ms): %f, Sample Std Dev: %f\n", avg_duration, std_dev_duration);

    printf("         ##### High Resolution Clock - Clock With Smallest Tick on System #####\n");
    printf("             High Resolution Timer - Sample Mean (ms): %f, Sample Std Dev: %f\n", avg_duration_dbl, stddev_duration_dbl);
    printf("             High Resolution Timer Normalized to Sample - Sample Mean (ns): %f, Sample Std Dev: %f\n", avg_duration_dbl*1000000/stim_len, stddev_duration_dbl*1000000/stim_len);
    printf("             High Resolution Timer - Sample Mean (MS/s): %f\n", stim_len*1.0/(1000.0*avg_duration_dbl));

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
                    printf("             %s %s[%2d] Mean (%s): %f, Sample Std Dev: %f\n", MeasurementHelper::MeasurementType_toString(measurementTypeToPrint[j]).c_str(), MeasurementHelper::HW_Granularity_toString(granularityToPrint[i]).c_str(), ind_to_search[k], unitStr.c_str(), stats.avg, stats.stdDev);
                }
            }
        }
    }
}

void Results::print_statistics(int stim_len, const std::vector<HW_Granularity> granularityToPrint, const std::vector<MeasurementType> measurementTypeToPrint)
{
    double avg_duration_dbl = this->avg_duration();
    double stddev_duration_dbl = this->stddev_duration();

    double avg_duration_clock_dbl = this->avg_duration_clock();
    double stddev_duration_clock_dbl = this->stddev_duration_clock();

    // printf("High Res Clock - Sample Mean (ms): %f, Sample Std Dev: %f\n", avg_duration, std_dev_duration);

    printf("         ##### High Resolution Clock - Clock With Smallest Tick on System #####\n");
    printf("             High Resolution Timer - Sample Mean (ms): %f, Sample Std Dev: %f\n", avg_duration_dbl, stddev_duration_dbl);
    printf("             High Resolution Timer Normalized to Sample - Sample Mean (ns): %f, Sample Std Dev: %f\n", avg_duration_dbl*1000000/stim_len, stddev_duration_dbl*1000000/stim_len);
    printf("             High Resolution Timer - Sample Mean (MS/s): %f\n", stim_len*1.0/(1000.0*avg_duration_dbl));

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
                    printf("             %s %s[%2lu] Mean (%s): %f, Sample Std Dev: %f\n", MeasurementHelper::MeasurementType_toString(measurementTypeToPrint[j]).c_str(), MeasurementHelper::HW_Granularity_toString(granularityToPrint[i]).c_str(), k, unitStr.c_str(), stats.avg, stats.stdDev);
                }
            }
        }
    }
}

void Results::print_statistics(std::set<int> sockets, std::set<int> dies, std::set<int> cores, int stim_len, const std::vector<HW_Granularity> granularityToPrint, const std::vector<MeasurementType> measurementTypeToPrint)
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

    print_statistics(socket_vec, dies_vec, core_vec, stim_len, granularityToPrint, measurementTypeToPrint);
}

void Results::write_csv(std::ofstream &csv_file, int socket, int core, const std::vector<HW_Granularity> granularityToPrint, const std::vector<MeasurementType> measurementTypeToPrint)
{
    write_csv(csv_file, socket, core, "", 0, granularityToPrint, measurementTypeToPrint);
}

void Results::write_csv(std::ofstream &csv_file, int socket, int core, std::string col0_name, int col0_val, const std::vector<HW_Granularity> granularityToPrint, const std::vector<MeasurementType> measurementTypeToPrint)
{
    std::map<MeasurementType, std::map<HW_Granularity, std::map<int, Unit>>> avail = measurementsAvailUnion(granularityToPrint, measurementTypeToPrint); 

    //Print Header
    csv_file << std::string(col0_name.empty() ? "" : "\"" + col0_name + "\",") << "\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"";

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

    size_t trials = trial_results.size();
    for(size_t trial = 0; trial < trials; trial++)
    {
        if(!col0_name.empty()){
            csv_file << col0_val;
        }
        csv_file << std::scientific << trial_results[trial].duration << "," << trial_results[trial].duration_clock << "," << trial_results[trial].duration_rdtsc;
        
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
        csv_file << "\"" << col0_name << "\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
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
        csv_file << "\"" << col0_name << "\",\"" << col1_name << "\",\"High Resolution Clock - Walltime (ms)\",\"Clock - Cycles/Cycle Time (ms)\",\"Clock - rdtsc\"" << std::endl;
    }

    size_t trials = trial_results.size();
    for(size_t i = 0; i < trials; i++)
    {
        csv_file << col0_val << "," << col1_val << "," << std::scientific <<  trial_results[i].duration << "," << trial_results[i].duration_clock << "," << trial_results[i].duration_rdtsc << std::endl;
    }
}
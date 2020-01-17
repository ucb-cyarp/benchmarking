#include "open_loop_fullness_tracker.h"

std::string FifolessBufferFullnessTrackerEndCondition::getTrialCSVHeader(){
    std::string granularityStr = getGranularityStr();
    return "\"Expected Block ID [" + granularityStr + "]\",\"Start Block ID [" + granularityStr + "]\",\"End Block ID [" + granularityStr + "]\",\"Errored [" + granularityStr + "]\",\"Error Source [" + granularityStr + "]\",\"Transaction [" + granularityStr + "]\",\"Fullness Start [" + granularityStr + "]\",\"Fullness End [" + granularityStr + "]\",\"Interrupts Start [" + granularityStr + "]\",\"Interrupts End [" + granularityStr + "]\"";
}

std::string FifolessBufferFullnessTrackerEndCondition::getTrialCSVData(){
    std::string str = std::to_string(expectedBlockID) + "," +
                      std::to_string(startBlockID) + "," +
                      std::to_string(endBlockID) + "," +
                      (errored ? "\"Yes\"" : "\"No\"") + "," +
                      (wasErrorSrc ? "\"Yes\"" : "\"No\"") + "," +
                      std::to_string(transaction) + ",";

    //Print the start array
    str += "\"";
    for(int i = 0; i<startTracker.size(); i++){
        if(i > 0){
            str += ",";
        }

        str += std::to_string(startTracker[i]);
    }
    str += "\"";

    //Print the end array
    str += ",\"";
    for(int i = 0; i<endTracker.size(); i++){
        if(i > 0){
            str += ",";
        }

        str += std::to_string(endTracker[i]);
    }
    str += "\"";

    //Print the start interrupt array
    str += ",\"";
    for(int i = 0; i<startInterruptTracker.size(); i++){
        if(i > 0){
            str += ",";
        }

        str += std::to_string(startInterruptTracker[i]);
    }
    str += "\"";

    //Print the end interrupt array
    str += ",\"";
    for(int i = 0; i<endInterruptTracker.size(); i++){
        if(i > 0){
            str += ",";
        }

        str += std::to_string(endInterruptTracker[i]);
    }
    str += "\"";

    return str;
}

FifolessBufferFullnessTrackerEndCondition::FifolessBufferFullnessTrackerEndCondition() {

}

void writeRawHeaderOpenLoopFullnessTracker(std::vector<std::shared_ptr<BenchmarkSpecificResult>> implSpecificResults, std::ofstream* raw_file){
    #if WRITE_CSV == 1
        *raw_file << "\"Array Length (Blocks)\","
                  << "\"Block Size (int32_t Elements)\","
                  << "\"Balancing NOPs\","
                  << "\"Initial NOPs\","
                  << "\"Check Period\","
                  << "\"Steady Clock - Walltime (ms)\","
                  << "\"Clock - Cycles/Cycle Time (ms)\","
                  << "\"Clock - rdtsc\"";

        for(int i = 0; i<implSpecificResults.size(); i++){
            if(implSpecificResults[i] != nullptr){
                std::string trialCSVHeader = implSpecificResults[i]->getTrialCSVHeader();
                *raw_file << "," << trialCSVHeader;
            }
        }

        *raw_file << std::endl;

        raw_file->flush();
    #endif
}
#include "open_loop_fullness_tracker.h"
#include <cstdio>

std::string FifolessBufferFullnessTrackerEndCondition::getTrialCSVHeader(){
    std::string granularityStr = getGranularityStr();
    return  "\"Expected Block ID [" + granularityStr + "]\","
            "\"Start Block ID [" + granularityStr + "]\","
            "\"End Block ID [" + granularityStr + "]\","
            "\"Errored [" + granularityStr + "]\","
            "\"Error Source [" + granularityStr + "]\","
            "\"Transaction [" + granularityStr + "]\","
            "\"Fullness Start [" + granularityStr + "]\","
            "\"Fullness End [" + granularityStr + "]\","
            "\"Interrupts Std Start [" + granularityStr + "]\","
            "\"Interrupts LOC Start [" + granularityStr + "]\","
            "\"Interrupts Other Arch Start [" + granularityStr + "]\","
            "\"Interrupts Std End [" + granularityStr + "]\","
            "\"Interrupts LOC End [" + granularityStr + "]\","
            "\"Interrupts Other Arch End [" + granularityStr + "]\","
            "\"Timing Start [" + granularityStr + "]\","
            "\"Timing End [" + granularityStr + "]\"";
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
    for(int i = 0; i<startStdInterruptTracker.size(); i++){
        if(i > 0){
            str += ",";
        }

        str += std::to_string(startStdInterruptTracker[i]);
    }
    str += "\"";
    str += ",\"";
    for(int i = 0; i<startLocInterruptTracker.size(); i++){
        if(i > 0){
            str += ",";
        }

        str += std::to_string(startLocInterruptTracker[i]);
    }
    str += "\"";
    str += ",\"";
    for(int i = 0; i<startOtherArchInterruptTracker.size(); i++){
        if(i > 0){
            str += ",";
        }

        str += std::to_string(startOtherArchInterruptTracker[i]);
    }
    str += "\"";


    //Print the end interrupt array
    str += ",\"";
    for(int i = 0; i<endStdInterruptTracker.size(); i++){
        if(i > 0){
            str += ",";
        }

        str += std::to_string(endStdInterruptTracker[i]);
    }
    str += "\"";
    str += ",\"";
    for(int i = 0; i<endLocInterruptTracker.size(); i++){
        if(i > 0){
            str += ",";
        }

        str += std::to_string(endLocInterruptTracker[i]);
    }
    str += "\"";
    str += ",\"";
    for(int i = 0; i<endOtherArchInterruptTracker.size(); i++){
        if(i > 0){
            str += ",";
        }

        str += std::to_string(endOtherArchInterruptTracker[i]);
    }
    str += "\"";

    //Print the start timing array
    str += ",\"";
    for(int i = 0; i<startTimingTracker.size(); i++){
        if(i > 0){
            str += ",";
        }

        char buf[32];
        snprintf(buf, 32, "%12.6e", startTimingTracker[i]);
        str += buf;
    }
    str += "\"";

    //Print the end timing array
    str += ",\"";
    for(int i = 0; i<endTimingTracker.size(); i++){
        if(i > 0){
            str += ",";
        }

        char buf[32];
        snprintf(buf, 32, "%12.6e", endTimingTracker[i]);
        str += buf;
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
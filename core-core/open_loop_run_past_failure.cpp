#include "open_loop_run_past_failure.h"

std::string FifolessBufferPastFailureEndCondition::getTrialResultsHeader(){
    return "Errored | Error Source | Final Transaction";
}

std::string FifolessBufferPastFailureEndCondition::getTrialResults(){
    char tmp[120];
    std::string erroredStr = errored ? "Yes" : "No";
    std::string wasErrorSrcStr = wasErrorSrc ? "Yes" : "No";
    snprintf(tmp, 120, "%9s|%13s|%18ld", erroredStr.c_str(), wasErrorSrcStr.c_str(), final_transaction);
    std::string str = tmp;
    return str;
}

std::string FifolessBufferPastFailureEndCondition::getTrialCSVHeader(){
    std::string granularityStr = getGranularityStr();
    return  "\"Errored [" + granularityStr + "]\","
            "\"Error Source [" + granularityStr + "]\"," + 
            "\"Final Transaction [" + granularityStr + "]\"," + 
            "\"Under/Overflow Iterations [" + granularityStr + "]\"," + 
            "\"Under/Overflow Amounts [" + granularityStr + "]\"," + 
            "\"Under/Overflow Partial [" + granularityStr + "]\"";
}

std::string FifolessBufferPastFailureEndCondition::getTrialCSVData(){
    std::string str = (errored ? "\"Yes\"" : "\"No\"") + std::string(",") +
                      (wasErrorSrc ? "\"Yes\"" : "\"No\"") + "," +
                      std::to_string(final_transaction) + ",";

    str += "\"";
    for(int i = 0; i<underOverflowIterations.size(); i++){
        if(i>0){
            str += ",";
        }
        str += underOverflowIterations[i];
    }
    str += "\",\"";

    for(int i = 0; i<underOverflowAmounts.size(); i++){
        if(i>0){
            str += ",";
        }
        str += underOverflowAmounts[i];
    }

    str += "\",\"";
    for(int i = 0; i<underOverflowPartial.size(); i++){
        if(i>0){
            str += ",";
        }
        str += underOverflowPartial[i] ? "true" : "false";
    }
    str += "\"";

    return str;
}

FifolessBufferPastFailureEndCondition::FifolessBufferPastFailureEndCondition() : wasErrorSrc(false), errored(false), final_transaction(-1) {

}
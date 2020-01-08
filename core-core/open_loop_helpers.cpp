#include "open_loop_helpers.h"

std::string FifolessBufferEndCondition::getTrialResultsHeader(){
    return "Expected Block ID | Start Block ID | End Block ID | Errored | Error Source";
}

std::string FifolessBufferEndCondition::getTrialResults(){
    char tmp[120];
    std::string erroredStr = errored ? "Yes" : "No";
    std::string wasErrorSrcStr = wasErrorSrc ? "Yes" : "No";
    snprintf(tmp, 120, "%18ld|%19ld|%14ld|%9s|%13s", expectedBlockID, startBlockID, endBlockID, erroredStr.c_str(), wasErrorSrcStr.c_str());
    std::string str = tmp;
    return str;
}

std::string FifolessBufferEndCondition::getTrialCSVHeader(){
    std::string granularityStr = getGranularityStr();
    return "\"Expected Block ID [" + granularityStr + "]\",\"Start Block ID [" + granularityStr + "]\",\"End Block ID [" + granularityStr + "]\",\"Errored [" + granularityStr + "]\",\"Error Source [" + granularityStr + "]\",\"Transaction [" + granularityStr + "]\"";
}

std::string FifolessBufferEndCondition::getTrialCSVData(){
    std::string str = std::to_string(expectedBlockID) + "," +
                      std::to_string(startBlockID) + "," +
                      std::to_string(endBlockID) + "," +
                      (errored ? "\"Yes\"" : "\"No\"") + "," +
                      (wasErrorSrc ? "\"Yes\"" : "\"No\"") + "," +
                      std::to_string(transaction);
    return str;
}

FifolessBufferEndCondition::FifolessBufferEndCondition() : expectedBlockID(-1), startBlockID(-1), endBlockID(-1), wasErrorSrc(false), errored(false) {

}
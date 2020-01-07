#include "open_loop_helpers.h"

std::string OpenLoopBufferEndCondition::getTrialResultsHeader(){
    return "Expected Block ID | Start Block ID | End Block ID | Error Source";
}

std::string OpenLoopBufferEndCondition::getTrialResults(){
    char tmp[120];
    std::string wasErrorSrcStr = wasErrorSrc ? "Yes" : "No";
    snprintf(tmp, 120, "%18d|%19d|%14d|%13s", expectedBlockID, startBlockID, endBlockID, wasErrorSrcStr.c_str());
    std::string str = tmp;
    return str;
}

std::string OpenLoopBufferEndCondition::getTrialCSVHeader(){
    std::string granularityStr = getGranularityStr();
    return "\"Expected Block ID [" + granularityStr + "]\",\"Start Block ID [" + granularityStr + "]\",\"End Block ID [" + granularityStr + "]\",\"Error Source [" + granularityStr + "]\"";
}

std::string OpenLoopBufferEndCondition::getTrialCSVData(){
    std::string str = std::to_string(expectedBlockID) + "," +
                      std::to_string(startBlockID) + "," +
                      std::to_string(endBlockID) + "," +
                      (wasErrorSrc ? "\"Yes\"" : "\"No\"");
    return str;
}
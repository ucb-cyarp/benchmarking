#include "closed_loop_helpers.h"

// int64_t controlChecks;
// int64_t speed_up_count;
// int64_t slow_down_count;
// int64_t serverNops;
// int64_t clientNops;
// bool errored;
// int64_t transaction;

std::string ClosedLoopServerEndCondition::getTrialResultsHeader(){
    return "Server Control Checks | Speedups | Slowdowns | Server NOPs | Client NOPs | Errored | Transaction";
}

std::string ClosedLoopServerEndCondition::getTrialResults(){
    char tmp[120];
    std::string erroredStr = errored ? "Yes" : "No";
    snprintf(tmp, 120, "%22ld|%12ld|%12ld|%13ld|%13ld|%9s|%12ld", controlChecks, speed_up_count, slow_down_count, serverNops, clientNops, erroredStr.c_str(), transaction);
    std::string str = tmp;
    return str;
}

std::string ClosedLoopServerEndCondition::getTrialCSVHeader(){
    std::string granularityStr = getGranularityStr();
    return "\"Server Control Checks [" + granularityStr + "]\","
           "\"Speedups [" + granularityStr + "]\","
           "\"Slowdowns [" + granularityStr + "]\","
           "\"Server NOPs [" + granularityStr + "]\","
           "\"Client NOPs [" + granularityStr + "]\","
           "\"Errored [" + granularityStr + "]\","
           "\"Transaction [" + granularityStr + "]\"";
}

std::string ClosedLoopServerEndCondition::getTrialCSVData(){
    std::string str = std::to_string(controlChecks) + "," +
                      std::to_string(speed_up_count) + "," +
                      std::to_string(slow_down_count) + "," +
                      std::to_string(serverNops) + "," +
                      std::to_string(clientNops) + "," +
                      (errored ? "\"Yes\"" : "\"No\"") + "," +
                      std::to_string(transaction);
    return str;
}

ClosedLoopServerEndCondition::ClosedLoopServerEndCondition() : controlChecks(-1), speed_up_count(-1), slow_down_count(-1), serverNops(-1), clientNops(-1), errored(false), transaction(-1) {

}
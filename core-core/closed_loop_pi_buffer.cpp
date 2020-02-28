#include "closed_loop_pi_buffer.h"

// int64_t base_speed_up_count;
// int64_t base_slow_down_count;
// float base_nops;

std::string ClosedLoopPIServerEndCondition::getTrialResultsHeader(){
    //Get the base header from the super function
    std::string baseString = ClosedLoopServerEndCondition::getTrialResultsHeader();
    return baseString + "| Base Speedups | Base Slowdowns |  Base";
}

std::string ClosedLoopPIServerEndCondition::getTrialResults(){
    //Get the base result from the super function
    std::string str = ClosedLoopServerEndCondition::getTrialResults();
    char tmp[120];
    snprintf(tmp, 120, "|%15ld|%16ld|%6.2f", base_speed_up_count, base_slow_down_count, base_nops);
    str += tmp;
    return str;
}

std::string ClosedLoopPIServerEndCondition::getTrialCSVHeader(){
    //Get the base result from the super function
    std::string granularityStr = getGranularityStr();
    std::string header = ClosedLoopServerEndCondition::getTrialCSVHeader();
    header += (",\"Base Speedups [" + granularityStr + "]\","
             + "\"Base Slowdowns [" + granularityStr + "]\","
             + "\"Base [" + granularityStr + "]\"");
    return header;
}

std::string ClosedLoopPIServerEndCondition::getTrialCSVData(){
    //Get the base result from the super function
    std::string str = ClosedLoopServerEndCondition::getTrialCSVData();
    
    str += ("," + std::to_string(base_speed_up_count) + "," +
            std::to_string(base_slow_down_count) + "," +
            std::to_string(base_nops));
    return str;
}

ClosedLoopPIServerEndCondition::ClosedLoopPIServerEndCondition() : ClosedLoopServerEndCondition(), base_speed_up_count(-1), base_slow_down_count(-1), base_nops(-1) {

}
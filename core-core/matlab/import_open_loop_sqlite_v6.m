function [ArrayLengthBlocks, ...
          BlockSizeint32_tElements, ...
          BalancingNOPs, ...
          InitialNOPs, ...
          CheckPeriod, ...
          SteadyClockWalltimems, ...
          ClockCyclesCycleTimems, ...
          Clockrdtsc, ...
          ExpectedBlockIDCore_Server, ...
          StartBlockIDCore_Server, ...
          EndBlockIDCore_Server, ...
          ErroredCore_Server, ...
          ErrorSourceCore_Server, ...
          TransactionCore_Server, ...
          InterruptsStdStartCore_Server, ...
          InterruptsLOCStartCore_Server, ...
          InterruptsOtherArchStartCore_Server, ...
          InterruptsStdEndCore_Server, ...
          InterruptsLOCEndCore_Server, ...
          InterruptsOtherArchEndCore_Server, ...
          InterruptsSoftirqTimerStartCore_Server, ...
          InterruptsSoftirqTimerEndCore_Server, ...
          InterruptsSoftirqOtherStartCore_Server, ...
          InterruptsSoftirqOtherEndCore_Server, ...
          TimingStartCore_Server, ...
          TimingEndCore_Server, ...
          ExpectedBlockIDCore_Client, ...
          StartBlockIDCore_Client, ...
          EndBlockIDCore_Client, ...
          ErroredCore_Client, ...
          ErrorSourceCore_Client, ...
          TransactionCore_Client, ...
          FullnessStartCore_Client, ...
          FullnessEndCore_Client, ...
          InterruptsStdStartCore_Client, ...
          InterruptsLOCStartCore_Client, ...
          InterruptsOtherArchStartCore_Client, ...
          InterruptsStdEndCore_Client, ...
          InterruptsLOCEndCore_Client, ...
          InterruptsOtherArchEndCore_Client, ...
          InterruptsSoftirqTimerStartCore_Client, ...
          InterruptsSoftirqTimerEndCore_Client, ...
          InterruptsSoftirqOtherStartCore_Client, ...
          InterruptsSoftirqOtherEndCore_Client, ...
          TimingStartCore_Client, ...
          TimingEndCore_Client] = import_open_loop_sqlite_v6(filename)
%import_open_loop_sqlite_v3 Imports a report file which had been converted
%to a sqlite file

conn = sqlite(filename);
sqlquery = 'SELECT * FROM results_tbl';
results = fetch(conn,sqlquery);

ArrayLengthBlocks = cell2mat(results(:,1));
BlockSizeint32_tElements = cell2mat(results(:,2));
BalancingNOPs = cell2mat(results(:,3));
InitialNOPs = cell2mat(results(:,4));
CheckPeriod = cell2mat(results(:,5));
SteadyClockWalltimems = cell2mat(results(:,6));
ClockCyclesCycleTimems = cell2mat(results(:,7));
Clockrdtsc = cell2mat(results(:,8));
ExpectedBlockIDCore_Server = cell2mat(results(:,9));
StartBlockIDCore_Server = cell2mat(results(:,10));
EndBlockIDCore_Server = cell2mat(results(:,11));
ErroredCore_Server = string(results(:,12));
ErrorSourceCore_Server = string(results(:,13));
TransactionCore_Server = cell2mat(results(:,14));
InterruptsStdStartCore_Server = string(results(:,17));
InterruptsLOCStartCore_Server = string(results(:,18));
InterruptsOtherArchStartCore_Server = string(results(:,19));
InterruptsStdEndCore_Server = string(results(:,20));
InterruptsLOCEndCore_Server = string(results(:,21));
InterruptsOtherArchEndCore_Server = string(results(:,22));
InterruptsSoftirqTimerStartCore_Server = string(results(:,23));
InterruptsSoftirqTimerEndCore_Server = string(results(:,24));
InterruptsSoftirqOtherStartCore_Server = string(results(:,25));
InterruptsSoftirqOtherEndCore_Server = string(results(:,26));
TimingStartCore_Server = string(results(:,27));
TimingEndCore_Server = string(results(:,28));
ExpectedBlockIDCore_Client = cell2mat(results(:,29));
StartBlockIDCore_Client = cell2mat(results(:,30));
EndBlockIDCore_Client = cell2mat(results(:,31));
ErroredCore_Client = string(results(:,32));
ErrorSourceCore_Client = string(results(:,33));
TransactionCore_Client = cell2mat(results(:,34));
FullnessStartCore_Client = string(results(:,35));
FullnessEndCore_Client = string(results(:,36));
InterruptsStdStartCore_Client = string(results(:,37));
InterruptsLOCStartCore_Client = string(results(:,38));
InterruptsOtherArchStartCore_Client = string(results(:,39));
InterruptsStdEndCore_Client = string(results(:,40));
InterruptsLOCEndCore_Client = string(results(:,41));
InterruptsOtherArchEndCore_Client = string(results(:,42));
InterruptsSoftirqTimerStartCore_Client = string(results(:,43));
InterruptsSoftirqTimerEndCore_Client = string(results(:,44));
InterruptsSoftirqOtherStartCore_Client = string(results(:,45));
InterruptsSoftirqOtherEndCore_Client = string(results(:,46));
TimingStartCore_Client = string(results(:,47));
TimingEndCore_Client = string(results(:,48));
end


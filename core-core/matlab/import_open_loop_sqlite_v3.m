function [ArrayLengthBlocks, BlockSizeint32_tElements, BalancingNOPs, InitialNOPs, CheckPeriod, SteadyClockWalltimems, ClockCyclesCycleTimems, Clockrdtsc, ExpectedBlockIDCore_Server, StartBlockIDCore_Server, EndBlockIDCore_Server, ErroredCore_Server, ErrorSourceCore_Server, TransactionCore_Server, InterruptsStartCore_Server, InterruptsEndCore_Server, TimingStartCore_Server, TimingEndCore_Server, ExpectedBlockIDCore_Client, StartBlockIDCore_Client, EndBlockIDCore_Client, ErroredCore_Client, ErrorSourceCore_Client, TransactionCore_Client, FullnessStartCore_Client, FullnessEndCore_Client, InterruptsStartCore_Client, InterruptsEndCore_Client, TimingStartCore_Client, TimingEndCore_Client] = import_open_loop_sqlite_v3(filename)
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
InterruptsStartCore_Server = string(results(:,17));
InterruptsEndCore_Server = string(results(:,18));
TimingStartCore_Server = string(results(:,19));
TimingEndCore_Server = string(results(:,20));
ExpectedBlockIDCore_Client = cell2mat(results(:,21));
StartBlockIDCore_Client = cell2mat(results(:,22));
EndBlockIDCore_Client = cell2mat(results(:,23));
ErroredCore_Client = string(results(:,24));
ErrorSourceCore_Client = string(results(:,25));
TransactionCore_Client = cell2mat(results(:,26));
FullnessStartCore_Client = string(results(:,27));
FullnessEndCore_Client = string(results(:,28));
InterruptsStartCore_Client = string(results(:,29));
InterruptsEndCore_Client = string(results(:,30));
TimingStartCore_Client = string(results(:,31));
TimingEndCore_Client = string(results(:,32));
end


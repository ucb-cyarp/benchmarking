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
          TimingEndCore_Client] = import_open_loop_sqlite_v6(filename, importInterruptTraces)
%import_open_loop_sqlite_v3 Imports a report file which had been converted
%to a sqlite file

if nargin < 2
    importInterruptTraces = false;
end

RowLim = 1000;
RowOffset = 0;

disp(['Fetching Records ' num2str(RowLim) ' at a time.  Do NOT modify database durring this operation!']);

ArrayLengthBlocks = [];
BlockSizeint32_tElements = [];
BalancingNOPs = [];
InitialNOPs = [];
CheckPeriod = [];
SteadyClockWalltimems = [];
ClockCyclesCycleTimems = [];
Clockrdtsc = [];
ExpectedBlockIDCore_Server = [];
StartBlockIDCore_Server = [];
EndBlockIDCore_Server = [];
ErroredCore_Server = [];
ErrorSourceCore_Server = [];
TransactionCore_Server = [];
InterruptsStdStartCore_Server = [];
InterruptsLOCStartCore_Server = [];
InterruptsOtherArchStartCore_Server = [];
InterruptsStdEndCore_Server = [];
InterruptsLOCEndCore_Server = [];
InterruptsOtherArchEndCore_Server = [];
InterruptsSoftirqTimerStartCore_Server = [];
InterruptsSoftirqTimerEndCore_Server = [];
InterruptsSoftirqOtherStartCore_Server = [];
InterruptsSoftirqOtherEndCore_Server = [];
TimingStartCore_Server = [];
TimingEndCore_Server = [];
ExpectedBlockIDCore_Client = [];
StartBlockIDCore_Client = [];
EndBlockIDCore_Client = [];
ErroredCore_Client = [];
ErrorSourceCore_Client = [];
TransactionCore_Client = [];
FullnessStartCore_Client = [];
FullnessEndCore_Client = [];
InterruptsStdStartCore_Client = [];
InterruptsLOCStartCore_Client = [];
InterruptsOtherArchStartCore_Client = [];
InterruptsStdEndCore_Client = [];
InterruptsLOCEndCore_Client = [];
InterruptsOtherArchEndCore_Client = [];
InterruptsSoftirqTimerStartCore_Client = [];
InterruptsSoftirqTimerEndCore_Client = [];
InterruptsSoftirqOtherStartCore_Client = [];
InterruptsSoftirqOtherEndCore_Client = [];
TimingStartCore_Client = [];
TimingEndCore_Client = [];

done = false;

conn = sqlite(filename);

sqlqueryCount = 'SELECT COUNT(*) FROM results_tbl';
resultsCount = fetch(conn,sqlqueryCount);
rowCount = resultsCount{1};

while ~done
    sqlquery = ['SELECT * FROM results_tbl ORDER BY rowid LIMIT ' num2str(RowLim) ' OFFSET ' num2str(RowOffset)];
    results = fetch(conn,sqlquery);
    
    RowOffset = RowOffset + RowLim;
    done = isempty(results);
    
    if ~done
        disp(['Offset ' num2str(RowOffset) ' of ' num2str(rowCount)]);

        ArrayLengthBlocks = [ArrayLengthBlocks; int32(cell2mat(results(:,1)))];
        BlockSizeint32_tElements = [BlockSizeint32_tElements; int32(cell2mat(results(:,2)))];
        BalancingNOPs = [BalancingNOPs; int32(cell2mat(results(:,3)))];
        InitialNOPs = [InitialNOPs; int32(cell2mat(results(:,4)))];
        CheckPeriod = [CheckPeriod; cell2mat(results(:,5))];
        SteadyClockWalltimems = [SteadyClockWalltimems; cell2mat(results(:,6))];
        ClockCyclesCycleTimems = [ClockCyclesCycleTimems; cell2mat(results(:,7))];
        Clockrdtsc = [Clockrdtsc; cell2mat(results(:,8))];
        ExpectedBlockIDCore_Server = [ExpectedBlockIDCore_Server; cell2mat(results(:,9))];
        StartBlockIDCore_Server = [StartBlockIDCore_Server; cell2mat(results(:,10))];
        EndBlockIDCore_Server = [EndBlockIDCore_Server; cell2mat(results(:,11))];
        ErroredCore_Server = [ErroredCore_Server; arrayfun(@(c) c == 'Yes' || c == 'yes' || c == 'True' || c == 'true' || c == '1', string(results(:,12)))];
        ErrorSourceCore_Server = [ErrorSourceCore_Server; arrayfun(@(c) c == 'Yes' || c == 'yes' || c == 'True' || c == 'true' || c == '1', string(results(:,13)))];
        TransactionCore_Server = [TransactionCore_Server; cell2mat(results(:,14))];
        if importInterruptTraces
            InterruptsStdStartCore_Server = [InterruptsStdStartCore_Server; string(results(:,17))];
            InterruptsLOCStartCore_Server = [InterruptsLOCStartCore_Server; string(results(:,18))];
            InterruptsOtherArchStartCore_Server = [InterruptsOtherArchStartCore_Server; string(results(:,19))];
            InterruptsStdEndCore_Server = [InterruptsStdEndCore_Server; string(results(:,20))];
            InterruptsLOCEndCore_Server = [InterruptsLOCEndCore_Server; string(results(:,21))];
            InterruptsOtherArchEndCore_Server = [InterruptsOtherArchEndCore_Server; string(results(:,22))];
            InterruptsSoftirqTimerStartCore_Server = [InterruptsSoftirqTimerStartCore_Server; string(results(:,23))];
            InterruptsSoftirqTimerEndCore_Server = [InterruptsSoftirqTimerEndCore_Server; string(results(:,24))];
            InterruptsSoftirqOtherStartCore_Server = [InterruptsSoftirqOtherStartCore_Server; string(results(:,25))];
            InterruptsSoftirqOtherEndCore_Server = [InterruptsSoftirqOtherEndCore_Server; string(results(:,26))];
        end
        TimingStartCore_Server = [TimingStartCore_Server; string(results(:,27))];
        TimingEndCore_Server = [TimingEndCore_Server; string(results(:,28))];
        ExpectedBlockIDCore_Client = [ExpectedBlockIDCore_Client; cell2mat(results(:,29))];
        StartBlockIDCore_Client = [StartBlockIDCore_Client; cell2mat(results(:,30))];
        EndBlockIDCore_Client = [EndBlockIDCore_Client; cell2mat(results(:,31))];
        ErroredCore_Client = [ErroredCore_Client; arrayfun(@(c) c == 'Yes' || c == 'yes' || c == 'True' || c == 'true' || c == '1', string(results(:,32)))];
        ErrorSourceCore_Client = [ErrorSourceCore_Client; arrayfun(@(c) c == 'Yes' || c == 'yes' || c == 'True' || c == 'true' || c == '1', string(results(:,33)))];
        TransactionCore_Client = [TransactionCore_Client; cell2mat(results(:,34))];
        FullnessStartCore_Client = [FullnessStartCore_Client; string(results(:,35))];
        FullnessEndCore_Client = [FullnessEndCore_Client; string(results(:,36))];
        if importInterruptTraces
            InterruptsStdStartCore_Client = [InterruptsStdStartCore_Client; string(results(:,37))];
            InterruptsLOCStartCore_Client = [InterruptsLOCStartCore_Client; string(results(:,38))];
            InterruptsOtherArchStartCore_Client = [InterruptsOtherArchStartCore_Client; string(results(:,39))];
            InterruptsStdEndCore_Client = [InterruptsStdEndCore_Client; string(results(:,40))];
            InterruptsLOCEndCore_Client = [InterruptsLOCEndCore_Client; string(results(:,41))];
            InterruptsOtherArchEndCore_Client = [InterruptsOtherArchEndCore_Client; string(results(:,42))];
            InterruptsSoftirqTimerStartCore_Client = [InterruptsSoftirqTimerStartCore_Client; string(results(:,43))];
            InterruptsSoftirqTimerEndCore_Client = [InterruptsSoftirqTimerEndCore_Client; string(results(:,44))];
            InterruptsSoftirqOtherStartCore_Client = [InterruptsSoftirqOtherStartCore_Client; string(results(:,45))];
            InterruptsSoftirqOtherEndCore_Client = [InterruptsSoftirqOtherEndCore_Client; string(results(:,46))];
        end
        TimingStartCore_Client = [TimingStartCore_Client; string(results(:,47))];
        TimingEndCore_Client = [TimingEndCore_Client; string(results(:,48))];
    end
end

close(conn);
end


function [ArrayLengthBlocks, BlockSizeint32_tElements, BalancingNOPs, InitialNOPs, CheckPeriod, SteadyClockWalltimems, ClockCyclesCycleTimems, Clockrdtsc, ExpectedBlockIDCore_Server, StartBlockIDCore_Server, EndBlockIDCore_Server, ErroredCore_Server, ErrorSourceCore_Server, TransactionCore_Server, InterruptsStartCore_Server, InterruptsEndCore_Server, TimingStartCore_Server, TimingEndCore_Server, ExpectedBlockIDCore_Client, StartBlockIDCore_Client, EndBlockIDCore_Client, ErroredCore_Client, ErrorSourceCore_Client, TransactionCore_Client, FullnessStartCore_Client, FullnessEndCore_Client, InterruptsStartCore_Client, InterruptsEndCore_Client, TimingStartCore_Client, TimingEndCore_Client] = import_open_loop_fullness_v3(filename, dataLines)
%IMPORTFILE Import data from a text file
%  [ARRAYLENGTHBLOCKS, BLOCKSIZEINT32_TELEMENTS, BALANCINGNOPS,
%  INITIALNOPS, CHECKPERIOD, STEADYCLOCKWALLTIMEMS,
%  CLOCKCYCLESCYCLETIMEMS, CLOCKRDTSC, EXPECTEDBLOCKIDCORE_Server,
%  STARTBLOCKIDCORE_Server, ENDBLOCKIDCORE_Server, ERROREDCORE_Server, ERRORSOURCECORE_Server,
%  TRANSACTIONCORE_Server, INTERRUPTSSTARTCORE_Server, INTERRUPTSENDCORE_Server,
%  TIMINGSTARTCORE_Server, TIMINGENDCORE_Server, EXPECTEDBLOCKIDCORE_Client,
%  STARTBLOCKIDCORE_Client, ENDBLOCKIDCORE_Client, ERROREDCORE_Client, ERRORSOURCECORE_Client,
%  TRANSACTIONCORE_Client, FULLNESSSTARTCORE_Client, FULLNESSENDCORE_Client,
%  INTERRUPTSSTARTCORE_Client, INTERRUPTSENDCORE_Client, TIMINGSTARTCORE_Client,
%  TIMINGENDCORE_Client] = IMPORTFILE(FILENAME) reads data from text file
%  FILENAME for the default selection.  Returns the data as column
%  vectors.
%
%  [ARRAYLENGTHBLOCKS, BLOCKSIZEINT32_TELEMENTS, BALANCINGNOPS,
%  INITIALNOPS, CHECKPERIOD, STEADYCLOCKWALLTIMEMS,
%  CLOCKCYCLESCYCLETIMEMS, CLOCKRDTSC, EXPECTEDBLOCKIDCORE_Server,
%  STARTBLOCKIDCORE_Server, ENDBLOCKIDCORE_Server, ERROREDCORE_Server, ERRORSOURCECORE_Server,
%  TRANSACTIONCORE_Server, INTERRUPTSSTARTCORE_Server, INTERRUPTSENDCORE_Server,
%  TIMINGSTARTCORE_Server, TIMINGENDCORE_Server, EXPECTEDBLOCKIDCORE_Client,
%  STARTBLOCKIDCORE_Client, ENDBLOCKIDCORE_Client, ERROREDCORE_Client, ERRORSOURCECORE_Client,
%  TRANSACTIONCORE_Client, FULLNESSSTARTCORE_Client, FULLNESSENDCORE_Client,
%  INTERRUPTSSTARTCORE_Client, INTERRUPTSENDCORE_Client, TIMINGSTARTCORE_Client,
%  TIMINGENDCORE_Client] = IMPORTFILE(FILE, DATALINES) reads data for the
%  specified row interval(s) of text file FILENAME. Specify DATALINES as
%  a positive scalar integer or a N-by-2 array of positive scalar
%  integers for dis-contiguous row intervals.
%
%  Example:
%  [ArrayLengthBlocks, BlockSizeint32_tElements, BalancingNOPs, InitialNOPs, CheckPeriod, SteadyClockWalltimems, ClockCyclesCycleTimems, Clockrdtsc, ExpectedBlockIDCore_Server, StartBlockIDCore_Server, EndBlockIDCore_Server, ErroredCore_Server, ErrorSourceCore_Server, TransactionCore_Server, InterruptsStartCore_Server, InterruptsEndCore_Server, TimingStartCore_Server, TimingEndCore_Server, ExpectedBlockIDCore_Client, StartBlockIDCore_Client, EndBlockIDCore_Client, ErroredCore_Client, ErrorSourceCore_Client, TransactionCore_Client, FullnessStartCore_Client, FullnessEndCore_Client, InterruptsStartCore_Client, InterruptsEndCore_Client, TimingStartCore_Client, TimingEndCore_Client] = importfile("/Users/cyarp/git/benchmarking-github/core-core/results/bwrc-sr-amd-32c/open_loop/test_interrupt6/trunk.csv", [2, Inf]);
%
%  See also READTABLE.
%
% Auto-generated by MATLAB on 2_Server-Jan-2020 11:_Client6:3_Server

%% Input handling

% If dataLines is not specified, define defaults
if nargin < 2
    dataLines = [2, Inf];
end

%% Setup the Import Options and import the data
opts = delimitedTextImportOptions("NumVariables", 32);

% Specify range and delimiter
opts.DataLines = dataLines;
opts.Delimiter = ",";

% Specify column names and types
opts.VariableNames = ["ArrayLengthBlocks", "BlockSizeint32_tElements", "BalancingNOPs", "InitialNOPs", "CheckPeriod", "SteadyClockWalltimems", "ClockCyclesCycleTimems", "Clockrdtsc", "ExpectedBlockIDCore_Server", "StartBlockIDCore_Server", "EndBlockIDCore_Server", "ErroredCore_Server", "ErrorSourceCore_Server", "TransactionCore_Server", "Var1_Client", "Var16", "InterruptsStartCore_Server", "InterruptsEndCore_Server", "TimingStartCore_Server", "TimingEndCore_Server", "ExpectedBlockIDCore_Client", "StartBlockIDCore_Client", "EndBlockIDCore_Client", "ErroredCore_Client", "ErrorSourceCore_Client", "TransactionCore_Client", "FullnessStartCore_Client", "FullnessEndCore_Client", "InterruptsStartCore_Client", "InterruptsEndCore_Client", "TimingStartCore_Client", "TimingEndCore_Client"];
opts.SelectedVariableNames = ["ArrayLengthBlocks", "BlockSizeint32_tElements", "BalancingNOPs", "InitialNOPs", "CheckPeriod", "SteadyClockWalltimems", "ClockCyclesCycleTimems", "Clockrdtsc", "ExpectedBlockIDCore_Server", "StartBlockIDCore_Server", "EndBlockIDCore_Server", "ErroredCore_Server", "ErrorSourceCore_Server", "TransactionCore_Server", "InterruptsStartCore_Server", "InterruptsEndCore_Server", "TimingStartCore_Server", "TimingEndCore_Server", "ExpectedBlockIDCore_Client", "StartBlockIDCore_Client", "EndBlockIDCore_Client", "ErroredCore_Client", "ErrorSourceCore_Client", "TransactionCore_Client", "FullnessStartCore_Client", "FullnessEndCore_Client", "InterruptsStartCore_Client", "InterruptsEndCore_Client", "TimingStartCore_Client", "TimingEndCore_Client"];
opts.VariableTypes = ["double", "double", "double", "double", "double", "double", "double", "double", "double", "double", "double", "categorical", "categorical", "double", "string", "string", "string", "string", "string", "string", "double", "double", "double", "categorical", "categorical", "double", "string", "string", "string", "string", "string", "string"];

% Specify file level properties
opts.ExtraColumnsRule = "ignore";
opts.EmptyLineRule = "read";

% Specify variable properties
opts = setvaropts(opts, ["Var1_Client", "Var16", "InterruptsStartCore_Server", "InterruptsEndCore_Server", "TimingStartCore_Server", "TimingEndCore_Server", "FullnessStartCore_Client", "FullnessEndCore_Client", "InterruptsStartCore_Client", "InterruptsEndCore_Client", "TimingStartCore_Client", "TimingEndCore_Client"], "WhitespaceRule", "preserve");
opts = setvaropts(opts, ["ErroredCore_Server", "ErrorSourceCore_Server", "Var1_Client", "Var16", "InterruptsStartCore_Server", "InterruptsEndCore_Server", "TimingStartCore_Server", "TimingEndCore_Server", "ErroredCore_Client", "ErrorSourceCore_Client", "FullnessStartCore_Client", "FullnessEndCore_Client", "InterruptsStartCore_Client", "InterruptsEndCore_Client", "TimingStartCore_Client", "TimingEndCore_Client"], "EmptyFieldRule", "auto");

% Import the data
tbl = readtable(filename, opts);

%% Convert to output type
ArrayLengthBlocks = tbl.ArrayLengthBlocks;
BlockSizeint32_tElements = tbl.BlockSizeint32_tElements;
BalancingNOPs = tbl.BalancingNOPs;
InitialNOPs = tbl.InitialNOPs;
CheckPeriod = tbl.CheckPeriod;
SteadyClockWalltimems = tbl.SteadyClockWalltimems;
ClockCyclesCycleTimems = tbl.ClockCyclesCycleTimems;
Clockrdtsc = tbl.Clockrdtsc;
ExpectedBlockIDCore_Server = tbl.ExpectedBlockIDCore_Server;
StartBlockIDCore_Server = tbl.StartBlockIDCore_Server;
EndBlockIDCore_Server = tbl.EndBlockIDCore_Server;
ErroredCore_Server = tbl.ErroredCore_Server;
ErrorSourceCore_Server = tbl.ErrorSourceCore_Server;
TransactionCore_Server = tbl.TransactionCore_Server;
InterruptsStartCore_Server = tbl.InterruptsStartCore_Server;
InterruptsEndCore_Server = tbl.InterruptsEndCore_Server;
TimingStartCore_Server = tbl.TimingStartCore_Server;
TimingEndCore_Server = tbl.TimingEndCore_Server;
ExpectedBlockIDCore_Client = tbl.ExpectedBlockIDCore_Client;
StartBlockIDCore_Client = tbl.StartBlockIDCore_Client;
EndBlockIDCore_Client = tbl.EndBlockIDCore_Client;
ErroredCore_Client = tbl.ErroredCore_Client;
ErrorSourceCore_Client = tbl.ErrorSourceCore_Client;
TransactionCore_Client = tbl.TransactionCore_Client;
FullnessStartCore_Client = tbl.FullnessStartCore_Client;
FullnessEndCore_Client = tbl.FullnessEndCore_Client;
InterruptsStartCore_Client = tbl.InterruptsStartCore_Client;
InterruptsEndCore_Client = tbl.InterruptsEndCore_Client;
TimingStartCore_Client = tbl.TimingStartCore_Client;
TimingEndCore_Client = tbl.TimingEndCore_Client;
end
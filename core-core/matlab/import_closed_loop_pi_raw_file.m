function [ArrayLengthBlocks, BlockSizeint32_tElements, ServerControlPeriodIterations, ClientControlPeriodIterations, ControlGainPNOPs, ControlGainINOPs, InitialNOPs, SteadyClockWalltimems, ServerControlChecksCore_Server, SpeedupsCore_Server, SlowdownsCore_Server, ServerNOPsCore_Server, ClientNOPsCore_Server, ErroredCore_Client, ErrorSourceCore_Client, TransactionCore_Client] = import_closed_loop_pi_raw_file(filename, dataLines)
%IMPORTFILE Import data from a text file
%  [ARRAYLENGTHBLOCKS, BLOCKSIZEINT32_TELEMENTS,
%  SERVERCONTROLPERIODITERATIONS, CLIENTCONTROLPERIODITERATIONS,
%  CONTROLGAINPNOPS, CONTROLGAININOPS, INITIALNOPS,
%  STEADYCLOCKWALLTIMEMS, SERVERCONTROLCHECKSCORE_Server, SPEEDUPSCORE_Server,
%  SLOWDOWNSCORE_Server, SERVERNOPSCORE_Server, CLIENTNOPSCORE_Server, ERROREDCORE_Client,
%  ERRORSOURCECORE_Client, TRANSACTIONCORE_Client] = IMPORTFILE(FILENAME) reads data
%  from text file FILENAME for the default selection.  Returns the data
%  as column vectors.
%
%  [ARRAYLENGTHBLOCKS, BLOCKSIZEINT32_TELEMENTS,
%  SERVERCONTROLPERIODITERATIONS, CLIENTCONTROLPERIODITERATIONS,
%  CONTROLGAINPNOPS, CONTROLGAININOPS, INITIALNOPS,
%  STEADYCLOCKWALLTIMEMS, SERVERCONTROLCHECKSCORE_Server, SPEEDUPSCORE_Server,
%  SLOWDOWNSCORE_Server, SERVERNOPSCORE_Server, CLIENTNOPSCORE_Server, ERROREDCORE_Client,
%  ERRORSOURCECORE_Client, TRANSACTIONCORE_Client] = IMPORTFILE(FILE, DATALINES)
%  reads data for the specified row interval(s) of text file FILENAME.
%  Specify DATALINES as a positive scalar integer or a N-by-2 array of
%  positive scalar integers for dis-contiguous row intervals.
%
%  Example:
%  [ArrayLengthBlocks, BlockSizeint32_tElements, ServerControlPeriodIterations, ClientControlPeriodIterations, ControlGainPNOPs, ControlGainINOPs, InitialNOPs, SteadyClockWalltimems, ServerControlChecksCore_Server, SpeedupsCore_Server, SlowdownsCore_Server, ServerNOPsCore_Server, ClientNOPsCore_Server, ErroredCore_Client, ErrorSourceCore_Client, TransactionCore_Client] = importfile("/Users/cyarp/git/benchmarking-github/core-core/results/bwrc-sr-amd-32c/closed_loop/results_interruptsDisabled_noNOHZFULL_piControl/report_closed_loop_pi_bang_control_raw.csv", [2, Inf]);
%
%  See also READTABLE.
%
% Auto-generated by MATLAB on 2_Client-Feb-2020 18:32:11

%% Input handling

% If dataLines is not specified, define defaults
if nargin < 2
    dataLines = [2, Inf];
end

%% Setup the Import Options and import the data
opts = delimitedTextImportOptions("NumVariables", 23);

% Specify range and delimiter
opts.DataLines = dataLines;
opts.Delimiter = ",";

% Specify column names and types
opts.VariableNames = ["ArrayLengthBlocks", "BlockSizeint32_tElements", "ServerControlPeriodIterations", "ClientControlPeriodIterations", "ControlGainPNOPs", "ControlGainINOPs", "InitialNOPs", "SteadyClockWalltimems", "Var9", "Var10", "ServerControlChecksCore_Server", "SpeedupsCore_Server", "SlowdownsCore_Server", "ServerNOPsCore_Server", "ClientNOPsCore_Server", "Var16", "Var17", "Var18", "Var19", "Var20", "ErroredCore_Client", "ErrorSourceCore_Client", "TransactionCore_Client"];
opts.SelectedVariableNames = ["ArrayLengthBlocks", "BlockSizeint32_tElements", "ServerControlPeriodIterations", "ClientControlPeriodIterations", "ControlGainPNOPs", "ControlGainINOPs", "InitialNOPs", "SteadyClockWalltimems", "ServerControlChecksCore_Server", "SpeedupsCore_Server", "SlowdownsCore_Server", "ServerNOPsCore_Server", "ClientNOPsCore_Server", "ErroredCore_Client", "ErrorSourceCore_Client", "TransactionCore_Client"];
opts.VariableTypes = ["double", "double", "double", "double", "double", "double", "double", "double", "string", "string", "double", "double", "double", "double", "double", "string", "string", "string", "string", "string", "categorical", "categorical", "double"];

% Specify file level properties
opts.ExtraColumnsRule = "ignore";
opts.EmptyLineRule = "read";

% Specify variable properties
opts = setvaropts(opts, ["Var9", "Var10", "Var16", "Var17", "Var18", "Var19", "Var20"], "WhitespaceRule", "preserve");
opts = setvaropts(opts, ["Var9", "Var10", "Var16", "Var17", "Var18", "Var19", "Var20", "ErroredCore_Client", "ErrorSourceCore_Client"], "EmptyFieldRule", "auto");

% Import the data
tbl = readtable(filename, opts);

%% Convert to output type
ArrayLengthBlocks = tbl.ArrayLengthBlocks;
BlockSizeint32_tElements = tbl.BlockSizeint32_tElements;
ServerControlPeriodIterations = tbl.ServerControlPeriodIterations;
ClientControlPeriodIterations = tbl.ClientControlPeriodIterations;
ControlGainPNOPs = tbl.ControlGainPNOPs;
ControlGainINOPs = tbl.ControlGainINOPs;
InitialNOPs = tbl.InitialNOPs;
SteadyClockWalltimems = tbl.SteadyClockWalltimems;
ServerControlChecksCore_Server = tbl.ServerControlChecksCore_Server;
SpeedupsCore_Server = tbl.SpeedupsCore_Server;
SlowdownsCore_Server = tbl.SlowdownsCore_Server;
ServerNOPsCore_Server = tbl.ServerNOPsCore_Server;
ClientNOPsCore_Server = tbl.ClientNOPsCore_Server;
ErroredCore_Client = tbl.ErroredCore_Client;
ErrorSourceCore_Client = tbl.ErrorSourceCore_Client;
TransactionCore_Client = tbl.TransactionCore_Client;
end
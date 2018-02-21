%% Report Results from Benchmark
%This script generates various reports from benchmarking results.  These
%results are contained in an sqlite database.

%% Init
clear; close all; clc;

%% Open DB
conn = sqlite('fir-icc-results.db', 'readonly'); %Do not need to write into the DB

%% Report Basics
%Get a list of Suites, Kernels, and Instances
suites = conn.fetch('SELECT DISTINCT Name FROM Suite');
disp(['Suites: '])
for suite = suites
    disp(['    ' suite{1}])
end

kernels = conn.fetch('SELECT DISTINCT Name, KernelID FROM Kernel');
disp(['Kernels: '])
[rows1, cols1] = size(kernels);
for i = 1:rows1
    disp(['    ' num2str(kernels{i, 2}) ': ' kernels{i, 1}])
    
    kernelInsts = conn.fetch(['SELECT DISTINCT Description, KernelInstanceID FROM KernelInstance WHERE KernelID=', num2str(kernels{i,2})]);
    [rows2, cols2] = size(kernelInsts);
    for j = 1:rows2
        disp(['        ' num2str(kernelInsts{j, 2}) ': ' kernelInsts{j, 1}])
    end
        
end

compiler = conn.fetch('SELECT DISTINCT CompilerID, Name FROM Compiler');
disp(['Compilers: '])
[rows1, cols1] = size(compiler);
for i = 1:rows1
    disp(['    ' num2str(compiler{i, 1}) ': ' compiler{i, 2}])
end

machines = conn.fetch('SELECT DISTINCT MachineID, Name FROM Machine');
disp(['Machines: '])
[rows1, cols1] = size(machines);
for i = 1:rows1
    disp(['    ' num2str(machines{i, 1}) ': ' machines{i, 2}])
end

disp(' ')

%% Specific Reports
% FIR Naive
kernel = conn.fetch('SELECT KernelID FROM Kernel WHERE Name=''FIR - Single''');
kernelID = kernel{1};

kernelInstanceDescription='Naive implementation with blocked input and output, shift register state, no explicit vectorization or unrolling.';
kernelInstance = conn.fetch(['SELECT KernelInstanceID FROM KernelInstance WHERE KernelID=' num2str(kernelID) ' AND Description=''' kernelInstanceDescription '''']);
instanceID = kernelInstance{1};
machine = conn.fetch('SELECT MachineID FROM Machine WHERE Name=''Chris Workstation VM''');
machineID = machine{1};

disp(['Report for Kernel ' num2str(kernelID) ' Instance ' num2str(instanceID)])

compilerParams = conn.fetch(['SELECT DISTINCT Parameter.ParameterID, Parameter.Name FROM Parameter JOIN ParameterValue ON Parameter.ParameterID=ParameterValue.ParameterID JOIN Config ON ParameterValue.ParameterValueID=Config.ParameterValueID JOIN Usage ON Config.UsageID=Usage.UsageID JOIN Run on Config.RunID=Run.RunID WHERE Usage.Name=''Compiler'' AND Run.KernelInstanceID=' num2str(instanceID)]);
disp(['Compiler Parameters: '])
[rows1, cols1] = size(compilerParams);
for i = 1:rows1
    disp(['    ' num2str(compilerParams{i, 1}) ': ' compilerParams{i, 2}])
    
    %compilerParamVals = conn.fetch(['SELECT ParameterValue.ParameterValueID, ParameterValue.FullString FROM ParameterValue WHERE ParameterValue.ParameterID=' num2str(compilerParams{i, 1}) ' AND (ParameterValue.ValueNumeric IS NOT NULL OR ParameterValue.ValueString IS NOT NULL)']);
    compilerParamVals = conn.fetch(['SELECT ParameterValue.ParameterValueID, ParameterValue.FullString FROM ParameterValue WHERE ParameterValue.ParameterID=' num2str(compilerParams{i, 1})]);

    [rows2, cols2] = size(compilerParamVals);
    for j = 1:rows2
        disp(['        ' num2str(compilerParamVals{j, 1}) ': ' compilerParamVals{j, 2}])
    end
end

compilerParams = conn.fetch(['SELECT DISTINCT Parameter.ParameterID, Parameter.Name FROM Parameter JOIN ParameterValue ON Parameter.ParameterID=ParameterValue.ParameterID JOIN Config ON ParameterValue.ParameterValueID=Config.ParameterValueID JOIN Usage ON Config.UsageID=Usage.UsageID JOIN Run on Config.RunID=Run.RunID WHERE Usage.Name=''Kernel'' AND Run.KernelInstanceID=' num2str(instanceID)]);
disp(['Kernel Parameters: '])
[rows1, cols1] = size(compilerParams);
for i = 1:rows1
    disp(['    ' num2str(compilerParams{i, 1}) ': ' compilerParams{i, 2}])
end

%Do some plots

%Pick a set of parameters
%Get a list of RunIDs with an exact set of compiler parameters, ordered by the value of the
%COEF_LEN parameter

%FIRST, WE NEED TO condition that the runs are associated with the kernel
%we are interested in.

%One Way to do the above is to find the unique set of runs that have each
%of the desired parameter values and then take the intersection of them to find
%the set that have all of the parameters we desire.  Do one more set intersection but this time
%to ensure that the run has at least one parameter value that belongs to the family we are going
%to use for the x axis.  This involves a join, and conditioning on
%parametervalues linked to our x axis.  At this point, all runs should
%contain at least the parameters we would like (including at least one
%instance of the x axis parameter).  To ensure each does not contain any
%more parameters, we take the count of
%parameters assigned to each run and select thoes runs which have 

%Get the results for each run and compute the throughput avg and std dev
%Note the exact number of stimulus items uses a floor, compute that

%Repeat for other parameter parameters of interes (ex. different
%optimization level, different datatype, etc...)

%Also, do some graphs vs block size


%Speed vs COEF_LEN.  Plot different optomization levels and march.  
%Hold Const: -std=c++11, DATATYPE=double, IO_LEN=0.  
%Check all have PRINT_TITLE, PRINT_TRIALS,
%PRINT_STATS, WRITE_CSV, TRIALS, STIM_LEN.  We will pick a specific value,
%100000 for now.  Same for Trials


%Things that may change from different experement runs but are constant for
%graphs
trials = 10;
stimLen = 100000;
compiler = 'Intel C/C++ Compiler';

compilerX = conn.fetch(['SELECT CompilerID FROM Compiler WHERE Name=''' compiler '''']);
compilerID = compilerX{1};
yAxisResultTypeName = 'Clock - Cycles/Cycle Time (ms)';
yAxisResultTypeID = getResultTypeID(conn, {yAxisResultTypeName});
stimLenName = 'STIM_LEN';
stimLenID = getParameterIDs(conn, {stimLenName});
ioLenName = 'IO_LEN';
ioLenID = getParameterIDs(conn, {ioLenName});

%% Make a Plot of Different Data Types (-0fast)
kernelInstanceDescriptions={
    'Naive implementation with blocked input and output, shift register state, no explicit vectorization or unrolling.', ...
    'Circular buffer implementation with blocked input and output, circular buffer state, no explicit vectorization or unrolling.', ...
    'Circular buffer implementation with no modulus, blocked input and output, circular buffer state, no explicit vectorization or unrolling.', ...
    'Circular buffer implementation with blocked input and output, circular buffer state, no explicit vectorization or unrolling. - Reverse Direction', ...
    'Circular buffer implementation with no modulus, blocked input and output, circular buffer state, no explicit vectorization or unrolling. - Reverse Direction', ...
    'Naive implementation with blocked input and output, shift register state, manually unrolled by 2.', ...
    'Naive implementation with blocked input and output, shift register state, manually unrolled by 4.', ...
    'FIR Intel IPP' ...
    };

kernelInstTitles = {'Shift Register', 'Circular Buffer w/ Mod', 'Circular Buffer w/o Mod', 'Circular Buffer w/ Mod - Reverse', 'Circular Buffer w/o Mod - Reverse', 'Shift Register Unroll 2',  'Shift Register Unroll 4', 'Intel IPP'};

%io_lens = [12, 12, 12, 12, 12, 12, 12];
%io_lens = [20, 20, 20, 20, 20, 20, 20];
%io_lens = [1, 1, 1, 1, 1, 1, 1, 1];
%io_lens = [28, 28, 28, 28, 28, 28, 28, 28];
%io_lens = [34, 34, 34, 34, 34, 34, 34, 34];
io_lens = [36, 36, 36, 36, 36, 36, 36, 36];

insts = length(kernelInstanceDescriptions);
for graphNum = 1:insts
    
    kernelInstanceDescription = kernelInstanceDescriptions{graphNum};
    kernelInstTitle = kernelInstTitles{graphNum};
    
    kernelInstance = conn.fetch(['SELECT KernelInstanceID FROM KernelInstance WHERE KernelID=' num2str(kernelID) ' AND Description=''' kernelInstanceDescription '''']);
    instanceID = kernelInstance{1};

    xAxisParamName = 'COEF_LEN';
    xAxisParam = getParameterIDs(conn, {xAxisParamName});

    %These are things that we have other options for but are holding constant
    %for the graph
    datatype = 'double';
    ioLen = io_lens(graphNum);
    
    optimizationLvlX = {'-Ofast'};
    optimizationLvl = optimizationLvlX{1};

    legendInd = 1;
    legendLbls = {};

    fig = figure;

    %for marchX = {'', 'core2', 'corei7', 'corei7-avx', 'core-avx-i', 'core-avx2', 'native'}
    for marchX = {'native'}
        march = marchX{1};
        for datatypeX = {'int16_t', 'int32_t', 'float', 'double'}
            datatype = datatypeX{1};

            legendLbls{legendInd} = '';
            legendLbls{legendInd} = [legendLbls{legendInd} strrep(datatype, '_', '\_')];

            %Parameter.Name, 0=No Val|1=Numeric|2=String, ParameterValue.Value
            constParams = {'DATATYPE', 2, datatype; ...
                           '-std=c++11', 0, ''; ... %Begin the Always Parameters
                           'PRINT_TITLE', 1, 0; ... 
                           'PRINT_TRIALS', 1, 0; ...
                           'PRINT_STATS', 1, 0; ...
                           'WRITE_CSV', 1, 1; ...
                           'TRIALS', 1, trials; ... %Change for future Runs
                           'STIM_LEN', 1, stimLen; ...
                           'IO_LEN', 1, ioLen; ...
                           };

            %Special Case these parameters as they sometimes are not
            %included
            if strcmp(march, '') == 0
               [rows, cols] = size(constParams);
               constParams{rows+1, 1} = 'march';
               constParams{rows+1, 2} = 2;
               constParams{rows+1, 3} = march;

                %legendLbls{legendInd} = [legendLbls{legendInd} '-march=' march ' '];
            end

            if strcmp(optimizationLvl, '') == 0
               [rows, cols] = size(constParams);
               constParams{rows+1, 1} = optimizationLvl;
               constParams{rows+1, 2} = 0;
               constParams{rows+1, 3} = '';
               %legendLbls{legendInd} = [legendLbls{legendInd} optimizationLvl];
            end

            atLeastOneParams = {xAxisParamName};

            plotResultSet(conn, constParams, atLeastOneParams, xAxisParam, yAxisResultTypeID, instanceID, compilerID, machineID, stimLenID, ioLenID, 'o-');
            hold all;
            legendInd = legendInd + 1;
        end
    end

    title(['FIR Filter (' kernelInstTitle ') Execution Time For Different Datatypes']);
    xlabel('Filter Order (Coefficients)');
    ylabel(['Execution Time for ' num2str(stimLen) ' samples (ms)']);
    leg = legend(legendLbls,'Location','northwest');
    title(leg, ['Error Bars Represent StdDev\newlineCompiler: ' compiler '\newlineIO Block Length: ' num2str(ioLen) '\newlineCompiler Flags: ' strrep(optimizationLvl, '_', '\_') ]);
    grid on;
    %ALSO DO A 2D PLOT OF IO_LEN and COEF_LEN
end


%% Make a Plot of Different Compiler Flags (double)
kernelInstanceDescriptions={
    'Naive implementation with blocked input and output, shift register state, no explicit vectorization or unrolling.', ...
    'Circular buffer implementation with blocked input and output, circular buffer state, no explicit vectorization or unrolling.', ...
    'Circular buffer implementation with no modulus, blocked input and output, circular buffer state, no explicit vectorization or unrolling.', ...
    'Circular buffer implementation with blocked input and output, circular buffer state, no explicit vectorization or unrolling. - Reverse Direction', ...
    'Circular buffer implementation with no modulus, blocked input and output, circular buffer state, no explicit vectorization or unrolling. - Reverse Direction', ...
    'Naive implementation with blocked input and output, shift register state, manually unrolled by 2.', ...
    'Naive implementation with blocked input and output, shift register state, manually unrolled by 4.', ...
    'FIR Intel IPP' ...
    };

kernelInstTitles = {'Shift Register', 'Circular Buffer w/ Mod', 'Circular Buffer w/o Mod', 'Circular Buffer w/ Mod - Reverse', 'Circular Buffer w/o Mod - Reverse', 'Shift Register Unroll 2',  'Shift Register Unroll 4', 'Intel IPP'};

%io_lens = [12, 12, 12, 12, 12, 12, 12, 12];
%io_lens = [20, 20, 20, 20, 20, 20, 20, 20];
%io_lens = [1, 1, 1, 1, 1, 1, 1, 1];
%io_lens = [28, 28, 28, 28, 28, 28, 28, 28];
%io_lens = [34, 34, 34, 34, 34, 34, 34, 34];
io_lens = [36, 36, 36, 36, 36, 36, 36, 36];

insts = length(kernelInstanceDescriptions);
for graphNum = 1:insts
    
    kernelInstanceDescription = kernelInstanceDescriptions{graphNum};
    kernelInstTitle = kernelInstTitles{graphNum};
    
    kernelInstance = conn.fetch(['SELECT KernelInstanceID FROM KernelInstance WHERE KernelID=' num2str(kernelID) ' AND Description=''' kernelInstanceDescription '''']);
    instanceID = kernelInstance{1};

    xAxisParamName = 'COEF_LEN';
    xAxisParam = getParameterIDs(conn, {xAxisParamName});

    %These are things that we have other options for but are holding constant
    %for the graph
    datatype = 'double';
    ioLen = io_lens(graphNum);

    legendInd = 1;
    legendLbls = {};

    fig = figure;

    %for marchX = {'', 'core2', 'corei7', 'corei7-avx', 'core-avx-i', 'core-avx2', 'native'}
    for marchX = {'native'}
        march = marchX{1};
        for optimizationLvlX = {'','-O2', '-O3', '-Ofast'}
            optimizationLvl = optimizationLvlX{1};

            legendLbls{legendInd} = '';

            %Parameter.Name, 0=No Val|1=Numeric|2=String, ParameterValue.Value
            constParams = {'DATATYPE', 2, datatype; ...
                           '-std=c++11', 0, ''; ... %Begin the Always Parameters
                           'PRINT_TITLE', 1, 0; ... 
                           'PRINT_TRIALS', 1, 0; ...
                           'PRINT_STATS', 1, 0; ...
                           'WRITE_CSV', 1, 1; ...
                           'TRIALS', 1, trials; ... %Change for future Runs
                           'STIM_LEN', 1, stimLen; ...
                           'IO_LEN', 1, ioLen; ...
                           };

            %Special Case these parameters as they sometimes are not
            %included
            if strcmp(march, '') == 0
               [rows, cols] = size(constParams);
               constParams{rows+1, 1} = 'march';
               constParams{rows+1, 2} = 2;
               constParams{rows+1, 3} = march;

                legendLbls{legendInd} = [legendLbls{legendInd} '-march=' march ' '];
            end

            if strcmp(optimizationLvl, '') == 0
               [rows, cols] = size(constParams);
               constParams{rows+1, 1} = optimizationLvl;
               constParams{rows+1, 2} = 0;
               constParams{rows+1, 3} = '';
               legendLbls{legendInd} = [legendLbls{legendInd} optimizationLvl];
            end

            atLeastOneParams = {xAxisParamName};

            plotResultSet(conn, constParams, atLeastOneParams, xAxisParam, yAxisResultTypeID, instanceID, compilerID, machineID, stimLenID, ioLenID, 'o-');
            hold all;
            legendInd = legendInd + 1;
        end
    end

    title(['FIR Filter (' kernelInstTitle ') Execution Time For Different Compiler Flags']);
    xlabel('Filter Order (Coefficients)');
    ylabel(['Execution Time for ' num2str(stimLen) ' samples (ms)']);
    leg = legend(legendLbls,'Location','northwest');
    title(leg, ['Error Bars Represent StdDev\newlineCompiler: ' compiler '\newlineIO Block Length: ' num2str(ioLen) '\newlineDataType: ' strrep(datatype, '_', '\_') ]);
    grid on;
    %ALSO DO A 2D PLOT OF IO_LEN and COEF_LEN
end

%% Make a Plot of Different Compiler Flags (int16_t)
kernelInstanceDescriptions={
    'Naive implementation with blocked input and output, shift register state, no explicit vectorization or unrolling.', ...
    'Circular buffer implementation with blocked input and output, circular buffer state, no explicit vectorization or unrolling.', ...
    'Circular buffer implementation with no modulus, blocked input and output, circular buffer state, no explicit vectorization or unrolling.', ...
    'Circular buffer implementation with blocked input and output, circular buffer state, no explicit vectorization or unrolling. - Reverse Direction', ...
    'Circular buffer implementation with no modulus, blocked input and output, circular buffer state, no explicit vectorization or unrolling. - Reverse Direction', ...
    'Naive implementation with blocked input and output, shift register state, manually unrolled by 2.', ...
    'Naive implementation with blocked input and output, shift register state, manually unrolled by 4.', ...
    'FIR Intel IPP' ...
    };

kernelInstTitles = {'Shift Register', 'Circular Buffer w/ Mod', 'Circular Buffer w/o Mod', 'Circular Buffer w/ Mod - Reverse', 'Circular Buffer w/o Mod - Reverse', 'Shift Register Unroll 2',  'Shift Register Unroll 4', 'Intel IPP'};

%io_lens = [12, 12, 12, 12, 12, 12, 12, 12];
%io_lens = [20, 20, 20, 20, 20, 20, 20, 20];
%io_lens = [28, 28, 28, 28, 28, 28, 28, 28];
%io_lens = [34, 34, 34, 34, 34, 34, 34, 34];
io_lens = [36, 36, 36, 36, 36, 36, 36, 36];

insts = length(kernelInstanceDescriptions);
for graphNum = 1:insts
    
    kernelInstanceDescription = kernelInstanceDescriptions{graphNum};
    kernelInstTitle = kernelInstTitles{graphNum};
    
    kernelInstance = conn.fetch(['SELECT KernelInstanceID FROM KernelInstance WHERE KernelID=' num2str(kernelID) ' AND Description=''' kernelInstanceDescription '''']);
    instanceID = kernelInstance{1};

    xAxisParamName = 'COEF_LEN';
    xAxisParam = getParameterIDs(conn, {xAxisParamName});

    %These are things that we have other options for but are holding constant
    %for the graph
    datatype = 'int16_t';
    ioLen = io_lens(graphNum);

    legendInd = 1;
    legendLbls = {};

    fig = figure;

    %for marchX = {'', 'core2', 'corei7', 'corei7-avx', 'core-avx-i', 'core-avx2', 'native'}
    for marchX = {'native'}
        march = marchX{1};
        for optimizationLvlX = {'', '-O2', '-O3', '-Ofast'}
            optimizationLvl = optimizationLvlX{1};

            legendLbls{legendInd} = '';

            %Parameter.Name, 0=No Val|1=Numeric|2=String, ParameterValue.Value
            constParams = {'DATATYPE', 2, datatype; ...
                           '-std=c++11', 0, ''; ... %Begin the Always Parameters
                           'PRINT_TITLE', 1, 0; ... 
                           'PRINT_TRIALS', 1, 0; ...
                           'PRINT_STATS', 1, 0; ...
                           'WRITE_CSV', 1, 1; ...
                           'TRIALS', 1, trials; ... %Change for future Runs
                           'STIM_LEN', 1, stimLen; ...
                           'IO_LEN', 1, ioLen; ...
                           };

            %Special Case these parameters as they sometimes are not
            %included
            if strcmp(march, '') == 0
               [rows, cols] = size(constParams);
               constParams{rows+1, 1} = 'march';
               constParams{rows+1, 2} = 2;
               constParams{rows+1, 3} = march;

                legendLbls{legendInd} = [legendLbls{legendInd} '-march=' march ' '];
            end

            if strcmp(optimizationLvl, '') == 0
               [rows, cols] = size(constParams);
               constParams{rows+1, 1} = optimizationLvl;
               constParams{rows+1, 2} = 0;
               constParams{rows+1, 3} = '';
               legendLbls{legendInd} = [legendLbls{legendInd} optimizationLvl];
            end

            atLeastOneParams = {xAxisParamName};

            plotResultSet(conn, constParams, atLeastOneParams, xAxisParam, yAxisResultTypeID, instanceID, compilerID, machineID, stimLenID, ioLenID, 'o-');
            hold all;
            legendInd = legendInd + 1;
        end
    end

    title(['FIR Filter (' kernelInstTitle ') Execution Time For Different Compiler Flags']);
    xlabel('Filter Order (Coefficients)');
    ylabel(['Execution Time for ' num2str(stimLen) ' samples (ms)']);
    leg = legend(legendLbls,'Location','northwest');
    title(leg, ['Error Bars Represent StdDev\newlineCompiler: ' compiler '\newlineIO Block Length: ' num2str(ioLen) '\newlineDataType: ' strrep(datatype, '_', '\_') ]);
    grid on;
    %ALSO DO A 2D PLOT OF IO_LEN and COEF_LEN
end

%% Different Block Lengths for Unrolling 2 -double

kernelInstanceDescription = 'Naive implementation with blocked input and output, shift register state, manually unrolled by 2.';
kernelInstTitle = 'Shift Register Unroll 2';

kernelInstance = conn.fetch(['SELECT KernelInstanceID FROM KernelInstance WHERE KernelID=' num2str(kernelID) ' AND Description=''' kernelInstanceDescription '''']);
instanceID = kernelInstance{1};

xAxisParamName = 'IO_LEN';
xAxisParam = getParameterIDs(conn, {xAxisParamName});

datatype = 'double';
optimizationLvl = '-Ofast';
march = 'native';

legendInd = 1;
legendLbls = {};

fig = figure;

for coefLenX = {4, 20, 36, 52, 68, 84, 100}
    coefLen=coefLenX{1};

    legendLbls{legendInd} = num2str(coefLen);

    %Parameter.Name, 0=No Val|1=Numeric|2=String, ParameterValue.Value
    constParams = {'DATATYPE', 2, datatype; ...
                   '-std=c++11', 0, ''; ... %Begin the Always Parameters
                   'PRINT_TITLE', 1, 0; ... 
                   'PRINT_TRIALS', 1, 0; ...
                   'PRINT_STATS', 1, 0; ...
                   'WRITE_CSV', 1, 1; ...
                   'TRIALS', 1, trials; ... %Change for future Runs
                   'STIM_LEN', 1, stimLen; ...
                   'COEF_LEN', 1, coefLen; ...
                   };

    %Special Case these parameters as they sometimes are not
    %included
    if strcmp(march, '') == 0
       [rows, cols] = size(constParams);
       constParams{rows+1, 1} = 'march';
       constParams{rows+1, 2} = 2;
       constParams{rows+1, 3} = march;
    end

    if strcmp(optimizationLvl, '') == 0
       [rows, cols] = size(constParams);
       constParams{rows+1, 1} = optimizationLvl;
       constParams{rows+1, 2} = 0;
       constParams{rows+1, 3} = '';
    end

    atLeastOneParams = {xAxisParamName};

    plotResultSet(conn, constParams, atLeastOneParams, xAxisParam, yAxisResultTypeID, instanceID, compilerID, machineID, stimLenID, ioLenID, 'o-');
    hold all;
    legendInd = legendInd + 1;
end

title(['FIR Filter (' kernelInstTitle ') Execution Time For Different Filter Orders']);
xlabel('IO Block Length (Samples)');
ylabel(['Execution Time for ' num2str(stimLen) ' samples (ms)']);
leg = legend(legendLbls,'Location','northwest');
compilerFlags = optimizationLvl;
if strcmp(march, '') == 0
    compilerFlags = [compilerFlags ' -march=' march];
end

title(leg, ['Error Bars Represent StdDev\newlineCompiler: ' compiler '\newlineCompiler Flags: ' compilerFlags '\newlineDataType: ' strrep(datatype, '_', '\_') ]);
grid on;

%% Different Block Lengths for Unrolling 4 -double
kernelInstanceDescription = 'Naive implementation with blocked input and output, shift register state, manually unrolled by 4.';
kernelInstTitle = 'Shift Register Unroll 4';

kernelInstance = conn.fetch(['SELECT KernelInstanceID FROM KernelInstance WHERE KernelID=' num2str(kernelID) ' AND Description=''' kernelInstanceDescription '''']);
instanceID = kernelInstance{1};

xAxisParamName = 'IO_LEN';
xAxisParam = getParameterIDs(conn, {xAxisParamName});

datatype = 'double';
optimizationLvl = '-Ofast';
march = 'native';

legendInd = 1;
legendLbls = {};

fig = figure;

for coefLenX = {4, 20, 36, 52, 68, 84, 100}
    coefLen=coefLenX{1};

    legendLbls{legendInd} = num2str(coefLen);

    %Parameter.Name, 0=No Val|1=Numeric|2=String, ParameterValue.Value
    constParams = {'DATATYPE', 2, datatype; ...
                   '-std=c++11', 0, ''; ... %Begin the Always Parameters
                   'PRINT_TITLE', 1, 0; ... 
                   'PRINT_TRIALS', 1, 0; ...
                   'PRINT_STATS', 1, 0; ...
                   'WRITE_CSV', 1, 1; ...
                   'TRIALS', 1, trials; ... %Change for future Runs
                   'STIM_LEN', 1, stimLen; ...
                   'COEF_LEN', 1, coefLen; ...
                   };

    %Special Case these parameters as they sometimes are not
    %included
    if strcmp(march, '') == 0
       [rows, cols] = size(constParams);
       constParams{rows+1, 1} = 'march';
       constParams{rows+1, 2} = 2;
       constParams{rows+1, 3} = march;
    end

    if strcmp(optimizationLvl, '') == 0
       [rows, cols] = size(constParams);
       constParams{rows+1, 1} = optimizationLvl;
       constParams{rows+1, 2} = 0;
       constParams{rows+1, 3} = '';
    end

    atLeastOneParams = {xAxisParamName};

    plotResultSet(conn, constParams, atLeastOneParams, xAxisParam, yAxisResultTypeID, instanceID, compilerID, machineID, stimLenID, ioLenID, 'o-');
    hold all;
    legendInd = legendInd + 1;
end

title(['FIR Filter (' kernelInstTitle ') Execution Time For Different Filter Orders']);
xlabel('IO Block Length (Samples)');
ylabel(['Execution Time for ' num2str(stimLen) ' samples (ms)']);
leg = legend(legendLbls,'Location','northwest');
compilerFlags = optimizationLvl;
if strcmp(march, '') == 0
    compilerFlags = [compilerFlags ' -march=' march];
end

title(leg, ['Error Bars Represent StdDev\newlineCompiler: ' compiler '\newlineCompiler Flags: ' compilerFlags '\newlineDataType: ' strrep(datatype, '_', '\_') ]);
grid on;

%% Different Block Lengths for Unrolling 2 -int16_t

kernelInstanceDescription = 'Naive implementation with blocked input and output, shift register state, manually unrolled by 2.';
kernelInstTitle = 'Shift Register Unroll 2';

kernelInstance = conn.fetch(['SELECT KernelInstanceID FROM KernelInstance WHERE KernelID=' num2str(kernelID) ' AND Description=''' kernelInstanceDescription '''']);
instanceID = kernelInstance{1};

xAxisParamName = 'IO_LEN';
xAxisParam = getParameterIDs(conn, {xAxisParamName});

datatype = 'int16_t';
optimizationLvl = '-Ofast';
march = 'native';

legendInd = 1;
legendLbls = {};

fig = figure;

for coefLenX = {4, 20, 36, 52, 68, 84, 100}
    coefLen=coefLenX{1};

    legendLbls{legendInd} = num2str(coefLen);

    %Parameter.Name, 0=No Val|1=Numeric|2=String, ParameterValue.Value
    constParams = {'DATATYPE', 2, datatype; ...
                   '-std=c++11', 0, ''; ... %Begin the Always Parameters
                   'PRINT_TITLE', 1, 0; ... 
                   'PRINT_TRIALS', 1, 0; ...
                   'PRINT_STATS', 1, 0; ...
                   'WRITE_CSV', 1, 1; ...
                   'TRIALS', 1, trials; ... %Change for future Runs
                   'STIM_LEN', 1, stimLen; ...
                   'COEF_LEN', 1, coefLen; ...
                   };

    %Special Case these parameters as they sometimes are not
    %included
    if strcmp(march, '') == 0
       [rows, cols] = size(constParams);
       constParams{rows+1, 1} = 'march';
       constParams{rows+1, 2} = 2;
       constParams{rows+1, 3} = march;
    end

    if strcmp(optimizationLvl, '') == 0
       [rows, cols] = size(constParams);
       constParams{rows+1, 1} = optimizationLvl;
       constParams{rows+1, 2} = 0;
       constParams{rows+1, 3} = '';
    end

    atLeastOneParams = {xAxisParamName};

    plotResultSet(conn, constParams, atLeastOneParams, xAxisParam, yAxisResultTypeID, instanceID, compilerID, machineID, stimLenID, ioLenID, 'o-');
    hold all;
    legendInd = legendInd + 1;
end

title(['FIR Filter (' kernelInstTitle ') Execution Time For Different Filter Orders']);
xlabel('IO Block Length (Samples)');
ylabel(['Execution Time for ' num2str(stimLen) ' samples (ms)']);
leg = legend(legendLbls,'Location','northwest');
compilerFlags = optimizationLvl;
if strcmp(march, '') == 0
    compilerFlags = [compilerFlags ' -march=' march];
end

title(leg, ['Error Bars Represent StdDev\newlineCompiler: ' compiler '\newlineCompiler Flags: ' compilerFlags '\newlineDataType: ' strrep(datatype, '_', '\_') ]);
grid on;

%% Different Block Lengths for Unrolling 4 -int16_t
kernelInstanceDescription = 'Naive implementation with blocked input and output, shift register state, manually unrolled by 4.';
kernelInstTitle = 'Shift Register Unroll 4';

kernelInstance = conn.fetch(['SELECT KernelInstanceID FROM KernelInstance WHERE KernelID=' num2str(kernelID) ' AND Description=''' kernelInstanceDescription '''']);
instanceID = kernelInstance{1};

xAxisParamName = 'IO_LEN';
xAxisParam = getParameterIDs(conn, {xAxisParamName});

datatype = 'int16_t';
optimizationLvl = '-Ofast';
march = 'native';

legendInd = 1;
legendLbls = {};

fig = figure;

for coefLenX = {4, 20, 36, 52, 68, 84, 100}
    coefLen=coefLenX{1};

    legendLbls{legendInd} = num2str(coefLen);

    %Parameter.Name, 0=No Val|1=Numeric|2=String, ParameterValue.Value
    constParams = {'DATATYPE', 2, datatype; ...
                   '-std=c++11', 0, ''; ... %Begin the Always Parameters
                   'PRINT_TITLE', 1, 0; ... 
                   'PRINT_TRIALS', 1, 0; ...
                   'PRINT_STATS', 1, 0; ...
                   'WRITE_CSV', 1, 1; ...
                   'TRIALS', 1, trials; ... %Change for future Runs
                   'STIM_LEN', 1, stimLen; ...
                   'COEF_LEN', 1, coefLen; ...
                   };

    %Special Case these parameters as they sometimes are not
    %included
    if strcmp(march, '') == 0
       [rows, cols] = size(constParams);
       constParams{rows+1, 1} = 'march';
       constParams{rows+1, 2} = 2;
       constParams{rows+1, 3} = march;
    end

    if strcmp(optimizationLvl, '') == 0
       [rows, cols] = size(constParams);
       constParams{rows+1, 1} = optimizationLvl;
       constParams{rows+1, 2} = 0;
       constParams{rows+1, 3} = '';
    end

    atLeastOneParams = {xAxisParamName};

    plotResultSet(conn, constParams, atLeastOneParams, xAxisParam, yAxisResultTypeID, instanceID, compilerID, machineID, stimLenID, ioLenID, 'o-');
    hold all;
    legendInd = legendInd + 1;
end

title(['FIR Filter (' kernelInstTitle ') Execution Time For Different Filter Orders']);
xlabel('IO Block Length (Samples)');
ylabel(['Execution Time for ' num2str(stimLen) ' samples (ms)']);
leg = legend(legendLbls,'Location','northwest');
compilerFlags = optimizationLvl;
if strcmp(march, '') == 0
    compilerFlags = [compilerFlags ' -march=' march];
end

title(leg, ['Error Bars Represent StdDev\newlineCompiler: ' compiler '\newlineCompiler Flags: ' compilerFlags '\newlineDataType: ' strrep(datatype, '_', '\_') ]);
grid on;

%% Different Block Lengths for Intel IPP -double
kernelInstanceDescription = 'FIR Intel IPP';
kernelInstTitle = 'Intel IPP';

kernelInstance = conn.fetch(['SELECT KernelInstanceID FROM KernelInstance WHERE KernelID=' num2str(kernelID) ' AND Description=''' kernelInstanceDescription '''']);
instanceID = kernelInstance{1};

xAxisParamName = 'IO_LEN';
xAxisParam = getParameterIDs(conn, {xAxisParamName});

datatype = 'double';
optimizationLvl = '-Ofast';
march = 'native';

legendInd = 1;
legendLbls = {};

fig = figure;

for coefLenX = {1, 4, 20, 36, 52, 68, 84, 100}
    coefLen=coefLenX{1};

    legendLbls{legendInd} = num2str(coefLen);

    %Parameter.Name, 0=No Val|1=Numeric|2=String, ParameterValue.Value
    constParams = {'DATATYPE', 2, datatype; ...
                   '-std=c++11', 0, ''; ... %Begin the Always Parameters
                   'PRINT_TITLE', 1, 0; ... 
                   'PRINT_TRIALS', 1, 0; ...
                   'PRINT_STATS', 1, 0; ...
                   'WRITE_CSV', 1, 1; ...
                   'TRIALS', 1, trials; ... %Change for future Runs
                   'STIM_LEN', 1, stimLen; ...
                   'COEF_LEN', 1, coefLen; ...
                   };

    %Special Case these parameters as they sometimes are not
    %included
    if strcmp(march, '') == 0
       [rows, cols] = size(constParams);
       constParams{rows+1, 1} = 'march';
       constParams{rows+1, 2} = 2;
       constParams{rows+1, 3} = march;
    end

    if strcmp(optimizationLvl, '') == 0
       [rows, cols] = size(constParams);
       constParams{rows+1, 1} = optimizationLvl;
       constParams{rows+1, 2} = 0;
       constParams{rows+1, 3} = '';
    end

    atLeastOneParams = {xAxisParamName};

    plotResultSet(conn, constParams, atLeastOneParams, xAxisParam, yAxisResultTypeID, instanceID, compilerID, machineID, stimLenID, ioLenID, 'o-');
    hold all;
    legendInd = legendInd + 1;
end

title(['FIR Filter (' kernelInstTitle ') Execution Time For Different Filter Orders']);
xlabel('IO Block Length (Samples)');
ylabel(['Execution Time for ' num2str(stimLen) ' samples (ms)']);
leg = legend(legendLbls,'Location','northeast');
compilerFlags = optimizationLvl;
if strcmp(march, '') == 0
    compilerFlags = [compilerFlags ' -march=' march];
end

title(leg, ['Error Bars Represent StdDev\newlineCompiler: ' compiler '\newlineCompiler Flags: ' compilerFlags '\newlineDataType: ' strrep(datatype, '_', '\_') ]);
grid on;

%% Different Block Lengths for Intel IPP -float
kernelInstanceDescription = 'FIR Intel IPP';
kernelInstTitle = 'Intel IPP';

kernelInstance = conn.fetch(['SELECT KernelInstanceID FROM KernelInstance WHERE KernelID=' num2str(kernelID) ' AND Description=''' kernelInstanceDescription '''']);
instanceID = kernelInstance{1};

xAxisParamName = 'IO_LEN';
xAxisParam = getParameterIDs(conn, {xAxisParamName});

datatype = 'float';
optimizationLvl = '-Ofast';
march = 'native';

legendInd = 1;
legendLbls = {};

fig = figure;

for coefLenX = {1, 4, 20, 36, 52, 68, 84, 100}
    coefLen=coefLenX{1};

    legendLbls{legendInd} = num2str(coefLen);

    %Parameter.Name, 0=No Val|1=Numeric|2=String, ParameterValue.Value
    constParams = {'DATATYPE', 2, datatype; ...
                   '-std=c++11', 0, ''; ... %Begin the Always Parameters
                   'PRINT_TITLE', 1, 0; ... 
                   'PRINT_TRIALS', 1, 0; ...
                   'PRINT_STATS', 1, 0; ...
                   'WRITE_CSV', 1, 1; ...
                   'TRIALS', 1, trials; ... %Change for future Runs
                   'STIM_LEN', 1, stimLen; ...
                   'COEF_LEN', 1, coefLen; ...
                   };

    %Special Case these parameters as they sometimes are not
    %included
    if strcmp(march, '') == 0
       [rows, cols] = size(constParams);
       constParams{rows+1, 1} = 'march';
       constParams{rows+1, 2} = 2;
       constParams{rows+1, 3} = march;
    end

    if strcmp(optimizationLvl, '') == 0
       [rows, cols] = size(constParams);
       constParams{rows+1, 1} = optimizationLvl;
       constParams{rows+1, 2} = 0;
       constParams{rows+1, 3} = '';
    end

    atLeastOneParams = {xAxisParamName};

    plotResultSet(conn, constParams, atLeastOneParams, xAxisParam, yAxisResultTypeID, instanceID, compilerID, machineID, stimLenID, ioLenID, 'o-');
    hold all;
    legendInd = legendInd + 1;
end

title(['FIR Filter (' kernelInstTitle ') Execution Time For Different Filter Orders']);
xlabel('IO Block Length (Samples)');
ylabel(['Execution Time for ' num2str(stimLen) ' samples (ms)']);
leg = legend(legendLbls,'Location','northeast');
compilerFlags = optimizationLvl;
if strcmp(march, '') == 0
    compilerFlags = [compilerFlags ' -march=' march];
end

title(leg, ['Error Bars Represent StdDev\newlineCompiler: ' compiler '\newlineCompiler Flags: ' compilerFlags '\newlineDataType: ' strrep(datatype, '_', '\_') ]);
grid on;

%% Make a Plot of Kernels for a Fixed Set of Compiler Flags (-march=native -Ofast) - double
kernelInstanceDescriptions={
    'Naive implementation with blocked input and output, shift register state, no explicit vectorization or unrolling.', ...
    'Circular buffer implementation with blocked input and output, circular buffer state, no explicit vectorization or unrolling.', ...
    'Circular buffer implementation with no modulus, blocked input and output, circular buffer state, no explicit vectorization or unrolling.', ...
    'Circular buffer implementation with blocked input and output, circular buffer state, no explicit vectorization or unrolling. - Reverse Direction', ...
    'Circular buffer implementation with no modulus, blocked input and output, circular buffer state, no explicit vectorization or unrolling. - Reverse Direction', ...
    'Naive implementation with blocked input and output, shift register state, manually unrolled by 2.', ...
    'Naive implementation with blocked input and output, shift register state, manually unrolled by 4.', ...
    'FIR Intel IPP' ...
    };

kernelInstTitles = {'Shift Register', 'Circular Buffer w/ Mod', 'Circular Buffer w/o Mod', 'Circular Buffer w/ Mod - Reverse', 'Circular Buffer w/o Mod - Reverse', 'Shift Register Unroll 2',  'Shift Register Unroll 4', 'Intel IPP'};

%io_lens = [12, 12, 12, 12, 12, 12, 12, 12];
%io_lens = [20, 20, 20, 20, 20, 20, 20, 20];
%io_lens = [28, 28, 28, 28, 28, 28, 28, 28];
%io_lens = [34, 34, 34, 34, 34, 34, 34, 34];
io_lens = [36, 36, 36, 36, 36, 36, 36, 36];

insts = length(kernelInstanceDescriptions);

fig = figure;

for graphNum = 1:insts
    
    kernelInstanceDescription = kernelInstanceDescriptions{graphNum};
    
    kernelInstance = conn.fetch(['SELECT KernelInstanceID FROM KernelInstance WHERE KernelID=' num2str(kernelID) ' AND Description=''' kernelInstanceDescription '''']);
    instanceID = kernelInstance{1};

    xAxisParamName = 'COEF_LEN';
    xAxisParam = getParameterIDs(conn, {xAxisParamName});

    %These are things that we have other options for but are holding constant
    %for the graph
    datatype = 'double';
    ioLen = io_lens(graphNum);

    legendInd = 1;
    legendLbls = {};

    march = 'native';
    optimizationLvl = '-Ofast';

    legendLbls{legendInd} = '';

    %Parameter.Name, 0=No Val|1=Numeric|2=String, ParameterValue.Value
    constParams = {'DATATYPE', 2, datatype; ...
                   '-std=c++11', 0, ''; ... %Begin the Always Parameters
                   'PRINT_TITLE', 1, 0; ... 
                   'PRINT_TRIALS', 1, 0; ...
                   'PRINT_STATS', 1, 0; ...
                   'WRITE_CSV', 1, 1; ...
                   'TRIALS', 1, trials; ... %Change for future Runs
                   'STIM_LEN', 1, stimLen; ...
                   'IO_LEN', 1, ioLen; ...
                   };

    %Special Case these parameters as they sometimes are not
    %included
    if strcmp(march, '') == 0
       [rows, cols] = size(constParams);
       constParams{rows+1, 1} = 'march';
       constParams{rows+1, 2} = 2;
       constParams{rows+1, 3} = march;

        legendLbls{legendInd} = [legendLbls{legendInd} '-march=' march ' '];
    end

    if strcmp(optimizationLvl, '') == 0
       [rows, cols] = size(constParams);
       constParams{rows+1, 1} = optimizationLvl;
       constParams{rows+1, 2} = 0;
       constParams{rows+1, 3} = '';
       legendLbls{legendInd} = [legendLbls{legendInd} optimizationLvl];
    end

    atLeastOneParams = {xAxisParamName};
    
    if graphNum < 8
        plotFormat = 'o-';
    else
        plotFormat = 'o-.';
    end

    plotResultSet(conn, constParams, atLeastOneParams, xAxisParam, yAxisResultTypeID, instanceID, compilerID, machineID, stimLenID, ioLenID, plotFormat);
    hold all;
    legendInd = legendInd + 1;


end

compilerOpt = '';
if strcmp(march, '') == 0
    compilerOpt = [compilerOpt '-march=' march ' '];
end

if strcmp(optimizationLvl, '') == 0
   compilerOpt = [compilerOpt optimizationLvl];
end

title(['Execution Time For Different FIR Kernels']);
xlabel('Filter Order (Coefficients)');
ylabel(['Execution Time for ' num2str(stimLen) ' samples (ms)']);
leg = legend(kernelInstTitles,'Location','northwest');
title(leg, ['Error Bars Represent StdDev\newlineCompiler: ' compiler '\newlineIO Block Length: ' num2str(ioLen) '\newlineDataType: ' strrep(datatype, '_', '\_') '\newlineCompiler Optimization: ' compilerOpt  ]);
grid on;

%% Make a Plot of Kernels for a Fixed Set of Compiler Flags (-march=native -O3) - double
kernelInstanceDescriptions={
    'Naive implementation with blocked input and output, shift register state, no explicit vectorization or unrolling.', ...
    'Circular buffer implementation with blocked input and output, circular buffer state, no explicit vectorization or unrolling.', ...
    'Circular buffer implementation with no modulus, blocked input and output, circular buffer state, no explicit vectorization or unrolling.', ...
    'Circular buffer implementation with blocked input and output, circular buffer state, no explicit vectorization or unrolling. - Reverse Direction', ...
    'Circular buffer implementation with no modulus, blocked input and output, circular buffer state, no explicit vectorization or unrolling. - Reverse Direction', ...
    'Naive implementation with blocked input and output, shift register state, manually unrolled by 2.', ...
    'Naive implementation with blocked input and output, shift register state, manually unrolled by 4.', ...
    'FIR Intel IPP' ...
    };

kernelInstTitles = {'Shift Register', 'Circular Buffer w/ Mod', 'Circular Buffer w/o Mod', 'Circular Buffer w/ Mod - Reverse', 'Circular Buffer w/o Mod - Reverse', 'Shift Register Unroll 2',  'Shift Register Unroll 4', 'Intel IPP'};

%io_lens = [12, 12, 12, 12, 12, 12, 12, 12];
%io_lens = [20, 20, 20, 20, 20, 20, 20, 20];
%io_lens = [28, 28, 28, 28, 28, 28, 28, 28];
%io_lens = [34, 34, 34, 34, 34, 34, 34, 34];
io_lens = [36, 36, 36, 36, 36, 36, 36, 36];

insts = length(kernelInstanceDescriptions);

fig = figure;

for graphNum = 1:insts
    
    kernelInstanceDescription = kernelInstanceDescriptions{graphNum};
    
    kernelInstance = conn.fetch(['SELECT KernelInstanceID FROM KernelInstance WHERE KernelID=' num2str(kernelID) ' AND Description=''' kernelInstanceDescription '''']);
    instanceID = kernelInstance{1};

    xAxisParamName = 'COEF_LEN';
    xAxisParam = getParameterIDs(conn, {xAxisParamName});

    %These are things that we have other options for but are holding constant
    %for the graph
    datatype = 'double';
    ioLen = io_lens(graphNum);

    legendInd = 1;
    legendLbls = {};

    march = 'native';
    optimizationLvl = '-O3';

    legendLbls{legendInd} = '';

    %Parameter.Name, 0=No Val|1=Numeric|2=String, ParameterValue.Value
    constParams = {'DATATYPE', 2, datatype; ...
                   '-std=c++11', 0, ''; ... %Begin the Always Parameters
                   'PRINT_TITLE', 1, 0; ... 
                   'PRINT_TRIALS', 1, 0; ...
                   'PRINT_STATS', 1, 0; ...
                   'WRITE_CSV', 1, 1; ...
                   'TRIALS', 1, trials; ... %Change for future Runs
                   'STIM_LEN', 1, stimLen; ...
                   'IO_LEN', 1, ioLen; ...
                   };

    %Special Case these parameters as they sometimes are not
    %included
    if strcmp(march, '') == 0
       [rows, cols] = size(constParams);
       constParams{rows+1, 1} = 'march';
       constParams{rows+1, 2} = 2;
       constParams{rows+1, 3} = march;

        legendLbls{legendInd} = [legendLbls{legendInd} '-march=' march ' '];
    end

    if strcmp(optimizationLvl, '') == 0
       [rows, cols] = size(constParams);
       constParams{rows+1, 1} = optimizationLvl;
       constParams{rows+1, 2} = 0;
       constParams{rows+1, 3} = '';
       legendLbls{legendInd} = [legendLbls{legendInd} optimizationLvl];
    end

    atLeastOneParams = {xAxisParamName};
    
    if graphNum < 8
        plotFormat = 'o-';
    else
        plotFormat = 'o-.';
    end

    plotResultSet(conn, constParams, atLeastOneParams, xAxisParam, yAxisResultTypeID, instanceID, compilerID, machineID, stimLenID, ioLenID, plotFormat);
    hold all;
    legendInd = legendInd + 1;


end

compilerOpt = '';
if strcmp(march, '') == 0
    compilerOpt = [compilerOpt '-march=' march ' '];
end

if strcmp(optimizationLvl, '') == 0
   compilerOpt = [compilerOpt optimizationLvl];
end

title(['Execution Time For Different FIR Kernels']);
xlabel('Filter Order (Coefficients)');
ylabel(['Execution Time for ' num2str(stimLen) ' samples (ms)']);
leg = legend(kernelInstTitles,'Location','northwest');
title(leg, ['Error Bars Represent StdDev\newlineCompiler: ' compiler '\newlineIO Block Length: ' num2str(ioLen) '\newlineDataType: ' strrep(datatype, '_', '\_') '\newlineCompiler Optimization: ' compilerOpt  ]);
grid on;

%% Make a Plot of Kernels for a Fixed Set of Compiler Flags (-march=native -Ofast) - float
kernelInstanceDescriptions={
    'Naive implementation with blocked input and output, shift register state, no explicit vectorization or unrolling.', ...
    'Circular buffer implementation with blocked input and output, circular buffer state, no explicit vectorization or unrolling.', ...
    'Circular buffer implementation with no modulus, blocked input and output, circular buffer state, no explicit vectorization or unrolling.', ...
    'Circular buffer implementation with blocked input and output, circular buffer state, no explicit vectorization or unrolling. - Reverse Direction', ...
    'Circular buffer implementation with no modulus, blocked input and output, circular buffer state, no explicit vectorization or unrolling. - Reverse Direction', ...
    'Naive implementation with blocked input and output, shift register state, manually unrolled by 2.', ...
    'Naive implementation with blocked input and output, shift register state, manually unrolled by 4.', ...
    'FIR Intel IPP' ...
    };

kernelInstTitles = {'Shift Register', 'Circular Buffer w/ Mod', 'Circular Buffer w/o Mod', 'Circular Buffer w/ Mod - Reverse', 'Circular Buffer w/o Mod - Reverse', 'Shift Register Unroll 2',  'Shift Register Unroll 4', 'Intel IPP'};

%io_lens = [12, 12, 12, 12, 12, 12, 12, 12];
%io_lens = [20, 20, 20, 20, 20, 20, 20, 20];
%io_lens = [28, 28, 28, 28, 28, 28, 28, 28];
%io_lens = [34, 34, 34, 34, 34, 34, 34, 34];
io_lens = [36, 36, 36, 36, 36, 36, 36, 36];

insts = length(kernelInstanceDescriptions);

fig = figure;

for graphNum = 1:insts
    
    kernelInstanceDescription = kernelInstanceDescriptions{graphNum};
    
    kernelInstance = conn.fetch(['SELECT KernelInstanceID FROM KernelInstance WHERE KernelID=' num2str(kernelID) ' AND Description=''' kernelInstanceDescription '''']);
    instanceID = kernelInstance{1};

    xAxisParamName = 'COEF_LEN';
    xAxisParam = getParameterIDs(conn, {xAxisParamName});

    %These are things that we have other options for but are holding constant
    %for the graph
    datatype = 'float';
    ioLen = io_lens(graphNum);

    legendInd = 1;
    legendLbls = {};

    march = 'native';
    optimizationLvl = '-Ofast';

    legendLbls{legendInd} = '';

    %Parameter.Name, 0=No Val|1=Numeric|2=String, ParameterValue.Value
    constParams = {'DATATYPE', 2, datatype; ...
                   '-std=c++11', 0, ''; ... %Begin the Always Parameters
                   'PRINT_TITLE', 1, 0; ... 
                   'PRINT_TRIALS', 1, 0; ...
                   'PRINT_STATS', 1, 0; ...
                   'WRITE_CSV', 1, 1; ...
                   'TRIALS', 1, trials; ... %Change for future Runs
                   'STIM_LEN', 1, stimLen; ...
                   'IO_LEN', 1, ioLen; ...
                   };

    %Special Case these parameters as they sometimes are not
    %included
    if strcmp(march, '') == 0
       [rows, cols] = size(constParams);
       constParams{rows+1, 1} = 'march';
       constParams{rows+1, 2} = 2;
       constParams{rows+1, 3} = march;

        legendLbls{legendInd} = [legendLbls{legendInd} '-march=' march ' '];
    end

    if strcmp(optimizationLvl, '') == 0
       [rows, cols] = size(constParams);
       constParams{rows+1, 1} = optimizationLvl;
       constParams{rows+1, 2} = 0;
       constParams{rows+1, 3} = '';
       legendLbls{legendInd} = [legendLbls{legendInd} optimizationLvl];
    end

    atLeastOneParams = {xAxisParamName};
    
    if graphNum < 8
        plotFormat = 'o-';
    else
        plotFormat = 'o-.';
    end

    plotResultSet(conn, constParams, atLeastOneParams, xAxisParam, yAxisResultTypeID, instanceID, compilerID, machineID, stimLenID, ioLenID, plotFormat);
    hold all;
    legendInd = legendInd + 1;


end

compilerOpt = '';
if strcmp(march, '') == 0
    compilerOpt = [compilerOpt '-march=' march ' '];
end

if strcmp(optimizationLvl, '') == 0
   compilerOpt = [compilerOpt optimizationLvl];
end

title(['Execution Time For Different FIR Kernels']);
xlabel('Filter Order (Coefficients)');
ylabel(['Execution Time for ' num2str(stimLen) ' samples (ms)']);
leg = legend(kernelInstTitles,'Location','northwest');
title(leg, ['Error Bars Represent StdDev\newlineCompiler: ' compiler '\newlineIO Block Length: ' num2str(ioLen) '\newlineDataType: ' strrep(datatype, '_', '\_') '\newlineCompiler Optimization: ' compilerOpt  ]);
grid on;

%% Make a Plot of Kernels for a Fixed Set of Compiler Flags (-march=native -O3) - float
kernelInstanceDescriptions={
    'Naive implementation with blocked input and output, shift register state, no explicit vectorization or unrolling.', ...
    'Circular buffer implementation with blocked input and output, circular buffer state, no explicit vectorization or unrolling.', ...
    'Circular buffer implementation with no modulus, blocked input and output, circular buffer state, no explicit vectorization or unrolling.', ...
    'Circular buffer implementation with blocked input and output, circular buffer state, no explicit vectorization or unrolling. - Reverse Direction', ...
    'Circular buffer implementation with no modulus, blocked input and output, circular buffer state, no explicit vectorization or unrolling. - Reverse Direction', ...
    'Naive implementation with blocked input and output, shift register state, manually unrolled by 2.', ...
    'Naive implementation with blocked input and output, shift register state, manually unrolled by 4.', ...
    'FIR Intel IPP' ...
    };

kernelInstTitles = {'Shift Register', 'Circular Buffer w/ Mod', 'Circular Buffer w/o Mod', 'Circular Buffer w/ Mod - Reverse', 'Circular Buffer w/o Mod - Reverse', 'Shift Register Unroll 2',  'Shift Register Unroll 4', 'Intel IPP'};

%io_lens = [12, 12, 12, 12, 12, 12, 12, 12];
%io_lens = [20, 20, 20, 20, 20, 20, 20, 20];
%io_lens = [28, 28, 28, 28, 28, 28, 28, 28];
%io_lens = [34, 34, 34, 34, 34, 34, 34, 34];
io_lens = [36, 36, 36, 36, 36, 36, 36, 36];

insts = length(kernelInstanceDescriptions);

fig = figure;

for graphNum = 1:insts
    
    kernelInstanceDescription = kernelInstanceDescriptions{graphNum};
    
    kernelInstance = conn.fetch(['SELECT KernelInstanceID FROM KernelInstance WHERE KernelID=' num2str(kernelID) ' AND Description=''' kernelInstanceDescription '''']);
    instanceID = kernelInstance{1};

    xAxisParamName = 'COEF_LEN';
    xAxisParam = getParameterIDs(conn, {xAxisParamName});

    %These are things that we have other options for but are holding constant
    %for the graph
    datatype = 'float';
    ioLen = io_lens(graphNum);

    legendInd = 1;
    legendLbls = {};

    march = 'native';
    optimizationLvl = '-O3';

    legendLbls{legendInd} = '';

    %Parameter.Name, 0=No Val|1=Numeric|2=String, ParameterValue.Value
    constParams = {'DATATYPE', 2, datatype; ...
                   '-std=c++11', 0, ''; ... %Begin the Always Parameters
                   'PRINT_TITLE', 1, 0; ... 
                   'PRINT_TRIALS', 1, 0; ...
                   'PRINT_STATS', 1, 0; ...
                   'WRITE_CSV', 1, 1; ...
                   'TRIALS', 1, trials; ... %Change for future Runs
                   'STIM_LEN', 1, stimLen; ...
                   'IO_LEN', 1, ioLen; ...
                   };

    %Special Case these parameters as they sometimes are not
    %included
    if strcmp(march, '') == 0
       [rows, cols] = size(constParams);
       constParams{rows+1, 1} = 'march';
       constParams{rows+1, 2} = 2;
       constParams{rows+1, 3} = march;

        legendLbls{legendInd} = [legendLbls{legendInd} '-march=' march ' '];
    end

    if strcmp(optimizationLvl, '') == 0
       [rows, cols] = size(constParams);
       constParams{rows+1, 1} = optimizationLvl;
       constParams{rows+1, 2} = 0;
       constParams{rows+1, 3} = '';
       legendLbls{legendInd} = [legendLbls{legendInd} optimizationLvl];
    end

    atLeastOneParams = {xAxisParamName};
    
    if graphNum < 8
        plotFormat = 'o-';
    else
        plotFormat = 'o-.';
    end

    plotResultSet(conn, constParams, atLeastOneParams, xAxisParam, yAxisResultTypeID, instanceID, compilerID, machineID, stimLenID, ioLenID, plotFormat);
    hold all;
    legendInd = legendInd + 1;


end

compilerOpt = '';
if strcmp(march, '') == 0
    compilerOpt = [compilerOpt '-march=' march ' '];
end

if strcmp(optimizationLvl, '') == 0
   compilerOpt = [compilerOpt optimizationLvl];
end

title(['Execution Time For Different FIR Kernels']);
xlabel('Filter Order (Coefficients)');
ylabel(['Execution Time for ' num2str(stimLen) ' samples (ms)']);
leg = legend(kernelInstTitles,'Location','northwest');
title(leg, ['Error Bars Represent StdDev\newlineCompiler: ' compiler '\newlineIO Block Length: ' num2str(ioLen) '\newlineDataType: ' strrep(datatype, '_', '\_') '\newlineCompiler Optimization: ' compilerOpt  ]);
grid on;

%% Make a Plot of Kernels for a Fixed Set of Compiler Flags (-march=native -Ofast) - int16_t
kernelInstanceDescriptions={
    'Naive implementation with blocked input and output, shift register state, no explicit vectorization or unrolling.', ...
    'Circular buffer implementation with blocked input and output, circular buffer state, no explicit vectorization or unrolling.', ...
    'Circular buffer implementation with no modulus, blocked input and output, circular buffer state, no explicit vectorization or unrolling.', ...
    'Circular buffer implementation with blocked input and output, circular buffer state, no explicit vectorization or unrolling. - Reverse Direction', ...
    'Circular buffer implementation with no modulus, blocked input and output, circular buffer state, no explicit vectorization or unrolling. - Reverse Direction', ...
    'Naive implementation with blocked input and output, shift register state, manually unrolled by 2.', ...
    'Naive implementation with blocked input and output, shift register state, manually unrolled by 4.' ...
    };

kernelInstTitles = {'Shift Register', 'Circular Buffer w/ Mod', 'Circular Buffer w/o Mod', 'Circular Buffer w/ Mod - Reverse', 'Circular Buffer w/o Mod - Reverse', 'Shift Register Unroll 2',  'Shift Register Unroll 4'};

%io_lens = [12, 12, 12, 12, 12, 12, 12];
%io_lens = [20, 20, 20, 20, 20, 20, 20];
%io_lens = [28, 28, 28, 28, 28, 28, 28];
%io_lens = [34, 34, 34, 34, 34, 34, 34];
io_lens = [36, 36, 36, 36, 36, 36, 36];

insts = length(kernelInstanceDescriptions);

fig = figure;

for graphNum = 1:insts
    
    kernelInstanceDescription = kernelInstanceDescriptions{graphNum};
    
    kernelInstance = conn.fetch(['SELECT KernelInstanceID FROM KernelInstance WHERE KernelID=' num2str(kernelID) ' AND Description=''' kernelInstanceDescription '''']);
    instanceID = kernelInstance{1};

    xAxisParamName = 'COEF_LEN';
    xAxisParam = getParameterIDs(conn, {xAxisParamName});

    %These are things that we have other options for but are holding constant
    %for the graph
    datatype = 'int16_t';
    ioLen = io_lens(graphNum);

    legendInd = 1;
    legendLbls = {};

    march = 'native';
    optimizationLvl = '-Ofast';

    legendLbls{legendInd} = '';

    %Parameter.Name, 0=No Val|1=Numeric|2=String, ParameterValue.Value
    constParams = {'DATATYPE', 2, datatype; ...
                   '-std=c++11', 0, ''; ... %Begin the Always Parameters
                   'PRINT_TITLE', 1, 0; ... 
                   'PRINT_TRIALS', 1, 0; ...
                   'PRINT_STATS', 1, 0; ...
                   'WRITE_CSV', 1, 1; ...
                   'TRIALS', 1, trials; ... %Change for future Runs
                   'STIM_LEN', 1, stimLen; ...
                   'IO_LEN', 1, ioLen; ...
                   };

    %Special Case these parameters as they sometimes are not
    %included
    if strcmp(march, '') == 0
       [rows, cols] = size(constParams);
       constParams{rows+1, 1} = 'march';
       constParams{rows+1, 2} = 2;
       constParams{rows+1, 3} = march;

        legendLbls{legendInd} = [legendLbls{legendInd} '-march=' march ' '];
    end

    if strcmp(optimizationLvl, '') == 0
       [rows, cols] = size(constParams);
       constParams{rows+1, 1} = optimizationLvl;
       constParams{rows+1, 2} = 0;
       constParams{rows+1, 3} = '';
       legendLbls{legendInd} = [legendLbls{legendInd} optimizationLvl];
    end

    atLeastOneParams = {xAxisParamName};

    if graphNum < 8
        plotFormat = 'o-';
    else
        plotFormat = 'o-.';
    end
    
    plotResultSet(conn, constParams, atLeastOneParams, xAxisParam, yAxisResultTypeID, instanceID, compilerID, machineID, stimLenID, ioLenID, plotFormat);
    hold all;
    legendInd = legendInd + 1;


end

compilerOpt = '';
if strcmp(march, '') == 0
    compilerOpt = [compilerOpt '-march=' march ' '];
end

if strcmp(optimizationLvl, '') == 0
   compilerOpt = [compilerOpt optimizationLvl];
end

title(['Execution Time For Different FIR Kernels']);
xlabel('Filter Order (Coefficients)');
ylabel(['Execution Time for ' num2str(stimLen) ' samples (ms)']);
leg = legend(kernelInstTitles,'Location','northwest');
title(leg, ['Error Bars Represent StdDev\newlineCompiler: ' compiler '\newlineIO Block Length: ' num2str(ioLen) '\newlineDataType: ' strrep(datatype, '_', '\_') '\newlineCompiler Optimization: ' compilerOpt  ]);
grid on;

%% Make a Plot of Kernels for a Fixed Set of Compiler Flags (-march=native -O3) - int16_t
kernelInstanceDescriptions={
    'Naive implementation with blocked input and output, shift register state, no explicit vectorization or unrolling.', ...
    'Circular buffer implementation with blocked input and output, circular buffer state, no explicit vectorization or unrolling.', ...
    'Circular buffer implementation with no modulus, blocked input and output, circular buffer state, no explicit vectorization or unrolling.', ...
    'Circular buffer implementation with blocked input and output, circular buffer state, no explicit vectorization or unrolling. - Reverse Direction', ...
    'Circular buffer implementation with no modulus, blocked input and output, circular buffer state, no explicit vectorization or unrolling. - Reverse Direction', ...
    'Naive implementation with blocked input and output, shift register state, manually unrolled by 2.', ...
    'Naive implementation with blocked input and output, shift register state, manually unrolled by 4.' ...
    };

kernelInstTitles = {'Shift Register', 'Circular Buffer w/ Mod', 'Circular Buffer w/o Mod', 'Circular Buffer w/ Mod - Reverse', 'Circular Buffer w/o Mod - Reverse', 'Shift Register Unroll 2',  'Shift Register Unroll 4'};

%io_lens = [12, 12, 12, 12, 12, 12, 12];
%io_lens = [20, 20, 20, 20, 20, 20, 20];
%io_lens = [28, 28, 28, 28, 28, 28, 28];
%io_lens = [34, 34, 34, 34, 34, 34, 34];
io_lens = [36, 36, 36, 36, 36, 36, 36];

insts = length(kernelInstanceDescriptions);

fig = figure;

for graphNum = 1:insts
    
    kernelInstanceDescription = kernelInstanceDescriptions{graphNum};
    
    kernelInstance = conn.fetch(['SELECT KernelInstanceID FROM KernelInstance WHERE KernelID=' num2str(kernelID) ' AND Description=''' kernelInstanceDescription '''']);
    instanceID = kernelInstance{1};

    xAxisParamName = 'COEF_LEN';
    xAxisParam = getParameterIDs(conn, {xAxisParamName});

    %These are things that we have other options for but are holding constant
    %for the graph
    datatype = 'int16_t';
    ioLen = io_lens(graphNum);

    legendInd = 1;
    legendLbls = {};

    march = 'native';
    optimizationLvl = '-O3';

    legendLbls{legendInd} = '';

    %Parameter.Name, 0=No Val|1=Numeric|2=String, ParameterValue.Value
    constParams = {'DATATYPE', 2, datatype; ...
                   '-std=c++11', 0, ''; ... %Begin the Always Parameters
                   'PRINT_TITLE', 1, 0; ... 
                   'PRINT_TRIALS', 1, 0; ...
                   'PRINT_STATS', 1, 0; ...
                   'WRITE_CSV', 1, 1; ...
                   'TRIALS', 1, trials; ... %Change for future Runs
                   'STIM_LEN', 1, stimLen; ...
                   'IO_LEN', 1, ioLen; ...
                   };

    %Special Case these parameters as they sometimes are not
    %included
    if strcmp(march, '') == 0
       [rows, cols] = size(constParams);
       constParams{rows+1, 1} = 'march';
       constParams{rows+1, 2} = 2;
       constParams{rows+1, 3} = march;

        legendLbls{legendInd} = [legendLbls{legendInd} '-march=' march ' '];
    end

    if strcmp(optimizationLvl, '') == 0
       [rows, cols] = size(constParams);
       constParams{rows+1, 1} = optimizationLvl;
       constParams{rows+1, 2} = 0;
       constParams{rows+1, 3} = '';
       legendLbls{legendInd} = [legendLbls{legendInd} optimizationLvl];
    end

    atLeastOneParams = {xAxisParamName};
    
    if graphNum < 8
        plotFormat = 'o-';
    else
        plotFormat = 'o-.';
    end

    plotResultSet(conn, constParams, atLeastOneParams, xAxisParam, yAxisResultTypeID, instanceID, compilerID, machineID, stimLenID, ioLenID, plotFormat);
    hold all;
    legendInd = legendInd + 1;


end

compilerOpt = '';
if strcmp(march, '') == 0
    compilerOpt = [compilerOpt '-march=' march ' '];
end

if strcmp(optimizationLvl, '') == 0
   compilerOpt = [compilerOpt optimizationLvl];
end

title(['Execution Time For Different FIR Kernels']);
xlabel('Filter Order (Coefficients)');
ylabel(['Execution Time for ' num2str(stimLen) ' samples (ms)']);
leg = legend(kernelInstTitles,'Location','northwest');
title(leg, ['Error Bars Represent StdDev\newlineCompiler: ' compiler '\newlineIO Block Length: ' num2str(ioLen) '\newlineDataType: ' strrep(datatype, '_', '\_') '\newlineCompiler Optimization: ' compilerOpt  ]);
grid on;

%% Close
conn.close()
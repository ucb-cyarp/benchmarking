%% Report Results from Benchmark
%This script generates various reports from benchmarking results.  These
%results are contained in an sqlite database.

%% Init
clear; close all; clc;

%% Open DB
conn = sqlite('results_naive.db', 'readonly'); %Do not need to write into the DB

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
kernelInstance = conn.fetch(['SELECT KernelInstanceID FROM KernelInstance WHERE KernelID=' num2str(kernelID) ' AND Description=''Naive implementation with blocked input and output, shift register state, no explicit vectorization or unrolling.''']);
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

%Things that may change from different experement runs but are constant for
%graphs
trials = 10;
stimLen = 100000;
compiler = 'gcc';

compilerX = conn.fetch(['SELECT CompilerID FROM Compiler WHERE Name=''' compiler '''']);
compilerID = compilerX{1};
yAxisResultTypeName = 'Clock - Cycles/Cycle Time (ms)';
yAxisResultTypeID = getResultTypeID(conn, {yAxisResultTypeName});
stimLenName = 'STIM_LEN';
stimLenID = getParameterIDs(conn, {stimLenName});
ioLenName = 'IO_LEN';
ioLenID = getParameterIDs(conn, {ioLenName});


%% Make a Plot of Different Compiler Flags (double)
xAxisParamName = 'COEF_LEN';
xAxisParam = getParameterIDs(conn, {xAxisParamName});

%These are things that we have other options for but are holding constant
%for the graph
datatype = 'double';
ioLen = 1;

legendInd = 1;
legendLbls = {};

fig = figure;

%for marchX = {'', 'core2', 'corei7', 'corei7-avx', 'core-avx-i', 'core-avx2', 'native'}
for marchX = {'native'}
    march = marchX{1};
    for optimizationLvlX = {'', '-O1','-O2', '-O3', '-Os', '-Ofast'}
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

title('Naive FIR Filter Execution Time For Different Compiler Flags');
xlabel('Filter Order (Coefficients)');
ylabel(['Execution Time for ' num2str(stimLen) ' samples (ms)']);
leg = legend(legendLbls,'Location','northwest');
title(leg, ['Error Bars Represent StdDev\newlineIO Block Length: ' num2str(ioLen) '\newlineDataType: ' strrep(datatype,'_','\_') ]);
%ALSO DO A 2D PLOT OF IO_LEN and COEF_LEN

%% Make a Plot of Different Compiler Flags (int16_t)
xAxisParamName = 'COEF_LEN';
xAxisParam = getParameterIDs(conn, {xAxisParamName});

%These are things that we have other options for but are holding constant
%for the graph
datatype = 'int16_t';
ioLen = 1;

legendInd = 1;
legendLbls = {};

fig = figure;

%for marchX = {'', 'core2', 'corei7', 'corei7-avx', 'core-avx-i', 'core-avx2', 'native'}
for marchX = {'native'}
    march = marchX{1};
    for optimizationLvlX = {'', '-O1','-O2', '-O3', '-Os', '-Ofast'}
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

title('Naive FIR Filter Execution Time For Different Compiler Flags');
xlabel('Filter Order (Coefficients)');
ylabel(['Execution Time for ' num2str(stimLen) ' samples (ms)']);
leg = legend(legendLbls,'Location','northwest');
title(leg, ['Error Bars Represent StdDev\newlineIO Block Length: ' num2str(ioLen) '\newlineDataType: ' strrep(datatype,'_','\_') ]);
%ALSO DO A 2D PLOT OF IO_LEN and COEF_LEN

%% Plot Datatype
xAxisParamName = 'COEF_LEN';
xAxisParam = getParameterIDs(conn, {xAxisParamName});

ioLen = 1;
optimizationLvl = '-Ofast';
march = 'native';

legendInd = 1;
legendLbls = {};

fig = figure;

for datatypeX = {'int8_t', 'int16_t','int32_t', 'int64_t', 'float', 'double'}
    datatype=datatypeX{1};

    legendLbls{legendInd} = strrep(datatype,'_','\_');

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

title('Naive FIR Filter Execution Time For Different DataTypes');
xlabel('Filter Order (Coefficients)');
ylabel(['Execution Time for ' num2str(stimLen) ' samples (ms)']);
leg = legend(legendLbls,'Location','northwest');
compilerFlags = optimizationLvl;
if strcmp(march, '') == 0
    compilerFlags = [compilerFlags ' -march=' march];
end

title(leg, ['Error Bars Represent StdDev\newlineIO Block Length: ' num2str(ioLen) '\newlineCompiler Flags: ' compilerFlags ]);
%ALSO DO A 2D PLOT OF IO_LEN and COEF_LEN

%% Plot Blocklength
xAxisParamName = 'IO_LEN';
xAxisParam = getParameterIDs(conn, {xAxisParamName});

datatype = 'double';
optimizationLvl = '-Ofast';
march = 'native';

legendInd = 1;
legendLbls = {};

fig = figure;

for coefLenX = {1, 5, 10, 20, 50}
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

title('Naive FIR Filter Execution Time For Different Filter Orders (Coefficient Length)');
xlabel('IO Block Length (Samples)');
ylabel(['Execution Time for ' num2str(stimLen) ' samples (ms)']);
leg = legend(legendLbls,'Location','northwest');
compilerFlags = optimizationLvl;
if strcmp(march, '') == 0
    compilerFlags = [compilerFlags ' -march=' march];
end

title(leg, ['Error Bars Represent StdDev\newlineCompiler Flags: ' compilerFlags '\newlineDataType: ' datatype ]);
%ALSO DO A 2D PLOT OF IO_LEN and COEF_LEN

%% Close
conn.close()
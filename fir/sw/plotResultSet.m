function handle = plotResultSet(conn, constParams, atLeastOneParams, xAxisParam, yAxisResultTypeID, instanceID, compilerID, machineID, stimLenID, ioLenID, formatStr)
%plotResultSet Plots a result set from the DB
%   This one 
    mustHaveParamValIDs = getParameterValueIDs(conn, constParams);
    atLeastOneOfParams = getParameterIDs(conn, atLeastOneParams);

    runIDs = getRunIDsWithExactParams(conn, mustHaveParamValIDs, atLeastOneOfParams, xAxisParam, instanceID, compilerID, machineID);
    runIDs = cell2mat(runIDs);

    xAxisVal = zeros(length(runIDs), 1);
    avgSampleRate = zeros(length(runIDs), 1);
    stdDevSampleRate = zeros(length(runIDs), 1);

    for i = 1:length(runIDs)
        [xAxisVal(i), avgSampleRate(i), stdDevSampleRate(i)] = runStatistics(conn, runIDs(i), yAxisResultTypeID, xAxisParam, stimLenID, ioLenID, false);
    end

    handle = errorbar(xAxisVal, avgSampleRate, stdDevSampleRate, formatStr);
end


function [xAxisVal, resultAvg, resuktStdDev] = runStatistics(sqlConn, runID, resultTypeID, xAxisParameterID, stimLenParameterID, ioLenParameterID, calculateRate)
%runStatistics Calculate result statistics for a particular run

    %Get the stimulus length for this run
    stimLenX = sqlConn.fetch(['SELECT ParameterValue.ValueNumeric FROM ParameterValue JOIN Config ON ParameterValue.ParameterValueID=Config.ParameterValueID WHERE Config.RunID=' num2str(runID) ' AND ParameterValue.ParameterID=' num2str(stimLenParameterID)]);
    stimLen = stimLenX{1};
    
    %Get the io block length for this run
    ioLenX = sqlConn.fetch(['SELECT ParameterValue.ValueNumeric FROM ParameterValue JOIN Config ON ParameterValue.ParameterValueID=Config.ParameterValueID WHERE Config.RunID=' num2str(runID) ' AND ParameterValue.ParameterID=' num2str(ioLenParameterID)]);
    ioLen = ioLenX{1};
    
    %Calculate the actual stimulus length given that the C++ effectivly
    %performs a floor operation (ie does not handle the last block if it is
    %not the same size as the io block length)
    realStimLen = stimLen - mod(stimLen, ioLen);
    
    %Get x Axis Value
    xAxisValX = sqlConn.fetch(['SELECT ParameterValue.ValueNumeric FROM ParameterValue JOIN Config ON ParameterValue.ParameterValueID=Config.ParameterValueID WHERE Config.RunID=' num2str(runID) ' AND ParameterValue.ParameterID=' num2str(xAxisParameterID)]);
    xAxisVal = xAxisValX{1};
    
    %Get the runs for the trials associated with this run
    results = sqlConn.fetch(['SELECT Result.Value FROM Result JOIN Trial ON Result.TrialID=Trial.TrialID WHERE Result.ResultTypeID=' num2str(resultTypeID) ' AND Trial.RunID=' num2str(runID)]);
    results = cell2mat(results);
    
    if(calculateRate == true)
        %Convert to Data Rate
        %                  samples
        %Stim_len/Result = --------- 
        %                    ms

        %                        samples
        %Stim_len*1000/Result = --------- 
        %                          s

        %Data Rate = Stim_len*1000/Result
        %Data Rate MHz = (Stim_len*1000/Result)/1e6 = (Stim_len/Result)/1e3
        dataRate = ((double(stimLen))./results)./1000;
        
        results = dataRate;
    end
    
    
    resultAvg = mean(results);
    resuktStdDev = std(results);
end


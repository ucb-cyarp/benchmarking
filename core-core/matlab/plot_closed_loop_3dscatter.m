function plot_closed_loop_3dscatter(x, y, TransactionCore_Client, Errored, SteadyClockMs, BlockSizeElements, ElementSizeBytes, filterConditions, filterConditionLbls, titleStr, xlbl, ylbl)
%plot_closed_loop_3dscatter Plots a 3D scatterplot of the number of
%itterations before failure (or finish) with different control periods and
%gains.  Also plots an estimated data rate for the run


%% Plot Iterations before Failure
figure;
for i = 1:length(filterConditions)
    [idx, ~] = find(filterConditions{i});

    if length(filterConditions)>1
        scatter3(x(idx), y(idx), TransactionCore_Client(idx), 75, 'filled', 'MarkerFaceAlpha', 0.2);
        hold all;
    else
        scatter3(x(idx), y(idx), TransactionCore_Client(idx), 75, TransactionCore_Client(idx), 'filled');
    end

end
hold off;
xlabel(xlbl); 
ylabel(ylbl); 
zlabel('Transactions Before Failure');

if length(filterConditions)>1
    legend(filterConditionLbls);
    title(titleStr, 'Interpreter', 'none');
else
    title({titleStr, filterConditionLbls{1}}, 'Interpreter', 'none')
end

axis vis3d;

%% Plot Data Rate of Runs
%Set the data rate of runs that failed to 0
dataRateGbps = zeros(size(TransactionCore_Client));

%Calculate the data rate of runs that did not fail
[succeedingRunsIdxs, ~] = find(Errored == false);
maxRateGbps = -1;
maxRateGbpsIdx = 0;
for i = 1:length(succeedingRunsIdxs)
    succeedingRunsIdx = succeedingRunsIdxs(i);
    bytesTransfered = BlockSizeElements(succeedingRunsIdx)*ElementSizeBytes(succeedingRunsIdx)*TransactionCore_Client(succeedingRunsIdx); %Transactions are in blocks transfered for open and closed loop tests
    bitsTransfered = bytesTransfered*8;
    gbps = bitsTransfered/(SteadyClockMs(succeedingRunsIdx)*1e6); %Time is already in ms
    dataRateGbps(succeedingRunsIdx) = gbps;
    if gbps > maxRateGbps
        maxRateGbps = gbps;
        maxRateGbpsIdx = succeedingRunsIdx;
    end   
end

%Plot
fig1 = figure;
for i = 1:length(filterConditions)
    [idx, ~] = find(filterConditions{i});

    if length(filterConditions)>1
        s = scatter3(x(idx), y(idx), dataRateGbps(idx), 75, 'filled', 'MarkerFaceAlpha', 0.2);
        hold all;
    else
        s = scatter3(x(idx), y(idx), dataRateGbps(idx), 75, dataRateGbps(idx), 'filled');
    end

end
hold off;
xlabel(xlbl); 
ylabel(ylbl); 
zlabel('Data Rate (Gbps)');

if length(filterConditions)>1
    legend(filterConditionLbls);
    title(titleStr, 'Interpreter', 'none');
else
    title({titleStr, filterConditionLbls{1}}, 'Interpreter', 'none')
end

%Put a datatip on the max
datatip(s, 'DataIndex', maxRateGbpsIdx);

axis vis3d;

%% plot the average rate (for configs that did not error)

figure;
for f = 1:length(filterConditions)
    [idxs, ~] = find(filterConditions{f});

    erroredMapX = containers.Map('KeyType','double','ValueType','Any');
    durationSumMapX = containers.Map('KeyType','double','ValueType','Any');
    bitsTransferedSumMapX = containers.Map('KeyType','double','ValueType','Any');
    countMapX = containers.Map('KeyType','double','ValueType','Any');

    for i = 1:length(idxs)
        idx = idxs(i);
        xVal = x(idx);
        yVal = y(idx);

        if ~isKey(erroredMapX, xVal)
            erroredMapY = containers.Map('KeyType','double', 'ValueType', 'logical');
            erroredMapX(xVal) = erroredMapY;
            durationSumMapY = containers.Map('KeyType','double', 'ValueType', 'double');
            durationSumMapX(xVal) = durationSumMapY;
            bitsTransferedSumMapY = containers.Map('KeyType','double', 'ValueType', 'double');
            bitsTransferedSumMapX(xVal) = bitsTransferedSumMapY;
            countMapY = containers.Map('KeyType','double', 'ValueType', 'double');
            countMapX(xVal) = countMapY;
        else
            erroredMapY = erroredMapX(xVal);
            durationSumMapY = durationSumMapX(xVal);
            bitsTransferedSumMapY = bitsTransferedSumMapX(xVal);
            countMapY = countMapX(xVal);
        end

        ptErrored = Errored(idx);
        bytesTransfered = BlockSizeElements(idx)*ElementSizeBytes(idx)*TransactionCore_Client(idx); %Transactions are in blocks transfered for open and closed loop tests
        bitsTransfered = bytesTransfered*8;

        if ~isKey(erroredMapY, yVal)
            erroredMapY(yVal) = ptErrored;
            durationSumMapY(yVal) = SteadyClockMs(idx);
            bitsTransferedSumMapY(yVal) = bitsTransfered;
            countMapY(yVal) = 1;
        else
            ptErrored = ptErrored || erroredMapY(yVal);
            erroredMapY(yVal) = ptErrored;
            durationSumMapY(yVal) = durationSumMapY(yVal) + SteadyClockMs(idx);
            bitsTransferedSumMapY(yVal) = bitsTransferedSumMapY(yVal) + bitsTransfered;
            countMapY(yVal) = countMapY(yVal) + 1;
        end
    end

    avgX = [];
    avgY = [];
    avgDuration = [];
    avgBitsTransfered = [];
    avgErrored = [];
    avgRateGbps = [];

    xKeys = keys(erroredMapX);
    for i = 1:length(xKeys)
        xKey = xKeys{i};
        erroredMapY = erroredMapX(xKey);
        durationSumMapY = durationSumMapX(xKey);
        bitsTransferedSumMapY = bitsTransferedSumMapX(xKey);
        countMapY = countMapX(xKey);

        yKeys = keys(erroredMapY);
        for j = 1:length(yKeys)
            yKey = yKeys{j};
            erroredPt = erroredMapY(yKey);
            durationSum = durationSumMapY(yKey);
            bitsTransfered = bitsTransferedSumMapY(yKey);
            count = countMapY(yKey);

            avgX = [avgX, xKey];
            avgY = [avgY, yKey];
            avgDurationPt = durationSum/count;
            avgDuration = [avgDuration, avgDurationPt];
            avgBitsTransferedPt = bitsTransfered/count;
            avgBitsTransfered = [avgBitsTransfered, avgBitsTransferedPt];
            avgErrored = [avgErrored, erroredPt];
            if erroredPt
                avgRateGbps = [avgRateGbps, 0];
            else
                gbpsPt = avgBitsTransferedPt/(avgDurationPt*1e6);
                avgRateGbps = [avgRateGbps, gbpsPt];
            end
        end
    end
    
    [~, maxDataRateIdx] = max(avgRateGbps);
    
    if length(filterConditions)>1
        s = scatter3(avgX, avgY, avgRateGbps, 75, 'filled', 'MarkerFaceAlpha', 0.2);
        hold all;
    else
        s = scatter3(avgX, avgY, avgRateGbps, 75, avgRateGbps, 'filled');
    end
    
    if ~isempty(avgRateGbps)
        datatip(s, 'DataIndex', maxDataRateIdx);
    end
    
    fprintf('    Average Data Rate (Gbps) - Plotted: %10.6f\n', avgRateGbps(maxDataRateIdx));
    
end

hold off;
xlabel(xlbl); 
ylabel(ylbl); 
zlabel('Avg Data Rate (Gbps)');

if length(filterConditions)>1
    legend(filterConditionLbls);
    title(titleStr, 'Interpreter', 'none');
else
    title({titleStr, filterConditionLbls{1}}, 'Interpreter', 'none')
end

axis vis3d;

end

    


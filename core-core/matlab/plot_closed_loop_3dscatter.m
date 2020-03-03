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
for i = 1:length(succeedingRunsIdxs)
    succeedingRunsIdx = succeedingRunsIdxs(i);
    bytesTransfered = BlockSizeElements(succeedingRunsIdx)*ElementSizeBytes(succeedingRunsIdx)*TransactionCore_Client(succeedingRunsIdx); %Transactions are in blocks transfered for open and closed loop tests
    bitsTransfered = bytesTransfered*8;
    gbps = bitsTransfered/(SteadyClockMs(succeedingRunsIdx)*1e6); %Time is already in ms
    dataRateGbps(succeedingRunsIdx) = gbps;
end

%Plot
figure;
for i = 1:length(filterConditions)
    [idx, ~] = find(filterConditions{i});

    if length(filterConditions)>1
        scatter3(x(idx), y(idx), dataRateGbps(idx), 75, 'filled', 'MarkerFaceAlpha', 0.2);
        hold all;
    else
        scatter3(x(idx), y(idx), dataRateGbps(idx), 75, dataRateGbps(idx), 'filled');
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

axis vis3d;


end


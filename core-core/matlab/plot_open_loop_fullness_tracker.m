%Plots the tracker vectors for an open loop fullness tracker run

%Plot & Filter Parameters
ArrayLength = 255;
BlockSize = 32;
InitNOPs = 0;
BalNOPs = 46;
PlotThreshold = 0;
PlotColors = true;

%Import Data
% %[ArrayLengthBlocks, BlockSizeint32_tElements, BalancingNOPs, InitialNOPs, CheckPeriod, SteadyClockWalltimems, ExpectedBlockIDCore_Client, StartBlockIDCore_Client, EndBlockIDCore_Client, ErroredCore_Client, TransactionCore_Client, FullnessStartCore_Client, FullnessEndCore_Client] = import_open_loop_fullness_tracker_raw_file('report_open_loop_fullness_tracker_raw');
% [ArrayLengthBlocks, BlockSizeint32_tElements, BalancingNOPs, InitialNOPs, CheckPeriod, SteadyClockWalltimems, ClockCyclesCycleTimems, Clockrdtsc, ExpectedBlockIDCore_Server, StartBlockIDCore_Server, EndBlockIDCore_Server, ErroredCore_Server, ErrorSourceCore_Server, TransactionCore_Server, ExpectedBlockIDCore_Client, StartBlockIDCore_Client, EndBlockIDCore_Client, ErroredCore_Client, ErrorSourceCore_Client, TransactionCore_Client, FullnessStartCore_Client, FullnessEndCore_Client] = import_open_loop_fullness_v2('report_open_loop_fullness_tracker_raw.csv');

%Filter Data
idxs = find( (ArrayLengthBlocks == ArrayLength) & (BlockSizeint32_tElements == BlockSize) & (InitialNOPs == InitNOPs) & (BalancingNOPs == BalNOPs) & (TransactionCore_Client >= PlotThreshold));
filterCond = ['(Array Length (Blocks): ' num2str(ArrayLength) ', Block Size (32b Words): ' num2str(BlockSize) ', Initial NOPs: ' num2str(InitNOPs) ', Balancing NOPs:: ' num2str(BalNOPs) ', Plot Threshold: ' num2str(PlotThreshold)];

%Get a color map

colors = cool(256);

%Get the min/max range of the transactions
minTransact = min(TransactionCore_Client(idxs));
maxTransact = max(TransactionCore_Client(idxs));

%Map from [minTransact, maxTransact] to color[1:256]

%Other params

%Plot the start tracker
figure
for i = 1:length(idxs)
    idx = idxs(i);
    startTracker = importTrackerVector(FullnessStartCore_Client(idx));
    if PlotColors
        colorInd = floor((TransactionCore_Client(idx)-minTransact)*255/(maxTransact-minTransact)+1);
        plot(startTracker, 'Color', [colors(colorInd, :), 0.2]);
    else
        plot(startTracker);
    end
    hold on;
    %Plot a marker for the last point
    scatter(length(startTracker), startTracker(length(startTracker)), 'or');
end
ylim([0, ArrayLength]);
ylabel('Items in Buffer');
title({'Start Trace', filterCond});
if PlotColors
    cb = colorbar;
    colormap('cool');
    caxis([minTransact, maxTransact])
    ylabel(cb, 'Transactions Before Failure')
end

%Plot the end tracker.  Only plot entries that did not fail early (ie. were
%not entireley captured in the start
figure
for i = 1:length(idxs)
    idx = idxs(i);
    endTrackerUntrimmed = str2num(FullnessEndCore_Client(idx));
    endTracker = importTrackerVector(FullnessEndCore_Client(idx));
    if(length(endTrackerUntrimmed) == length(endTracker))
        if PlotColors
            colorInd = floor((TransactionCore_Client(idx)-minTransact)*255/(maxTransact-minTransact)+1);
            plot(endTracker, 'Color', [colors(colorInd, :), 0.2]);
        else
            plot(endTracker);
        end
        hold on;
        %Plot a marker for the last point
        %Check if it ended because it reached the itteration limit or due
        %to a failure
        if ErroredCore_Client(idx) == 'Yes'
            scatter(length(endTracker), endTracker(length(endTracker)), 'or');
        else
            scatter(length(endTracker), endTracker(length(endTracker)), '*g');
        end

    end
end
ylim([0, ArrayLength]);
ylabel('Items in Buffer');
title({'End Trace', filterCond});
if PlotColors
    cb = colorbar;
    colormap('cool');
    caxis([minTransact, maxTransact])
    ylabel(cb, 'Transactions Before Failure')
end
%Plots the tracker vectors for an open loop fullness tracker run

%Plot & Filter Parameters
ArrayLength = 255;
BlockSize = 32;
InitNOPs = 0;
BalNOPs = 32;
PlotThreshold = 0;
PlotColors = false;

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
    endTrackerServer = importTrackerVector(InterruptsEndCore_Server(idx));
    endTrackerClient = importTrackerVector(InterruptsEndCore_Client(idx));
    if PlotColors
        colorInd = floor((TransactionCore_Client(idx)-minTransact)*255/(maxTransact-minTransact)+1);
        plot(endTrackerServer, 'Color', [colors(colorInd, :), 0.2]);
        plot(endTrackerClient, '--', 'Color', [colors(colorInd, :), 0.2]);
    else
        plot(endTrackerServer);
        plot(endTrackerClient, '--');
    end
    hold on;
end
ylabel('Interrupts');
title({'End Trace', filterCond});
if PlotColors
    cb = colorbar;
    colormap('cool');
    caxis([minTransact, maxTransact])
    ylabel(cb, 'Transactions Before Failure')
end
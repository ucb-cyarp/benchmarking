%Plots the tracker vectors for an open loop fullness tracker run

%Plot & Filter Parameters
ArrayLength = 255;
BlockSize = 32;
InitNOPs = 0;
BalNOPs = 32;
PlotThreshold = 0;

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
for i = 1:length(idxs)
    idx = idxs(i);
    endTrackerServer = importTrackerVector(InterruptsEndCore_Server(idx));
    endTrackerClient = importTrackerVector(InterruptsEndCore_Client(idx));
    if ~isempty(find(endTrackerServer, 1)) || ~isempty(find(endTrackerClient, 1))
        figure;
        ax1 = subplot(3, 1, 1);
        endTracker = importTrackerVector(FullnessEndCore_Client(idx));
        plot(ax1, endTracker);
        ylabel('Items in Buffer');
        title({'End Trace', filterCond, ['Transactions: ' num2str(TransactionCore_Client(idx))]});
        
        ax2 = subplot(3, 1, 2);
        plot(ax2, endTrackerServer)
        hold on;
        plot(ax2, endTrackerClient)
        hold off;
        legend('Server', 'Client')
        ylabel('Interrupts')
        
        ax3 = subplot(3, 1, 3);
        endTimingServer = importTrackerVector(TimingEndCore_Server(idx));
        endTimingClient = importTrackerVector(TimingEndCore_Client(idx));
        plot(ax3, endTimingServer)
        hold on;
        plot(ax3, endTimingClient)
        hold off;
        legend('Server', 'Client')
        ylabel('Iteration Period')
        
        linkaxes([ax1, ax2, ax3], 'x')
    end
end
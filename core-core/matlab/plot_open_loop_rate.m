%Plots the the data transfer rate for open loop benchmarks.  This is
%plotted against the number of itterations before failure to avoid
%erronious speed reporting

%Plot & Filter Parameters
ArrayLength = 255;
BlockSize = 130;
InitNOPs = 0;
BalNOPs = 414;

%Change this if element size changes (variable name should also change)
ElementSizeBytes = 4;

%Filter Data
idxs = find( (ArrayLengthBlocks == ArrayLength) & (BlockSizeint32_tElements == BlockSize) & (InitialNOPs == InitNOPs) & (BalancingNOPs == BalNOPs));
filterCond = ['(Array Length (Blocks): ' num2str(ArrayLength) ', Block Size (32b Words): ' num2str(BlockSize) ', Initial NOPs: ' num2str(InitNOPs) ', Balancing NOPs:: ' num2str(BalNOPs)];


%% Plot Data Rate of Runs
bytesTransfered = BlockSizeint32_tElements(idxs).*ElementSizeBytes.*TransactionCore_Client(idxs); %Transactions are in blocks transfered for open and closed loop tests
bitsTransfered = bytesTransfered*8;
dataRateGbps = bitsTransfered./(SteadyClockWalltimems(idxs)*1e6); %Time is already in ms

figure;
s = scatter(TransactionCore_Client(idxs), dataRateGbps, 30, [0 0.4470 0.7410], 'filled');
s.MarkerFaceAlpha = 0.2;

hold off;
xlabel('Transactions Before Failure'); 
ylabel('Data Rate (Gbps)');

%Specify if this is with or without tracking
if exist('startTracker')
    title1 = 'With Tracking';
else
    title1 = 'Without Tracking';
end

title({title1, filterCond}, 'Interpreter', 'none')

% Display Average of Plotted Points (Inverse of Avg Duration)
sumBitsTransfered = sum(bitsTransfered);
sumTimeMs = sum(SteadyClockWalltimems(idxs));
avgDataRateGbps = sumBitsTransfered/(sumTimeMs*1e6); %Time is already in ms

fprintf('Average Rate: %10.6f\n', avgDataRateGbps);
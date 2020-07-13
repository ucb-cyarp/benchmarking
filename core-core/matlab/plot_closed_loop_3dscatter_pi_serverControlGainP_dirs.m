function plot_closed_loop_3dscatter_pi_serverControlGainP_dirs(directories, ClientControlPeriodIterationsFilter, InitialNOPsFilter, GainIFilter, GainBaseFilter, BlockSize, satValue, plotZeroGain)
%plot_closed_loop_3dscatter_dirs Summary of this function goes here
%   Detailed explanation goes here
if nargin<8
    plotZeroGain = false;
end

if nargin<7
    satValue = -1;
end

if nargin<6
    BlockSize = -1;
end

satValStr = '';
if(satValue>=0)
    satValStr = [', Saturated: ' num2str(satValue)];
end

blockSizeStr = ', Block Size: Multiple';
if(BlockSize>=0)
    blockSizeStr = [' Block Size: ' num2str(BlockSize)];
end


for i = 1:length(directories)
    disp([directories{i} ':'])
    dirPath = ['./' directories{i} '/report_closed_loop_pi_control_raw.csv'];
    %[ArrayLengthBlocks, BlockSizeint32_tElements, ServerControlPeriodIterations, ClientControlPeriodIterations, ControlGainPNOPs, ControlGainINOPs, InitialNOPs, SteadyClockWalltimems, ~, ~, ~, ~, ~, ErroredStr, ~, TransactionCore_Client] = import_closed_loop_pi_raw_file(dirPath, [2, inf]);
    [ArrayLengthBlocks, BlockSizeint32_tElements, ServerControlPeriodIterations, ClientControlPeriodIterations, ControlGainPNOPs, ControlGainINOPs, ControlGainBaseNOPs, InitialNOPs, SteadyClockWalltimems, ~, ~, ~, ~, ~, ~, ~, ~, ErroredStr, ~, TransactionCore_Client] = import_closed_loop_pi_base_adj_raw_file(dirPath, [2, inf]);
    TransactionCore_Client_Stat = TransactionCore_Client;
    
    %The element size does not change
    BlockSizeElements = BlockSizeint32_tElements;
    ElementSizeBytes = 4.*ones(size(BlockSizeElements));
    
    %Convert Errored to Bool
    Errored = true(size(ErroredStr));
    for n = 1:length(ErroredStr)
        if ErroredStr(n) == 'No' || ErroredStr(n) == 'no' || ErroredStr(n) == 'False' || ErroredStr(n) == 'false' || ErroredStr(n) == '0'
            Errored(n) = false;
        end
    end
    
    if(satValue>0)
        TransactionCore_Client_Stat(TransactionCore_Client>satValue) = satValue;
    end
    
    filterArr = (ClientControlPeriodIterations==ClientControlPeriodIterationsFilter) & (InitialNOPs == InitialNOPsFilter) & (ControlGainINOPs == GainIFilter);
    filterArrStr = ['(ClientControlPeriodIterations==', num2str(ClientControlPeriodIterationsFilter) ') && (InitialNOPs==' num2str(InitialNOPsFilter) ') && (ControlGainINOPs == ' num2str(GainIFilter) ')'];
    
    if exist('ControlGainBaseNOPs')
        filterArr = filterArr & (ControlGainBaseNOPs == GainBaseFilter);
        filterArrStr = [filterArrStr ' && (ControlGainBaseNOPs==', num2str(GainBaseFilter) ')'];
    end
    
    if ~plotZeroGain
        filterArr = filterArr & (ControlGainPNOPs ~= 0);
        filterArrStr = [filterArrStr ', Omitted Gain 0'];
    end
    
    if(BlockSize > 0)
        filterArr = filterArr & (BlockSizeint32_tElements == BlockSize);
    end 
    
    filterArrStr = [filterArrStr blockSizeStr satValStr];
    
    plot_closed_loop_3dscatter(ServerControlPeriodIterations, ControlGainPNOPs, TransactionCore_Client_Stat, Errored, SteadyClockWalltimems, BlockSizeElements, ElementSizeBytes, { filterArr }, {filterArrStr}, directories{i}, 'Server Control Period (Transfers)', 'Control Gain P (NOPs / Block Offset)');
        
    %Also Report which configuration had the max itteration count
    [~, maxIndex] = max(TransactionCore_Client);
    fprintf('    Max Overall : TransactionCore_Client: %10d, ServerControlPeriodIterations: %5d, ClientControlPeriodIterations: %5d, ControlGainPNOPs: %6.3f, ControlGainINOPs: %6.3f, InitialNOPs: %5d, BlockSizeint32_tElements: %5d, ArrayLengthBlocks: %5d\n', TransactionCore_Client(maxIndex), ServerControlPeriodIterations(maxIndex), ClientControlPeriodIterations(maxIndex), ControlGainPNOPs(maxIndex), ControlGainINOPs(maxIndex), InitialNOPs(maxIndex), BlockSizeint32_tElements(maxIndex), ArrayLengthBlocks(maxIndex));
    
    [posGainIdxs, ~] = find(ControlGainPNOPs ~= 0);
    [~, maxPosGainSubIndex] = max(TransactionCore_Client(posGainIdxs));
    maxPosGainIndex = posGainIdxs(maxPosGainSubIndex);
    fprintf('    Max Gain>0  : TransactionCore_Client: %10d, ServerControlPeriodIterations: %5d, ClientControlPeriodIterations: %5d, ControlGainPNOPs: %6.3f, ControlGainINOPs: %6.3f, InitialNOPs: %5d, BlockSizeint32_tElements: %5d, ArrayLengthBlocks: %5d\n', TransactionCore_Client(maxPosGainIndex), ServerControlPeriodIterations(maxPosGainIndex), ClientControlPeriodIterations(maxPosGainIndex), ControlGainPNOPs(maxPosGainIndex), ControlGainINOPs(maxIndex), InitialNOPs(maxPosGainIndex), BlockSizeint32_tElements(maxIndex), ArrayLengthBlocks(maxIndex))
    
    [filteredIdxs, ~] = find(filterArr);
    [~, maxFilteredSubIdxs] = max(TransactionCore_Client(filteredIdxs));
    maxFilteredIdxs = filteredIdxs(maxFilteredSubIdxs);
    fprintf('    Max Filtered: TransactionCore_Client: %10d, ServerControlPeriodIterations: %5d, ClientControlPeriodIterations: %5d, ControlGainPNOPs: %6.3f, ControlGainINOPs: %6.3f, InitialNOPs: %5d, BlockSizeint32_tElements: %5d, ArrayLengthBlocks: %5d\n', TransactionCore_Client(maxFilteredIdxs), ServerControlPeriodIterations(maxFilteredIdxs), ClientControlPeriodIterations(maxFilteredIdxs), ControlGainPNOPs(maxFilteredIdxs), ControlGainINOPs(maxIndex), InitialNOPs(maxFilteredIdxs), BlockSizeint32_tElements(maxIndex), ArrayLengthBlocks(maxIndex))
    
    dataRateGbps = zeros(size(TransactionCore_Client));

    %Calculate the data rate of runs that did not fail
    [succeedingRunsIdxs, ~] = find(Errored == false);
    maxRateGbps = -1;
    maxRateGbpsIdx = 0;
    for i = 1:length(succeedingRunsIdxs)
        succeedingRunsIdx = succeedingRunsIdxs(i);
        bytesTransfered = BlockSizeElements(succeedingRunsIdx)*ElementSizeBytes(succeedingRunsIdx)*TransactionCore_Client(succeedingRunsIdx); %Transactions are in blocks transfered for open and closed loop tests
        bitsTransfered = bytesTransfered*8;
        gbps = bitsTransfered/(SteadyClockWalltimems(succeedingRunsIdx)*1e6); %Time is already in ms
        dataRateGbps(succeedingRunsIdx) = gbps;
        if gbps > maxRateGbps
            maxRateGbps = gbps;
            maxRateGbpsIdx = succeedingRunsIdx;
        end   
    end
    
    %Also Report which configuration had the max itteration count
    [~, maxIndex] = max(TransactionCore_Client);
    fprintf('    Max Rate Overall (Gbps): %f, TransactionCore_Client: %10d, ServerControlPeriodIterations: %5d, ClientControlPeriodIterations: %5d, ControlGainPNOPs: %6.3f, ControlGainINOPs: %6.3f, InitialNOPs: %5d, BlockSizeint32_tElements: %5d, ArrayLengthBlocks: %5d\n', maxRateGbps, TransactionCore_Client(maxRateGbpsIdx), ServerControlPeriodIterations(maxRateGbpsIdx), ClientControlPeriodIterations(maxRateGbpsIdx), ControlGainPNOPs(maxRateGbpsIdx), ControlGainINOPs(maxRateGbpsIdx), InitialNOPs(maxRateGbpsIdx), BlockSizeint32_tElements(maxRateGbpsIdx), ArrayLengthBlocks(maxRateGbpsIdx));
    
    
end
end


function plot_closed_loop_3dscatter_pi_serverControlGainP_dirs(directories, ClientControlPeriodIterationsFilter, InitialNOPsFilter, GainIFilter, satValue, plotZeroGain)
%plot_closed_loop_3dscatter_dirs Summary of this function goes here
%   Detailed explanation goes here
if nargin<6
    plotZeroGain = false;
end

if nargin<5
    satValue = -1;
end

satValStr = '';
if(satValue>=0)
    satValStr = [' Saturated: ' num2str(satValue)];
end


for i = 1:length(directories)
    disp([directories{i} ':'])
    dirPath = ['./' directories{i} '/report_closed_loop_pi_control_raw.csv'];
    [~, BlockSizeint32_tElements, ServerControlPeriodIterations, ClientControlPeriodIterations, ControlGainPNOPs, ControlGainINOPs, InitialNOPs, SteadyClockWalltimems, ~, ~, ~, ~, ~, ErroredStr, ~, TransactionCore_Client] = import_closed_loop_pi_raw_file(dirPath, [2, inf]);
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
    
    if plotZeroGain
        filterArr = (ClientControlPeriodIterations==ClientControlPeriodIterationsFilter) & (InitialNOPs == InitialNOPsFilter) & (ControlGainINOPs == GainIFilter);
        plot_closed_loop_3dscatter(ServerControlPeriodIterations, ControlGainPNOPs, TransactionCore_Client_Stat, Errored, SteadyClockWalltimems, BlockSizeElements, ElementSizeBytes, { filterArr }, {['(ClientControlPeriodIterations==', num2str(ClientControlPeriodIterationsFilter) ') && (InitialNOPs==' num2str(InitialNOPsFilter) ') && (ControlGainINOPs == ' num2str(GainIFilter) ')' satValStr]}, directories{i}, 'ServerControlPeriodIterations', 'ControlGainPNOPS');
    else
        filterArr = (ClientControlPeriodIterations==ClientControlPeriodIterationsFilter) & (InitialNOPs == InitialNOPsFilter) & (ControlGainINOPs == GainIFilter) & (ControlGainPNOPs ~= 0);
        plot_closed_loop_3dscatter(ServerControlPeriodIterations, ControlGainPNOPs, TransactionCore_Client_Stat, Errored, SteadyClockWalltimems, BlockSizeElements, ElementSizeBytes, { filterArr }, {['(ClientControlPeriodIterations==', num2str(ClientControlPeriodIterationsFilter) ') && (InitialNOPs==' num2str(InitialNOPsFilter) ') && (ControlGainINOPs == ' num2str(GainIFilter) ')' satValStr ', Omitted Gain 0']}, directories{i}, 'ServerControlPeriodIterations', 'ControlGainPNOPS');
    end
        
    %Also Report which configuration had the max itteration count
    [~, maxIndex] = max(TransactionCore_Client);
    fprintf('    Max Overall : TransactionCore_Client: %10d, ServerControlPeriodIterations: %5d, ClientControlPeriodIterations: %5d, ControlGainPNOPs: %6.3f, ControlGainINOPs: %6.3f, InitialNOPs: %5d\n', TransactionCore_Client(maxIndex), ServerControlPeriodIterations(maxIndex), ClientControlPeriodIterations(maxIndex), ControlGainPNOPs(maxIndex), ControlGainINOPs(maxIndex), InitialNOPs(maxIndex));
    
    [posGainIdxs, ~] = find(ControlGainPNOPs ~= 0);
    [~, maxPosGainSubIndex] = max(TransactionCore_Client(posGainIdxs));
    maxPosGainIndex = posGainIdxs(maxPosGainSubIndex);
    fprintf('    Max Gain>0  : TransactionCore_Client: %10d, ServerControlPeriodIterations: %5d, ClientControlPeriodIterations: %5d, ControlGainPNOPs: %6.3f, ControlGainINOPs: %6.3f, InitialNOPs: %5d\n', TransactionCore_Client(maxPosGainIndex), ServerControlPeriodIterations(maxPosGainIndex), ClientControlPeriodIterations(maxPosGainIndex), ControlGainPNOPs(maxPosGainIndex), ControlGainINOPs(maxIndex),  InitialNOPs(maxPosGainIndex))
    
    [filteredIdxs, ~] = find(filterArr);
    [~, maxFilteredSubIdxs] = max(TransactionCore_Client(filteredIdxs));
    maxFilteredIdxs = filteredIdxs(maxFilteredSubIdxs);
    fprintf('    Max Filtered: TransactionCore_Client: %10d, ServerControlPeriodIterations: %5d, ClientControlPeriodIterations: %5d, ControlGainPNOPs: %6.3f, ControlGainINOPs: %6.3f, InitialNOPs: %5d\n', TransactionCore_Client(maxFilteredIdxs), ServerControlPeriodIterations(maxFilteredIdxs), ClientControlPeriodIterations(maxFilteredIdxs), ControlGainPNOPs(maxFilteredIdxs), ControlGainINOPs(maxIndex), InitialNOPs(maxFilteredIdxs))
    
end
end


function plot_closed_loop_3dscatter_dirs(directories, ClientControlPeriodIterationsFilter, InitialNOPsFilter, satValue, plotZeroGain)
%plot_closed_loop_3dscatter_dirs Summary of this function goes here
%   Detailed explanation goes here
if nargin<5
    plotZeroGain = false;
end

if nargin<4
    satValue = -1;
end

satValStr = '';
if(satValue>=0)
    satValStr = [' Saturated: ' num2str(satValue)];
end

for i = 1:length(directories)
    disp([directories{i} ':'])
    dirPath = ['./' directories{i} '/report_closed_loop_bang_control_raw.csv'];
    [~, BlockSizeint32_tElements, ServerControlPeriodIterations, ClientControlPeriodIterations, ControlGainNOPs, InitialNOPs, SteadyClockWalltimems, ~, ~, ~, ~, ~, ErroredStr, ~, TransactionCore_Client] = import_closed_loop_raw_file_v2(dirPath, [2, inf]); %Version after open-loop refactoring
    %[~, ~, ServerControlPeriodIterations, ClientControlPeriodIterations, ControlGainNOPs, InitialNOPs, ~, ~, TransactionCore_Client, ~, ~, ~, ~, ~] = import_closed_loop_raw_file(dirPath, [2, inf]); % Version before open-loop refactoring
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
        filterArr = (ClientControlPeriodIterations==ClientControlPeriodIterationsFilter) & (InitialNOPs == InitialNOPsFilter);
        plot_closed_loop_3dscatter(ServerControlPeriodIterations, ControlGainNOPs, TransactionCore_Client_Stat, Errored, SteadyClockWalltimems, BlockSizeElements, ElementSizeBytes, { filterArr }, {['(ClientControlPeriodIterations==', num2str(ClientControlPeriodIterationsFilter) ') && (InitialNOPs==' num2str(InitialNOPsFilter) ')' satValStr]}, directories{i}, 'ServerControlPeriodIterations', 'ControlGainNOPS');
    else
        filterArr = (ClientControlPeriodIterations==ClientControlPeriodIterationsFilter) & (InitialNOPs == InitialNOPsFilter) & (ControlGainNOPs ~= 0);
        plot_closed_loop_3dscatter(ServerControlPeriodIterations, ControlGainNOPs, TransactionCore_Client_Stat, Errored, SteadyClockWalltimems, BlockSizeElements, ElementSizeBytes, { filterArr }, {['(ClientControlPeriodIterations==', num2str(ClientControlPeriodIterationsFilter) ') && (InitialNOPs==' num2str(InitialNOPsFilter) ')' satValStr ', Omitted Gain 0']}, directories{i}, 'ServerControlPeriodIterations', 'ControlGainNOPS');
    end
        
    %Also Report which configuration had the max itteration count
    [~, maxIndex] = max(TransactionCore_Client);
    %disp(['    Max Overall: ServerControlPeriodIterations=' num2str(ServerControlPeriodIterations(maxIndex)) ' ClientControlPeriodIterations=' num2str(ClientControlPeriodIterations(maxIndex)) ' ControlGainNOPs=' num2str(ControlGainNOPs(maxIndex)) ' InitialNOPs=' num2str(InitialNOPs(maxIndex)) ' TransactionCore_Client=' num2str(TransactionCore_Client(maxIndex))])
    fprintf('    Max Overall : TransactionCore_Client: %10d, ServerControlPeriodIterations: %5d, ClientControlPeriodIterations: %5d, ControlGainNOPs: %6.3f, InitialNOPs: %5d\n', TransactionCore_Client(maxIndex), ServerControlPeriodIterations(maxIndex), ClientControlPeriodIterations(maxIndex), ControlGainNOPs(maxIndex), InitialNOPs(maxIndex));
    
    [posGainIdxs, ~] = find(ControlGainNOPs ~= 0);
    [~, maxPosGainSubIndex] = max(TransactionCore_Client(posGainIdxs));
    maxPosGainIndex = posGainIdxs(maxPosGainSubIndex);
    %disp(['    Max Gain>0 : ServerControlPeriodIterations=' num2str(ServerControlPeriodIterations(maxPosGainIndex)) ' ClientControlPeriodIterations=' num2str(ClientControlPeriodIterations(maxPosGainIndex)) ' ControlGainNOPs=' num2str(ControlGainNOPs(maxPosGainIndex)) ' InitialNOPs=' num2str(InitialNOPs(maxPosGainIndex)) ' TransactionCore_Client=' num2str(TransactionCore_Client(maxPosGainIndex))])
    fprintf('    Max Gain>0  : TransactionCore_Client: %10d, ServerControlPeriodIterations: %5d, ClientControlPeriodIterations: %5d, ControlGainNOPs: %6.3f, InitialNOPs: %5d\n', TransactionCore_Client(maxPosGainIndex), ServerControlPeriodIterations(maxPosGainIndex), ClientControlPeriodIterations(maxPosGainIndex), ControlGainNOPs(maxPosGainIndex), InitialNOPs(maxPosGainIndex))
    
    [filteredIdxs, ~] = find(filterArr);
    [~, maxFilteredSubIdxs] = max(TransactionCore_Client(filteredIdxs));
    maxFilteredIdxs = filteredIdxs(maxFilteredSubIdxs);
    fprintf('    Max Filtered: TransactionCore_Client: %10d, ServerControlPeriodIterations: %5d, ClientControlPeriodIterations: %5d, ControlGainNOPs: %6.3f, InitialNOPs: %5d\n', TransactionCore_Client(maxFilteredIdxs), ServerControlPeriodIterations(maxFilteredIdxs), ClientControlPeriodIterations(maxFilteredIdxs), ControlGainNOPs(maxFilteredIdxs), InitialNOPs(maxFilteredIdxs))
    
end
end


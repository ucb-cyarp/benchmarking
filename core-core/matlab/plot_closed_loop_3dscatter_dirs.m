function plot_closed_loop_3dscatter_dirs(directories, ClientControlPeriodIterationsFilter, InitialNOPsFilter, satValue)
%plot_closed_loop_3dscatter_dirs Summary of this function goes here
%   Detailed explanation goes here

if nargin<4
    satValue = -1;
end

satValStr = '';
if(satValue>=0)
    satValStr = [' Saturated: ' num2str(satValue)];
end

for i = 1:length(directories)
    dirPath = ['./' directories{i} '/report_closed_loop_bang_control_raw.csv'];
    [~, ~, ServerControlPeriodIterations, ClientControlPeriodIterations, ControlGainNOPs, InitialNOPs, ~, ~, TransactionCore_Client, ~, ~, ~, ~, ~] = import_closed_loop_raw_file(dirPath, [2, inf]);
    TransactionCore_Client_Stat = TransactionCore_Client;
    if(satValue>0)
        TransactionCore_Client_Stat(TransactionCore_Client>satValue) = satValue;
    end
    plot_closed_loop_3dscatter(ServerControlPeriodIterations, ControlGainNOPs, TransactionCore_Client_Stat, {(ClientControlPeriodIterations==ClientControlPeriodIterationsFilter) & (InitialNOPs == InitialNOPsFilter) }, {['(ClientControlPeriodIterations==', num2str(ClientControlPeriodIterationsFilter) ') && (InitialNOPs==' num2str(InitialNOPsFilter) ')' satValStr]}, directories{i})
end
end


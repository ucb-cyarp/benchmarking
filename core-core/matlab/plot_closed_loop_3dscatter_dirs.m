function plot_closed_loop_3dscatter_dirs(directories, ClientControlPeriodIterationsFilter)
%plot_closed_loop_3dscatter_dirs Summary of this function goes here
%   Detailed explanation goes here

for i = 1:length(directories)
    dirPath = ['./' directories{i} '/report_closed_loop_bang_control_raw.csv'];
    [~, ~, ServerControlPeriodIterations, ClientControlPeriodIterations, ControlGainNOPs, ~, TransactionCore_Client, ~, ~, ~] = import_closed_loop_raw_file(dirPath, [2, inf]);
    plot_closed_loop_3dscatter(ServerControlPeriodIterations, ControlGainNOPs, TransactionCore_Client, {ClientControlPeriodIterations==ClientControlPeriodIterationsFilter}, {['ClientControlPeriodIterations==', num2str(ClientControlPeriodIterationsFilter)]}, directories{i})
end
end


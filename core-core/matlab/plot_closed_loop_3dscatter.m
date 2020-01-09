function plot_closed_loop_3dscatter(ServerControlPeriodIterations, ControlGainNOPs, TransactionCore_Client, filterConditions, filterConditionLbls, titleStr)
%plot_closed_loop_3dscatter Plots a 3D scatterplot of the number of
%itterations before failure (or finish) with different control periods and
%gains

figure;

for i = 1:length(filterConditions)
    [idx, ~] = find(filterConditions{i});

    if length(filterConditions)>1
        scatter3(ServerControlPeriodIterations(idx), ControlGainNOPs(idx), TransactionCore_Client(idx), 75, 'filled', 'MarkerFaceAlpha', 0.2);
        hold all;
    else
        scatter3(ServerControlPeriodIterations(idx), ControlGainNOPs(idx), TransactionCore_Client(idx), 75, TransactionCore_Client(idx), 'filled');
    end

end
hold off;
xlabel('ServerControlPeriodIterations'); 
ylabel('ControlGainNOPs'); 
zlabel('Transactions Before Failure');

if length(filterConditions)>1
    legend(filterConditionLbls);
    title(titleStr, 'Interpreter', 'none');
else
    title([titleStr ' - ' filterConditionLbls{1}], 'Interpreter', 'none')
end

axis vis3d;

end


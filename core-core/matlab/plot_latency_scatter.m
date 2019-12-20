function plot_latency_scatter(directoryList, includeWallTime, includeProcTime, normalizeAll, size, alpha)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

%sane alpha = 0.2
%sane size = 20

maxSingleArray = 0;
maxDoubleArray = 0;
maxFlowCtrlArray = 0;
maxFlowControlArrayBlockRead = 0;

singleArrayPlots = [];
doubleArrayPlots = [];
flowCtrlArrayPlots = [];
flowControlArrayBlockReadPlots = [];

for i = 1:length(directoryList)
    directory = directoryList{i};
    
    %% Data Import
    start_row = 2;
    end_row = inf;
    [single_array_length, single_array_highResClk, single_array_procTime, single_array_rdtsc] = importRawArrayFile([directory '/report_single_array_raw.csv'], start_row, end_row);
    [dual_array_length, dual_array_highResClk, dual_array_procTime, dual_array_rdtsc] = importRawArrayFile([directory '/report_dual_array_raw.csv'], start_row, end_row);
    [flow_ctrl_array_length, flow_ctrl_array_highResClk, flow_ctrl_array_procTime, flow_ctrl_array_rdtsc] = importRawArrayFile([directory '/report_flow_ctrl_array_raw.csv'], start_row, end_row);
    [flow_ctrl_block_read_array_length, flow_ctrl_block_read_array_highResClk, flow_ctrl_block_read_array_procTime, flow_ctrl_block_read_array_rdtsc] = importRawArrayFile([directory '/report_flow_ctrl_blocked_read_array_raw.csv'], start_row, end_row);

    [STIM_LEN,TRIALS,CPUA,CPUB,CPUC,CPUD,profiler,methodology_version] = importParametersFile([directory '/report_parameters.csv']);

    %% Scale Data
    single_array_length_bytes = single_array_length*32/8;
    dual_array_length_bytes = dual_array_length*32/8;
    flow_ctrl_array_length_bytes = flow_ctrl_array_length*32/8;
    flow_ctrl_block_read_array_length_bytes = flow_ctrl_block_read_array_length*32/8;

    %Scale ms to ns and divide by the number of transactions per trial (not trials)
    %StimLen is the number of transactions per trial is STIM_LEN

    single_timeToCompletion_highResClk = single_array_highResClk*1e6/STIM_LEN;
    single_timeToCompletion_procTime = single_array_procTime*1e6/STIM_LEN;

    dual_timeToCompletion_highResClk = dual_array_highResClk*1e6/STIM_LEN;
    dual_timeToCompletion_procTime = dual_array_procTime*1e6/STIM_LEN;

    flow_ctrl_timeToCompletion_highResClk = flow_ctrl_array_highResClk*1e6/(STIM_LEN/2);
    flow_ctrl_timeToCompletion_procTime = flow_ctrl_array_procTime*1e6/(STIM_LEN/2);

    flow_ctrl_block_read_timeToCompletion_highResClk = flow_ctrl_block_read_array_highResClk*1e6/(STIM_LEN/2);
    flow_ctrl_block_read_timeToCompletion_procTime = flow_ctrl_block_read_array_procTime*1e6/(STIM_LEN/2);

    %% Plot 1
    figure;
    %subplot(2, 2, 1);
    a1 = axes;
    if includeProcTime
        s2 = scatter(a1, single_array_length_bytes, single_timeToCompletion_procTime, size, [0.8500 0.3250 0.0980], 'filled');
        maxSingleArray = max([maxSingleArray, max(max(single_timeToCompletion_procTime))]);
        s2.MarkerFaceAlpha = alpha;
        hold all;
    end
    if includeWallTime
        s1 = scatter(a1, single_array_length_bytes, single_timeToCompletion_highResClk, size, [0 0.4470 0.7410], 'filled');
        singleArrayPlots = [singleArrayPlots, a1];
        maxSingleArray = max([maxSingleArray, max(max(single_timeToCompletion_highResClk))]);
        s1.MarkerFaceAlpha = alpha;
    end
    hold off;
    xlabel('Transaction Length (Bytes)', 'Interpreter', 'none');
    ylabel('Transaction Time to Completion - One Way (ns)')
    title({'Core-Core Transactions with Single Shared Array', directory}, 'Interpreter', 'none');
    if includeProcTime && includeWallTime
        legend([s1, s2], {'High Resolution Clock', 'Thread Process Time'});
    elseif includeProcTime
        legend([s2], {'Thread Process Time'});
    elseif includeWallTime
        legend([s1], {'High Resolution Clock'});
    end
    grid on;

    %% Plot 2
    figure;
    %subplot(2, 2, 2);
    a1 = axes;
    if includeProcTime
        s2 = scatter(a1, single_array_length_bytes, dual_timeToCompletion_procTime, size, [0.8500 0.3250 0.0980], 'filled');
        maxDoubleArray = max([maxDoubleArray, max(max(dual_timeToCompletion_procTime))]);
        s2.MarkerFaceAlpha = alpha;
        hold all;
    end
    if includeWallTime
        s1 = scatter(a1, single_array_length_bytes, dual_timeToCompletion_highResClk, size, [0 0.4470 0.7410], 'filled');
        doubleArrayPlots = [doubleArrayPlots, a1];
        maxDoubleArray = max([maxDoubleArray, max(max(dual_timeToCompletion_highResClk))]);
        s1.MarkerFaceAlpha = alpha;
    end
    xlabel('Transaction Length (Bytes)', 'Interpreter', 'none');
    ylabel('Transaction Time to Completion - One Way (ns)')
    title({'Core-Core Transactions with Dual Shared Arrays', directory}, 'Interpreter', 'none');
    hold off;
    if includeProcTime && includeWallTime
        legend([s1, s2], {'High Resolution Clock', 'Thread Process Time'});
    elseif includeProcTime
        legend([s2], {'Thread Process Time'});
    elseif includeWallTime
        legend([s1], {'High Resolution Clock'});
    end
    grid on;

    %% Plot 3
    figure;
    %subplot(2, 2, 3);
    a1 = axes;
    if includeProcTime
        s2 = scatter(a1, single_array_length_bytes, flow_ctrl_timeToCompletion_procTime, size, [0.8500 0.3250 0.0980], 'filled');
        maxFlowCtrlArray = max([maxFlowCtrlArray, max(max(flow_ctrl_timeToCompletion_procTime))]);
        s2.MarkerFaceAlpha = alpha;
        hold all;
    end
    if includeWallTime
        s1 = scatter(a1, single_array_length_bytes, flow_ctrl_timeToCompletion_highResClk, size, [0 0.4470 0.7410], 'filled');
        flowCtrlArrayPlots = [flowCtrlArrayPlots, a1];
        maxFlowCtrlArray = max([maxFlowCtrlArray, max(max(flow_ctrl_timeToCompletion_highResClk))]);
        s1.MarkerFaceAlpha = alpha;
    end
    xlabel('Transaction Length (Bytes)', 'Interpreter', 'none');
    ylabel('Transaction Time to Completion - One Way (ns)')
    title({'Core-Core Flow Control Transactions with Ack', directory}, 'Interpreter', 'none');
    hold off;
    if includeProcTime && includeWallTime
        legend([s1, s2], {'High Resolution Clock', 'Thread Process Time'});
        hold off;
    elseif includeProcTime
        legend([s2], {'Thread Process Time'});
    elseif includeWallTime
        legend([s1], {'High Resolution Clock'});
    end
    grid on;

    %% Plot 4
    figure;
    a1 = axes;
    %subplot(2, 2, 4);
    if includeProcTime
        s2 = scatter(a1, single_array_length_bytes, flow_ctrl_block_read_timeToCompletion_procTime, size, [0.8500 0.3250 0.0980], 'filled');
        maxFlowControlArrayBlockRead = max([maxFlowControlArrayBlockRead, max(max(flow_ctrl_block_read_timeToCompletion_procTime))]);
        s2.MarkerFaceAlpha = alpha;
        hold all;
    end
    if includeWallTime
        s1 = scatter(a1, single_array_length_bytes, flow_ctrl_block_read_timeToCompletion_highResClk, size, [0 0.4470 0.7410], 'filled');
        flowControlArrayBlockReadPlots = [flowControlArrayBlockReadPlots, a1];
        maxFlowControlArrayBlockRead = max([maxFlowControlArrayBlockRead, max(max(flow_ctrl_block_read_timeToCompletion_highResClk))]);
        s1.MarkerFaceAlpha = alpha;
    end
    xlabel('Transaction Length (Bytes)', 'Interpreter', 'none');
    ylabel('Transaction Time to Completion - One Way (ns)')
    title({'Core-Core Flow Control (Blocked Read) Transactions with Ack', directory}, 'Interpreter', 'none');
    hold off;
    if includeProcTime && includeWallTime
        legend([s1, s2], {'High Resolution Clock', 'Thread Process Time'});
        hold off;
    elseif includeProcTime
        legend([s2], {'Thread Process Time'});
    elseif includeWallTime
        legend([s1], {'High Resolution Clock'});
    end
    grid on;

end

%% Scale

if normalizeAll
    maxVal = max([maxSingleArray, maxDoubleArray, maxFlowCtrlArray, maxFlowControlArrayBlockRead]);
    maxSingleArray = maxVal;
    maxDoubleArray = maxVal;
    maxFlowCtrlArray = maxVal;
    maxFlowControlArrayBlockRead = maxVal;
end

for i = 1:length(singleArrayPlots)
    ylim(singleArrayPlots(i), [1, maxSingleArray]);
end

for i = 1:length(doubleArrayPlots)
    ylim(doubleArrayPlots(i), [1, maxDoubleArray]);
end

for i = 1:length(flowCtrlArrayPlots)
    ylim(flowCtrlArrayPlots(i), [1, maxFlowCtrlArray]);
end

for i = 1:length(flowControlArrayBlockReadPlots)
    ylim(flowControlArrayBlockReadPlots(i), [1, maxFlowControlArrayBlockRead]);
end


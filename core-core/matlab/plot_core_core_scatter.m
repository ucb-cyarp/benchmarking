%% Init
clear;% close all; clc;

%% Data Import
start_row = 2;
end_row = inf;
[single_array_length, single_array_highResClk, single_array_procTime, single_array_rdtsc] = importRawArrayFile('report_single_array_raw.csv', start_row, end_row);
[dual_array_length, dual_array_highResClk, dual_array_procTime, dual_array_rdtsc] = importRawArrayFile('report_dual_array_raw.csv', start_row, end_row);
[flow_ctrl_array_length, flow_ctrl_array_highResClk, flow_ctrl_array_procTime, flow_ctrl_array_rdtsc] = importRawArrayFile('report_flow_ctrl_array_raw.csv', start_row, end_row);
[flow_ctrl_block_read_array_length, flow_ctrl_block_read_array_highResClk, flow_ctrl_block_read_array_procTime, flow_ctrl_block_read_array_rdtsc] = importRawArrayFile('report_flow_ctrl_blocked_read_array_raw.csv', start_row, end_row);

[STIM_LEN,TRIALS,CPUA,CPUB,CPUC,CPUD,profiler,methodology_version] = importParametersFile('report_parameters.csv');

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

flow_ctrl_timeToCompletion_highResClk = flow_ctrl_array_highResClk*1e6/STIM_LEN;
flow_ctrl_timeToCompletion_procTime = flow_ctrl_array_procTime*1e6/STIM_LEN;

flow_ctrl_block_read_timeToCompletion_highResClk = flow_ctrl_block_read_array_highResClk*1e6/STIM_LEN;
flow_ctrl_block_read_timeToCompletion_procTime = flow_ctrl_block_read_array_procTime*1e6/STIM_LEN;

%% Plot 1
figure;
%subplot(2, 2, 1);
s2 = scatter(single_array_length_bytes, single_timeToCompletion_procTime, 20, [0.8500 0.3250 0.0980], 'filled');
s2.MarkerFaceAlpha = 0.2;
hold all;
s1 = scatter(single_array_length_bytes, single_timeToCompletion_highResClk, 20, [0 0.4470 0.7410], 'filled');
s1.MarkerFaceAlpha = 0.2;
xlabel('Transaction Length (Bytes)', 'Interpreter', 'none');
ylabel('Transaction Time to Completion - One Way (ns)')
title('Core-Core Transactions with Single Shared Array');
legend([s1, s2], {'High Resolution Clock', 'Thread Process Time'});
hold off;
grid on;

%% Plot 2
figure;
%subplot(2, 2, 2);
s2 = scatter(single_array_length_bytes, dual_timeToCompletion_procTime, 20, [0.8500 0.3250 0.0980], 'filled');
s2.MarkerFaceAlpha = 0.2;
hold all;
s1 = scatter(single_array_length_bytes, dual_timeToCompletion_highResClk, 20, [0 0.4470 0.7410], 'filled');
s1.MarkerFaceAlpha = 0.2;
xlabel('Transaction Length (Bytes)', 'Interpreter', 'none');
ylabel('Transaction Time to Completion - One Way (ns)')
title('Core-Core Transactions with Dual Shared Arrays');
legend([s1, s2], {'High Resolution Clock', 'Thread Process Time'});
hold off;
grid on;

%% Plot 3
figure;
%subplot(2, 2, 3);
s2 = scatter(single_array_length_bytes, flow_ctrl_timeToCompletion_procTime, 20, [0.8500 0.3250 0.0980], 'filled');
s2.MarkerFaceAlpha = 0.2;
hold all;
s1 = scatter(single_array_length_bytes, flow_ctrl_timeToCompletion_highResClk, 20, [0 0.4470 0.7410], 'filled');
s1.MarkerFaceAlpha = 0.2;
xlabel('Transaction Length (Bytes)', 'Interpreter', 'none');
ylabel('Transaction Time to Completion - One Way (ns)')
title('Core-Core Flow Control Transactions with Ack');
legend([s1, s2], {'High Resolution Clock', 'Thread Process Time'});
hold off;
grid on;

%% Plot 4
figure;
%subplot(2, 2, 4);
s2 = scatter(single_array_length_bytes, single_timeToCompletion_procTime, 20, [0.8500 0.3250 0.0980], 'filled');
s2.MarkerFaceAlpha = 0.2;
hold all;
s1 = scatter(single_array_length_bytes, single_timeToCompletion_highResClk, 20, [0 0.4470 0.7410], 'filled');
s1.MarkerFaceAlpha = 0.2;
xlabel('Transaction Length (Bytes)', 'Interpreter', 'none');
ylabel('Transaction Time to Completion - One Way (ns)')
title('Core-Core Flow Control (Blocked Read) Transactions with Ack');
legend([s1, s2], {'High Resolution Clock', 'Thread Process Time'});
hold off;
grid on;

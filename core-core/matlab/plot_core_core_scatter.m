%% Init
clear;% close all; clc;

%% Data Import
start_row = 2;
end_row = inf;
[single_array_length, single_array_highResClk, single_array_procTime, single_array_rdtsc] = importRawArrayFile('report_single_array_raw.csv', start_row, end_row);
[dual_array_length, dual_array_highResClk, dual_array_procTime, dual_array_rdtsc] = importRawArrayFile('report_dual_array_raw.csv', start_row, end_row);
[flow_ctrl_array_length, flow_ctrl_array_highResClk, flow_ctrl_array_procTime, flow_ctrl_array_rdtsc] = importRawArrayFile('report_flow_ctrl_array_raw.csv', start_row, end_row);
[flow_ctrl_block_read_array_length, flow_ctrl_block_read_array_highResClk, flow_ctrl_block_read_array_procTime, flow_ctrl_block_read_array_rdtsc] = importRawArrayFile('report_flow_ctrl_blocked_read_array_raw.csv', start_row, end_row);

%% Scale Data
single_array_length_bytes = single_array_length*32/8;
dual_array_length_bytes = dual_array_length*32/8;
flow_ctrl_array_length_bytes = flow_ctrl_array_length*32/8;
flow_ctrl_block_read_array_length_bytes = flow_ctrl_block_read_array_length*32/8;

single_timeToCompletion_highResClk = single_array_highResClk*1e6;
single_timeToCompletion_procTime = single_array_procTime*1e6;

dual_timeToCompletion_highResClk = dual_array_highResClk*1e6;
dual_timeToCompletion_procTime = dual_array_procTime*1e6;

flow_ctrl_timeToCompletion_highResClk = flow_ctrl_array_highResClk*1e6;
flow_ctrl_timeToCompletion_procTime = flow_ctrl_array_procTime*1e6;

flow_ctrl_block_read_timeToCompletion_highResClk = flow_ctrl_block_read_array_highResClk*1e6;
flow_ctrl_block_read_timeToCompletion_procTime = flow_ctrl_block_read_array_procTime*1e6;

%% Plot 1
figure;
%subplot(2, 2, 1);
scatter(single_array_length_bytes, single_timeToCompletion_highResClk);
xlabel('Transaction Length (Bytes)', 'Interpreter', 'none');
ylabel('Transaction Time to Completion - One Way (ns)')
title('Core-Core Transactions with Single Shared Array');
hold all;
scatter(single_array_length_bytes, single_timeToCompletion_procTime, '*');
legend('High Resolution Clock', 'Thread Process Time')
hold off;
grid on;

%% Plot 2
figure;
%subplot(2, 2, 2);
scatter(dual_array_length_bytes, dual_timeToCompletion_highResClk);
xlabel('Transaction Length (Bytes)', 'Interpreter', 'none');
ylabel('Transaction Time to Completion - One Way (ns)')
title('Core-Core Transactions with Dual Shared Arrays');
hold all;
scatter(dual_array_length_bytes, dual_timeToCompletion_procTime, '*');
legend('High Resolution Clock', 'Thread Process Time')
hold off;
grid on;

%% Plot 3
figure;
%subplot(2, 2, 3);
scatter(flow_ctrl_array_length_bytes, flow_ctrl_timeToCompletion_highResClk);
xlabel('Transaction Length (Bytes)', 'Interpreter', 'none');
ylabel('Transaction Time to Completion - One Way (ns)')
title('Core-Core Flow Control Transactions with Ack');
hold all;
scatter(flow_ctrl_array_length_bytes, flow_ctrl_timeToCompletion_procTime, '*');
legend('High Resolution Clock', 'Thread Process Time')
hold off;
grid on;

%% Plot 4
figure;
%subplot(2, 2, 4);
scatter(flow_ctrl_block_read_array_length_bytes, single_timeToCompletion_highResClk);
xlabel('Transaction Length (Bytes)', 'Interpreter', 'none');
ylabel('Transaction Time to Completion - One Way (ns)')
title('Core-Core Flow Control (Blocked Read) Transactions with Ack');
hold all;
scatter(flow_ctrl_block_read_array_length_bytes, single_timeToCompletion_procTime, '*');
legend('High Resolution Clock', 'Thread Process Time')
hold off;
grid on;

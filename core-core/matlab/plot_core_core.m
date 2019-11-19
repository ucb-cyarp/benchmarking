%% Init
clear;% close all; clc;

%% Data Import
start_row = 2;
end_row = inf;
[single_array_length, single_latency_avg, single_latency_stddev, single_transaction_rate, single_data_rate] = import_core_core_file('report_single_array.csv', start_row, end_row);
[dual_array_length, dual_latency_avg, dual_latency_stddev, dual_transaction_rate, dual_data_rate] = import_core_core_file('report_dual_array.csv', start_row, end_row);
[flow_ctrl_array_length, flow_ctrl_latency_avg, flow_ctrl_latency_stddev, flow_ctrl_transaction_rate, flow_ctrl_data_rate] = import_core_core_file('report_flow_ctrl_array.csv', start_row, end_row);
[flow_ctrl_block_read_array_length, flow_ctrl_block_read_latency_avg, flow_ctrl_block_read_latency_stddev, flow_ctrl_block_read_transaction_rate, flow_ctrl_block_read_data_rate] = import_core_core_file('report_flow_ctrl_blocked_read_array.csv', start_row, end_row);

single_array_length_bytes = single_array_length*32/8;
dual_array_length_bytes = dual_array_length*32/8;
flow_ctrl_array_length_bytes = flow_ctrl_array_length*32/8;
flow_ctrl_block_read_array_length_bytes = flow_ctrl_block_read_array_length*32/8;

single_data_rate_gbps = single_data_rate/1000;
dual_data_rate_gbps = dual_data_rate/1000;
flow_ctrl_data_rate_gbps = flow_ctrl_data_rate/1000;
flow_ctrl_block_read_data_rate_gbps = flow_ctrl_block_read_data_rate/1000;

%% Plot 1
figure;
%subplot(2, 2, 1);
yyaxis left;
errorbar(single_array_length_bytes, single_latency_avg, single_latency_stddev);
xlabel('Transaction Length (Bytes)', 'Interpreter', 'none');
ylabel('Transaction Time to Completion - One Way (ns)')
title('Core-Core Transactions with Single Shared Array');
hold all;
yyaxis right;
plot(single_array_length_bytes, single_data_rate_gbps);
ylabel('Throughput (Gbps)')
hold off;
grid on;

%% Plot 2

%subplot(2, 2, 2);
figure;
yyaxis left;
errorbar(dual_array_length_bytes, dual_latency_avg, dual_latency_stddev);
xlabel('Transaction Length (Bytes)', 'Interpreter', 'none');
ylabel('Transaction Time to Completion - One Way (ns)')
title('Core-Core Transactions with Dual Shared Arrays');
hold all;
yyaxis right;
ylabel('Throughput (Gbps)')
plot(dual_array_length_bytes, dual_data_rate_gbps);
hold off;
grid on;

%% Plot 3

%subplot(2, 2, 3);
figure;
yyaxis left;
errorbar(flow_ctrl_array_length_bytes, flow_ctrl_latency_avg, flow_ctrl_latency_stddev);
xlabel('Transaction Length (Bytes)', 'Interpreter', 'none');
ylabel('Transaction Time to Completion - Round Trip (ns)')
title('Core-Core Flow Control Transactions with Ack');
hold all;
yyaxis right;
ylabel('Throughput (Gbps)')
plot(flow_ctrl_array_length_bytes, flow_ctrl_data_rate_gbps);
hold off;
grid on;

%% Plot 4

%subplot(2, 2, 4);
figure;
yyaxis left;
errorbar(flow_ctrl_block_read_array_length_bytes, flow_ctrl_block_read_latency_avg, flow_ctrl_block_read_latency_stddev);
xlabel('Transaction Length (Bytes)', 'Interpreter', 'none');
ylabel('Transaction Time to Completion - Round Trip (ns)')
title('Core-Core Flow Control (Blocked Read) Transactions with Ack');
hold all;
yyaxis right;
ylabel('Throughput (Gbps)')
plot(flow_ctrl_block_read_array_length_bytes, flow_ctrl_block_read_data_rate_gbps);
hold off;
grid on;

%% Init
clear; close all; clc;

%% Data Import 1
start_row = 2;
end_row = inf;
[single_array_length_a, single_latency_avg_a, single_latency_stddev_a, single_transaction_rate_a, single_data_rate_a] = import_core_core_file('report_single_array_simultanious_a.csv', start_row, end_row);
[single_array_length_b, single_latency_avg_b, single_latency_stddev_b, single_transaction_rate_b, single_data_rate_b] = import_core_core_file('report_single_array_simultanious_b.csv', start_row, end_row);

single_array_length_bytes_a = single_array_length_a*32/8;
single_array_length_bytes_b = single_array_length_b*32/8;

single_data_rate_gbps_a = single_data_rate_a/1000;
single_data_rate_gbps_b = single_data_rate_b/1000;

%% Plot 1
figure;
%subplot(2, 2, 1);
yyaxis left;
errorbar(single_array_length_bytes_a, single_latency_avg_a, single_latency_stddev_a, '-');
hold on;
errorbar(single_array_length_bytes_b, single_latency_avg_b, single_latency_stddev_b, 'b-');
xlabel('Transaction Length (Bytes)', 'Interpreter', 'none');
ylabel('Transaction Time to Completion - One Way (ns)')
title('Core-Core Transactions (Simultaneous) with Single Shared Array');
yyaxis right;
plot(single_array_length_bytes_a, single_data_rate_gbps_a, '-');
hold on;
plot(single_array_length_bytes_b, single_data_rate_gbps_b, 'r-');
ylabel('Throughput (Gbps)')
hold off;
grid on;
legend('CPU A <-> CPU B', 'CPU C <-> CPU D', 'CPU A <-> CPU B', 'CPU C <-> CPU D', 'Location', 'southeast')

%% Data Import 2
[dual_array_length_a, dual_latency_avg_a, dual_latency_stddev_a, dual_transaction_rate_a, dual_data_rate_a] = import_core_core_file('report_dual_array_simultanious_a.csv', start_row, end_row);
[dual_array_length_b, dual_latency_avg_b, dual_latency_stddev_b, dual_transaction_rate_b, dual_data_rate_b] = import_core_core_file('report_dual_array_simultanious_b.csv', start_row, end_row);

dual_array_length_bytes_a = dual_array_length_a*32/8;
dual_array_length_bytes_b = dual_array_length_b*32/8;

dual_data_rate_gbps_a = dual_data_rate_a/1000;
dual_data_rate_gbps_b = dual_data_rate_b/1000;

%% Plot 2
figure;
%subplot(2, 2, 2);
yyaxis left;
errorbar(dual_array_length_bytes_a, dual_latency_avg_a, dual_latency_stddev_a, '-');
hold on;
errorbar(dual_array_length_bytes_b, dual_latency_avg_b, dual_latency_stddev_b, 'b-');
xlabel('Transaction Length (Bytes)', 'Interpreter', 'none');
ylabel('Transaction Time to Completion - One Way (ns)')
title('Core-Core Transactions (Simultaneous) with Dual Shared Arrays');
yyaxis right;
plot(dual_array_length_bytes_a, dual_data_rate_gbps_a, '-');
hold on;
plot(dual_array_length_bytes_b, dual_data_rate_gbps_b, 'r-');
ylabel('Throughput (Gbps)')
hold off;
grid on;
legend('CPU A <-> CPU B', 'CPU C <-> CPU D', 'CPU A <-> CPU B', 'CPU C <-> CPU D', 'Location', 'southeast')

%% Data Import 3
[flow_ctrl_array_length_a, flow_ctrl_latency_avg_a, flow_ctrl_latency_stddev_a, flow_ctrl_transaction_rate_a, flow_ctrl_data_rate_a] = import_core_core_file('report_flow_ctrl_array_simultanious_a.csv', start_row, end_row);
[flow_ctrl_array_length_b, flow_ctrl_latency_avg_b, flow_ctrl_latency_stddev_b, flow_ctrl_transaction_rate_b, flow_ctrl_data_rate_b] = import_core_core_file('report_flow_ctrl_array_simultanious_b.csv', start_row, end_row);

flow_ctrl_array_length_bytes_a = flow_ctrl_array_length_a*32/8;
flow_ctrl_array_length_bytes_b = flow_ctrl_array_length_b*32/8;

flow_ctrl_data_rate_gbps_a = flow_ctrl_data_rate_a/1000;
flow_ctrl_data_rate_gbps_b = flow_ctrl_data_rate_b/1000;

%% Plot 3
figure;
%subplot(2, 2, 3);
yyaxis left;
errorbar(flow_ctrl_array_length_bytes_a, flow_ctrl_latency_avg_a, flow_ctrl_latency_stddev_a, '-');
hold on;
errorbar(flow_ctrl_array_length_bytes_b, flow_ctrl_latency_avg_b, flow_ctrl_latency_stddev_b, 'b-');
xlabel('Transaction Length (Bytes)', 'Interpreter', 'none');
ylabel('Transaction Time to Completion - One Way (ns)')
title('Core-Core Transactions (Simultaneous) with Flow Control');
yyaxis right;
plot(flow_ctrl_array_length_bytes_a, flow_ctrl_data_rate_gbps_a, '-');
hold on;
plot(flow_ctrl_array_length_bytes_b, flow_ctrl_data_rate_gbps_b, 'r-');
ylabel('Throughput (Gbps)')
hold off;
grid on;
legend('CPU A <-> CPU B', 'CPU C <-> CPU D', 'CPU A <-> CPU B', 'CPU C <-> CPU D', 'Location', 'southeast')

%% Data Import 4
[flow_ctrl_blocked_read_array_length_a, flow_ctrl_blocked_read_latency_avg_a, flow_ctrl_blocked_read_latency_stddev_a, flow_ctrl_blocked_read_transaction_rate_a, flow_ctrl_blocked_read_data_rate_a] = import_core_core_file('report_flow_ctrl_blocked_read_array_simultanious_a.csv', start_row, end_row);
[flow_ctrl_blocked_read_array_length_b, flow_ctrl_blocked_read_latency_avg_b, flow_ctrl_blocked_read_latency_stddev_b, flow_ctrl_blocked_read_transaction_rate_b, flow_ctrl_blocked_read_data_rate_b] = import_core_core_file('report_flow_ctrl_blocked_read_array_simultanious_b.csv', start_row, end_row);

flow_ctrl_blocked_read_array_length_bytes_a = flow_ctrl_blocked_read_array_length_a*32/8;
flow_ctrl_blocked_read_array_length_bytes_b = flow_ctrl_blocked_read_array_length_b*32/8;

flow_ctrl_blocked_read_data_rate_gbps_a = flow_ctrl_blocked_read_data_rate_a/1000;
flow_ctrl_blocked_read_data_rate_gbps_b = flow_ctrl_blocked_read_data_rate_b/1000;

%% Plot 4
figure;
%subplot(2, 2, 4);
yyaxis left;
errorbar(flow_ctrl_blocked_read_array_length_bytes_a, flow_ctrl_blocked_read_latency_avg_a, flow_ctrl_blocked_read_latency_stddev_a, '-');
hold on;
errorbar(flow_ctrl_blocked_read_array_length_bytes_b, flow_ctrl_blocked_read_latency_avg_b, flow_ctrl_blocked_read_latency_stddev_b, 'b-');
xlabel('Transaction Length (Bytes)', 'Interpreter', 'none');
ylabel('Transaction Time to Completion - One Way (ns)')
title('Core-Core Transactions (Simultaneous) with Flow Control (Blocked Read)');
yyaxis right;
plot(flow_ctrl_blocked_read_array_length_bytes_a, flow_ctrl_blocked_read_data_rate_gbps_a, '-');
hold on;
plot(flow_ctrl_blocked_read_array_length_bytes_b, flow_ctrl_blocked_read_data_rate_gbps_b, 'r-');
ylabel('Throughput (Gbps)')
hold off;
grid on;
legend('CPU A <-> CPU B', 'CPU C <-> CPU D', 'CPU A <-> CPU B', 'CPU C <-> CPU D', 'Location', 'southeast')

%% Init
clear; close all; clc;

%% Parameters from Run
data_len = 1000000;
bits_per_element = 32;

%% Data Import
start_row = 2;
end_row = 65;
[single_array_length, single_latency_avg, single_latency_stddev, single_transaction_rate, single_data_rate] = import_core_core_file('report_single_array.csv', start_row, end_row);
[dual_array_length, dual_latency_avg, dual_latency_stddev, dual_transaction_rate, dual_data_rate] = import_core_core_file('report_dual_array.csv', start_row, end_row);
[fifo_array_length, fifo_latency_avg, fifo_latency_stddev, fifo_transaction_rate, fifo_data_rate] = import_core_core_file('report_fifo_array.csv', start_row, end_row);

%% Plot
figure;
subplot(2, 1, 1);
semilogy(single_array_length, single_latency_avg);
xlim([0,66]);
xlabel('Transaction Length (int32_t Elements)', 'Interpreter', 'none');
ylabel('Transaction Latency - One Way (ns)')
title('Core-Core Transactions with Single Shared Array');
grid on;
subplot(2, 1, 2);
% boxplot(single_data_rate, single_array_length);
semilogy(single_array_length, single_data_rate);
xlim([0,66]);
ylabel('Throughput (Mbps)');
xlabel('Transaction Length (int32_t Elements)', 'Interpreter', 'none');
grid on;

%subplot(3, 1, 2);
figure;
subplot(2, 1, 1);
semilogy(dual_array_length, dual_latency_avg);
xlim([0,66]);
xlabel('Transaction Length (int32_t Elements)', 'Interpreter', 'none');
ylabel('Transaction Latency - One Way (ns)')
title('Core-Core Transactions with Dual Shared Arrays');
grid on;
subplot(2, 1, 2);
% boxplot(dual_data_rate, dual_array_length);
semilogy(dual_array_length, dual_data_rate);
xlim([0,66]);
ylabel('Throughput (Mbps)');
xlabel('Transaction Length (int32_t Elements)', 'Interpreter', 'none');
grid on;

%subplot(3, 1, 3);
figure;
subplot(2, 1, 1);
semilogy(fifo_array_length, fifo_latency_avg);
xlim([0,66]);
ylim([10, 1000]);
xlabel('Transaction Length (int32_t Elements)', 'Interpreter', 'none');
ylabel('Transaction Latency - Round Trip (ns)')
title('Core-Core FIFO Transactions with Ack');
grid on;
subplot(2, 1, 2);
% boxplot(fifo_data_rate, fifo_array_length);
semilogy(fifo_array_length, fifo_data_rate);
xlim([0,66]);
ylabel('Throughput (Mbps)');
xlabel('Transaction Length (int32_t Elements)', 'Interpreter', 'none');
grid on;

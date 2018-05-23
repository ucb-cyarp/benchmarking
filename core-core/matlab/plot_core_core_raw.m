%% Init
clear; close all; clc;

%% Parameters from Run
data_len = 1000000;
bits_per_element = 32;

%% Data Import
start_row = 2;
end_row = inf;
[single_array_length, single_duration_high_res_clock, single_duration_clock, single_duration_rdtsc] = import_core_core_raw_file('report_single_array_raw.csv', start_row, end_row);
[dual_array_length, dual_duration_high_res_clock, dual_duration_clock, dual_duration_rdtsc] = import_core_core_raw_file('report_dual_array_raw.csv', start_row, end_row);
[fifo_array_length, fifo_duration_high_res_clock, fifo_duration_clock, fifo_duration_rdtsc] = import_core_core_raw_file('report_fifo_array_raw.csv', start_row, end_row);

start_row = 2;
end_row = inf;
[single_array_length_avg, single_latency_avg, single_latency_stddev, single_transaction_rate, single_data_rate_avg] = import_core_core_file('report_single_array.csv', start_row, end_row);
[dual_array_length_avg, dual_latency_avg, dual_latency_stddev, dual_transaction_rate, dual_data_rate_avg] = import_core_core_file('report_dual_array.csv', start_row, end_row);
[fifo_array_length_avg, fifo_latency_avg, fifo_latency_stddev, fifo_transaction_rate, fifo_data_rate_avg] = import_core_core_file('report_fifo_array.csv', start_row, end_row);

%% Scale Durations
single_latency = single_duration_high_res_clock.*1000000./data_len;
dual_latency = (dual_duration_high_res_clock.*1000000./data_len);
fifo_latency = fifo_duration_high_res_clock.*1000000./(data_len/2); % Div by 2 because transaction increments counter twice


%% Calculate Throughput
single_data_rate = (bits_per_element.*single_array_length.*data_len./1000000)./(single_duration_high_res_clock./1000);
dual_data_rate = (bits_per_element.*dual_array_length.*data_len./1000000)./(dual_duration_high_res_clock./1000);
fifo_data_rate = (bits_per_element.*fifo_array_length.*(data_len/2)./1000000)./(fifo_duration_high_res_clock./1000);


%% Plot
figure;
%subplot(3, 1, 1);
yyaxis left;
boxplot(single_latency, single_array_length);
xlabel('Transaction Length (int32_t Elements)', 'Interpreter', 'none');
ylabel('Transaction Latency - One Way (ns)')
title('Core-Core Transactions with Single Shared Array');
hold all;
plot(single_array_length_avg, single_latency_avg);
yyaxis right;
% boxplot(single_data_rate, single_array_length);
plot(single_array_length_avg, single_data_rate_avg);
ylabel('Throughput (Mbps)')
hold off;
grid on;

%subplot(3, 1, 2);
figure;
yyaxis left;
boxplot(dual_latency, dual_array_length);
xlabel('Transaction Length (int32_t Elements)', 'Interpreter', 'none');
ylabel('Transaction Latency - One Way (ns)')
title('Core-Core Transactions with Dual Shared Arrays');
hold all;
plot(dual_array_length_avg, dual_latency_avg);
yyaxis right;
% boxplot(dual_data_rate, dual_array_length);
plot(dual_array_length_avg, dual_data_rate_avg);
ylabel('Throughput (Mbps)')
hold off;
grid on;

%subplot(3, 1, 3);
figure;
yyaxis left;
boxplot(fifo_latency, fifo_array_length);
xlabel('Transaction Length (int32_t Elements)', 'Interpreter', 'none');
ylabel('Transaction Latency - Round Trip (ns)')
title('Core-Core FIFO Transactions with Ack');
hold all;
plot(fifo_array_length_avg, fifo_latency_avg);
yyaxis right;
% boxplot(fifo_data_rate, fifo_array_length);
plot(fifo_array_length_avg, fifo_data_rate_avg);
ylabel('Throughput (Mbps)')
hold off;
grid on;

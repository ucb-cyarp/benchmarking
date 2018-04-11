%% Init
clear; close all; clc;

%% Data Import
start_row = 2;
end_row = 65;
[single_array_length, single_latency_avg, single_latency_stddev, single_transaction_rate, single_data_rate] = import_core_core_file('report_single_array.csv', start_row, end_row);
[dual_array_length, dual_latency_avg, dual_latency_stddev, dual_transaction_rate, dual_data_rate] = import_core_core_file('report_dual_array.csv', start_row, end_row);
[fifo_array_length, fifo_latency_avg, fifo_latency_stddev, fifo_transaction_rate, fifo_data_rate] = import_core_core_file('report_fifo_array.csv', start_row, end_row);

%% Plot
figure;
%subplot(3, 1, 1);
yyaxis left;
errorbar(single_array_length, single_latency_avg, single_latency_stddev);
xlabel('Transaction Length (int32_t Elements)', 'Interpreter', 'none');
ylabel('Transaction Latency - One Way (ns)')
title('Core-Core Transactions with Single Shared Array');
hold all;
yyaxis right;
plot(single_array_length, single_data_rate);
ylabel('Throughput (Mbps)')
hold off;
grid on;

%subplot(3, 1, 2);
figure;
yyaxis left;
errorbar(dual_array_length, dual_latency_avg, dual_latency_stddev);
xlabel('Transaction Length (int32_t Elements)', 'Interpreter', 'none');
ylabel('Transaction Latency - One Way (ns)')
title('Core-Core Transactions with Dual Shared Arrays');
hold all;
yyaxis right;
ylabel('Throughput (Mbps)')
plot(dual_array_length, dual_data_rate);
hold off;
grid on;

%subplot(3, 1, 3);
figure;
yyaxis left;
errorbar(fifo_array_length, fifo_latency_avg, fifo_latency_stddev);
xlabel('Transaction Length (int32_t Elements)', 'Interpreter', 'none');
ylabel('Transaction Latency - Round Trip (ns)')
title('Core-Core FIFO Transactions with Ack');
hold all;
yyaxis right;
ylabel('Throughput (Mbps)')
plot(fifo_array_length, fifo_data_rate);
hold off;
grid on;

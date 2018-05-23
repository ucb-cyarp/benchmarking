%% Init
clear; close all; clc;

%% Data Import
start_row = 2;
end_row = 65;
[single_array_length, single_latency_avg, single_latency_stddev, single_transaction_rate, single_data_rate] = import_core_core_file('report_single_array.csv', start_row, end_row);
[dual_array_length, dual_latency_avg, dual_latency_stddev, dual_transaction_rate, dual_data_rate] = import_core_core_file('report_dual_array.csv', start_row, end_row);
[fifo_array_length, fifo_latency_avg, fifo_latency_stddev, fifo_transaction_rate, fifo_data_rate] = import_core_core_file('report_fifo_array.csv', start_row, end_row);

single_array_length_bytes = single_array_length*32/8;
dual_array_length_bytes = dual_array_length*32/8;
fifo_array_length_bytes = fifo_array_length*32/8;

single_data_rate_gbps = single_data_rate/1000;
dual_data_rate_gbps = dual_data_rate/1000;
fifo_data_rate_gbps = fifo_data_rate/1000;

%% Plot 1
figure;
%subplot(3, 1, 1);
yyaxis left;
errorbar(single_array_length_bytes, single_latency_avg, single_latency_stddev);
title('Core-Core Transfer Times & Bandwidth (Shared Array)');
hold all;

%% Plot Trends
[fits, gof] = createFits(single_array_length_bytes, single_latency_avg);
[rsqr1, rsqr2] = gof.rsquare;

h1 = plot(fits{1}, '-k');
h2 = plot(fits{2}, '--k');
hold off;

label1 = sprintf('%s\n     r^2: %f', matlab.unittest.diagnostics.ConstraintDiagnostic.getDisplayableString(fits{1}), rsqr1);
label2 = sprintf('%s\n     r^2: %f', matlab.unittest.diagnostics.ConstraintDiagnostic.getDisplayableString(fits{2}), rsqr2);

xlabel('Transaction Length (Bytes)', 'Interpreter', 'none');
ylabel('Transaction Time to Completion - One Way (ns)')

%% Plot throughput

yyaxis right;
plot(single_array_length_bytes, single_data_rate_gbps);
ylabel('Throughput (Gbps)')
hold off;
grid on;

legend([h1 h2], {sprintf('Linear Fit for Points >72 Bytes\n%s\n', label1), sprintf('\nLinear Fit for All Points\n%s\n', label2)});


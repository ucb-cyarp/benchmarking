%% Init
clear; close all; clc;

%% Data Import
start_row = 2;
end_row = inf;
[single_array_length, single_latency_avg, single_latency_stddev, single_transaction_rate, single_data_rate] = import_core_core_file('report_single_array.csv', start_row, end_row);
[flow_ctrl_array_length, flow_ctrl_latency_avg, flow_ctrl_latency_stddev, flow_ctrl_transaction_rate, flow_ctrl_data_rate] = import_core_core_file('report_flow_ctrl_array.csv', start_row, end_row);

single_array_length_bytes = single_array_length*32/8;
flow_ctrl_array_length_bytes = flow_ctrl_array_length*32/8;

single_data_rate_gbps = single_data_rate/1000;
flow_ctrl_data_rate_gbps = flow_ctrl_data_rate/1000;

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

%% Plot 2
figure;
%subplot(3, 1, 1);
yyaxis left;
errorbar(flow_ctrl_array_length_bytes, flow_ctrl_latency_avg, flow_ctrl_latency_stddev);
title('Core-Core Transfer Times & Bandwidth (Flow Control)');
hold all;

%% Plot Trends
[fits, gof] = createFits(flow_ctrl_array_length_bytes, flow_ctrl_latency_avg);
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
plot(flow_ctrl_array_length_bytes, flow_ctrl_data_rate_gbps);
ylabel('Throughput (Gbps)')
hold off;
grid on;

legend([h1 h2], {sprintf('Linear Fit for Points >72 Bytes\n%s\n', label1), sprintf('\nLinear Fit for All Points\n%s\n', label2)});

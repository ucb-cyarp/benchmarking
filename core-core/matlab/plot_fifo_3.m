%% Init
clear; close all; clc;

%% Import
reportfifoarray = importfifofile('report_fifo_array.csv', 2, 513, 512);
reportfifoarray_read_limit = importfifofile('report_fifo_read_limit_array.csv', 2, 513, 512);
reportfifoarray_blocked = importfifofile('report_fifo_blocked_array.csv', 2, 513, 512);

%% Scale Results
%Convert from Mbps to Gbps
reportfifoarray = reportfifoarray./1000;
reportfifoarray_read_limit = reportfifoarray_read_limit./1000;
reportfifoarray_blocked = reportfifoarray_blocked./1000;

minVal = min([min(min(reportfifoarray)), min(min(reportfifoarray_read_limit)), min(min(reportfifoarray_blocked))]);
maxVal = max([max(max(reportfifoarray)), max(max(reportfifoarray_read_limit)), max(max(reportfifoarray_blocked))]);

%% Plot
figure;
subplot(2,2,1);
colormap('parula')
imagesc(reportfifoarray)
caxis([minVal, maxVal])
set(gca,'YDir','normal')
xlabel('Transaction Length (int32\_t Elements)')
ylabel('Array Length (int32\_t Elements)')
title('FIFO (Write Limited) Data Transfer Rate (Gbps)')
colorbar;

subplot(2,2,2);
colormap('parula')
imagesc(reportfifoarray_read_limit)
caxis([minVal, maxVal])
set(gca,'YDir','normal')
xlabel('Block Length (int32\_t Elements)')
ylabel('Array Length (int32\_t Elements)')
title('FIFO (Read & Write Limited) Data Transfer Rate (Gbps)')
colorbar;

subplot(2,2,3);
colormap('parula')
imagesc(reportfifoarray_blocked)
caxis([minVal, maxVal])
set(gca,'YDir','normal')
xlabel('Block Length (int32\_t Elements)')
ylabel('Array Length (int32\_t Elements)')
title('FIFO (Blocked) Data Transfer Rate (Gbps)')
colorbar;
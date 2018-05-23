%% Init
clear; close all; clc;

%% Import
reportfifoarray = importfifofile('report_fifo_array.csv', 2, 513, 512);

%% Scale Results
%Convert from Mbps to Gbps
reportfifoarray = reportfifoarray./1000;

%% Plot
figure;
surf(reportfifoarray)
colormap('jet')
xlabel('Transaction Length (int32_t Elements)')
ylabel('Array Length (int32_t Elements)')
zlabel('Data Rate (Gbps)')
title('FIFO Data Transfer Rate')
colorbar;

figure;
colormap('jet')
imagesc(reportfifoarray)
set(gca,'YDir','normal')
xlabel('Transaction Length (int32_t Elements)')
ylabel('Array Length (int32_t Elements)')
title('FIFO Data Transfer Rate (Gbps)')
colorbar;

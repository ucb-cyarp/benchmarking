%% Init
clear; close all; clc;

%% Import
reportfifoarray = importfifofile('report_fifo_array.csv', 2, 130);

%% Scale Results
%Convert from Mbps to Gbps
reportfifoarray = reportfifoarray./1000;

%% Plot
figure;
surf(reportfifoarray)
colormap('jet')
xlabel('Transaction Length')
ylabel('Array Length')
zlabel('Data Rate (Gbps)')
title('FIFO Data Transfer Rate')
colorbar;

figure;
colormap('jet')
imagesc(reportfifoarray)
set(gca,'YDir','normal')
xlabel('Transaction Length')
ylabel('Array Length')
title('FIFO Data Transfer Rate (Gbps)')
colorbar;

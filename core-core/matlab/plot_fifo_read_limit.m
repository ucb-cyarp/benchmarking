%% Init
%clear; close all; clc;
clear; clc;

%% Import
reportfifoarray = importfifofile('report_fifo_read_limit_array.csv', 2, 130);

%% Scale Results
%Convert from Mbps to Gbps
reportfifoarray = reportfifoarray./1000;

%% Plot
figure;
surf(reportfifoarray)
colormap('jet')
xlabel('Block Length (int32_t Elements)')
ylabel('Array Length (int32_t Elements)')
zlabel('Data Rate (Gbps)')
title('FIFO Data Transfer Rate')
colorbar;

figure;
colormap('jet')
imagesc(reportfifoarray)
set(gca,'YDir','normal')
xlabel('Block Length (int32_t Elements)')
ylabel('Array Length (int32_t Elements)')
title('FIFO Data Transfer Rate (Gbps)')
colorbar;

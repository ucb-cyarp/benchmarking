%% Init
%clear; close all; clc;
clear; clc;

%% Import
reportfifoarray = importfifofile('report_fifo_read_limit_array.csv', 2, 130);

%% Scale Results
%Convert from Mbps to Gbps
reportfifoarray = reportfifoarray./1000;

dimensions_32b_elements = size(reportfifoarray);

x_scale = 1:dimensions_32b_elements(1);
y_scale = 1:dimensions_32b_elements(2);

x_scale_bytes = x_scale.*(32/8);
y_scale_bytes = y_scale.*(32/8);

%% Plot
figure;
surf(x_scale_bytes, y_scale_bytes, reportfifoarray)
colormap('jet')
xlabel('Block Length (Bytes)')
ylabel('Array Length (Bytes)')
zlabel('Data Rate (Gbps)')
title('FIFO Data Transfer Rate - Read & Write Limited')
colorbar;

figure;
colormap('jet')
imagesc(x_scale_bytes, y_scale_bytes, reportfifoarray)
set(gca,'YDir','normal')
xlabel('Block Length (Bytes)')
ylabel('Array Length (Bytes)')
title('FIFO Data Transfer Rate - Read & Write Limited (Gbps)')
colorbar;

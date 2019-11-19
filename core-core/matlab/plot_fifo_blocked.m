%% Init
%clear; close all; clc;

%% Plot
points = 512;

fig = plot_fifo_img({'.'}, 'report_fifo_blocked_array.csv', 'FIFO Data Transfer Rate (Complete Block Transfers)', 'Block Size (Bytes)', points);
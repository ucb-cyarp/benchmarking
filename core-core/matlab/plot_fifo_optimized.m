%% Init
%clear; close all; clc;

%% Plot
blockPoints = 149;
arrayPoints = 256;

fig = plot_fifo_img_optimized({'.'}, 'report_fifo_blocked_optimized_array.csv', 'FIFO Data Transfer Rate (Complete Block Transfers)', 'Block Size (Bytes)', blockPoints, arrayPoints);
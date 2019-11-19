%% Init
%clear; close all; clc;

%% Plot
points = 512;

fig = plot_fifo_img({'.'}, 'report_fifo_read_limit_array.csv', 'FIFO Data Transfer Rate (Read & Write Limited)', 'Max Data Written, Max Data Read in a Single Transaction (Bytes)', points);
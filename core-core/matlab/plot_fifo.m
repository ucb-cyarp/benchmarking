%% Init
%clear; close all; clc;

%% Plot
points = 512;

fig = plot_fifo_img({'.'}, 'report_fifo_array.csv', 'FIFO Data Transfer Rate (Write Limited)', 'Max Data Written in a Single Transaction (Bytes)', points);
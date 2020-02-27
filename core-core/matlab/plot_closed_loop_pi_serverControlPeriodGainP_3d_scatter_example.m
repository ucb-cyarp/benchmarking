%% Init
clear; clc;

%% Filter Params
%dirs = {'results_interruptsEnabled_noNOHZFULL_piControl', 'results_interruptsDisabled_noNOHZFULL_piControl'};
% dirs = {'results_interruptsEnabled_noNOHZFULL_piControl2', 'results_interruptsDisabled_noNOHZFULL_piControl2'};
%dirs = {'results_interruptsEnabled_noNOHZFULL_piControl3'};
%dirs = {'results_interruptsEnabled_noNOHZFULL_piControl4'};
% dirs = {'results_interruptsEnabled_noNOHZFULL_piControl_largerBlocks2', 'results_interruptsDisabled_noNOHZFULL_piControl_largerBlocks2'};
dirs = {'results_interruptsEnabled_noNOHZFULL_piControl_largerBlocks3'};


clientControlPeriod = 0;
gainI = 0;
initialNOPs = 20;
satValue = -1; % Negative for no saturation

%% Plot

plot_closed_loop_3dscatter_pi_serverControlGainP_dirs(dirs, clientControlPeriod, initialNOPs, gainI, satValue)
%% Init
clear; clc;

%% Filter Params
dirs = {'results_interruptsEnabled_noNOHZFULL_floatNOPs2', 'results_interruptsDisabled_noNOHZFULL_floatNOPs2'};
clientControlPeriod = 0;
initialNOPs = 200;
satValue = -1; % Negative for no saturation

%% Plot

plot_closed_loop_3dscatter_dirs(dirs, clientControlPeriod, initialNOPs, satValue);
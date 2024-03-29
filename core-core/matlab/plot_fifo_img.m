function [figures] = plot_fifo_img(directoryList, filename, titleTxt, xAxisTxt, points)
%PLOT_FIFO_IMG Summary of this function goes here
%   Detailed explanation goes here

figures = [];
axs = [];

minVal = 0;
maxVal = 0;

for i = 1:length(directoryList)
    fullName = [directoryList{i} '/' filename];
    reportfifoarray = importfifofile(fullName, 2, points+1, points);

    %% Scale Results
    %Convert from Mbps to Gbps
    reportfifoarray = reportfifoarray./1000;

    dimensions_32b_elements = size(reportfifoarray);

    x_scale = 1:dimensions_32b_elements(1);
    y_scale = 1:dimensions_32b_elements(2);

    x_scale_bytes = x_scale.*(32/8);
    y_scale_bytes = y_scale.*(32/8);
    
    if i == 1
        minVal = min(min(reportfifoarray));
        maxVal = max(max(reportfifoarray));
    else
        minVal = min([minVal, min(min(reportfifoarray))]);
        maxVal = max([maxVal, max(max(reportfifoarray))]);
    end

    %% Plot
    fig = figure;
    figures = [figures, fig];
    colormap('parula')
    ax = axes;
    axs = [axs, ax];
    imagesc(ax, x_scale_bytes, y_scale_bytes, reportfifoarray)
    set(ax,'YDir','normal')
    xlabel(xAxisTxt)
    ylabel('FIFO Array Length (Bytes)')
    title(titleTxt)
    cb = colorbar;
    title(cb, 'Gbps')

end

%% Scale Plots
for i = 1:length(axs)
   caxis(axs(i), [minVal, maxVal]);
end

end


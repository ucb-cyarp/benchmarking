function [fitresult, gof] = createFits(single_array_length_bytes, single_latency_avg)
%CREATEFITS(SINGLE_ARRAY_LENGTH_BYTES,SINGLE_LATENCY_AVG)
%  Create fits.
%
%  Data for 'untitled fit 1' fit:
%      X Input : single_array_length_bytes
%      Y Output: single_latency_avg
%  Data for 'untitled fit 2' fit:
%      X Input : single_array_length_bytes
%      Y Output: single_latency_avg
%  Output:
%      fitresult : a cell-array of fit objects representing the fits.
%      gof : structure array with goodness-of fit info.
%
%  See also FIT, CFIT, SFIT.

%  Auto-generated by MATLAB on 17-May-2018 13:26:21

%% Initialization.

% Initialize arrays to store fits and goodness-of-fit.
fitresult = cell( 2, 1 );
gof = struct( 'sse', cell( 2, 1 ), ...
    'rsquare', [], 'dfe', [], 'adjrsquare', [], 'rmse', [] );

%% Fit: 'untitled fit 1'.
[xData, yData] = prepareCurveData( single_array_length_bytes, single_latency_avg );

% Set up fittype and options.
ft = fittype( 'poly1' );
excludedPoints = excludedata( xData, yData, 'Indices', [1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18] );
opts = fitoptions( 'Method', 'LinearLeastSquares' );
opts.Exclude = excludedPoints;

% Fit model to data.
[fitresult{1}, gof(1)] = fit( xData, yData, ft, opts );

% Plot fit with data.
%figure( 'Name', 'untitled fit 1' );
%h = plot( fitresult{1}, xData, yData, excludedPoints );
%legend( h, 'single_latency_avg vs. single_array_length_bytes', 'Excluded single_latency_avg vs. single_array_length_bytes', 'untitled fit 1', 'Location', 'NorthEast' );
% Label axes
%xlabel single_array_length_bytes
%ylabel single_latency_avg
%grid on

%% Fit: 'untitled fit 2'.
[xData, yData] = prepareCurveData( single_array_length_bytes, single_latency_avg );

% Set up fittype and options.
ft = fittype( 'poly1' );

% Fit model to data.
[fitresult{2}, gof(2)] = fit( xData, yData, ft );

% Plot fit with data.
%figure( 'Name', 'untitled fit 2' );
%h = plot( fitresult{2}, xData, yData );
%legend( h, 'single_latency_avg vs. single_array_length_bytes', 'untitled fit 2', 'Location', 'NorthEast' );
% Label axes
%xlabel single_array_length_bytes
%ylabel single_latency_avg
%grid on



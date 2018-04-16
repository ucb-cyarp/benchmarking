function [array_length,duration_high_res_clock,duration_clock,duration_rdtsc] = import_core_core_raw_file(filename, startRow, endRow)
%IMPORTFILE Import numeric data from a text file as column vectors.
%   [ARRAY_LENGTH,DURATION_HIGH_RES_CLOCK,DURATION_CLOCK,DURATION_RDTSC] =
%   IMPORTFILE(FILENAME) Reads data from text file FILENAME for the default
%   selection.
%
%   [ARRAY_LENGTH,DURATION_HIGH_RES_CLOCK,DURATION_CLOCK,DURATION_RDTSC] =
%   IMPORTFILE(FILENAME, STARTROW, ENDROW) Reads data from rows STARTROW
%   through ENDROW of text file FILENAME.
%
% Example:
%   [array_length,duration_high_res_clock,duration_clock,duration_rdtsc] = importfile('report_single_array_raw.csv',2, 25601);
%
%    See also TEXTSCAN.

% Auto-generated by MATLAB on 2018/04/12 09:43:44

%% Initialize variables.
delimiter = ',';
if nargin<=2
    startRow = 2;
    endRow = inf;
end

%% Format for each line of text:
%   column1: double (%f)
%	column2: double (%f)
%   column3: double (%f)
%	column4: double (%f)
% For more information, see the TEXTSCAN documentation.
formatSpec = '%f%f%f%f%[^\n\r]';

%% Open the text file.
fileID = fopen(filename,'r');

%% Read columns of data according to the format.
% This call is based on the structure of the file used to generate this
% code. If an error occurs for a different file, try regenerating the code
% from the Import Tool.
dataArray = textscan(fileID, formatSpec, endRow(1)-startRow(1)+1, 'Delimiter', delimiter, 'TextType', 'string', 'EmptyValue', NaN, 'HeaderLines', startRow(1)-1, 'ReturnOnError', false, 'EndOfLine', '\r\n');
for block=2:length(startRow)
    frewind(fileID);
    dataArrayBlock = textscan(fileID, formatSpec, endRow(block)-startRow(block)+1, 'Delimiter', delimiter, 'TextType', 'string', 'EmptyValue', NaN, 'HeaderLines', startRow(block)-1, 'ReturnOnError', false, 'EndOfLine', '\r\n');
    for col=1:length(dataArray)
        dataArray{col} = [dataArray{col};dataArrayBlock{col}];
    end
end

%% Close the text file.
fclose(fileID);

%% Post processing for unimportable data.
% No unimportable data rules were applied during the import, so no post
% processing code is included. To generate code which works for
% unimportable data, select unimportable cells in a file and regenerate the
% script.

%% Allocate imported array to column variable names
array_length = dataArray{:, 1};
duration_high_res_clock = dataArray{:, 2};
duration_clock = dataArray{:, 3};
duration_rdtsc = dataArray{:, 4};


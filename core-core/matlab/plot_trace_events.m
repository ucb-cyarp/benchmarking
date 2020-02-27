%% Parameters
databaseFilename = 'out.db';
cpusOfInterest = [4, 5];
% eventsOfInterest = {'hrtimer_expire_entry'};
eventsOfInterest = {'timer_expire_entry'};
% eventsOfInterest = {'hrtimer_expire_entry', 'timer_expire_entry'};
timeRange = [];
historgramBins = 10000;

if isempty(cpusOfInterest)
    error('Need to provide CPUs of interest');
end

if isempty(eventsOfInterest)
    error('Need to provide events of interest')
end

fullPath = pwd;
[filepath,foldername,ext] = fileparts(fullPath);

%% Open Connection
conn = sqlite(databaseFilename, 'readonly');

%% List Available CPUs
cpusAvail = conn.fetch('SELECT DISTINCT cpu from trace ORDER BY cpu');
disp('CPU IDs in Trace:');
for ind = 1:length(cpusAvail)
    disp(['    ' num2str(cpusAvail{ind})])
end

%% List Available Event Types
eventTypesAvail = conn.fetch('SELECT DISTINCT event from trace ORDER BY event');
disp('Event Types in Trace:');
for ind = 1:length(eventTypesAvail)
    disp(['    ' eventTypesAvail{ind}])
end

%% Find Min and Max Extent of Time and Find Histogram Divisions
conditionStr = 'WHERE (';
for cpuInd = 1:length(cpusOfInterest)
    cpu = cpusOfInterest(cpuInd);
    if cpuInd>1
        conditionStr = [conditionStr ' OR '];
    end
    conditionStr = [conditionStr 'cpu = ' num2str(cpu)];
end

conditionStr = [conditionStr ') AND ('];

for eventInd  = 1:length(eventsOfInterest)
    event = eventsOfInterest{eventInd};
    if eventInd>1
        conditionStr = [conditionStr ' OR '];
    end
    conditionStr = [conditionStr 'event = ''' event ''''];
end

conditionStr = [conditionStr ')'];

if ~isempty(timeRange)
    conditionStr = [conditionStr ' AND (time = ' num2str(timeRange(1)) ' OR time = ' num2str(timeRange(2)) ')'];
end

selection = conn.fetch(['SELECT MIN(time) from trace ' conditionStr]);
minTime = selection{1};

selection = conn.fetch(['SELECT MAX(time) from trace ' conditionStr]);
maxTime = selection{1};

histogramStep = (maxTime - minTime)/historgramBins;
historgramEdges = ((0:historgramBins).*histogramStep)+minTime-histogramStep/2; %-histogramStep/2 is meant to center the bins on the min and max times

%% Plot
histFig = figure;
histAx = axes(histFig);

interArrivalFig = figure;
interArrivalAx = axes(interArrivalFig);

figLabels = {};

for cpuInd = 1:length(cpusOfInterest)
    cpu = cpusOfInterest(cpuInd);
    for eventInd  = 1:length(eventsOfInterest)
        event = eventsOfInterest{eventInd};
        
        if isempty(timeRange)
            %Potentially unsafe due to possibiltity of SQL Injection however the
            %DB was opened in readonly mode so this should be somewhat
            %mitigated. The sqlite fetch interface does not appear to support
            %the normal SQL Injection protection mechanisms provided by JDBC
            %I can use jdbc but it requires installing the driver
            queryStr = ['SELECT time FROM trace WHERE cpu = ' ...
                        num2str(cpu) ' AND event = ''' event ...
                        ''' ORDER BY time'];
        else
            queryStr = ['SELECT time FROM trace WHERE cpu = ' ...
                        num2str(cpu) ' AND event = ''' event ...
                        ''' AND time >= ' num2str(timeRange(1)) ...
                        ' AND time < ' num2str(timeRange(2)) ...
                        ' ORDER BY time'];
        end
        
        timelineEventsCell = conn.fetch(queryStr);
        
        timelineEvents = cell2mat(timelineEventsCell);
        
        histogram(histAx, timelineEvents, historgramEdges);
        hold(histAx, 'ALL');
        
        %Get the difference between vector elements
        interArrivalTime = diff(timelineEvents);
        plot(interArrivalAx, interArrivalTime);
        hold(interArrivalAx, 'ALL');
        
        figLabels{length(figLabels)+1} = ['CPU' num2str(cpu) ' - ' event];
    end
end

legend(histAx, figLabels, 'Interpreter', 'none');
legend(interArrivalAx, figLabels, 'Interpreter', 'none');

title(histAx, ['Event Timeline (' foldername ')'], 'Interpreter', 'none');
title(interArrivalAx, ['Event Inter-Arrival Times (' foldername ')'], 'Interpreter', 'none');

xlabel(histAx, 'Time (s)');
ylabel(histAx, 'Event Count');

xlabel(interArrivalAx, 'Event #');
ylabel(interArrivalAx, 'Inter-Arrival Time [Difference from Last Event] (s)');

%% Cleanup
conn.close();
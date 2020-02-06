%Plots the tracker vectors for an open loop fullness tracker run

%Plot & Filter Parameters
ArrayLength = 255;
BlockSize = 32;
InitNOPs = 0;
BalNOPs = 16;
PlotThreshold = 0;
Summary = true; % If a summary run, do not plot interrupts
PlotAll = true; %If true, plot every trace, not just the ones with interrupts
PlotThickLines = true;
VideoFilename = 'interrupt_cases.mp4';

%Filter Data
idxs = find( (ArrayLengthBlocks == ArrayLength) & (BlockSizeint32_tElements == BlockSize) & (InitialNOPs == InitNOPs) & (BalancingNOPs == BalNOPs) & (TransactionCore_Client >= PlotThreshold));
filterCond = ['Array Len (Blks): ' num2str(ArrayLength) ', Blk Size (32b Words): ' num2str(BlockSize) ', Init. NOPs: ' num2str(InitNOPs) ', Bal NOPs: ' num2str(BalNOPs) ', Plot Thresh: ' num2str(PlotThreshold)];

%Get a color map

colors = cool(256);

%Get the min/max range of the transactions
minTransact = min(TransactionCore_Client(idxs));
maxTransact = max(TransactionCore_Client(idxs));

%Map from [minTransact, maxTransact] to color[1:256]

%Other params

frameCount = 0;

clear figs;
clear frames;

%Plot the start tracker
for i = 1:length(idxs)
    idx = idxs(i);
    endTrackerStdServer = str2num(InterruptsStdEndCore_Server(idx));
    endTrackerLocServer = str2num(InterruptsLOCEndCore_Server(idx));
    endTrackerOtherArchServer = str2num(InterruptsOtherArchEndCore_Server(idx));
    endTrackerSoftirqServer = str2num(InterruptsSoftirqEndCore_Server(idx));
    endTrackerStdClient = str2num(InterruptsStdEndCore_Client(idx));
    endTrackerLocClient = str2num(InterruptsLOCEndCore_Client(idx));
    endTrackerOtherArchClient = str2num(InterruptsOtherArchEndCore_Client(idx));
    endTrackerSoftirqClient = str2num(InterruptsSoftirqEndCore_Client(idx));
    if PlotAll || ~isempty(find(endTrackerStdServer)) || ...
                  ~isempty(find(endTrackerLocServer)) || ...
                  ~isempty(find(endTrackerOtherArchServer)) || ...
                  ~isempty(find(endTrackerSoftirqServer)) || ...
                  ~isempty(find(endTrackerStdClient)) || ...
                  ~isempty(find(endTrackerLocClient)) || ...
                  ~isempty(find(endTrackerOtherArchClient)) || ...
                  ~isempty(find(endTrackerSoftirqClient))
                  
        frameCount = frameCount+1;
        fig = figure;
        set(fig,'Position',[200 200 1000 700])
        figs(frameCount) = fig;
        if Summary
            ax1 = subplot(2, 1, 1);
        else
            ax1 = subplot(4, 1, 1);
        end
        endTracker = importTrackerVector(FullnessEndCore_Client(idx));
        plot(ax1, endTracker);
        ylabel('Items in Buffer');
        xlabel('Iterations');
        ylim([0, ArrayLength]);
        if Summary
            title({'End Trace', filterCond, ['Transactions: ' num2str(TransactionCore_Client(idx))], ...
                                            ['Writer Trial Interrupts: ' num2str(endTrackerStdServer + endTrackerLocServer + endTrackerOtherArchServer + endTrackerSoftirqTimerServer + endTrackerSoftirqOtherServer)], ...
                                            ['Reader Trial Interrupts: ' num2str(endTrackerStdClient + endTrackerLocClient + endTrackerOtherArchClient + endTrackerSoftirqTimerClient + endTrackerSoftirqOtherClient)]});
        else
            title({'End Trace', filterCond, ['Transactions: ' num2str(TransactionCore_Client(idx))], ...
                                            ['Writer Trial Interrupts (In Window): ' num2str(sum(endTrackerStdServer) + sum(endTrackerLocServer) + sum(endTrackerOtherArchServer) + sum(endTrackerSoftirqTimerServer) + sum(endTrackerSoftirqOtherServer))], ...
                                            ['Reader Trial Interrupts (In Window): ' num2str(sum(endTrackerStdClient) + sum(endTrackerLocClient) + sum(endTrackerOtherArchClient) + sum(endTrackerSoftirqTimerClient) + sum(endTrackerSoftirqOtherClient))]});
        end
        
        if ~Summary
            ax2 = subplot(4, 1, 2);
            ax3 = subplot(4, 1, 3);
            if PlotThickLines
                plot(ax2, endTrackerStdServer,          '-', 'LineWidth', 10)
                hold (ax2, 'on');
                plot(ax2, endTrackerLocServer,          '-', 'LineWidth', 8)
                plot(ax2, endTrackerOtherArchServer,    '-', 'LineWidth', 6)
                plot(ax2, endTrackerSoftirqTimerServer, '-', 'LineWidth', 4)
                plot(ax2, endTrackerSoftirqOtherServer, '-', 'LineWidth', 2)
                hold (ax2, 'off');
                plot(ax3, endTrackerStdClient,          '-', 'LineWidth', 10)
                hold (ax3, 'on');
                plot(ax3, endTrackerLocClient,          '-', 'LineWidth', 8)
                plot(ax3, endTrackerOtherArchClient,    '-', 'LineWidth', 6)
                plot(ax3, endTrackerSoftirqTimerClient, '-', 'LineWidth', 4)
                plot(ax3, endTrackerSoftirqOtherClient, '-', 'LineWidth', 2)
                hold (ax3, 'off');
            else
                plot(ax2, endTrackerStdServer)
                hold (ax2, 'on');
                plot(ax2, endTrackerLocServer)
                plot(ax2, endTrackerOtherArchServer)
                plot(ax2, endTrackerSoftirqTimerServer)
                plot(ax2, endTrackerSoftirqOtherServer)
                hold (ax2, 'off');
                plot(ax3, endTrackerStdClient)
                hold (ax3, 'on');
                plot(ax3, endTrackerLocClient)
                plot(ax3, endTrackerOtherArchClient)
                plot(ax3, endTrackerSoftirqTimerClient)
                plot(ax3, endTrackerSoftirqOtherClient)
                hold (ax3, 'off');
            end
            hold off;
            legend(ax2, {'Writer Std', 'Writer LOC', 'Writer Other Arch', 'Writer TIMER softirq', 'Writer Other softirq'}, ...
                    'Location', 'northwest')
            ylabel(ax2, 'Interrupts');
            xlabel(ax2, 'Iterations');
            
            legend(ax3, {'Reader Std', 'Reader LOC', 'Reader Other Arch', 'Reader TIMER softirq', 'Reader Other softirq'}, ...
                    'Location', 'northwest')
            ylabel(ax3, 'Interrupts');
            xlabel(ax3, 'Iterations');
            
        end
        
        if Summary
            ax4 = subplot(2, 1, 2);
        else
            ax4 = subplot(4, 1, 4);
        end
        endTimingServer = importTrackerVector(TimingEndCore_Server(idx));
        endTimingClient = importTrackerVector(TimingEndCore_Client(idx));
        plot(ax4, endTimingServer)
        hold on;
        plot(ax4, endTimingClient)
        hold off;
        legend({'Server', 'Client'}, 'Location', 'northwest')
        ylabel('Iteration Period (s)');
        xlabel('Iterations');
        
        if Summary
            linkaxes([ax1, ax4], 'x')
        else
            linkaxes([ax1, ax2, ax3, ax4], 'x')
        end
        
        frames(frameCount) = getframe(fig);
    end
end

% implay(frames, 2);
% close(figs);

v = VideoWriter(VideoFilename, 'MPEG-4');
v.Quality = 100;
v.FrameRate = 2;
open(v);
for f = 1:length(frames)
    writeVideo(v,frames(f));
end
close(v);
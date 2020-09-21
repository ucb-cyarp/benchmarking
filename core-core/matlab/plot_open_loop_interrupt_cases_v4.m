%Plots the tracker vectors for an open loop fullness tracker run

%Plot & Filter Parameters
ArrayLength = 255;
BlockSize = 32;
InitNOPs = 0;
BalNOPs = 33;
PlotThreshold = 0;
Summary = false; % If a summary run, do not plot interrupts
PlotAll = true; %If true, plot every trace, not just the ones with interrupts
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
    endTrackerStdClient = str2num(InterruptsStdEndCore_Client(idx));
    endTrackerLocClient = str2num(InterruptsLOCEndCore_Client(idx));
    endTrackerOtherArchClient = str2num(InterruptsOtherArchEndCore_Client(idx));
    if PlotAll || ~isempty(find(endTrackerServer)) || ~isempty(find(endTrackerClient))
        frameCount = frameCount+1;
        fig = figure;
        set(fig,'Position',[200 200 1000 700])
        figs(frameCount) = fig;
        if Summary
            ax1 = subplot(2, 1, 1);
        else
            ax1 = subplot(3, 1, 1);
        end
        endTracker = importTrackerVector(FullnessEndCore_Client(idx));
        plot(ax1, endTracker);
        ylabel('Items in Buffer');
        xlabel('Iterations');
        ylim([0, ArrayLength]);
        if Summary
            title({'End Trace', filterCond, ['Transactions: ' num2str(TransactionCore_Client(idx))], ...
                                            ['Writer Trial Interrupts: ' num2str(endTrackerStdServer + endTrackerLocServer + endTrackerOtherArchServer)], ...
                                            ['Reader Trial Interrupts: ' num2str(endTrackerStdClient + endTrackerLocClient + endTrackerOtherArchClient)]});
        else
            title({'End Trace', filterCond, ['Transactions: ' num2str(TransactionCore_Client(idx))], ...
                                            ['Writer Trial Interrupts (In Window): ' num2str(sum(endTrackerStdServer) + sum(endTrackerLocServer) + sum(endTrackerOtherArchServer))], ...
                                            ['Reader Trial Interrupts (In Window): ' num2str(sum(endTrackerStdClient) + sum(endTrackerLocClient) + sum(endTrackerOtherArchClient))]});
        end
        
        if ~Summary
            ax2 = subplot(3, 1, 2);
            plot(ax2, endTrackerStdServer)
            hold on;
            plot(ax2, endTrackerLocServer)
            plot(ax2, endTrackerOtherArchServer)
            plot(ax2, endTrackerStdClient)
            plot(ax2, endTrackerLocClient)
            plot(ax2, endTrackerOtherArchClient)
            hold off;
            legend({'Writer Std', 'Writer LOC', 'Writer Other Arch', ...
                    'Reader Std', 'Reader LOC', 'Reader Other Arch'}, ...
                    'Location', 'northwest')
            ylabel('Interrupts');
            xlabel('Iterations');
        end
        
        if Summary
            ax3 = subplot(2, 1, 2);
        else
            ax3 = subplot(3, 1, 3);
        end
        endTimingServer = importTrackerVector(TimingEndCore_Server(idx));
        endTimingClient = importTrackerVector(TimingEndCore_Client(idx));
        plot(ax3, endTimingServer)
        hold on;
        plot(ax3, endTimingClient)
        hold off;
        legend({'Server', 'Client'}, 'Location', 'northwest')
        ylabel('Iteration Period (s)');
        xlabel('Iterations');
        
        if Summary
            linkaxes([ax1, ax3], 'x')
        else
            linkaxes([ax1, ax2, ax3], 'x')
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
function plot_closed_loop_pi_rateVsBlockSizes(directories, ClientControlPeriodIterationsFilter, InitialNOPsFilter, GainIFilter, GainBaseFilter, satValue, plotZeroGain)
%plot_closed_loop_3dscatter_dirs Summary of this function goes here
%   Detailed explanation goes here
if nargin<7
    plotZeroGain = false;
end

if nargin<6
    satValue = -1;
end

satValStr = '';
if(satValue>=0)
    satValStr = [', Saturated: ' num2str(satValue)];
end

for i = 1:length(directories)
    disp([directories{i} ':'])
    dirPath = ['./' directories{i} '/report_closed_loop_pi_control_raw.csv'];
    %[ArrayLengthBlocks, BlockSizeint32_tElements, ServerControlPeriodIterations, ClientControlPeriodIterations, ControlGainPNOPs, ControlGainINOPs, InitialNOPs, SteadyClockWalltimems, ~, ~, ~, ~, ~, ErroredStr, ~, TransactionCore_Client] = import_closed_loop_pi_raw_file(dirPath, [2, inf]);
    [ArrayLengthBlocks, BlockSizeint32_tElements, ServerControlPeriodIterations, ClientControlPeriodIterations, ControlGainPNOPs, ControlGainINOPs, ControlGainBaseNOPs, InitialNOPs, SteadyClockWalltimems, ~, ~, ~, ~, ~, ~, ~, ~, ErroredStr, ~, TransactionCore_Client] = import_closed_loop_pi_base_adj_raw_file(dirPath, [2, inf]);
    TransactionCore_Client_Stat = TransactionCore_Client;
    SteadyClockMs = SteadyClockWalltimems;
    
    %The element size does not change
    BlockSizeElements = BlockSizeint32_tElements;
    ElementSizeBytesSingle = 4;
    ElementSizeBytes = ElementSizeBytesSingle.*ones(size(BlockSizeElements));
    
    %Convert Errored to Bool
    Errored = true(size(ErroredStr));
    for n = 1:length(ErroredStr)
        if ErroredStr(n) == 'No' || ErroredStr(n) == 'no' || ErroredStr(n) == 'False' || ErroredStr(n) == 'false' || ErroredStr(n) == '0'
            Errored(n) = false;
        end
    end
    
    if(satValue>0)
        TransactionCore_Client_Stat(TransactionCore_Client>satValue) = satValue;
    end
    
    filterArr = (ClientControlPeriodIterations==ClientControlPeriodIterationsFilter) & (InitialNOPs == InitialNOPsFilter) & (ControlGainINOPs == GainIFilter);
    filterArrStr = ['(ClientControlPeriodIterations==', num2str(ClientControlPeriodIterationsFilter) ') && (InitialNOPs==' num2str(InitialNOPsFilter) ') && (ControlGainINOPs == ' num2str(GainIFilter) ')'];
    
    if exist('ControlGainBaseNOPs')
        filterArr = filterArr & (ControlGainBaseNOPs == GainBaseFilter);
        filterArrStr = [filterArrStr ' && (ControlGainBaseNOPs==', num2str(GainBaseFilter) ')'];
    end
    
    if ~plotZeroGain
        filterArr = filterArr & (ControlGainPNOPs ~= 0);
        filterArrStr = [filterArrStr ', Omitted Gain 0'];
    end
    
    %Get block sizes
    blockSizes = sort(unique(BlockSizeint32_tElements));
    blockSizeBits = zeros(size(blockSizes));
    ratesGbs = zeros(size(blockSizes));
    timeToTransferBlockNs = zeros(size(blockSizes));
    
    for blockSizeIdx = 1:length(blockSizes)
        BlockSize = blockSizes(blockSizeIdx);
        
        filterArrBlock = filterArr & (BlockSizeint32_tElements == BlockSize);
            
        %Calculate the average rate of the configurations that did not fail
        [idxs, ~] = find(filterArrBlock);

        erroredMapX = containers.Map('KeyType','double','ValueType','Any');
        durationSumMapX = containers.Map('KeyType','double','ValueType','Any');
        bitsTransferedSumMapX = containers.Map('KeyType','double','ValueType','Any');
        countMapX = containers.Map('KeyType','double','ValueType','Any');

        for i = 1:length(idxs)
            idx = idxs(i);
            xVal = ServerControlPeriodIterations(idx);
            yVal = ControlGainPNOPs(idx);

            if ~isKey(erroredMapX, xVal)
                erroredMapY = containers.Map('KeyType','double', 'ValueType', 'logical');
                erroredMapX(xVal) = erroredMapY;
                durationSumMapY = containers.Map('KeyType','double', 'ValueType', 'double');
                durationSumMapX(xVal) = durationSumMapY;
                bitsTransferedSumMapY = containers.Map('KeyType','double', 'ValueType', 'double');
                bitsTransferedSumMapX(xVal) = bitsTransferedSumMapY;
                countMapY = containers.Map('KeyType','double', 'ValueType', 'double');
                countMapX(xVal) = countMapY;
            else
                erroredMapY = erroredMapX(xVal);
                durationSumMapY = durationSumMapX(xVal);
                bitsTransferedSumMapY = bitsTransferedSumMapX(xVal);
                countMapY = countMapX(xVal);
            end

            ptErrored = Errored(idx);
            bytesTransfered = BlockSizeElements(idx)*ElementSizeBytes(idx)*TransactionCore_Client(idx); %Transactions are in blocks transfered for open and closed loop tests
            bitsTransfered = bytesTransfered*8;

            if ~isKey(erroredMapY, yVal)
                erroredMapY(yVal) = ptErrored;
                durationSumMapY(yVal) = SteadyClockMs(idx);
                bitsTransferedSumMapY(yVal) = bitsTransfered;
                countMapY(yVal) = 1;
            else
                ptErrored = ptErrored || erroredMapY(yVal);
                erroredMapY(yVal) = ptErrored;
                durationSumMapY(yVal) = durationSumMapY(yVal) + SteadyClockMs(idx);
                bitsTransferedSumMapY(yVal) = bitsTransferedSumMapY(yVal) + bitsTransfered;
                countMapY(yVal) = countMapY(yVal) + 1;
            end
        end

        avgX = [];
        avgY = [];
        avgDuration = [];
        avgBitsTransfered = [];
        avgErrored = [];
        avgRateGbps = [];

        xKeys = keys(erroredMapX);
        for i = 1:length(xKeys)
            xKey = xKeys{i};
            erroredMapY = erroredMapX(xKey);
            durationSumMapY = durationSumMapX(xKey);
            bitsTransferedSumMapY = bitsTransferedSumMapX(xKey);
            countMapY = countMapX(xKey);

            yKeys = keys(erroredMapY);
            for j = 1:length(yKeys)
                yKey = yKeys{j};
                erroredPt = erroredMapY(yKey);
                durationSum = durationSumMapY(yKey);
                bitsTransfered = bitsTransferedSumMapY(yKey);
                count = countMapY(yKey);

                avgX = [avgX, xKey];
                avgY = [avgY, yKey];
                avgDurationPt = durationSum/count;
                avgDuration = [avgDuration, avgDurationPt];
                avgBitsTransferedPt = bitsTransfered/count;
                avgBitsTransfered = [avgBitsTransfered, avgBitsTransferedPt];
                avgErrored = [avgErrored, erroredPt];
                if erroredPt
                    avgRateGbps = [avgRateGbps, 0];
                else
                    gbpsPt = avgBitsTransferedPt/(avgDurationPt*1e6);
                    avgRateGbps = [avgRateGbps, gbpsPt];
                end
            end
        end

        [~, maxDataRateIdx] = max(avgRateGbps);
        
        blockSizeBits(blockSizeIdx) = BlockSize*ElementSizeBytesSingle*8;
        if ~isempty(maxDataRateIdx)
            ratesGbs(blockSizeIdx) = avgRateGbps(maxDataRateIdx);
            timeToTransferBlockNs(blockSizeIdx) = blockSizeBits(blockSizeIdx)/ratesGbs(blockSizeIdx);
        end
    end
    
    figure;
    plot(blockSizeBits, timeToTransferBlockNs);
    xlabel('Block Size (Bits)');
    ylabel('Time to Transfer Block (Avg - ns)');
    
    figure;
    plot(blockSizeBits, ratesGbs);
    xlabel('Block Size (Bits)');
    ylabel('Data Rate (Avg - Gbps)');
    
    disp('Block Size (Bits):');
    disp(mat2str(transpose(blockSizeBits)));
    disp('Data Rate (Gbps):');
    disp(mat2str(transpose(ratesGbs)));
    disp('Data Rate (Mbps):');
    disp(mat2str(transpose(ratesGbs.*1000)));
    disp('Time to Transfer Block (ns):');
    disp(mat2str(transpose(timeToTransferBlockNs)));
    
end
end


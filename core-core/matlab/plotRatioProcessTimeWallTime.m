[ArrayLengthBlocks, BlockSizeint32_tElements, BalancingNOPs, InitialNOPs, CheckPeriod, SteadyClockWalltimems, ClockCyclesCycleTimems, Clockrdtsc, ExpectedBlockIDCore_Client, StartBlockIDCore_Client, EndBlockIDCore_Client, ErroredCore_Client, ErrorSourceCore_Client, TransactionCore_Client, FullnessStartCore_Client, FullnessEndCore_Client, ExpectedBlockIDCore_Server, StartBlockIDCore_Server, EndBlockIDCore_Server, ErroredCore_Server, ErrorSourceCore_Server, TransactionCore_Server] = import_open_loop_fullness_tracker_all_cols("report_open_loop_fullness_tracker_raw.csv");

%filter
ArrayLen = 255;
BalancingNOP = 37;

%Min wallclock
%It appears that the process clock is unreliable below a certain run length
MinWallClock = 1e3; % ms

idx = find( (ArrayLengthBlocks == ArrayLen) & (BlockSizeint32_tElements == 32) & (InitialNOPs == 0) & (BalancingNOPs == BalancingNOP) & (SteadyClockWalltimems >= MinWallClock) );
filterCond = ['(ArrayLengthBlocks == ' num2str(ArrayLen) ') & (BlockSizeint32_tElements == 32) & (InitialNOPs == 0) & (BalancingNOPs == ' num2str(BalancingNOP) ') & (SteadyClockWalltimems >= ' num2str(MinWallClock) ')'];

procTimeToWallTimeRatio = ClockCyclesCycleTimems(idx)./SteadyClockWalltimems(idx);

figure;
histogram(procTimeToWallTimeRatio);
ylabel('Trials');
xlabel('Ratio of Process Time To Wallclock Time (Expect 2)');
title({'Ratio of Process Time To Wallclock Time', filterCond}, 'Interpreter', 'none');
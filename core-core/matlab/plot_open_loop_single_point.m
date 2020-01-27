%Plot a single point without tracking

% [ArrayLengthBlocks, BlockSizeint32_tElements, BalancingNOPs, InitialNOPs, SteadyClockWalltimems, ExpectedBlockIDCore_Client, StartBlockIDCore_Client, EndBlockIDCore_Client, ErroredCore_Client, TransactionCore_Client] = import_open_loop_raw_file('report_open_loop_raw');
% 
% %filter
ArrayLen = 255;
% 
% idx = find( (ArrayLengthBlocks == ArrayLen) & (BlockSizeint32_tElements == 32) & (InitialNOPs == 0) );
% filterCond = ['(ArrayLengthBlocks == ' num2str(ArrayLen) ') & (BlockSizeint32_tElements == 32) & (InitialNOPs == 0)'];
% 
% figure;
% s = scatter(BalancingNOPs(idx), TransactionCore_Client(idx), 30, [0 0.4470 0.7410], 'filled');
% s.MarkerFaceAlpha = 0.2;
% title({'Without Tracking', filterCond}, 'Interpreter', 'none');
% 
% %Plot a single point with tracking
% %[ArrayLengthBlocks, BlockSizeint32_tElements, BalancingNOPs, InitialNOPs, CheckPeriod, SteadyClockWalltimems, ClockCyclesCycleTimems, Clockrdtsc, ExpectedBlockIDCore_Server, StartBlockIDCore_Server, EndBlockIDCore_Server, ErroredCore_Server, ErrorSourceCore_Server, TransactionCore_Server, TransactionCore_Client, FullnessStartCore_Client, FullnessEndCore_Client] = import_open_loop_fullness_tracker_all_cols_v2('report_open_loop_raw');
% [ArrayLengthBlocks, BlockSizeint32_tElements, BalancingNOPs, InitialNOPs, CheckPeriod, SteadyClockWalltimems, ClockCyclesCycleTimems, Clockrdtsc, ExpectedBlockIDCore_Server, StartBlockIDCore_Server, EndBlockIDCore_Server, ErroredCore_Server, ErrorSourceCore_Server, TransactionCore_Server, ExpectedBlockIDCore_Client, StartBlockIDCore_Client, EndBlockIDCore_Client, ErroredCore_Client, ErrorSourceCore_Client, TransactionCore_Client, FullnessStartCore_Client, FullnessEndCore_Client] = import_open_loop_fullness_v2('report_open_loop_fullness_tracker_raw.csv');
% %[ArrayLengthBlocks, BlockSizeint32_tElements, BalancingNOPs, InitialNOPs, CheckPeriod, SteadyClockWalltimems, ClockCyclesCycleTimems, Clockrdtsc, ExpectedBlockIDCore_Server, StartBlockIDCore_Server, EndBlockIDCore_Server, ErroredCore_Server, ErrorSourceCore_Server, TransactionCore_Server, InterruptsStartCore_Server, InterruptsEndCore_Server, TimingStartCore_Server, TimingEndCore_Server, ExpectedBlockIDCore_Client, StartBlockIDCore_Client, EndBlockIDCore_Client, ErroredCore_Client, ErrorSourceCore_Client, TransactionCore_Client, FullnessStartCore_Client, FullnessEndCore_Client, InterruptsStartCore_Client, InterruptsEndCore_Client, TimingStartCore_Client, TimingEndCore_Client] = import_open_loop_fullness_v3('report_open_loop_fullness_tracker_raw.csv');
idx = find( (ArrayLengthBlocks == ArrayLen) & (BlockSizeint32_tElements == 32) & (InitialNOPs == 0) );
filterCond = ['(ArrayLengthBlocks == ' num2str(ArrayLen) ') & (BlockSizeint32_tElements == 32) & (InitialNOPs == 0)'];

figure;
s = scatter(BalancingNOPs(idx), TransactionCore_Client(idx), 30, [0 0.4470 0.7410], 'filled');
s.MarkerFaceAlpha = 0.2;
title({'With Tracking', filterCond}, 'Interpreter', 'none');
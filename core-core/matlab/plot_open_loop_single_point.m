%Plot a single point without tracking

[ArrayLengthBlocks, BlockSizeint32_tElements, BalancingNOPs, InitialNOPs, SteadyClockWalltimems, ExpectedBlockIDCore_Client, StartBlockIDCore_Client, EndBlockIDCore_Client, ErroredCore_Client, TransactionCore_Client] = import_open_loop_raw_file('report_open_loop_raw');

%filter

idx = find( (ArrayLengthBlocks == 28) & (BlockSizeint32_tElements == 32) & (InitialNOPs == 0) );
filterCond = '(ArrayLengthBlocks == 28) & (BlockSizeint32_tElements == 32) & (InitialNOPs == 0)';

figure;
s = scatter(BalancingNOPs(idx), TransactionCore_Client(idx), 30, [0 0.4470 0.7410], 'filled');
s.MarkerFaceAlpha = 0.2;
title({'Without Tracking', filterCond}, 'Interpreter', 'none');

%Plot a single point with tracking
[ArrayLengthBlocks, BlockSizeint32_tElements, BalancingNOPs, InitialNOPs, CheckPeriod, SteadyClockWalltimems, ExpectedBlockIDCore_Client, StartBlockIDCore_Client, EndBlockIDCore_Client, ErroredCore_Client, TransactionCore_Client, FullnessStartCore_Client, FullnessEndCore_Client] = import_open_loop_fullness_tracker_raw_file('report_open_loop_fullness_tracker_raw');
idx = find( (ArrayLengthBlocks == 28) & (BlockSizeint32_tElements == 32) & (InitialNOPs == 0) );
filterCond = '(ArrayLengthBlocks == 28) & (BlockSizeint32_tElements == 32) & (InitialNOPs == 0)';

figure;
s = scatter(BalancingNOPs(idx), TransactionCore_Client(idx), 30, [0 0.4470 0.7410], 'filled');
s.MarkerFaceAlpha = 0.2;
title({'With Tracking', filterCond}, 'Interpreter', 'none');
% Plot Open Loop Raw Scatter

[ArrayLengthBlocks, BlockSizeint32_tElements, BalancingNOPs, InitialNOPs, SteadyClockWalltimems, ExpectedBlockIDCore_Client, StartBlockIDCore_Client, EndBlockIDCore_Client, ErroredCore_Client, TransactionCore_Client] = import_open_loop_raw_file('report_open_loop_raw');

figure; 
scatter3(ArrayLengthBlocks, BlockSizeint32_tElements, TransactionCore_Client, 75, BalancingNOPs); 
xlabel('Array Length (Blocks)'); 
ylabel('Block Size (4 Byte Words)'); 
zlabel('Transactions Before Failure (Blocks)'); 
axis vis3d;
c = colorbar; 
ylabel(c, 'NOPs Per Itteration')

figure
scatter3(ArrayLengthBlocks, BlockSizeint32_tElements, TransactionCore_Client, 75, TransactionCore_Client, 'filled'); 
xlabel('Array Length (Bytes)'); 
ylabel('Block Size (Bytes)'); 
zlabel('Transactions Before Failure (Blocks)'); 
axis vis3d;
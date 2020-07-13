%% Init
clear; clc;

%% Filter Params
%dirs = {'results_interruptsEnabled_noNOHZFULL_piControl', 'results_interruptsDisabled_noNOHZFULL_piControl'};
%dirs = {'results_interruptsEnabled_noNOHZFULL_piControl2', 'results_interruptsDisabled_noNOHZFULL_piControl2'};
%dirs = {'results_interruptsEnabled_noNOHZFULL_piControl3'};
%dirs = {'results_interruptsEnabled_noNOHZFULL_piControl4'};
% dirs = {'results_interruptsEnabled_noNOHZFULL_piControl_largerBlocks2', 'results_interruptsDisabled_noNOHZFULL_piControl_largerBlocks2'};
% dirs = {'results_interruptsEnabled_noNOHZFULL_piControl_largerBlocks3'};
% dirs = {'results_interruptsEnabled_noNOHZFULL_piControl_largerBlocks4'};
%dirs = {'results_interruptsEnabled_noNOHZFULL_piControl_evenLargerBlocks'};
% dirs = {'results_baseAdjust2'};
%dirs = {'results_interruptsEnabled_noNOHZFULL_CXX'};
%dirs = {'results_interruptsEnabled_noNOHZFULL_NUMA'};
%dirs = {'results_interruptsEnabled_noNOHZFULL_NUMA2', 'results_interruptsDisabled_noNOHZFULL_NUMA2'};
%dirs = {'results_interruptsEnabled_noNOHZFULL_NUMA3'};
% dirs = {'results_blockSizeSweep_ccx'};

% dirs = {'results_closedLoop_fixedLengths_longerTrials_revertArray'};
% dirs = {'results_closedLoop_fixedLengths_longerTrials_revertArray_forwardRead'};
% dirs = {'closed_loop_forward_read_content1'};
% dirs = {'realigned_align4'};
% dirs = {'closed_loop_forward_read_content_reorg_avx'};
% dirs = {'closed_loop_forward_read_content_reorg_avx2'};
% dirs = {'closed_loop_forward_read_content_reorg_avx3'};
% dirs = {'closed_loop_forward_read_content_reorg_avx3_largerBlocks'};
% dirs = {'closed_loop_forward_read_content_reorg_avx4'};
% dirs = {'closed_loop_forward_read_content_reorg_avx4_sweepSize'};
% dirs = {'closed_loop_forward_read_content_reorg_avx5_largerBlock'};
% dirs = {'closed_loop_forward_read_content_reorg_avxSemiAligned_largerBlock'};
% dirs = {'closed_loop_forward_read_content_reorg_avxUnaligned_largerBlock'};
% dirs = {'closed_loop_forward_read_content_reorg_avxUnaligned_extLocal_largerBlock'};
% dirs = {'closed_loop_forward_read_content_reorg_avxUnaligned_extLocal_largerBlock2'};
% dirs = {'closed_loop_forward_read_content_reorg_avxSemiAligned_extLocal_largerBlock'};
% dirs = {'closed_loop_forward_read_content_reorg_avxSemiAligned_extLocal_largerBlock2'};
% dirs = {'closed_loop_forward_read_content_reorg_avxUnlignedRampOutFix_extLocal_largerBlock'};
% dirs = {'closed_loop_forward_read_content_reorg_avxSemiAlignedRampOutFix_extLocal_largerBlock'};
% dirs = {'closed_loop_forward_read_content_reorg_avxSemiAlignedRampInFix_extLocal_largerBlock2'};
% dirs = {'closed_loop_forward_read_content_reorg_avxSemiAlignedSrcFix_extLocal_largerBlock'}; % Probably the best so far with IDs.  Not entirely suprising as it forces alignment
% dirs = {'closed_loop_forward_read_content_reorg_avxUnalignedRampDownFix_extLocal_largerBlock3'};
% dirs = {'closed_loop_forward_read_content_reorg_avxAlignedFix_extLocal_largerBlock'};
% dirs = {'closed_loop_forward_read_content_reorg_avxAligned_noIDs_extLocal_largerBlock'};
% dirs = {'closed_loop_forward_read_content_reorg_avxAligned_noIDs_extLocal_largerBlock144'}; %There is overhead for alligning here and no alignment should be nessisary with this block size.  Explains why it is not the best (but still performs reasonably well)
% dirs = {'closed_loop_forward_read_content_reorg_avxUnalignedRampIn_noIDs_extLocal_largerBlock144'}; % Best So Far for No IDs
% dirs = {'closed_loop_forward_read_content_reorg_avxUnalignedRampOut_noIDs_extLocal_largerBlock144'};
% dirs = {'closed_loop_forward_read_content_reorg_avxSemiAlignedDst_noIDs_extLocal_largerBlock144'};
% dirs = {'closed_loop_forward_read_content_reorg_avxUnalignedRampIn_noIDs_fixedSizes_extLocal_largerBlock144'}; %Tried fixed sizes again.  Not bad but still bellow the max I had seen.
% dirs = {'closed_loop_forward_read_content_reorg_avxUnalignedRampIn_noIDs_extLocal_largerBlock144_2'}; % Best So Far for No IDs
% dirs = {'closed_loop_forward_read_content_reorg_avxUnalignedRampIn_extLocal_largerBlock142'};
% dirs = {'closed_loop_forward_read_content_reorg_avxAlignedMultiple_noIDs_extLocal_largerBlock144'}; % Very weird, this did not help and in fact made things worse.  Could be bad optimizations by the compiler
% dirs = {'closed_loop_forward_read_content_reorg_Simple_noIDs_extLocal_largerBlock144'}; %This is a pass at just letting the compiler try to optimize the copy.  Not as bad as some but definitly not great
% dirs = {'closed_loop_forward_read_content_reorg_SimpleSimplifiedPointers_noIDs_extLocal_largerBlock144'}; %Same as above except pointer cast to elements immediatly.  Idea was that it would potentially help the compiler.  Had very poor results.  Should double check
% dirs = {'closed_loop_forward_read_content_reorg_avxAlignedFix_extLocal_noBlockCheck_largerBlock142'}; %No block content checking but ID checking
% dirs = {'closed_loop_forward_read_content_reorg_avxUnalignedRampIn_noIDs_extLocal_noBlockCheck_largerBlock144'}; %No block content checking or ID checking.  
                                                                                                                 %Relies on the server to perform error checking.  
                                                                                                                 %Does well but not substantially better than 
                                                                                                                 %closed_loop_forward_read_content_reorg_avxUnalignedRampIn_noIDs_extLocal_largerBlock144.  
                                                                                                                 %No IDs seems to help a fair amount but not checking the array seems to be more marginal.  
                                                                                                                 %This makes some degree of sense as it could be done out of order and could.
                                                                                                                 %Note there are some runs that report very good results but they look like errors to me
                                                                                                                 %that just were not caught
                                                                                                                 
% dirs = {'closed_loop_forward_read_content_reorg_avxUnalignedRampIn_extLocal_noBlockCheck_block32'};
% dirs = {'closed_loop_forward_read_content_reorg_avxUnalignedRampIn_noIDs_extLocal_noBlockCheck_block32'};
% dirs = {'closed_loop_forward_read_content_reorg_avxUnalignedRampIn_noIDs_extLocal_block144_ccx'};
% dirs = {'closed_loop_forward_read_content_reorg_avxUnalignedRampIn_noIDs_extLocal_block144'};
% dirs = {'closed_loop_forward_read_content_reorg_avxUnalignedRampIn_noIDs_extLocal_block144_numa'};

% dirs = {'results-reorg/fast_copy_unaligned_ramp_in/blockSize144'};
% dirs = {'results-reorg/fast_copy_unaligned_ramp_in/blockSize144_ccx'};
% dirs = {'results-reorg/fast_copy_unaligned_ramp_in/blockSize144_numa'};

% dirs = {'results-reorg/fast_copy_aligned/blockSize144'};
% dirs = {'results-reorg/fast_copy_aligned/blockSize144_ccx'};
% dirs = {'results-reorg/fast_copy_aligned/blockSize144_numa'};

% dirs = {'results-reorg-writerFastCopy/fast_copy_unaligned_ramp_in/blockSize144'};
% dirs = {'results-reorg-writerFastCopy/fast_copy_unaligned_ramp_in/blockSize144_ccx'};
% dirs = {'results-reorg-writerFastCopy/fast_copy_unaligned_ramp_in/blockSize144_numa'};

% dirs = {'results-reorg-writerFastCopy-memcpy/memcpy/blockSize144'};
% dirs = {'results-reorg-writerFastCopy-memcpy/memcpy/blockSize144_ccx'};
% dirs = {'results-reorg-writerFastCopy-memcpy/memcpy/blockSize144_numa'};

% dirs = {'results-noIDs-writerFastCopy/fast_copy_unaligned_ramp_in/blockSize144'};
% dirs = {'results-noIDs-writerFastCopy/fast_copy_unaligned_ramp_in/blockSize144_ccx'};
% dirs = {'results-noIDs-writerFastCopy/fast_copy_unaligned_ramp_in/blockSize144_numa'};

% dirs = {'blockSweepFull'};
% dirs = {'blockSweepFull2'};
% dirs = {'blockSweepFull2LongerRuns'};
% dirs = {'blockSweepFull_WriterIndividualElementsForLoop'};
% dirs = {'blockSweepFull_FastCopyInt'};
% dirs = {'blockSweepFull_FastCopyInt_smallerInitNOP'};
% dirs = {'blockSweepFull_FastCopyInt_smallerInitNOP_increasedBaseGain'};

% dirs = {'blockSweepFull_FastCopyInt_ErrorCheckPartOfControl_ClientControlPeriod10'};
% dirs = {'blockSweepFull_FastCopyInt_ErrorCheckPartOfControl'};
% dirs = {'blockSweepFull_FastCopyInt_ErrorCheckPartOfControl_NoBreakOnCheck'};
% dirs = {'blockSweepFull_FastCopyInt_ErrorCheckPartOfControl_NoBreakOnCheck_shortRun'}; %Did not reboot, also signed in to vscode
% dirs = {'blockSweepFull_FastCopyInt_ErrorCheckPartOfControl_shortRun'}; % Did not reboot, also signed in to vscode
% dirs = {'blockSweepFull_FastCopyInt_ErrorCheckPartOfControl_shortRun2'};

% dirs = {'blockSweepFull_config_FastCopyInt'};
% dirs = {'blockSweepFull_config_FastCopyInt_IDs_NoContentCheck'};
% dirs = {'blockSweepFull_config_FastCopyInt_NoIDs_ContentCheck'};
% dirs = {'blockSweepFull_config_FastCopyInt_NoIDs_NoContentCheck'};

% dirs = {'blockSweepFull_config_FastCopyInt_IDs_NoContentCheck_expanded'};
% dirs = {'blockSweepFull_config_FastCopyInt_IDs_NoContentCheck_expanded_ccx'};
% dirs = {'blockSweepFull_config_FastCopyInt_IDs_NoContentCheck_expanded2_ccx'};
% dirs = {'blockSweepFull_config_FastCopyInt_IDs_NoContentCheck_expanded2_numa'};
% dirs = {'blockSweepFull_config_FastCopyInt_IDs_NoContentCheck_expanded3_numa'};
dirs = {'blockSweepFull_config_FastCopyInt_IDs_NoContentCheck_expanded4_numa'};

%It looks like throughput caps out at around 54 Gbps for no IDs with 144*4
%byte blocks and 255 (plus extra element) long array
                                                                                                                 
clientControlPeriod = 0;
gainI = 0;
% initialNOPs = 130;
initialNOPs = 260;
% initialNOPs = 20;
% initialNOPs = 10;
gainBase = 0.01;
% BlockSizeElements = 144; %Negative to plot all
BlockSizeElements = -1; %Negative to plot all
satValue = -1; % Negative for no saturation

%% Plot

% plot_closed_loop_3dscatter_pi_serverControlGainP_dirs(dirs, clientControlPeriod, initialNOPs, gainI, gainBase, BlockSizeElements, satValue)
plot_closed_loop_pi_rateVsBlockSizes(dirs, clientControlPeriod, initialNOPs, gainI, gainBase, satValue);
BUILD_DIR=build
DEPENDS_DIR=../depends
COMMON_DIR=../common
SRC_DIR=.

#Parameters that can be supplied to this makefile
USE_PCM=1
USE_AMDuPROF=1
USE_SCHED_FIFO=1

#Get Cache Sizes
# L1D_CACHE_SIZE = $(shell ./get_l1d_cache_size.py)
# L2_CACHE_SIZE = $(shell ./get_l2_cache_size.py)
# L3_CACHE_SIZE = $(shell ./get_l3_cache_size.py)

UNAME:=$(shell uname)

#Compiler Parameters
#CXX=g++
#CXX = $(MPICXX)
#Main Benchmark file is not optomized to avoid timing code being re-organized
CFLAGS = -O3 -c -g -std=c++11 -march=native -masm=att
#Most kernels are allowed to be optomized.  Most assembly kernels use asm 'volitile' to force execution
KERNEL_CFLAGS = -O3 -c -g -std=c++11 -march=native -masm=att
#For kernels that should not be optimized, the following is used
KERNEL_NO_OPT_CFLAGS = -O0 -c -g -std=c++11 -march=native -masm=att
INC=-I $(COMMON_DIR)
LIB_DIRS=-L $(COMMON_DIR)
LIB=-pthread -lrt -lProfilerCommon -latomic

TEST_CLASSES = -DTEST_SINGLE=1 -DTEST_FIFO=1 -DTEST_SIMULTANIOUS=1 -DTEST_FANIN_FANOUT=1 -DTEST_OPEN_LOOP=1 -DTEST_CLOSED_LOOP=1

#Will accept default defines
DEFINES=-DWRITE_CSV=1
DEFINES+= $(TEST_CLASSES)
#DEFINES = #-DL1D_CACHE_SIZE=${L1D_CACHE_SIZE} -DL2_CACHE_SIZE=${L2_CACHE_SIZE} -DL3_CACHE_SIZE=${L3_CACHE_SIZE}
#DEFINES = $(TEST_CLASSES) -DPRINT_TRIALS=1
#DEFINES = $(TEST_CLASSES) -DPRINT_FREQ_CHANGE_EVENT=1
#DEFINES = $(TEST_CLASSES) -DPRINT_FREQ_CHANGE_EVENT=1 -DPRINT_TRIALS=1 -DSTIM_LEN=2000000 -DTRIALS=20
#DEFINES = $(TEST_CLASSES) -DSTIM_LEN=16000000 -DTRIALS=20
#DEFINES = $(TEST_CLASSES) -DSTIM_LEN=10000000 -DTRIALS=10 -DMAX_DISCARD=4000
#DEFINES = $(TEST_CLASSES) -DSTIM_LEN=1000000 -DTRIALS=10 -DMAX_DISCARD=4000
#DEFINES = $(TEST_CLASSES) -DSTIM_LEN=200000 -DTRIALS=10 -DMAX_DISCARD=20000

DEPENDS=
DEPENDS_LIB=$(COMMON_DIR)/libProfilerCommon.a

ifeq ($(USE_PCM), 1)
DEFINES+= -DUSE_PCM=1
DEPENDS+= $(DEPENDS_DIR)/pcm/Makefile
DEPENDS_LIB+= $(DEPENDS_DIR)/pcm/libPCM.a
INC+= -I $(DEPENDS_DIR)/pcm
LIB_DIRS+= -L $(DEPENDS_DIR)/pcm
#Need an additional include directory if on MacOS.
#Using the technique in pcm makefile to detect MacOS
UNAME:=$(shell uname)
ifeq ($(UNAME), Darwin)
INC+= -I $(DEPENDS_DIR)/pcm/MacMSRDriver
LIB+= -lPCM -lPcmMsr
APPLE_DRIVER = $(DEPENDS_DIR)/pcm/MacMSRDriver/build/Release/libPcmMsr.dylib
LIB_DIRS+= -L $(DEPENDS_DIR)/pcm/MacMSRDriver/build/Release
else
LIB+= -lrt -lPCM
APPLE_DRIVER = 
endif
else
DEFINES+= -DUSE_PCM=0
endif

ifeq ($(USE_AMDuPROF), 1)
DEFINES+= -DUSE_AMDuPROF=1
LIB+= -lAMDPowerProfileAPI
else
DEFINES+= -DUSE_AMDuPROF=0
endif

ifeq ($(USE_SCHED_FIFO), 1)
DEFINES+= -DUSE_SCHED_FIFO=1
else
DEFINES+= -DUSE_SCHED_FIFO=0
endif

MAIN_FILE = core-core.cpp
KERNEL_SRCS = latency_single_kernel.cpp latency_dual_kernel.cpp latency_single_array_kernel.cpp latency_dual_array_kernel.cpp latency_flow_ctrl_kernel.cpp latency_flow_ctrl_blocked_read_kernel.cpp bandwidth_circular_fifo_kernel.cpp bandwidth_circular_fifo_blocked_kernel.cpp bandwidth_circular_fifo_read_limit_kernel.cpp bandwidth_circular_fifo_blocked_cachedptr_kernel.cpp
LIB_SRCS = reporting_helpers.cpp fifo_helpers.cpp open_loop_helpers.cpp print_results.cpp kernel_server_runner.cpp single_kernels.cpp array_kernels.cpp fifo_kernels.cpp open_loop_kernels.cpp closed_loop_kernels.cpp closed_loop_helpers.cpp fifoless_helpers.cpp open_loop_fullness_tracker.cpp

SRCS=$(MAIN_FILE)
SRCS+=$(LIB_SRCS)
OBJS=$(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))
KERNEL_OBJS=$(patsubst %.cpp,$(BUILD_DIR)/%.o,$(KERNEL_SRCS))
KERNEL_NO_OPT_OBJS=$(patsubst %.cpp,$(BUILD_DIR)/%.o,$(KERNEL_NO_OPT_SRCS))

#Production
all: core-core
#all: 

core-core: $(OBJS) $(KERNEL_OBJS) $(KERNEL_NO_OPT_OBJS) $(DEPENDS_LIB)
	$(CXX) $(INC) $(LIB_DIRS) -o core-core $(OBJS) $(KERNEL_OBJS) $(KERNEL_NO_OPT_OBJS) $(LIB)

$(KERNEL_NO_OPT_OBJS): $(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp | $(BUILD_DIR)/ $(DEPENDS)
	$(CXX) $(KERNEL_NO_OPT_CFLAGS) $(INC) $(DEFINES) -o $@ $<

$(KERNEL_OBJS): $(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp | $(BUILD_DIR)/ $(DEPENDS)
	$(CXX) $(KERNEL_CFLAGS) $(INC) $(DEFINES) -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)/ $(DEPENDS)
	$(CXX) $(CFLAGS) $(INC) $(DEFINES) -o $@ $<

$(BUILD_DIR)/:
	mkdir -p $@

$(DEPENDS_DIR)/pcm/Makefile:
	git submodule update --init --recursive $(DEPENDS_DIR)/pcm

$(DEPENDS_DIR)/pcm/libPCM.a: $(DEPENDS_DIR)/pcm/Makefile
	cd $(DEPENDS_DIR)/pcm; make CXX=$(CXX) CC=$(CC) libPCM.a

$(COMMON_DIR)/libProfilerCommon.a:
	cd $(COMMON_DIR); make CXX=$(CXX) CC=$(CC) USE_PCM=$(USE_PCM) USE_AMDuPROF=$(USE_AMDuPROF) USE_SCHED_FIFO=${USE_SCHED_FIFO}
	
clean:
	rm -f core-core
	rm -rf build/*

# print_cfg:
# 	@echo "L1D_CACHE_SIZE = ${L1D_CACHE_SIZE}"
# 	@echo "L2_CACHE_SIZE  = ${L2_CACHE_SIZE}"
# 	@echo "L3_CACHE_SIZE  = ${L3_CACHE_SIZE}"

#.PHONY: clean print_cfg
.PHONY: clean

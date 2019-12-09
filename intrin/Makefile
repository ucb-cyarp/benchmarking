BUILD_DIR=build
DEPENDS_DIR=../depends
COMMON_DIR=../common
SRC_DIR=.

#Parameters that can be supplied to this makefile
USE_PCM=1
USE_AMDuPROF=1
USE_SCHED_FIFO=0

#Get Cache Sizes
# L1D_CACHE_SIZE = $(shell ./get_l1d_cache_size.py)
# L2_CACHE_SIZE = $(shell ./get_l2_cache_size.py)
# L3_CACHE_SIZE = $(shell ./get_l3_cache_size.py)

UNAME:=$(shell uname)

#Compiler Parameters
#CXX=g++
#Main Benchmark file is not optomized to avoid timing code being re-organized
CFLAGS = -O0 -c -g -std=c++11 -march=native -masm=att
#Most kernels are allowed to be optomized.  Most assembly kernels use asm 'volitile' to force execution
KERNEL_CFLAGS = -O3 -c -g -std=c++11 -march=native -masm=att
#For kernels that should not be optimized, the following is used
KERNEL_NO_OPT_CFLAGS = -O0 -c -g -std=c++11 -march=native -masm=att
INC=-I $(COMMON_DIR)
LIB_DIRS=-L $(COMMON_DIR)
LIB=-pthread -lProfilerCommon

DEFINES= #-DL1D_CACHE_SIZE=${L1D_CACHE_SIZE} -DL2_CACHE_SIZE=${L2_CACHE_SIZE} -DL3_CACHE_SIZE=${L3_CACHE_SIZE}
#DEFINES = -DPRINT_TRIALS=1

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

MAIN_FILE = benchmark_throughput_test.cpp
LIB_SRCS = micro_bench.cpp #These files are not optomized. micro_bench it calls the kernel runner (which starts the timers by calling functions in the profilers).  Re-ordering code is not desired
KERNEL_SRCS = store_kernel.cpp load_store_kernel.cpp load_add_store_kernel.cpp load_mult_store_kernel.cpp load_div_store_kernel.cpp load_fma_store_kernel.cpp load_add_store_unroll2_kernel.cpp load_add_store_nointrin_kernel.cpp add_kernel_asm.cpp add_kernel_asm_unroll2.cpp add_kernel_asm_unroll4.cpp add_kernel_asm_unroll8.cpp add_kernel_asm_unroll120.cpp div_kernel_asm.cpp div_kernel_asm_unroll2.cpp div_kernel_asm_unroll4.cpp div_kernel_asm_unroll120.cpp fma_kernel_asm.cpp fma_kernel_asm_unroll2.cpp fma_kernel_asm_unroll4.cpp fma_kernel_asm_unroll8.cpp fma_kernel_asm_unroll120.cpp mult_kernel_asm.cpp mult_kernel_asm_unroll2.cpp mult_kernel_asm_unroll4.cpp mult_kernel_asm_unroll8.cpp mult_kernel_asm_unroll120.cpp load_kernel_asm.cpp load_kernel_asm_local.cpp load_kernel_asm_unroll2.cpp load_kernel_asm_local_unroll2.cpp load_kernel_asm_local_unroll120.cpp store_kernel_asm.cpp store_kernel_asm_local.cpp add_kernel_scalar_asm.cpp mult_kernel_scalar_asm.cpp mult_kernel_scalar_asm_unroll2.cpp mult_kernel_scalar_asm_unroll4.cpp mult_kernel_scalar_asm_unroll120.cpp add_kernel_scalar_asm_unroll2.cpp add_kernel_scalar_asm_unroll4.cpp add_kernel_scalar_asm_unroll6.cpp add_kernel_scalar_asm_unroll120.cpp store_kernel_asm_unroll2.cpp store_kernel_asm_local_unroll2.cpp store_kernel_asm_local_unroll120.cpp div_kernel_scalar_asm.cpp div_kernel_scalar_asm_unroll2.cpp div_kernel_scalar_asm_unroll120.cpp fctn_call_kernel.cpp zero_arg_fctn.cpp load_kernel_scalar_asm.cpp load_kernel_scalar_asm_local.cpp store_kernel_scalar_asm.cpp store_kernel_scalar_asm_local.cpp
#KERNEL_NO_OPT_SRCS = load_kernel.cpp add_kernel.cpp mult_kernel.cpp div_kernel.cpp fma_kernel.cpp
KERNEL_NO_OPT_SRCS = 

SRCS=$(MAIN_FILE)
SRCS+=$(LIB_SRCS) #This file is not optomized because it calls the kernel runner (which starts the timers).  Re-ordering code is not desired
OBJS=$(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))
KERNEL_OBJS=$(patsubst %.cpp,$(BUILD_DIR)/%.o,$(KERNEL_SRCS))
KERNEL_NO_OPT_OBJS=$(patsubst %.cpp,$(BUILD_DIR)/%.o,$(KERNEL_NO_OPT_SRCS))

#Production
all: micro_bench_throughput_test

micro_bench_throughput_test: $(OBJS) $(KERNEL_OBJS) $(KERNEL_NO_OPT_OBJS) $(DEPENDS_LIB)
	$(CXX) $(INC) $(LIB_DIRS) -o micro_bench_throughput_test $(OBJS) $(KERNEL_OBJS) $(KERNEL_NO_OPT_OBJS) $(LIB)

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
	cd $(DEPENDS_DIR)/pcm; make libPCM.a

$(COMMON_DIR)/libProfilerCommon.a:
	cd $(COMMON_DIR); make USE_PCM=$(USE_PCM) USE_AMDuPROF=$(USE_AMDuPROF) USE_SCHED_FIFO=${USE_SCHED_FIFO}
	
clean:
	rm -f micro_bench_throughput_test
	rm -rf build/*

# print_cfg:
# 	@echo "L1D_CACHE_SIZE = ${L1D_CACHE_SIZE}"
# 	@echo "L2_CACHE_SIZE  = ${L2_CACHE_SIZE}"
# 	@echo "L3_CACHE_SIZE  = ${L3_CACHE_SIZE}"

#.PHONY: clean print_cfg
.PHONY: clean

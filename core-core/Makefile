BUILD_DIR=build
DEPENDS_DIR=../depends
COMMON_DIR=../common
SRC_DIR=.

#Get Cache Sizes
# L1D_CACHE_SIZE = $(shell ./get_l1d_cache_size.py)
# L2_CACHE_SIZE = $(shell ./get_l2_cache_size.py)
# L3_CACHE_SIZE = $(shell ./get_l3_cache_size.py)

#Compiler Parameters
CXX=g++
#CXX = $(MPICXX)
CFLAGS = -O0 -c -g -std=c++11 -march=native -masm=att
KERNEL_CFLAGS = -O3 -c -g -std=c++11 -march=native -masm=att
KERNEL_NO_OPT_CFLAGS = -O0 -c -g -std=c++11 -march=native -masm=att
INC=-I $(DEPENDS_DIR)/pcm -I $(COMMON_DIR)
LIB_DIRS=-L $(DEPENDS_DIR)/pcm -L $(COMMON_DIR)
LIB=-pthread -lrt -lpcmHelper -lPCM
#DEFINES = #-DL1D_CACHE_SIZE=${L1D_CACHE_SIZE} -DL2_CACHE_SIZE=${L2_CACHE_SIZE} -DL3_CACHE_SIZE=${L3_CACHE_SIZE}
#DEFINES = -DPRINT_TRIALS=1
#DEFINES = -DPRINT_FREQ_CHANGE_EVENT=1
#DEFINES = -DPRINT_FREQ_CHANGE_EVENT=1 -DPRINT_TRIALS=1 -DSTIM_LEN=2000000 -DTRIALS=20
#DEFINES = -DSTIM_LEN=16000000 -DTRIALS=20
DEFINES = -DSTIM_LEN=400000 -DTRIALS=40

MAIN_FILE = core-core.cpp
KERNEL_SRCS = latency_single_kernel.cpp latency_dual_kernel.cpp latency_single_array_kernel.cpp latency_dual_array_kernel.cpp latency_fifo_kernel.cpp

SRCS=$(MAIN_FILE)
SRCS+=$(LIB_SRCS)
OBJS=$(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))
KERNEL_OBJS=$(patsubst %.cpp,$(BUILD_DIR)/%.o,$(KERNEL_SRCS))

#Production
all: core-core
#all: 

core-core: $(OBJS) $(KERNEL_OBJS) $(KERNEL_NO_OPT_OBJS) $(COMMON_DIR)/libpcmHelper.a $(DEPENDS_DIR)/pcm/libPCM.a 
	$(CXX) $(INC) $(LIB_DIRS) -o core-core $(OBJS) $(KERNEL_OBJS) $(KERNEL_NO_OPT_OBJS) $(LIB)

$(KERNEL_OBJS): $(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp | $(BUILD_DIR)/ $(DEPENDS_DIR)/pcm/Makefile
	$(CXX) $(KERNEL_CFLAGS) $(INC) $(DEFINES) -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)/ $(DEPENDS_DIR)/pcm/Makefile
	$(CXX) $(CFLAGS) $(INC) $(DEFINES) -o $@ $<

$(BUILD_DIR)/:
	mkdir -p $@

$(DEPENDS_DIR)/pcm/Makefile:
	git submodule update --init --recursive $(DEPENDS_DIR)/pcm

$(DEPENDS_DIR)/pcm/libPCM.a: $(DEPENDS_DIR)/pcm/Makefile
	cd $(DEPENDS_DIR)/pcm; make libPCM.a

$(COMMON_DIR)/libpcmHelper.a: $(DEPENDS_DIR)/pcm/Makefile $(DEPENDS_DIR)/pcm/libPCM.a
	cd $(COMMON_DIR); make
	
clean:
	rm -f core-core
	rm -rf build/*

# print_cfg:
# 	@echo "L1D_CACHE_SIZE = ${L1D_CACHE_SIZE}"
# 	@echo "L2_CACHE_SIZE  = ${L2_CACHE_SIZE}"
# 	@echo "L3_CACHE_SIZE  = ${L3_CACHE_SIZE}"

#.PHONY: clean print_cfg
.PHONY: clean

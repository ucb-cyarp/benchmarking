BUILD_DIR=build
DEPENDS_DIR=depends
SRC_DIR=.

#Get Cache Sizes
L1D_CACHE_SIZE = $(shell ./get_l1d_cache_size.py)
L2_CACHE_SIZE = $(shell ./get_l2_cache_size.py)
L3_CACHE_SIZE = $(shell ./get_l3_cache_size.py)

#Compiler Parameters
CXX=g++
#CXX = $(MPICXX)
CFLAGS = -O0 -c -g -std=c++11 -march=native
KERNEL_CFLAGS = -O3 -c -g -std=c++11 -march=native
KERNEL_NO_OPT_CFLAGS = -O0 -c -g -std=c++11 -march=native
INC=
LIB=
DEFINES= -DL1D_CACHE_SIZE=${L1D_CACHE_SIZE} -DL2_CACHE_SIZE=${L2_CACHE_SIZE} -DL3_CACHE_SIZE=${L3_CACHE_SIZE}

MAIN_FILE = intrin_throughput_test.cpp 
LIB_SRCS = statistics.cpp
KERNEL_SRCS = store_kernel.cpp load_store_kernel.cpp load_add_store_kernel.cpp load_mult_store_kernel.cpp load_div_store_kernel.cpp load_fma_store_kernel.cpp load_add_store_unroll2_kernel.cpp load_add_store_nointrin_kernel.cpp
KERNEL_NO_OPT_SRCS = load_kernel.cpp add_kernel.cpp mult_kernel.cpp div_kernel.cpp fma_kernel.cpp

SRCS=$(MAIN_FILE)
SRCS+=$(LIB_SRCS)
OBJS=$(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))
KERNEL_OBJS=$(patsubst %.cpp,$(BUILD_DIR)/%.o,$(KERNEL_SRCS))
KERNEL_NO_OPT_OBJS=$(patsubst %.cpp,$(BUILD_DIR)/%.o,$(KERNEL_NO_OPT_SRCS))

#Production
all: intrin_throughput_test
#all: ray_tracer ray_tracer_mpi pb2obj_txt obj2boost $(PB_REDUCE_FILE) $(CLUSTER_GRAPH_FILE) $(GRAPH_GEN_FILE) $(CLUSTER_AN_FILE)

intrin_throughput_test: $(OBJS) $(KERNEL_OBJS) $(KERNEL_NO_OPT_OBJS)
	$(CXX) $(INC) -o intrin_throughput_test $(OBJS) $(KERNEL_OBJS) $(KERNEL_NO_OPT_OBJS) $(LIB)

$(KERNEL_NO_OPT_OBJS): $(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp | $(BUILD_DIR)/ $(DEPENDS_DIR)/pcm/libPCM.a
	$(CXX) $(KERNEL_NO_OPT_CFLAGS) $(INC) $(DEFINES) -o $@ $<

$(KERNEL_OBJS): $(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp | $(BUILD_DIR)/
	$(CXX) $(KERNEL_CFLAGS) $(INC) $(DEFINES) -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)/ $(DEPENDS_DIR)/pcm/libPCM.a
	$(CXX) $(CFLAGS) $(INC) $(DEFINES) -o $@ $<

$(BUILD_DIR)/:
	mkdir -p $@

$(DEPENDS_DIR)/pcm/Makefile:
	git submodule update --init --recursive depends/pcm

$(DEPENDS_DIR)/pcm/libPCM.a: $(DEPENDS_DIR)/pcm/Makefile
	cd $(DEPENDS_DIR)/pcm; make libPCM.a
	
clean:
	rm -f intrin_throughput_test
	rm -rf build/*

print_cfg:
	@echo "L1D_CACHE_SIZE = ${L1D_CACHE_SIZE}"
	@echo "L2_CACHE_SIZE  = ${L2_CACHE_SIZE}"
	@echo "L3_CACHE_SIZE  = ${L3_CACHE_SIZE}"

.PHONY: clean print_cfg

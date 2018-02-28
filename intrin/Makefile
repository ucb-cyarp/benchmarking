BUILD_DIR=build
SRC_DIR=.

#Compiler Parameters
CXX=g++
#CXX = $(MPICXX)
CFLAGS = -O0 -c -std=c++11 -march=native -mavx2
KERNEL_CFLAGS = -O3 -c -std=c++11 -march=native -mavx2
INC=
LIB=
DEFINES=

MAIN_FILE = intrin_throughput_test.cpp 
LIB_SRCS = statistics.cpp load_add_store.cpp load_mult_store.cpp load_add_store_unroll2.cpp
KERNEL_SRCS = load_add_store_kernel.cpp

SRCS=$(MAIN_FILE)
SRCS+=$(LIB_SRCS)
OBJS=$(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))
KERNEL_OBJS=$(patsubst %.cpp,$(BUILD_DIR)/%.o,$(KERNEL_SRCS))
KERNEL_SRCS_FULLPATH=$(patsubst %.cpp,$(SRC_DIR)/%.cpp,$(KERNEL_SRCS))

#Production
all: intrin_throughput_test
#all: ray_tracer ray_tracer_mpi pb2obj_txt obj2boost $(PB_REDUCE_FILE) $(CLUSTER_GRAPH_FILE) $(GRAPH_GEN_FILE) $(CLUSTER_AN_FILE)

intrin_throughput_test: $(OBJS) $(KERNEL_OBJS)
	$(CXX) $(INC) -o intrin_throughput_test $(OBJS) $(KERNEL_OBJS) $(LIB)

$(KERNEL_OBJS): $(KERNEL_SRCS_FULLPATH) | $(BUILD_DIR)/ 
	$(CXX) $(KERNEL_CFLAGS) $(INC) $(DEFINES) -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)/ 
	$(CXX) $(CFLAGS) $(INC) $(DEFINES) -o $@ $<

$(BUILD_DIR)/:
	mkdir -p $@


clean:
	rm -f intrin_throughput_test
	rm -rf build/*

.PHONY: clean
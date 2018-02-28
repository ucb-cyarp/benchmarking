BUILD_DIR=build
SRC_DIR=.

#Compiler Parameters
CXX=g++
#CXX = $(MPICXX)
CFLAGS = -O3 -c -std=c++11 -march=native -mavx2
INC=
LIB=
DEFINES=

MAIN_FILE = intrin_throughput_test.cpp 
LIB_SRCS = statistics.cpp load_add_store.cpp

SRCS=$(MAIN_FILE)
SRCS+=$(LIB_SRCS)
OBJS=$(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))

#Production
all: intrin_throughput_test
#all: ray_tracer ray_tracer_mpi pb2obj_txt obj2boost $(PB_REDUCE_FILE) $(CLUSTER_GRAPH_FILE) $(GRAPH_GEN_FILE) $(CLUSTER_AN_FILE)

intrin_throughput_test: $(OBJS)
	$(CXX) $(INC) -o intrin_throughput_test $(OBJS) $(LIB)


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)/ 
	$(CXX) $(CFLAGS) $(INC) $(DEFINES) -o $@ $<

$(BUILD_DIR)/:
	mkdir -p $@


clean:
	rm -f intrin_throughput_test
	rm -rf build/*

.PHONY: clean

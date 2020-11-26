BUILD_DIR=build
SRC_DIR=.

UNAME:=$(shell uname)

#Compiler Parameters
CC=clang
#Main Benchmark file is not optomized to avoid timing code being re-organized
CFLAGS = -O3 -c -g -std=c++11 -march=native -masm=att
LIB=-pthread -lrt -latomic

SRCS=core-core-single-rx.c core-core-single-threadhelpers.c core-core-single-tx.c core-core-singleside.c telemetry_helpers.c vitisNumaAllocHelpers.c
OBJS=$(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))

#Production
all: core-core-singleside
#all: 

core-core-singleside: $(OBJS)
	$(CC) -o core-core-singleside $(OBJS) $(LIB)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)/ 
	$(CC) $(CFLAGS) -o $@ $<

$(BUILD_DIR)/:
	mkdir -p $@
	
clean:
	rm -f core-core
	rm -rf build/*

# print_cfg:
# 	@echo "L1D_CACHE_SIZE = ${L1D_CACHE_SIZE}"
# 	@echo "L2_CACHE_SIZE  = ${L2_CACHE_SIZE}"
# 	@echo "L3_CACHE_SIZE  = ${L3_CACHE_SIZE}"

#.PHONY: clean print_cfg
.PHONY: clean

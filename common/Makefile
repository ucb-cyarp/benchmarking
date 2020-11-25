BUILD_DIR=build
DEPENDS_DIR=../depends
SRC_DIR=.

#Parameters that can be supplied to this makefile
USE_PCM=1
USE_AMDuPROF=1

#Compiler Parameters
CXX=g++
#CXX = $(MPICXX)
CFLAGS = -O3 -c -g -std=c++11 -march=native -masm=att
LIB_CFLAGS = -O0 -c -g -std=c++11 -march=native -masm=att
INC=
LIB_DIRS=
LIB=-pthread
DEFINES=

LIB_NAME = libProfilerCommon.a
SRCS = results.cpp measurement.cpp mallocHelpers.cpp
LIB_SRCS = profiler.cpp generic_profiler.cpp #These files are not optomized since they deal with starting timers and reordering is not desired

DEPENDS=

ifeq ($(USE_PCM), 1)
LIB_SRCS+= pcm_profiler.cpp
DEFINES+= -DUSE_PCM=1
DEPENDS+= $(DEPENDS_DIR)/pcm/Makefile
INC+= -I $(DEPENDS_DIR)/pcm
LIB_DIRS+= -L $(DEPENDS_DIR)/pcm
LIB+= -lrt -lPCM
#Need an additional include directory if on MacOS.
#Using the technique in pcm makefile to detect MacOS
UNAME:=$(shell uname)
ifeq ($(UNAME), Darwin)
INC+= -I $(DEPENDS_DIR)/pcm/MacMSRDriver
APPLE_DRIVER = $(DEPENDS_DIR)/pcm/MacMSRDriver/build/Release/libPcmMsr.dylib
else
APPLE_DRIVER =
endif
else
DEFINES+= -DUSE_PCM=0
endif

ifeq ($(USE_AMDuPROF), 1)
LIB_SRCS+= amdUprof_profiler.cpp
DEFINES+= -DUSE_AMDuPROF=1
else
DEFINES+= -DUSE_AMDuPROF=0
endif

OBJS=$(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))
LIB_OBJS=$(patsubst %.cpp,$(BUILD_DIR)/%.o,$(LIB_SRCS))

#Production
all: $(LIB_NAME)

$(LIB_NAME): $(OBJS) $(LIB_OBJS) $(APPLE_DRIVER)
	ar rcs $(LIB_NAME) $(OBJS) $(LIB_OBJS)

$(LIB_OBJS): $(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)/ $(DEPENDS)
	$(CXX) $(LIB_CFLAGS) $(INC) $(DEFINES) -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)/ $(DEPENDS)
	$(CXX) $(CFLAGS) $(INC) $(DEFINES) -o $@ $<

$(BUILD_DIR)/:
	mkdir -p $@

$(DEPENDS_DIR)/pcm/Makefile:
	git submodule update --init --recursive $(DEPENDS_DIR)/pcm

$(DEPENDS_DIR)/pcm/libPCM.a: $(DEPENDS_DIR)/pcm/Makefile
	cd $(DEPENDS_DIR)/pcm; make libPCM.a

$(DEPENDS_DIR)/pcm/MacMSRDriver/build/Release/libPcmMsr.dylib: $(DEPENDS_DIR)/pcm/Makefile
	cd $(DEPENDS_DIR)/pcm/MacMSRDriver; make library
	
clean:
	rm -f $(LIB_NAME)
	rm -rf build/*

.PHONY: clean

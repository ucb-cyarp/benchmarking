BUILD_DIR=build
DEPENDS_DIR=../depends
SRC_DIR=.

#Compiler Parameters
CXX=g++
#CXX = $(MPICXX)
CFLAGS = -O3 -c -g -std=c++11 -march=native -masm=att
INC=-I $(DEPENDS_DIR)/pcm
LIB_DIRS=-L $(DEPENDS_DIR)/pcm
LIB=-pthread -lrt -lPCM
DEFINES=

#Need an additional include directory if on MacOS.
#Using the technique in pcm makefile to detect MacOS
UNAME:=$(shell uname)
ifeq ($(UNAME), Darwin)
INC+= -I $(DEPENDS_DIR)/pcm/MacMSRDriver
APPLE_DRIVER = $(DEPENDS_DIR)/pcm/MacMSRDriver/build/Release/libPcmMsr.dylib
else
APPLE_DRIVER = 
endif

LIB_NAME = libpcmHelper.a
SRCS = pcm_helper.cpp profiler.cpp generic_profiler.cpp pcm_profiler.cpp results.cpp measurement.cpp 
LIB_SRCS = 

SRCS+=$(LIB_SRCS)
OBJS=$(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))

#Production
all: $(LIB_NAME)

$(LIB_NAME): $(OBJS) $(APPLE_DRIVER)
	ar rcs $(LIB_NAME) $(OBJS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)/ $(DEPENDS_DIR)/pcm/Makefile
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

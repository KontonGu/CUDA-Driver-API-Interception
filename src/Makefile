
# path to CUDA installation
CUDA_PATH ?= /usr/local/cuda
PWD := $(shell pwd)
PREFIX ?= $(PWD)/..


SMS ?=  60 61 70 75 80
GENCODE_FLAGS += $(foreach sm,$(SMS),-gencode arch=compute_$(sm),code=sm_$(sm))

CXX ?= g++ 
NVCC ?= $(CUDA_PATH)/bin/nvcc -ccbin $(CXX)

CUDA_LDFLAGS += -lcuda -L$(CUDA_PATH)/lib64 -L$(CUDA_PATH)/lib64/stubs 
LDFLAGS += -ldl -lrt 
TH_FLAGS += -lpthread
CUDA_INCLUDE += -I$(CUDA_PATH)/include -I./
UTIL_INCLUDE += -I../util/ -I./

ENABLE_DEBUG = -DENABLE_DEBUG
# ENABLE_DEBUG = -DDISABLE_DEBUG

BUILD_FOLDER += ../build

CXXFLAGS += -std=c++11 -fPIC 

CXX_OPT = -O3


cuhook_api_show.o: cuhook_api_show.cpp cuhook_api_show.h log_debug.h 
	$(NVCC) -m64 --compiler-options "$(CXXFLAGS)" $(GENCODE_FLAGS) -o $@ -c $< ${ENABLE_DEBUG} 

libcuhook_api_show.so.1: cuhook_api_show.o
	$(NVCC) -shared -m64 $(GENCODE_FLAGS) -o $@ $^ $(CUDA_LDFLAGS) $(LDFLAGS) ${CXX_OPT}
	mkdir -p $(PREFIX)/lib
	cp $@ $(PREFIX)/lib
	mv $@ ${BUILD_FOLDER}/
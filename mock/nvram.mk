NVRAM_MOCK := libnvram.so
NVRAM_SRCS := mock/nvram.c
NVRAM_OBJS := $(NVRAM_SRCS:.c=.o)
NVRAM_MAKE := $(lastword $(MAKEFILE_LIST))

$(NVRAM_MOCK): $(NVRAM_OBJS)
	$(CXX) -shared -o $@ $<


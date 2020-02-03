LIB_SRCS := process_nvram.cc dnsmasq_config.cc host_info.cc
LIB_OBJS := $(LIB_SRCS:.cc=.o)

EXE_SRCS := main.cc
EXE_OBJS := $(EXE_SRCS:.cc=.o)

TEST_SRCS := host_info.cc process_nvram.cc test.cc

DIST := $(BUILD_APP)-$(BUILD_HOST)-$(BUILD_STRING)

all: $(BUILD_APP)

include mock/nvram.mk
include version.mk

$(BUILD_APP): $(LIB_OBJS) $(EXE_OBJS) $(NVRAM_MOCK)
	$(TGT_CXX) -o $@ $(EXE_OBJS) $(LIB_OBJS) $(TGT_LDFLAGS)
	$(TGT_STRIP) $@

clean:
	rm -rf *.o $(BUILD_APP) *.tgz .dist version.h

test: $(TEST_SRCS)
	$(HOSTCXX) $(CXXFLAGS) -o $@ $^

dist: all
	mkdir -p .dist/bin
	mkdir -p .dist/etc/init.d
	mkdir -p .dist/lib/ipkg/info
	cp $(BUILD_APP).rc .dist/etc/init.d/S50$(BUILD_APP)
	chmod a+x .dist/etc/init.d/S50$(BUILD_APP)
	cp $(BUILD_APP).control .dist/lib/ipkg/info/$(BUILD_APP).control
	cp $(BUILD_APP) .dist/bin/$(BUILD_APP)
	tar -C .dist -zcvf $(DIST).tgz .

%.o: %.cc Makefile version.h
	$(TGT_CXX) -c $(TGT_CXXFLAGS) -o $@ $<


CC  := arm-linux-gnueabi-gcc-8
CXX := arm-linux-gnueabi-g++-8
LD  := arm-linux-gnueabi-ld

LIB_SRCS := process_nvram.cc dnsmasq_config.cc host_info.cc
LIB_OBJS := $(LIB_SRCS:.cc=.o)

EXE_SRCS := main.cc
EXE_OBJS := $(EXE_SRCS:.cc=.o)

TEST_SRCS := host_info.cc process_nvram.cc test.cc

CXXFLAGS := -std=c++17 -Wall -O9
LDFLAGS := -lstdc++fs -L. -Wl,-rpath,/lib -lnvram

BUILD_YEAR    := $(shell date +%Y)
BUILD_MONTH   := $(shell date +%-m)
BUILD_DAY     := $(shell date +%-d)
BUILD_WEEK    := $(shell date +%V)
BUILD_WEEKDAY := $(shell date +%-u)
BUILD_HOUR    := $(shell date +%-H)
BUILD_MINUTE  := $(shell date +%-M)
BUILD_STAMP   := $(shell date +%s)

BUILD_REC_MAJOR := $(shell echo $$(( $(BUILD_YEAR) - 2017)) )
BUILD_REC_MINOR := $(BUILD_WEEK)
BUILD_REC_PATCH := $(shell echo $$(( $(BUILD_WEEKDAY) * 24 * 6 + $(BUILD_HOUR) * 6 + $(BUILD_MINUTE) / 10 )) )

BUILD_MAJOR     := 1
BUILD_MINOR     := 12
BUILD_PATCH     := 978

APP  := dnsmasq-surrogate
HOST := $(shell hostname)
DIST := $(APP)-$(HOST)-$(BUILD_MAJOR).$(BUILD_MINOR).$(BUILD_PATCH)

all: $(APP)

include mock/nvram.mk

$(APP): $(LIB_OBJS) $(EXE_OBJS) $(NVRAM_MOCK)
	$(CXX) -o $@ $(EXE_OBJS) $(LIB_OBJS) $(LDFLAGS)

clean:
	rm -rf *.o $(APP) *.tgz .dist version.h

test: $(TEST_SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^

dist: all
	mkdir -p .dist/bin
	mkdir -p .dist/etc/init.d
	mkdir -p .dist/lib/ipkg/info
	cp $(APP).rc .dist/etc/init.d/S50$(APP)
	chmod a+x .dist/etc/init.d/S50$(APP)
	cp $(APP).control .dist/lib/ipkg/info/$(APP).control
	cp $(APP) .dist/bin/$(APP)
	tar -C .dist -zcvf $(DIST).tgz .

%.o: %.cc Makefile version.h
	$(CXX) -c $(CXXFLAGS) -o $@ $<

version.h: version.h.template
	@echo "Building version : $(BUILD_MAJOR).$(BUILD_MINOR).$(BUILD_PATCH)"
	@echo "Recommending     : $(BUILD_REC_MAJOR).$(BUILD_REC_MINOR).$(BUILD_REC_PATCH)"
	@cp $< $@
	@sed -i'' "s/{BUILD_YEAR}/$(BUILD_YEAR)/g" $@
	@sed -i'' "s/{BUILD_MONTH}/$(BUILD_MONTH)/g" $@
	@sed -i'' "s/{BUILD_DAY}/$(BUILD_DAY)/g" $@
	@sed -i'' "s/{BUILD_WEEK_DAY}/$(BUILD_WEEKDAY)/g" $@
	@sed -i'' "s/{BUILD_WEEK}/$(BUILD_WEEK)/g" $@
	@sed -i'' "s/{BUILD_HOUR}/$(BUILD_HOUR)/g" $@
	@sed -i'' "s/{BUILD_MINUTE}/$(BUILD_MINUTE)/g" $@
	@sed -i'' "s/{BUILD_STAMP}/$(BUILD_STAMP)/g" $@
	@sed -i'' "s/{BUILD_MAJOR}/$(BUILD_MAJOR)/g" $@
	@sed -i'' "s/{BUILD_MINOR}/$(BUILD_MINOR)/g" $@
	@sed -i'' "s/{BUILD_PATCH}/$(BUILD_PATCH)/g" $@


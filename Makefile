LIB_SRCS := process_nvram.cc dnsmasq_config.cc host_info.cc
LIB_OBJS := $(LIB_SRCS:.cc=.o)

EXE_SRCS := main.cc
EXE_OBJS := $(EXE_SRCS:.cc=.o)

CXXFLAGS := -std=c++17 -Wall -O9
LDFLAGS := -lstdc++fs -lnvram -lwlcsm -lshared

APP := dnsmasq-surrogate

all: $(APP)

$(APP): $(LIB_OBJS) $(EXE_OBJS)
	g++ -o $@ $^ $(LDFLAGS)

clean:
	rm -rf *.o $(APP) *.tgz .dist version.h

dist: all
	mkdir -p .dist/bin
	mkdir -p .dist/etc/init.d
	mkdir -p .dist/lib/ipkg/info
	cp $(APP).rc .dist/etc/init.d/S50$(APP)
	chmod a+x .dist/etc/init.d/S50$(APP)
	cp $(APP).control .dist/lib/ipkg/info/$(APP).control
	cp $(APP) .dist/bin/$(APP)
	tar -C .dist -zcvf $(APP).tgz .

%.o: %.cc Makefile version.h
	$(CXX) -c $(CXXFLAGS) -o $@ $<

version.h: version.h.template
	cp $< $@
	sed -i'' "s/{BUILD_YEAR}/$$(date +%Y)/g" $@
	sed -i'' "s/{BUILD_MONTH}/$$(date +%-m)/g" $@
	sed -i'' "s/{BUILD_DAY}/$$(date +%-d)/g" $@
	sed -i'' "s/{BUILD_WEEK_DAY}/$$(date +%-u)/g" $@
	sed -i'' "s/{BUILD_WEEK}/$$(date +%V)/g" $@
	sed -i'' "s/{BUILD_HOUR}/$$(date +%-H)/g" $@
	sed -i'' "s/{BUILD_MINUTE}/$$(date +%-M)/g" $@
	sed -i'' "s/{BUILD_STAMP}/$$(date +%s)/g" $@

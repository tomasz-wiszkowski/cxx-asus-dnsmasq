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
	rm -rf *.o $(APP) *.tgz .dist

dist: all
	mkdir -p .dist/bin
	mkdir -p .dist/etc/init.d
	mkdir -p .dist/lib/ipkg/info
	cp $(APP).rc .dist/etc/init.d/S50$(APP)
	chmod a+x .dist/etc/init.d/S50$(APP)
	cp $(APP).control .dist/lib/ipkg/info/$(APP).control
	cp $(APP) .dist/bin/$(APP)
	tar -C .dist -zcvf $(APP).tgz .


%.o: %.cc Makefile
	$(CXX) -c $(CXXFLAGS) -o $@ $<



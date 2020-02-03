export BUILD_MAJOR  := 1
export BUILD_MINOR  := 20
export BUILD_PATCH  := 0
export BUILD_APP    := dnsmasq-surrogate
export BUILD_STRING := $(BUILD_MAJOR).$(BUILD_MINOR).$(BUILD_PATCH)

export TARGET       := arm-linux-gnueabi
export TGT_CC       := $(TARGET)-gcc-9
export TGT_CXX      := $(TARGET)-g++-9
export TGT_LD       := $(TARGET)-ld
export TGT_STRIP    := $(TARGET)-strip
export CXXFLAGS     := -std=c++17 -Wall -O9
export TGT_CXXFLAGS := $(CXXFLAGS)
export LDFLAGS      :=
export TGT_LDFLAGS  := $(LDFLAGS) -lstdc++fs -L. -Wl,-rpath,/lib -lnvram

DIST := $(BUILD_APP)-$(BUILD_STRING)

dist: all
	@mkdir -p data/bin
	@cp src/$(BUILD_APP) data/bin/$(BUILD_APP)
	@tar -C data  -cf $(DIST).tar .
	@gzip -9v $(DIST).tar

all:
	@cd src && $(MAKE) $@

clean: dist-clean
	@cd src && $(MAKE) $@

test:
	@cd src && $(MAKE) $@

dist-clean:
	@git clean -nX



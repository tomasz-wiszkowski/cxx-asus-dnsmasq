REV_SHA           := $(shell git rev-parse --short HEAD)
REV_DIRTY         := $(shell git diff --quiet HEAD && echo "" || echo "-dirty")

BUILD_HEAD      := $(REV_SHA)$(REV_DIRTY)
BUILD_STAMP     := $(shell date +%s)
BUILD_USER      := $(shell whoami)
BUILD_HOST      := $(shell hostname)

version.h: version.h.template
	@echo "Building version : $(BUILD_STRING)-$(BUILD_HEAD)"
	@cp $< $@
	@sed -i'' "s/{BUILD_STAMP}/$(BUILD_STAMP)/g" $@
	@sed -i'' "s/{BUILD_MAJOR}/$(BUILD_MAJOR)/g" $@
	@sed -i'' "s/{BUILD_MINOR}/$(BUILD_MINOR)/g" $@
	@sed -i'' "s/{BUILD_PATCH}/$(BUILD_PATCH)/g" $@
	@sed -i'' "s/{BUILD_HEAD}/$(BUILD_HEAD)/g"   $@
	@sed -i'' "s/{BUILD_USER}/$(BUILD_USER)/g"   $@
	@sed -i'' "s/{BUILD_HOST}/$(BUILD_HOST)/g"   $@


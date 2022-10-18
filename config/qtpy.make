PICO_SDK_DIR := ../pico-sdk
QTPY_SRC_DIR := $(SRC_DIR)/platform/rp2040/qtpy
QTPY_BUILD_DIR := ./build_qtpy
QTPY_MOUNT ?= /Volumes/RPI-RP2

PICO_SDK_INCLUDE_DIRS :=
PICO_SDK_INCLUDE_DIRS += $(QTPY_BUILD_DIR)/generated/pico_base
PICO_SDK_INCLUDE_DIRS += $(shell find -L $(PICO_SDK_DIR) -name include -type d | grep -v pico_stdio_usb)
PICO_SDK_INCLUDE_DIRS += $(PICO_SDK_DIR)/lib/tinyusb/src

TUSB_CONFIG_INCLUDE_DIR := $(SRC_DIR)/platform/rp2040/qtpy

QTPY_CXXFLAGS :=
QTPY_CXXFLAGS += $(patsubst %,-isystem%,$(PICO_SDK_INCLUDE_DIRS))
QTPY_CXXFLAGS += -I$(TUSB_CONFIG_INCLUDE_DIR)

CXXFLAGS += $(QTPY_CXXFLAGS)

$(BUILD_DIR)/platform/rp2040/qtpy/app.cc.o: $(QTPY_BUILD_DIR)/Makefile

$(QTPY_BUILD_DIR)/Makefile: $(SRC_DIR)/platform/rp2040/qtpy/CMakeLists.txt
	$(echo_recipe)rm -rf $(QTPY_BUILD_DIR)
	$(echo_recipe)cmake -S $(QTPY_SRC_DIR) -B $(QTPY_BUILD_DIR)

.PHONEY: qtpy
qtpy: $(QTPY_BUILD_DIR)/Makefile
	$(echo_recipe)VERBOSE= COLOR= $(MAKE) -C $(QTPY_BUILD_DIR)

.PHONEY: qtpy_install
qtpy_install: qtpy
	$(echo_recipe)cp $(QTPY_BUILD_DIR)/atari-fw_qtpy.uf2 $(QTPY_MOUNT)

.PHONEY: qtpy_distribute
qtpy_distribute: qtpy
	$(echo_recipe)cp $(QTPY_BUILD_DIR)/atari-fw_qtpy.uf2 ../atari-hw/firmware

.PHONEY: qtpy_clean
qtpy_clean:
	$(echo_recipe)rm -rf $(QTPY_BUILD_DIR)

all: qtpy
clean: qtpy_clean

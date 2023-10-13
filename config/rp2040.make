RP2040_SRC_DIR := $(SRC_DIR)/platform/rp2040
BUILD_DIR_RP2040 := ./build_rp2040

RP2040_BOARD ?= waveshare_rp2040_zero
RP2040_BOARD_BUILD_DIR := $(BUILD_DIR_RP2040)/$(RP2040_BOARD)

PICO_SDK_DIR := ../pico-sdk

PICO_SDK_INCLUDE_DIRS :=
PICO_SDK_INCLUDE_DIRS += $(RP2040_BOARD_BUILD_DIR)/generated/pico_base
PICO_SDK_INCLUDE_DIRS += $(shell find -L $(PICO_SDK_DIR) -name include -type d | grep -v pico_stdio_usb)
PICO_SDK_INCLUDE_DIRS += $(PICO_SDK_DIR)/lib/tinyusb/src

PICO_WS2812_PIO_INCLUDE_DIR := $(RP2040_BOARD_BUILD_DIR)

TUSB_CONFIG_INCLUDE_DIR := $(RP2040_SRC_DIR)

RP2040_CXXFLAGS :=
RP2040_CXXFLAGS += $(patsubst %,-isystem %,$(PICO_SDK_INCLUDE_DIRS))
RP2040_CXXFLAGS += $(patsubst %,-isystem %,$(PICO_WS2812_PIO_INCLUDE_DIR))
RP2040_CXXFLAGS += -I$(TUSB_CONFIG_INCLUDE_DIR)
CXXFLAGS += $(RP2040_CXXFLAGS)

RP2040_MOUNT ?= /Volumes/RPI-RP2

.PHONY: rp2040-src-list
rp2040-src-list: media-disk-library
	$(echo_recipe)find src -type f -name '*.cc' -o -name '*.h' | sed 's@^src/platform/rp2040/\(.*\)/app.cc@src/platform/rp2040/$${RP2040_BOARD}/app.cc@' | sed 's@^src/@$${ATARI_FW_ROOT}/src/@' | grep -v '/test/' | sort | uniq

$(RP2040_BOARD_BUILD_DIR)/Makefile: $(RP2040_SRC_DIR)/CMakeLists.txt
	$(echo_build_message)
	$(echo_recipe)rm -rf $(RP2040_BOARD_BUILD_DIR)
	$(echo_recipe)cmake -S $(RP2040_SRC_DIR) -B $(RP2040_BOARD_BUILD_DIR) -DRP2040_BOARD=$(RP2040_BOARD)

.PHONY: rp2040
rp2040: $(RP2040_BOARD_BUILD_DIR)/Makefile media-disk-library
	$(echo_build_message)
	$(echo_recipe)VERBOSE= COLOR= make -C $(RP2040_BOARD_BUILD_DIR)

.PHONY: rp2040-all
rp2040-all:
	$(echo_build_message)
	$(echo_recipe)RP2040_BOARD=adafruit_qtpy_rp2040 make rp2040
	$(echo_recipe)RP2040_BOARD=waveshare_rp2040_zero make rp2040

.PHONY: rp2040-install
rp2040-install: rp2040
	$(echo_build_message)
	$(echo_recipe)cp $(RP2040_BOARD_BUILD_DIR)/atari-fw_$(RP2040_BOARD).uf2 $(RP2040_MOUNT)

.PHONY: rp2040-distribute
rp2040-distribute: rp2040 test
	$(echo_build_message)
	$(echo_recipe)cp $(RP2040_BOARD_BUILD_DIR)/atari-fw_$(RP2040_BOARD).uf2 ../atari-hw/prebuilt/

.PHONY: distribute
distribute:
	$(echo_build_message)
	$(echo_recipe)ATR_FILES=$(LOCAL_ATR_FILES) RP2040_BOARD=adafruit_qtpy_rp2040 make rp2040-distribute
	$(echo_recipe)ATR_FILES=$(LOCAL_ATR_FILES) RP2040_BOARD=waveshare_rp2040_zero make rp2040-distribute

PHONY: rp2040-clean
rp2040-clean:
	$(echo_recipe)rm -rf $(BUILD_DIR_RP2040)

init: | rp2040
clean: rp2040-clean

RP2040_SRC_DIR := $(SRC_DIR)/platform/rp2040
RP2040_BUILD_DIR := ./build_rp2040

RP2040_BOARD ?= adafruit_qtpy_rp2040
RP2040_BOARD_BUILD_DIR := $(RP2040_BUILD_DIR)/$(RP2040_BOARD)

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

$(RP2040_BOARD_BUILD_DIR)/Makefile: $(RP2040_SRC_DIR)/CMakeLists.txt
	$(echo_build_message)
	$(echo_recipe)rm -rf $(RP2040_BOARD_BUILD_DIR)
	$(echo_recipe)cmake -S $(RP2040_SRC_DIR) -B $(RP2040_BOARD_BUILD_DIR) -DRP2040_BOARD=$(RP2040_BOARD)

.PHONEY: rp2040
rp2040: $(RP2040_BOARD_BUILD_DIR)/Makefile
	$(echo_build_message)
	$(echo_recipe)VERBOSE= COLOR= $(MAKE) -C $(RP2040_BOARD_BUILD_DIR)

.PHONEY: rp2040_install
rp2040_install: rp2040
	$(echo_build_message)
	$(echo_recipe)cp $(RP2040_BOARD_BUILD_DIR)/atari-fw_$(RP2040_BOARD).uf2 $(RP2040_MOUNT)

.PHONEY: rp2040_distribute
rp2040_distribute: rp2040
	$(echo_build_message)
	$(echo_recipe)cp $(RP2040_BOARD_BUILD_DIR)/atari-fw_$(RP2040_BOARD).uf2 ../atari-hw/prebuilt/

.PHONEY: distribute
distribute:
	$(echo_build_message)
	$(echo_recipe)RP2040_BOARD=adafruit_qtpy_rp2040 make rp2040_distribute
	$(echo_recipe)RP2040_BOARD=waveshare_rp2040_zero make rp2040_distribute

.PHONEY: rp2040_clean
rp2040_clean:
	$(echo_recipe)rm -rf $(RP2040_BUILD_DIR)

all: rp2040
clean: rp2040_clean

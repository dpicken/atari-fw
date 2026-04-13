RP_SRC_DIR := $(SRC_DIR)/platform/rp
BUILD_DIR_RP := ./build_rp

RP_BOARD ?= waveshare_rp2040_zero
RP_BOARD_BUILD_DIR := $(BUILD_DIR_RP)/$(RP_BOARD)

PICO_SDK_DIR := ../third-party/pico-sdk
PICOTOOL_DIR := $(shell realpath ../third-party/)/picotool

PICO_SDK_INCLUDE_DIRS :=
PICO_SDK_INCLUDE_DIRS += $(RP_BOARD_BUILD_DIR)/generated/pico_base
PICO_SDK_INCLUDE_DIRS += $(shell find -L $(PICO_SDK_DIR) -name include -type d | grep -v pico_stdio_usb)
PICO_SDK_INCLUDE_DIRS += $(PICO_SDK_DIR)/lib/tinyusb/src

PICO_WS2812_PIO_INCLUDE_DIR := $(RP_BOARD_BUILD_DIR)

TUSB_CONFIG_INCLUDE_DIR := $(RP_SRC_DIR)

RP_CXXFLAGS :=
RP_CXXFLAGS += $(patsubst %,-isystem %,$(PICO_SDK_INCLUDE_DIRS))
RP_CXXFLAGS += $(patsubst %,-isystem %,$(PICO_WS2812_PIO_INCLUDE_DIR))
RP_CXXFLAGS += -I$(TUSB_CONFIG_INCLUDE_DIR)
CXXFLAGS += $(RP_CXXFLAGS)

RP_MOUNT ?= /Volumes/RPI-RP2

.PHONY: rp-src-list
rp-src-list: fs-builtin-library
	$(echo_recipe)find src -type f -name '*.cc' -o -name '*.h' | sed 's@^src/platform/rp/\(.*\)/app.cc@src/platform/rp/$${RP_BOARD}/app.cc@' | sed 's@^src/@$${ATARI_FW_ROOT}/src/@' | grep -v '/test/' | sort | uniq

$(RP_BOARD_BUILD_DIR)/Makefile: $(RP_SRC_DIR)/CMakeLists.txt | init
	$(echo_build_message)
	$(echo_recipe)rm -rf $(RP_BOARD_BUILD_DIR)
	$(echo_recipe)cmake -S $(RP_SRC_DIR) -B $(RP_BOARD_BUILD_DIR) -DRP_BOARD=$(RP_BOARD) -DPICOTOOL_FETCH_FROM_GIT_PATH=$(PICOTOOL_DIR)

.PHONY: rp
rp: $(RP_BOARD_BUILD_DIR)/Makefile
	$(echo_build_message)
	$(echo_recipe)VERBOSE= COLOR= make -C $(RP_BOARD_BUILD_DIR)

.PHONY: rp-board-%
rp-board-%:
	$(echo_build_message)
	$(echo_recipe)RP_BOARD=$(patsubst rp-board-%,%,$@) make rp

.PHONY: rp-all
rp-all: rp-board-adafruit_qtpy_rp2040 rp-board-pimoroni_tiny2350 rp-board-waveshare_rp2040_zero

all: rp-all

.PHONY: rp-install
rp-install: rp
	$(echo_build_message)
	$(echo_recipe)cp $(RP_BOARD_BUILD_DIR)/atari-fw_$(RP_BOARD).uf2 $(RP_MOUNT)

.PHONY: rp-distribute
rp-distribute: rp
	$(echo_build_message)
	$(echo_recipe)cp $(RP_BOARD_BUILD_DIR)/atari-fw_$(RP_BOARD).uf2 ../atari-hw/prebuilt/

.PHONY: distribute
distribute: | test
	$(echo_build_message)
	$(echo_recipe)RP_BOARD=adafruit_qtpy_rp2040 make rp-distribute
	$(echo_recipe)RP_BOARD=pimoroni_tiny2350 make rp-distribute
	$(echo_recipe)RP_BOARD=waveshare_rp2040_zero make rp-distribute

PHONY: rp-clean
rp-clean:
	$(echo_recipe)rm -rf $(BUILD_DIR_RP)

clean: rp-clean

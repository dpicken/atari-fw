cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(FAMILY rp2040)
set(BOARD ${RP2040_BOARD})

set(ATARI_FW_ROOT ${CMAKE_CURRENT_LIST_DIR}/../../..)
set(PICO_SDK_DIR ${ATARI_FW_ROOT}/../pico-sdk)

include(${PICO_SDK_DIR}/pico_sdk_init.cmake)

project(atari-fw_${RP2040_BOARD})

pico_sdk_init()

include_directories(${ATARI_FW_ROOT}/src)

add_executable(atari-fw_${RP2040_BOARD}

  ${ATARI_FW_ROOT}/src/hardware/OutputSignal.h
  ${ATARI_FW_ROOT}/src/hardware/misc.h
  ${ATARI_FW_ROOT}/src/hardware/OutputSignal.cc
  ${ATARI_FW_ROOT}/src/hardware/InputSignal.h
  ${ATARI_FW_ROOT}/src/hardware/InputSignal.cc
  ${ATARI_FW_ROOT}/src/app/App.cc
  ${ATARI_FW_ROOT}/src/app/App.h
  ${ATARI_FW_ROOT}/src/util/type_traits.h
  ${ATARI_FW_ROOT}/src/platform/rp2040/AppLauncher.h
  ${ATARI_FW_ROOT}/src/platform/rp2040/App.cc
  ${ATARI_FW_ROOT}/src/platform/rp2040/App.h
  ${ATARI_FW_ROOT}/src/platform/rp2040/AppLauncher.inl.h
  ${ATARI_FW_ROOT}/src/platform/rp2040/tusb_config.h
  ${ATARI_FW_ROOT}/src/platform/rp2040/NeoPixelSerializeColor.h
  ${ATARI_FW_ROOT}/src/platform/rp2040/${RP2040_BOARD}/app.cc
  ${ATARI_FW_ROOT}/src/keyboard/Controller.h
  ${ATARI_FW_ROOT}/src/keyboard/usb/ModifierBit.h
  ${ATARI_FW_ROOT}/src/keyboard/usb/InputReport.h
  ${ATARI_FW_ROOT}/src/keyboard/usb/InputReport.cc
  ${ATARI_FW_ROOT}/src/keyboard/usb/KeyCode.h
  ${ATARI_FW_ROOT}/src/keyboard/usb/KeyCode.cc
  ${ATARI_FW_ROOT}/src/keyboard/pokey/Traits.h
  ${ATARI_FW_ROOT}/src/keyboard/pokey/InputReport.h
  ${ATARI_FW_ROOT}/src/keyboard/pokey/InputReport.cc
  ${ATARI_FW_ROOT}/src/keyboard/pokey/Controller.h
  ${ATARI_FW_ROOT}/src/keyboard/pokey/KeyCode.h
  ${ATARI_FW_ROOT}/src/keyboard/pokey/ModifierCode.h
  ${ATARI_FW_ROOT}/src/keyboard/pokey/Controller.cc
  ${ATARI_FW_ROOT}/src/keyboard/Controller.cc
  ${ATARI_FW_ROOT}/src/keyboard/console/Controller.h
  ${ATARI_FW_ROOT}/src/keyboard/console/KeyBit.h
  ${ATARI_FW_ROOT}/src/keyboard/console/Controller.cc
)

pico_generate_pio_header(atari-fw_${RP2040_BOARD} ${ATARI_FW_ROOT}/../pico-examples/pio/ws2812/ws2812.pio)

target_include_directories(atari-fw_${RP2040_BOARD} PRIVATE
  ${ATARI_FW_ROOT}/src/platform/rp2040
  ${ATARI_FW_ROOT}/build_${RP2040_BOARD}
)

target_link_libraries(atari-fw_${RP2040_BOARD} PRIVATE pico_stdlib tinyusb_host tinyusb_board hardware_pio)

pico_add_extra_outputs(atari-fw_${RP2040_BOARD})

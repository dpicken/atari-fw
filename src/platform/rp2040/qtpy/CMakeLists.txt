cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PICO_BOARD adafruit_qtpy_rp2040)

set(FAMILY rp2040)
set(BOARD adafruit_qtpy_rp2040)

set(ATARI_FW_ROOT ../../../..)
set(PICO_SDK_DIR ${ATARI_FW_ROOT}/../pico-sdk)

include(${PICO_SDK_DIR}/pico_sdk_init.cmake)

project(atari-fw_qtpy)

pico_sdk_init()

include_directories(${ATARI_FW_ROOT}/src)

add_executable(atari-fw_qtpy
  ${ATARI_FW_ROOT}/src/app/App.h
  ${ATARI_FW_ROOT}/src/app/App.cc
  ${ATARI_FW_ROOT}/src/hardware/InputSignal.h
  ${ATARI_FW_ROOT}/src/hardware/InputSignal.cc
  ${ATARI_FW_ROOT}/src/hardware/OutputSignal.h
  ${ATARI_FW_ROOT}/src/hardware/OutputSignal.cc
  ${ATARI_FW_ROOT}/src/keyboard/Controller.h
  ${ATARI_FW_ROOT}/src/keyboard/Controller.cc
  ${ATARI_FW_ROOT}/src/keyboard/console/Controller.h
  ${ATARI_FW_ROOT}/src/keyboard/console/Controller.cc
  ${ATARI_FW_ROOT}/src/keyboard/console/KeyBit.h
  ${ATARI_FW_ROOT}/src/keyboard/pokey/Controller.h
  ${ATARI_FW_ROOT}/src/keyboard/pokey/Controller.cc
  ${ATARI_FW_ROOT}/src/keyboard/pokey/InputReport.h
  ${ATARI_FW_ROOT}/src/keyboard/pokey/InputReport.cc
  ${ATARI_FW_ROOT}/src/keyboard/pokey/KeyCode.h
  ${ATARI_FW_ROOT}/src/keyboard/pokey/ModifierCode.h
  ${ATARI_FW_ROOT}/src/keyboard/pokey/Traits.h
  ${ATARI_FW_ROOT}/src/keyboard/usb/InputReport.h
  ${ATARI_FW_ROOT}/src/keyboard/usb/InputReport.cc
  ${ATARI_FW_ROOT}/src/keyboard/usb/KeyCode.h
  ${ATARI_FW_ROOT}/src/keyboard/usb/KeyCode.cc
  ${ATARI_FW_ROOT}/src/keyboard/usb/ModifierBit.h
  ${ATARI_FW_ROOT}/src/platform/rp2040/qtpy/app.cc
  ${ATARI_FW_ROOT}/src/util/type_traits.h
)

pico_generate_pio_header(atari-fw_qtpy ${CMAKE_CURRENT_LIST_DIR}/${ATARI_FW_ROOT}/../pico-examples/pio/ws2812/ws2812.pio)

target_include_directories(atari-fw_qtpy PRIVATE
  ${ATARI_FW_ROOT}/src/platform/rp2040/qtpy
  ${ATARI_FW_ROOT}/build_qtpy
)

target_link_libraries(atari-fw_qtpy PRIVATE pico_stdlib tinyusb_host tinyusb_board hardware_pio)

pico_add_extra_outputs(atari-fw_qtpy)

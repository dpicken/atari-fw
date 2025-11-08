cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PICO_BOARD ${RP_BOARD})

set(ATARI_FW_ROOT ${CMAKE_CURRENT_LIST_DIR}/../../..)
set(PICO_SDK_DIR ${ATARI_FW_ROOT}/../third-party/pico-sdk)

include(${PICO_SDK_DIR}/pico_sdk_init.cmake)

project(atari-fw_${RP_BOARD})

pico_sdk_init()

include_directories(${ATARI_FW_ROOT}/src)

add_executable(atari-fw_${RP_BOARD}
  ${ATARI_FW_ROOT}/src/fs/automount/Manager.cc
  ${ATARI_FW_ROOT}/src/fs/automount/Manager.h
  ${ATARI_FW_ROOT}/src/fs/BlockSize.h
  ${ATARI_FW_ROOT}/src/fs/builtin/File.cc
  ${ATARI_FW_ROOT}/src/fs/builtin/File.h
  ${ATARI_FW_ROOT}/src/fs/builtin/FileSystem.cc
  ${ATARI_FW_ROOT}/src/fs/builtin/FileSystem.h
  ${ATARI_FW_ROOT}/src/fs/builtin/Library.cc
  ${ATARI_FW_ROOT}/src/fs/builtin/Library.gen.data.h
  ${ATARI_FW_ROOT}/src/fs/builtin/Library.gen.paths.h
  ${ATARI_FW_ROOT}/src/fs/builtin/Library.gen.sizes.h
  ${ATARI_FW_ROOT}/src/fs/builtin/Library.h
  ${ATARI_FW_ROOT}/src/fs/builtin/LibraryEnumerator.h
  ${ATARI_FW_ROOT}/src/fs/Directory.cc
  ${ATARI_FW_ROOT}/src/fs/Directory.h
  ${ATARI_FW_ROOT}/src/fs/DirectoryEntry.cc
  ${ATARI_FW_ROOT}/src/fs/DirectoryEntry.h
  ${ATARI_FW_ROOT}/src/fs/DirectoryEnumerator.cc
  ${ATARI_FW_ROOT}/src/fs/DirectoryEnumerator.h
  ${ATARI_FW_ROOT}/src/fs/Enumerator.cc
  ${ATARI_FW_ROOT}/src/fs/Enumerator.h
  ${ATARI_FW_ROOT}/src/fs/exfat/AddressTypes.h
  ${ATARI_FW_ROOT}/src/fs/exfat/BootSector.cc
  ${ATARI_FW_ROOT}/src/fs/exfat/BootSector.h
  ${ATARI_FW_ROOT}/src/fs/exfat/ClusterAllocation.h
  ${ATARI_FW_ROOT}/src/fs/exfat/ClusterChain.cc
  ${ATARI_FW_ROOT}/src/fs/exfat/ClusterChain.h
  ${ATARI_FW_ROOT}/src/fs/exfat/ClusterChainEntry.cc
  ${ATARI_FW_ROOT}/src/fs/exfat/ClusterChainEntry.h
  ${ATARI_FW_ROOT}/src/fs/exfat/ClusterChainEnumerator.h
  ${ATARI_FW_ROOT}/src/fs/exfat/ClusterHeap.cc
  ${ATARI_FW_ROOT}/src/fs/exfat/ClusterHeap.h
  ${ATARI_FW_ROOT}/src/fs/exfat/ClusterRun.cc
  ${ATARI_FW_ROOT}/src/fs/exfat/ClusterRun.h
  ${ATARI_FW_ROOT}/src/fs/exfat/ClusterRunEntry.h
  ${ATARI_FW_ROOT}/src/fs/exfat/ClusterRunEnumerator.h
  ${ATARI_FW_ROOT}/src/fs/exfat/Configuration.cc
  ${ATARI_FW_ROOT}/src/fs/exfat/Configuration.h
  ${ATARI_FW_ROOT}/src/fs/exfat/Directory.cc
  ${ATARI_FW_ROOT}/src/fs/exfat/Directory.h
  ${ATARI_FW_ROOT}/src/fs/exfat/DirectoryEntryCustomDefinedTypes.h
  ${ATARI_FW_ROOT}/src/fs/exfat/DirectoryEntryTypes.h
  ${ATARI_FW_ROOT}/src/fs/exfat/File.cc
  ${ATARI_FW_ROOT}/src/fs/exfat/File.h
  ${ATARI_FW_ROOT}/src/fs/exfat/FileAttributes.h
  ${ATARI_FW_ROOT}/src/fs/exfat/FileSystem.cc
  ${ATARI_FW_ROOT}/src/fs/exfat/FileSystem.h
  ${ATARI_FW_ROOT}/src/fs/exfat/GenericDirectoryEntry.h
  ${ATARI_FW_ROOT}/src/fs/exfat/GenericDirectoryEntryCustomDefinedTypes.h
  ${ATARI_FW_ROOT}/src/fs/exfat/GenericDirectoryEntryTypes.h
  ${ATARI_FW_ROOT}/src/fs/exfat/LogicalDirectory.cc
  ${ATARI_FW_ROOT}/src/fs/exfat/LogicalDirectory.h
  ${ATARI_FW_ROOT}/src/fs/exfat/LogicalDirectoryEntry.cc
  ${ATARI_FW_ROOT}/src/fs/exfat/LogicalDirectoryEntry.h
  ${ATARI_FW_ROOT}/src/fs/exfat/LogicalDirectoryEnumerator.h
  ${ATARI_FW_ROOT}/src/fs/exfat/PhysicalDirectory.cc
  ${ATARI_FW_ROOT}/src/fs/exfat/PhysicalDirectory.h
  ${ATARI_FW_ROOT}/src/fs/exfat/PhysicalDirectoryEntry.cc
  ${ATARI_FW_ROOT}/src/fs/exfat/PhysicalDirectoryEntry.h
  ${ATARI_FW_ROOT}/src/fs/exfat/PhysicalDirectoryEnumerator.h
  ${ATARI_FW_ROOT}/src/fs/exfat/SortingDirectory.cc
  ${ATARI_FW_ROOT}/src/fs/exfat/SortingDirectory.h
  ${ATARI_FW_ROOT}/src/fs/exfat/Unicode.cc
  ${ATARI_FW_ROOT}/src/fs/exfat/Unicode.h
  ${ATARI_FW_ROOT}/src/fs/exfat/Volume.cc
  ${ATARI_FW_ROOT}/src/fs/exfat/Volume.h
  ${ATARI_FW_ROOT}/src/fs/File.cc
  ${ATARI_FW_ROOT}/src/fs/File.h
  ${ATARI_FW_ROOT}/src/fs/FileSlice.cc
  ${ATARI_FW_ROOT}/src/fs/FileSlice.h
  ${ATARI_FW_ROOT}/src/fs/FileSystem.cc
  ${ATARI_FW_ROOT}/src/fs/FileSystem.h
  ${ATARI_FW_ROOT}/src/fs/gpt/GPT.cc
  ${ATARI_FW_ROOT}/src/fs/gpt/GPT.h
  ${ATARI_FW_ROOT}/src/fs/mbr/MBR.cc
  ${ATARI_FW_ROOT}/src/fs/mbr/MBR.h
  ${ATARI_FW_ROOT}/src/fs/ram/Directory.cc
  ${ATARI_FW_ROOT}/src/fs/ram/Directory.h
  ${ATARI_FW_ROOT}/src/fs/ResolvePath.cc
  ${ATARI_FW_ROOT}/src/fs/ResolvePath.h
  ${ATARI_FW_ROOT}/src/fs/root/FileSystem.cc
  ${ATARI_FW_ROOT}/src/fs/root/FileSystem.h
  ${ATARI_FW_ROOT}/src/hal/duration_us.h
  ${ATARI_FW_ROOT}/src/hal/Indicator.cc
  ${ATARI_FW_ROOT}/src/hal/Indicator.h
  ${ATARI_FW_ROOT}/src/hal/InputSignal.cc
  ${ATARI_FW_ROOT}/src/hal/InputSignal.h
  ${ATARI_FW_ROOT}/src/hal/misc.h
  ${ATARI_FW_ROOT}/src/hal/OutputSignal.cc
  ${ATARI_FW_ROOT}/src/hal/OutputSignal.h
  ${ATARI_FW_ROOT}/src/hal/Spi.cc
  ${ATARI_FW_ROOT}/src/hal/Spi.h
  ${ATARI_FW_ROOT}/src/hal/Uart.cc
  ${ATARI_FW_ROOT}/src/hal/Uart.h
  ${ATARI_FW_ROOT}/src/keyboard/App.cc
  ${ATARI_FW_ROOT}/src/keyboard/App.h
  ${ATARI_FW_ROOT}/src/keyboard/console/Controller.cc
  ${ATARI_FW_ROOT}/src/keyboard/console/Controller.h
  ${ATARI_FW_ROOT}/src/keyboard/console/KeyBit.h
  ${ATARI_FW_ROOT}/src/keyboard/console/KeyReleaseObserver.cc
  ${ATARI_FW_ROOT}/src/keyboard/console/KeyReleaseObserver.h
  ${ATARI_FW_ROOT}/src/keyboard/Controller.cc
  ${ATARI_FW_ROOT}/src/keyboard/Controller.h
  ${ATARI_FW_ROOT}/src/keyboard/Pipe.cc
  ${ATARI_FW_ROOT}/src/keyboard/Pipe.h
  ${ATARI_FW_ROOT}/src/keyboard/pokey/Controller.cc
  ${ATARI_FW_ROOT}/src/keyboard/pokey/Controller.h
  ${ATARI_FW_ROOT}/src/keyboard/pokey/InputReport.cc
  ${ATARI_FW_ROOT}/src/keyboard/pokey/InputReport.h
  ${ATARI_FW_ROOT}/src/keyboard/pokey/KeyCode.h
  ${ATARI_FW_ROOT}/src/keyboard/pokey/ModifierCode.h
  ${ATARI_FW_ROOT}/src/keyboard/pokey/Traits.h
  ${ATARI_FW_ROOT}/src/keyboard/usb/InputReport.cc
  ${ATARI_FW_ROOT}/src/keyboard/usb/InputReport.h
  ${ATARI_FW_ROOT}/src/keyboard/usb/KeyCode.cc
  ${ATARI_FW_ROOT}/src/keyboard/usb/KeyCode.h
  ${ATARI_FW_ROOT}/src/keyboard/usb/ModifierBit.h
  ${ATARI_FW_ROOT}/src/media/Atr.cc
  ${ATARI_FW_ROOT}/src/media/Atr.h
  ${ATARI_FW_ROOT}/src/media/Disk.cc
  ${ATARI_FW_ROOT}/src/media/Disk.h
  ${ATARI_FW_ROOT}/src/media/Xex.cc
  ${ATARI_FW_ROOT}/src/media/Xex.h
  ${ATARI_FW_ROOT}/src/media/XexEntry.cc
  ${ATARI_FW_ROOT}/src/media/XexEntry.h
  ${ATARI_FW_ROOT}/src/media/XexEnumerator.cc
  ${ATARI_FW_ROOT}/src/media/XexEnumerator.h
  ${ATARI_FW_ROOT}/src/platform/rp/${RP_BOARD}/app.cc
  ${ATARI_FW_ROOT}/src/platform/rp/App.cc
  ${ATARI_FW_ROOT}/src/platform/rp/App.h
  ${ATARI_FW_ROOT}/src/platform/rp/AppLauncher.h
  ${ATARI_FW_ROOT}/src/platform/rp/AppLauncher.inl.h
  ${ATARI_FW_ROOT}/src/platform/rp/NeoPixelSerializeColor.h
  ${ATARI_FW_ROOT}/src/platform/rp/tusb_config.h
  ${ATARI_FW_ROOT}/src/sd/Block.h
  ${ATARI_FW_ROOT}/src/sd/BlockSize.h
  ${ATARI_FW_ROOT}/src/sd/Card.cc
  ${ATARI_FW_ROOT}/src/sd/Card.h
  ${ATARI_FW_ROOT}/src/sd/Command.cc
  ${ATARI_FW_ROOT}/src/sd/Command.h
  ${ATARI_FW_ROOT}/src/sd/Controller.cc
  ${ATARI_FW_ROOT}/src/sd/Controller.fwd.h
  ${ATARI_FW_ROOT}/src/sd/Controller.h
  ${ATARI_FW_ROOT}/src/sd/CSD.h
  ${ATARI_FW_ROOT}/src/sd/Response.cc
  ${ATARI_FW_ROOT}/src/sd/Response.h
  ${ATARI_FW_ROOT}/src/sd/SpiAccessor.cc
  ${ATARI_FW_ROOT}/src/sd/SpiAccessor.h
  ${ATARI_FW_ROOT}/src/sd/Traits.h
  ${ATARI_FW_ROOT}/src/sd/VHS.h
  ${ATARI_FW_ROOT}/src/sio/App.cc
  ${ATARI_FW_ROOT}/src/sio/App.h
  ${ATARI_FW_ROOT}/src/sio/AtariControl.cc
  ${ATARI_FW_ROOT}/src/sio/AtariControl.h
  ${ATARI_FW_ROOT}/src/sio/checksum.cc
  ${ATARI_FW_ROOT}/src/sio/checksum.h
  ${ATARI_FW_ROOT}/src/sio/Controller.cc
  ${ATARI_FW_ROOT}/src/sio/Controller.h
  ${ATARI_FW_ROOT}/src/sio/Device.cc
  ${ATARI_FW_ROOT}/src/sio/Device.h
  ${ATARI_FW_ROOT}/src/sio/DiskDrive.cc
  ${ATARI_FW_ROOT}/src/sio/DiskDrive.h
  ${ATARI_FW_ROOT}/src/sio/FileSystem.cc
  ${ATARI_FW_ROOT}/src/sio/FileSystem.h
  ${ATARI_FW_ROOT}/src/sio/Frame.cc
  ${ATARI_FW_ROOT}/src/sio/Frame.h
  ${ATARI_FW_ROOT}/src/sio/Pipe.cc
  ${ATARI_FW_ROOT}/src/sio/Pipe.h
  ${ATARI_FW_ROOT}/src/sio/sdr/DiskDrive.h
  ${ATARI_FW_ROOT}/src/sio/sdr/FileSystem.h
  ${ATARI_FW_ROOT}/src/sio/sdr/Types.h
  ${ATARI_FW_ROOT}/src/sio/Traits.h
  ${ATARI_FW_ROOT}/src/thread/Pipe.cc
  ${ATARI_FW_ROOT}/src/thread/Pipe.h
  ${ATARI_FW_ROOT}/src/util/Ascii.cc
  ${ATARI_FW_ROOT}/src/util/Ascii.h
  ${ATARI_FW_ROOT}/src/util/crc.cc
  ${ATARI_FW_ROOT}/src/util/crc.h
  ${ATARI_FW_ROOT}/src/util/hex.cc
  ${ATARI_FW_ROOT}/src/util/hex.h
  ${ATARI_FW_ROOT}/src/util/PACKED.h
  ${ATARI_FW_ROOT}/src/util/type_traits.h
)

pico_generate_pio_header(atari-fw_${RP_BOARD} ${ATARI_FW_ROOT}/../third-party/pico-examples/pio/ws2812/ws2812.pio)

target_include_directories(atari-fw_${RP_BOARD} PRIVATE
  ${ATARI_FW_ROOT}/src/platform/rp
  ${ATARI_FW_ROOT}/build_${RP_BOARD}
)

target_link_libraries(atari-fw_${RP_BOARD} PRIVATE pico_stdlib pico_multicore tinyusb_host tinyusb_board hardware_pio hardware_spi hardware_uart)

pico_enable_stdio_uart(atari-fw_${RP_BOARD} 0)

pico_add_extra_outputs(atari-fw_${RP_BOARD})

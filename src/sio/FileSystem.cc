#include "FileSystem.h"

sio::FileSystem::FileSystem(::hal::Uart uart, ::hal::BusyWait busyWait)
  : Device(uart, busyWait) {
}

void sio::FileSystem::handle(const Command* command) {
  switch (command->commandId) {
    case 0x00:
      handleCurrentDirChangeUp();
      break;

    case 0x01:
      handleCurrentDirChangeDown(command->aux<sdr::DirEntryInfo>());
      break;

    case 0x02:
      handleCurrentDirSetFilter();
      break;

    case 0x03:
      handleCurrentDirReadNextPage();
      break;

    case 0x04:
      handleCurrentDirReadPreviousPage();
      break;

    case 0x05:
      handleCurrentFileSet(command->aux<sdr::DirEntryInfo>());
      break;

    case 0x06:
      handleCurrentFileApply(command->aux1);
      break;

    default:
      commandNack();
  }
}

void sio::FileSystem::handleCurrentDirChangeUp() {
}

void sio::FileSystem::handleCurrentDirChangeDown(const sdr::DirEntryInfo& dirEntryInfo) {
  (void) dirEntryInfo;
}

void sio::FileSystem::handleCurrentDirSetFilter() {
}

void sio::FileSystem::handleCurrentDirReadNextPage() {
}

void sio::FileSystem::handleCurrentDirReadPreviousPage() {
}

void sio::FileSystem::handleCurrentFileSet(const sdr::DirEntryInfo& dirEntryInfo) {
  (void) dirEntryInfo;
}

void sio::FileSystem::handleCurrentFileApply(std::uint8_t deviceId) {
  (void) deviceId;
}

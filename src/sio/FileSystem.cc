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
      handleCurrentDirChangeDown(command->aux<DirEntryId>());
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
      handleCurrentFileSet(command->aux<DirEntryId>());
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

void sio::FileSystem::handleCurrentDirChangeDown(DirEntryId dirEntryId) {
  (void) dirEntryId;
}

void sio::FileSystem::handleCurrentDirSetFilter() {
}

void sio::FileSystem::handleCurrentDirReadNextPage() {
}

void sio::FileSystem::handleCurrentDirReadPreviousPage() {
}

void sio::FileSystem::handleCurrentFileSet(DirEntryId dirEntryId) {
  (void) dirEntryId;
}

void sio::FileSystem::handleCurrentFileApply(std::uint8_t deviceId) {
  (void) deviceId;
}

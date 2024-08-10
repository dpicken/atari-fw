#include "FileSystem.h"

#include "Frame.h"

#include "media/Atr.h"
#include "media/BuiltinAtrFileLibrary.h"

sio::FileSystem::FileSystem(::hal::Uart uart, ::hal::BusyWait busyWait, DiskDrive* d1)
  : Device(uart, busyWait)
  , m_d1(d1) {
}

void sio::FileSystem::handle(const Command* command) {
  switch (command->commandId) {
    case 0x00:
      handleGetCurrentDir();
      break;

    case 0x01:
      handleReadDir(command->aux());
      break;

    case 0x02:
      handleSelectDirEntry(command->aux());
      break;

    default:
      commandNack();
  }
}

void sio::FileSystem::handleGetCurrentDir() {
  Frame<sdr::Path> pathFrame(sdr::Path("/builtin"));

  commandAck();
  commandComplete();
  pathFrame.tx(uart());
}

void sio::FileSystem::handleReadDir(sdr::DirEntry::index_type index) {
  unsigned int count = ::media::BuiltinAtrFileLibrary::getAtrCount();
  if (index > count) {
    commandNack();
    return;
  }

  Frame<sdr::DirEntryPage> pageFrame;

  sdr::DirEntryPage* page = pageFrame.data();
  for (; index != count && !page->full(); ++index) {
    page->emplace_back(::media::BuiltinAtrFileLibrary::getAtrTitle(index), index);
  }
  page->setEOS(index == count);

  commandAck();
  commandComplete();
  pageFrame.tx(uart());
}

void sio::FileSystem::handleSelectDirEntry(sdr::DirEntry::index_type index) {
  if (index >= ::media::BuiltinAtrFileLibrary::getAtrCount()) {
    commandNack();
    return;
  }

  m_d1->insert(::media::makeAtr(::media::BuiltinAtrFileLibrary::makeRomFile(index)));

  commandAck();
  commandComplete();
}

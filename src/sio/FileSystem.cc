#include "FileSystem.h"

#include "Frame.h"

#include "fs/ResolvePath.h"
#include "media/Atr.h"

sio::FileSystem::FileSystem(::hal::Uart uart, ::hal::BusyWait busyWait, DiskDrive* d1)
  : Device(uart, busyWait)
  , m_cwdPath("/")
  , m_cwdEnumerator(::fs::resolveDirectory(m_cwdPath))
  , m_d1(d1) {
}

void sio::FileSystem::handle(const Command* command) {
  switch (command->commandId) {
    case 0x00:
      handleGetCurrentDir();
      break;

    case 0x01:
      handleSelectParentDir();
      break;

    case 0x02:
      handleReadDir(command->aux());
      break;

    case 0x03:
      handleSelectDirEntry(command->aux());
      break;

    default:
      commandNack();
  }
}

void sio::FileSystem::handleGetCurrentDir() {
  Frame<sdr::Path> pathFrame(sdr::Path(m_cwdPath.string()));

  commandAck();
  commandComplete();
  pathFrame.tx(uart());
}

void sio::FileSystem::handleSelectParentDir() {
  do {
    m_cwdPath = m_cwdPath.parent_path();
    auto directory = ::fs::resolveDirectory(m_cwdPath);
    if (directory) {
      m_cwdEnumerator = ::fs::DirectoryEnumerator(std::move(directory));
    }
  } while (m_cwdPath.has_relative_path() && !m_cwdEnumerator.isValid());

  commandAck();
  commandComplete();
}

void sio::FileSystem::handleReadDir(sdr::DirEntry::index_type index) {
  m_cwdEnumerator.reposition(index);

  Frame<sdr::DirEntryPage> pageFrame;

  sdr::DirEntryPage* page = pageFrame.data();
  for (; m_cwdEnumerator.isValid() && !page->full(); m_cwdEnumerator.next()) {
    page->emplace_back(m_cwdEnumerator.entry().name(), m_cwdEnumerator.entry().isDirectory(), m_cwdEnumerator.entry().index());
  }
  page->setEOS(!m_cwdEnumerator.isValid());

  commandAck();
  commandComplete();
  pageFrame.tx(uart());
}

void sio::FileSystem::handleSelectDirEntry(sdr::DirEntry::index_type index) {
  m_cwdEnumerator.reposition(index);

  if (!m_cwdEnumerator.isValid()) {
    commandNack();
    return;
  }

  if (m_cwdEnumerator.entry().isDirectory()) {
    auto directory = m_cwdEnumerator.openDirectory();
    if (directory == nullptr) {
      commandNack();
      return;
    }
    m_cwdPath /= m_cwdEnumerator.entry().name();
    m_cwdEnumerator = ::fs::DirectoryEnumerator(std::move(directory));
  } else {
    auto disk = ::media::makeAtr(m_cwdEnumerator.openFile());
    if (disk == nullptr) {
      commandNack();
      return;
    }
    m_d1->insert(std::move(disk));
  }

  commandAck();
  commandComplete();
}

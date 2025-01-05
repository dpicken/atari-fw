#include "FileSystem.h"

#include "Frame.h"

#include "fs/ResolvePath.h"
#include "fs/exfat/Configuration.h"
#include "media/Atr.h"

sio::FileSystem::FileSystem(::hal::Uart uart, ::hal::BusyWait busyWait, DiskDrive* d1)
  : Device(uart, busyWait)
  , m_cwdPath("/")
  , m_cwdEnumerator(::fs::resolveDirectory(m_cwdPath))
  , m_d1(d1) {
  ::fs::exfat::Configuration::instance()->nonAsciiReplacementCharacter(0xBF);
  ::fs::exfat::Configuration::instance()->excludeHiddenDirectoryEntries(true);
  ::fs::exfat::Configuration::instance()->excludeSystemDirectoryEntries(true);
  ::fs::exfat::Configuration::instance()->excludeUnixHiddenDirectoryEntries(true);
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
  commandAck();

  using sdr_path_type = sdr::CurrentDirPath;
  Frame<sdr_path_type> pathFrame(sdr_path_type(m_cwdPath.string(), sdr_path_type::left_truncate));

  commandComplete();
  pathFrame.tx(uart());
}

void sio::FileSystem::handleSelectParentDir() {
  commandAck();

  do {
    m_cwdPath = m_cwdPath.parent_path();
    auto directory = ::fs::resolveDirectory(m_cwdPath);
    if (directory) {
      m_cwdEnumerator = ::fs::DirectoryEnumerator(std::move(directory));
    }
  } while (m_cwdPath.has_relative_path() && !m_cwdEnumerator.isValid());
  commandComplete();
}

void sio::FileSystem::handleReadDir(sdr::DirEntry::index_type index) {
  commandAck();

  m_cwdEnumerator.reposition(index);

  Frame<sdr::DirEntryPage> pageFrame;
  sdr::DirEntryPage* page = pageFrame.data();
  for (; m_cwdEnumerator.isValid() && !page->full(); m_cwdEnumerator.next()) {
    page->emplace_back(m_cwdEnumerator.entry().name(), m_cwdEnumerator.entry().isDirectory(), m_cwdEnumerator.entry().index());
  }
  page->setEOS(!m_cwdEnumerator.isValid());

  commandComplete();
  pageFrame.tx(uart());
}

void sio::FileSystem::handleSelectDirEntry(sdr::DirEntry::index_type index) {
  commandAck();

  m_cwdEnumerator.reposition(index);
  if (!m_cwdEnumerator.isValid()) {
    commandError();
    return;
  }

  if (m_cwdEnumerator.entry().isDirectory()) {
    auto directory = m_cwdEnumerator.openDirectory();
    if (directory == nullptr) {
      commandError();
      return;
    }
    m_cwdPath /= m_cwdEnumerator.entry().name();
    m_cwdEnumerator = ::fs::DirectoryEnumerator(std::move(directory));
  } else {
    auto disk = ::media::makeAtr(m_cwdEnumerator.openFile());
    if (disk == nullptr) {
      commandError();
      return;
    }
    m_d1->insert(std::move(disk));
  }

  commandComplete();
}

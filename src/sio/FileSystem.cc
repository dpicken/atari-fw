#include "FileSystem.h"

#include "fs/ResolvePath.h"
#include "fs/exfat/Configuration.h"
#include "media/Atr.h"

sio::FileSystem::FileSystem(::hal::Uart uart, ::hal::BusyWait busyWait, DiskDrive* d1)
  : Device(uart, busyWait)
  , m_xexBootBuiltin(::media::makeAtr(::fs::resolveWellKnownFile("!xex-loader.atr")))
  , m_d1(d1)
  , m_cwdPath("/")
  , m_cwdEnumerator(::fs::resolveDirectory(m_cwdPath))
  , m_xexEnumerator(media::Xex{}) {
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

    case 0x04:
      handleReadXexSegmentEntry(command->aux());
      break;

    case 0x05:
      handleReadXexSegmentData();
      break;

    default:
      commandNack();
  }
}

void sio::FileSystem::handleGetCurrentDir() {
  commandAck();
  commandComplete();
  sendData(sdr::CurrentDirPath{m_cwdPath.string(), sdr::CurrentDirPath::left_truncate});
}

void sio::FileSystem::handleSelectParentDir() {
  commandAck();
  do {
    m_cwdPath = m_cwdPath.parent_path();
    m_cwdEnumerator = ::fs::DirectoryEnumerator(::fs::resolveDirectory(m_cwdPath));
  } while (m_cwdPath.has_relative_path() && !m_cwdEnumerator.isValid());
  commandComplete();
}

void sio::FileSystem::handleReadDir(sdr::DirEntry::index_type index) {
  commandAck();

  m_cwdEnumerator.reposition(index);
  sdr::DirEntryPage page;
  for (; m_cwdEnumerator.isValid() && !page.full(); m_cwdEnumerator.next()) {
    page.emplace_back(m_cwdEnumerator.entry().name(), m_cwdEnumerator.entry().isDirectory(), m_cwdEnumerator.entry().index());
  }
  page.setEOS(!m_cwdEnumerator.isValid());

  commandComplete();
  sendData(page);
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
  } else if (m_cwdEnumerator.entry().name().ends_with(".atr")) {
    auto disk = ::media::makeAtr(m_cwdEnumerator.openFile());
    if (disk == nullptr) {
      commandError();
      return;
    }
    m_d1->insert(std::move(disk));
  } else if (m_cwdEnumerator.entry().name().ends_with(".xex")) {
    ::media::XexEnumerator xexEnumerator(media::Xex{m_cwdEnumerator.openFile()});
    if (!xexEnumerator.isValid()) {
      commandError();
      return;
    }
    m_xexEnumerator = std::move(xexEnumerator);
    m_d1->insert(m_xexBootBuiltin);
  } else {
    commandError();
    return;
  }

  commandComplete();
}

void sio::FileSystem::handleReadXexSegmentEntry(std::uint16_t index) {
  m_xexEnumerator.reposition(index);

  auto entry = m_xexEnumerator.isValid()
      ? sdr::XexEntry{
            m_xexEnumerator.entry().segmentHeader().loadAddressBegin,
            m_xexEnumerator.entry().segmentHeader().loadAddressLast}
      : sdr::XexEntry::makeEOS();

  commandAck();
  commandComplete();
  sendData(entry);
}

void sio::FileSystem::handleReadXexSegmentData() {
  if (!m_xexEnumerator.isValid()) {
    commandNack();
    return;
  }

  commandAck();
  commandComplete();
  sinkData([this](const data_sink_type& sink) { m_xexEnumerator.container().readSegmentData(m_xexEnumerator.entry(), sink); });
}

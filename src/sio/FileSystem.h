#ifndef sio_FileSystem_h
#define sio_FileSystem_h

#include "Device.h"
#include "DiskDrive.h"
#include "sdr/FileSystem.h"

#include "fs/DirectoryEnumerator.h"
#include "media/XexEnumerator.h"

#include <cstdint>
#include <filesystem>

namespace sio {

class FileSystem : public Device {
public:
  FileSystem(::hal::Uart uart, ::hal::BusyWait busyWait, DiskDrive* d1);

  void handle(const Command* command) override;

private:
  void handleGetCurrentDir();
  void handleSelectParentDir();
  void handleReadDir(sdr::DirEntry::index_type index);
  void handleSelectDirEntry(sdr::DirEntry::index_type index);

  void handleReadXexSegmentEntry(sdr::XexEntry::index_type index);
  void handleReadXexSegmentData(sdr::XexEntry::index_type index);

  std::filesystem::path m_cwdPath;
  ::fs::DirectoryEnumerator m_cwdEnumerator;

  DiskDrive* const m_d1;

  ::media::XexEnumerator m_xexEnumerator;
};

} // namespace sio

#endif // ifndef sio_FileSystem_h

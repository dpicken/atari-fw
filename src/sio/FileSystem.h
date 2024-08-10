#ifndef sio_FileSystem_h
#define sio_FileSystem_h

#include "Device.h"
#include "DiskDrive.h"
#include "sdr/FileSystem.h"

#include <cstdint>

namespace sio {

class FileSystem : public Device {
public:
  FileSystem(::hal::Uart uart, ::hal::BusyWait busyWait, DiskDrive* d1);

  void handle(const Command* command) override;

private:
  void handleGetCurrentDir();
  void handleReadDir(sdr::DirEntry::index_type index);
  void handleSelectDirEntry(sdr::DirEntry::index_type index);

  DiskDrive* const m_d1;
};

} // namespace sio

#endif // ifndef sio_FileSystem_h

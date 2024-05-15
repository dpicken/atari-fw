#ifndef sio_FileSystem_h
#define sio_FileSystem_h

#include "Device.h"

#include "sdr/FileSystem.h"

#include <cstdint>

namespace sio {

class FileSystem : public Device {
public:
  FileSystem(::hal::Uart uart, ::hal::BusyWait busyWait);

  void handle(const Command* command) override;

private:
  void handleCurrentDirChangeUp();
  void handleCurrentDirChangeDown(const sdr::DirEntryInfo& dirEntryInfo);
  void handleCurrentDirSetFilter();

  void handleCurrentDirReadNextPage();
  void handleCurrentDirReadPreviousPage();

  void handleCurrentFileSet(const sdr::DirEntryInfo& dirEntryInfo);
  void handleCurrentFileApply(std::uint8_t deviceId);
};

} // namespace sio

#endif // ifndef sio_FileSystem_h

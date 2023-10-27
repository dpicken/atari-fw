#ifndef sio_DiskDrive_h
#define sio_DiskDrive_h

#include "Device.h"

#include "media/Disk.h"
#include "util/PACKED.h"

#include <memory>

namespace sio {

class DiskDrive : public Device {
public:
  using disk_type = ::media::Disk;
  using disk_ptr = std::unique_ptr<disk_type>;
  using sector_address_type = disk_type::sector_address_type;

  DiskDrive(::hal::Uart uart, ::hal::BusyWait busyWait);

  void insert(disk_ptr&& disk);
  disk_ptr eject();

  void handle(const Command* command) override;

  struct Status {
    std::uint8_t driveStatus = 0x00;
    std::uint8_t fdcStatus = 0xFF;
    std::uint8_t formatTimeout = 0xE0;
    std::uint8_t reserved = 0x00;
  } PACKED;

private:
  void handleStatus();
  void handleRead(sector_address_type sectorAddress);

  const disk_type::sector_sink_type m_sink;

  disk_ptr m_disk;

  Status m_status;
};

} // namespace sio

#endif // ifndef sio_DiskDrive_h

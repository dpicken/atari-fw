#ifndef sio_DiskDrive_h
#define sio_DiskDrive_h

#include "Device.h"

#include "media/Disk.h"
#include "sdr/DiskDrive.h"

#include <memory>

namespace sio {

class DiskDrive : public Device {
public:
  using disk_type = ::media::Disk;
  using disk_ptr = std::unique_ptr<disk_type>;
  using sector_address_type = disk_type::sector_address_type;

  DiskDrive(::hal::Uart uart, ::hal::BusyWait busyWait);

  void insert(disk_ptr disk);
  void eject();

  void handle(const Command* command) override;

private:
  void handleStatus();
  void handleRead(sector_address_type sectorAddress);

  const disk_type::sector_sink_type m_sink;

  disk_ptr m_disk;

  sdr::Status m_status;
};

} // namespace sio

#endif // ifndef sio_DiskDrive_h

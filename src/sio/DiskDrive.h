#ifndef sio_DiskDrive_h
#define sio_DiskDrive_h

#include "Device.h"

#include "media/Disk.h"
#include "sdr/DiskDrive.h"

#include <memory>

namespace sio {

class DiskDrive : public Device {
public:
  using sector_address_type = ::media::Disk::sector_address_type;

  DiskDrive(::hal::Uart uart, ::hal::BusyWait busyWait);

  void insert(const ::media::Disk::ptr_type& disk);
  void eject();

  void handle(const Command* command) override;

private:
  void handleStatus();
  void handleRead(sector_address_type sectorAddress);

  ::media::Disk::ptr_type m_disk;

  sdr::Status m_status;
};

} // namespace sio

#endif // ifndef sio_DiskDrive_h

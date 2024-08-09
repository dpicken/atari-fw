#include "DiskDrive.h"

#include "checksum.h"
#include "Frame.h"

sio::DiskDrive::DiskDrive(::hal::Uart uart, ::hal::BusyWait busyWait)
  : Device(uart, busyWait)
  , m_sink([device = this](const std::uint8_t* sector, std::size_t sectorSize) {
      device->uart()->tx(sector, sectorSize);
      device->uart()->tx(checksum(sector, sectorSize));
    }) {
}

void sio::DiskDrive::insert(disk_ptr&& disk) {
  m_status.driveStatus = 0x00;

  m_disk = std::move(disk);
  if (m_disk == nullptr) {
    return;
  }

  switch (m_disk->getDensity()) {
    case ::media::Disk::Density::Single:
      break;
    case ::media::Disk::Density::Enhanced:
      m_status.driveStatus |= 0x80;
      break;
    case ::media::Disk::Density::Double:
      m_status.driveStatus |= 0x20;
      break;
  }
}

sio::DiskDrive::disk_ptr sio::DiskDrive::eject() {
  disk_ptr disk;
  m_disk.swap(disk);
  return disk;
}

void sio::DiskDrive::handle(const Command* command) {
  if (!m_disk) {
    return;
  }

  switch (command->commandId) {
    case 'S':
      handleStatus();
      break;

    case 'R':
      handleRead(command->aux());
      break;

    default:
      commandNack();
  }
}

void sio::DiskDrive::handleStatus() {
  Frame<sdr::Status> statusFrame(m_status);

  commandAck();
  commandComplete();
  statusFrame.tx(uart());
}

void sio::DiskDrive::handleRead(sector_address_type sectorAddress) {
  if (!m_disk->hasSector(sectorAddress)) {
    commandNack();
    return;
  }

  commandAck();
  commandComplete();
  // TODO: Handle readSector failure.
  m_disk->readSector(sectorAddress, m_sink);
}

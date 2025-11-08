#include "DiskDrive.h"

sio::DiskDrive::DiskDrive(::hal::Uart uart, ::hal::BusyWait busyWait)
  : Device(uart, busyWait) {
}

void sio::DiskDrive::insert(const ::media::Disk::ptr_type& disk) {
  m_status.driveStatus = 0x00;

  m_disk = disk;
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

void sio::DiskDrive::eject() {
  insert(nullptr);
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
  commandAck();
  commandComplete();
  sendData(m_status);
}

void sio::DiskDrive::handleRead(sector_address_type sectorAddress) {
  if (!m_disk->hasSector(sectorAddress)) {
    commandNack();
    return;
  }

  commandAck();
  commandComplete();
  sinkData([this, sectorAddress](const data_sink_type& sink) { m_disk->readSector(sectorAddress, sink); });
}

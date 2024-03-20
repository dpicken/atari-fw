#include "AtariControl.h"

sio::AtariControl::AtariControl(::hal::Uart uart, ::hal::BusyWait busyWait)
  : Device(uart, busyWait) {
}

void sio::AtariControl::handle(const Command* command) {
  switch (command->commandId) {
    case 0x00:
      handleDiskEject(command->aux1);
      break;

    case 0x01:
      handleDiskEjectAll();
      break;

    case 0x02:
      handleResetWarm();
      break;

    case 0x03:
      handleResetCold();
      break;

    case 0x04:
      handlePowerOff();
      break;
  };
}

void sio::AtariControl::handleDiskEject(std::uint8_t deviceId) {
  (void) deviceId;
}

void sio::AtariControl::handleDiskEjectAll() {
}

void sio::AtariControl::handleResetWarm() {
}

void sio::AtariControl::handleResetCold() {
}

void sio::AtariControl::handlePowerOff() {
}

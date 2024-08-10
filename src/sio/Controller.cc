#include "Controller.h"

#include "Frame.h"

namespace {

constexpr hal::duration_us SioCommandInactiveTimeout = 950;

} // namespace

sio::Controller::Controller(
    ::hal::InputSignal command,
    ::hal::Uart uart,
    ::hal::BusyWaitEq busyWaitEq,
    DiskDrive* d1,
    AtariControl* atariControl,
    FileSystem* fileSystem)
  : m_command(command)
  , m_uart(uart)
  , m_busyWaitEq(busyWaitEq)
  , m_devices({
      {0x31, d1},
      {0x80, atariControl},
      {0x81, fileSystem}}) {
}

void sio::Controller::poll() {
  if (!m_command.isActive()) {
    return;
  }

  Frame<Device::Command> commandFrame;
  if (!commandFrame.rx(&m_uart)) {
    return;
  }

  if (!m_busyWaitEq(m_command, false, SioCommandInactiveTimeout)) {
    return;
  }

  auto deviceIt = m_devices.find(commandFrame.data()->deviceId);
  if (deviceIt == m_devices.end()) {
    return;
  }

  deviceIt->second->handle(commandFrame.data());
}

#include "Device.h"

namespace {

constexpr hal::duration_us CommandDeadTime = 250;

} // namespace

sio::Device::Device(::hal::Uart uart, ::hal::BusyWait busyWait)
  : m_uart(uart)
  , m_busyWait(busyWait) {
}

sio::Device::~Device() = default;

void sio::Device::busyWaitCommandDeadTime() const {
  m_busyWait(CommandDeadTime);
}

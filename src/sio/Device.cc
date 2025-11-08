#include "Device.h"

#include "checksum.h"

namespace {

constexpr hal::duration_us CommandDeadTime = 250;

} // namespace

sio::Device::Device(::hal::Uart uart, ::hal::BusyWait busyWait)
  : m_uart(uart)
  , m_busyWait(busyWait) {
}

sio::Device::~Device() = default;

void sio::Device::sinkData(const data_source_type& source) const {
  Checksum checksum;
  source([this, &checksum](const std::uint8_t* data, std::size_t size) mutable {
    m_uart.tx(data, size);
    checksum.update(data, size);
  });
  m_uart.tx(checksum.get());
}

void sio::Device::busyWaitCommandDeadTime() const {
  m_busyWait(CommandDeadTime);
}

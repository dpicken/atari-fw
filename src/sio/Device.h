#ifndef sio_Device_h
#define sio_Device_h

#include "hal/misc.h"
#include "hal/Uart.h"
#include "util/PACKED.h"

#include <cstdint>
#include <functional>

namespace sio {

class Device {
public:
  Device(::hal::Uart uart, ::hal::BusyWait busyWait);

  virtual ~Device();

  struct Command {
    std::uint8_t deviceId;
    std::uint8_t commandId;
    std::uint8_t aux1;
    std::uint8_t aux2;

    template<typename T = std::uint16_t>
    T aux() const {
      std::uint16_t value = aux2;
      value <<= 8;
      value |= aux1;
      return T(value);
    }
  } PACKED;

  virtual void handle(const Command* command) = 0;

  void commandAck() const {
    m_uart.tx('A');
  }

  void commandNack() const {
    m_uart.tx('N');
  }

  void commandComplete() const {
    busyWaitCommandDeadTime();
    m_uart.tx('C');
  }

  void commandError() const {
    busyWaitCommandDeadTime();
    m_uart.tx('E');
  }

  using data_sink_type = std::function<void(const std::uint8_t* data, std::size_t size)>;
  using data_source_type = std::function<void(const data_sink_type& sink)>;

  void sinkData(const data_source_type& source) const;

  template<typename PackedType>
  void sendData(const PackedType& value) const {
    sinkData([&value](const data_sink_type& sink) {
      sink(reinterpret_cast<const std::uint8_t*>(&value), sizeof(value));
    });
  }

  const ::hal::Uart* uart() const {
    return &m_uart;
  }

private:
  void busyWaitCommandDeadTime() const;

  const ::hal::Uart m_uart;
  const ::hal::BusyWait m_busyWait;
};

} // namespace sio

#endif // ifndef sio_Device_h

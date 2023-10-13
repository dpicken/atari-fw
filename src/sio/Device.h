#ifndef sio_Device_h
#define sio_Device_h

#include "hal/misc.h"
#include "hal/Uart.h"

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

    std::uint16_t aux() const {
      std::uint16_t value = aux2;
      value <<= 8;
      value |= aux1;
      return value;
    }
  } PACKED;

  virtual void handle(const Command* command) = 0;

  void commandAck() const {
    m_uart.txByte('A');
  }

  void commandNack() const {
    m_uart.txByte('N');
  }

  void commandComplete() const {
    busyWaitCommandDeadTime();
    m_uart.txByte('C');
  }

  void commandError() const {
    busyWaitCommandDeadTime();
    m_uart.txByte('E');
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

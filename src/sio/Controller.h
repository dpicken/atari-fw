#ifndef sio_Controller_h
#define sio_Controller_h

#include "Device.h"
#include "DiskDrive.h"

#include "hal/InputSignal.h"
#include "hal/misc.h"
#include "hal/Uart.h"

#include <unordered_map>

namespace sio {

class Controller {
public:
  Controller(
      ::hal::InputSignal command,
      ::hal::Uart uart,
      ::hal::BusyWaitEq busyWaitEq,
      DiskDrive* m_d1);

  void poll();

private:
  const ::hal::InputSignal m_command;
  const ::hal::Uart m_uart;
  const ::hal::BusyWaitEq m_busyWaitEq;

  const std::unordered_map<std::uint8_t, Device*> m_devices;
};

} // namespace sio

#endif // ifndef sio_Controller_h

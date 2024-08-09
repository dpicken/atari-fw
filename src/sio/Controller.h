#ifndef sio_Controller_h
#define sio_Controller_h

#include "AtariControl.h"
#include "Device.h"
#include "DiskDrive.h"
#include "FileSystem.h"

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
      DiskDrive* d1,
      AtariControl* atariControl,
      FileSystem* fileSystem);

  void poll();

private:
  const ::hal::InputSignal m_command;
  const ::hal::Uart m_uart;
  const ::hal::BusyWaitEq m_busyWaitEq;

  const std::unordered_map<std::uint8_t, Device*> m_devices;
};

} // namespace sio

#endif // ifndef sio_Controller_h

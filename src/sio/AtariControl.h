#ifndef sio_AtariControl_h
#define sio_AtariControl_h

#include "Device.h"

namespace sio {

class AtariControl : public Device {
public:
  AtariControl(::hal::Uart uart, ::hal::BusyWait busyWait);

  void handle(const Command* command) override;

private:
  void handlePowerOff();
  void handlePowerCycle();
  void handleReset();
};

} // namespace sio

#endif // ifndef sio_AtariControl_h

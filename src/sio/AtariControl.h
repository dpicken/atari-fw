#ifndef sio_AtariControl_h
#define sio_AtariControl_h

#include "Device.h"

#include "util/PACKED.h"

namespace sio {

class AtariControl : public Device {
public:
  AtariControl(::hal::Uart uart, ::hal::BusyWait busyWait);

  void handle(const Command* command) override;

private:
  void handleDiskEject(std::uint8_t deviceId);
  void handleDiskEjectAll();
  void handleResetWarm();
  void handleResetCold();
  void handlePowerOff();
};

} // namespace sio

#endif // ifndef sio_AtariControl_h

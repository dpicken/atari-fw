#ifndef keyboard_pokey_Controller_h
#define keyboard_pokey_Controller_h

#include "InputReport.h"

#include "hal/InputSignal.h"
#include "hal/misc.h"
#include "hal/OutputSignal.h"

#include <cstdint>

namespace keyboard { namespace pokey {

class Controller {
public:
  Controller(
      ::hal::InputSignal k0,
      ::hal::InputSignal k5,
      ::hal::OutputSignal kr1,
      ::hal::OutputSignal kr2,
      ::hal::BusyWaitEq busyWaitEq);

  /** Receive a keyboard report. */
  void receiveInputReport(const InputReport& report) const;

private:
  const ::hal::InputSignal m_k0;
  const ::hal::InputSignal m_k5;
  const ::hal::OutputSignal m_kr1;
  const ::hal::OutputSignal m_kr2;

  const ::hal::BusyWaitEq m_busyWaitEq;
};

} } // namespace keyboard::pokey

#endif // ifndef keyboard_pokey_Controller_h

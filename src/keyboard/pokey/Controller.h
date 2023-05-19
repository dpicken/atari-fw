#ifndef keyboard_pokey_Controller_h
#define keyboard_pokey_Controller_h

#include "InputReport.h"

#include "hardware/InputSignal.h"
#include "hardware/misc.h"
#include "hardware/OutputSignal.h"

#include <cstdint>

namespace keyboard { namespace pokey {

class Controller {
public:
  Controller(
      ::hardware::InputSignal k0,
      ::hardware::InputSignal k5,
      ::hardware::OutputSignal kr1,
      ::hardware::OutputSignal kr2,
      ::hardware::BusyWaitEq busyWaitEq);

  /** Receive a keyboard report. */
  void receiveInputReport(const InputReport& report) const;

private:
  const ::hardware::InputSignal m_k0;
  const ::hardware::InputSignal m_k5;
  const ::hardware::OutputSignal m_kr1;
  const ::hardware::OutputSignal m_kr2;

  const ::hardware::BusyWaitEq m_busyWaitEq;
};

} } // namespace keyboard::pokey

#endif // ifndef keyboard_pokey_Controller_h

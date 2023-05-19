#ifndef keyboard_console_Controller_h
#define keyboard_console_Controller_h

#include "KeyBit.h"

#include "hardware/misc.h"
#include "hardware/OutputSignal.h"

namespace keyboard { namespace console {

class Controller {
public:
  Controller(
      ::hardware::OutputSignal start,
      ::hardware::OutputSignal select,
      ::hardware::OutputSignal option,
      ::hardware::OutputSignal reset,
      ::hardware::OutputSignal power,
      ::hardware::PowerOnSequence powerOnSequence);

  /** Receive a keyboard report. */
  void receiveInputReport(const KeyBitset keyBitset);

private:
  template<KeyBit keyBit>
  void processKeyBit(const KeyBitset keyBitset, const ::hardware::OutputSignal& signal);

  const ::hardware::OutputSignal m_start;
  const ::hardware::OutputSignal m_select;
  const ::hardware::OutputSignal m_option;
  const ::hardware::OutputSignal m_reset;
  const ::hardware::OutputSignal m_power;
  const ::hardware::PowerOnSequence m_powerOnSequence;
  bool m_powerLatched;
  bool m_powerActive;
};

} } // namespace keyboard::console

#endif // ifndef keyboard_console_Controller_h

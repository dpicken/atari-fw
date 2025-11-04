#ifndef keyboard_console_Controller_h
#define keyboard_console_Controller_h

#include "KeyBit.h"
#include "KeyReleaseObserver.h"

#include "hal/misc.h"
#include "hal/OutputSignal.h"

#include <functional>

namespace keyboard { namespace console {

class Controller {
public:
  Controller(
      ::hal::OutputSignal start,
      ::hal::OutputSignal select,
      ::hal::OutputSignal option,
      ::hal::OutputSignal reset,
      ::hal::OutputSignal power,
      ::hal::PowerOnSequence powerOnSequence);

  /** Receive a keyboard report. */
  void receiveInputReport(const KeyBitset keyBitset);

private:
  template<KeyBit keyBit>
  void processKeyBit(const KeyBitset keyBitset, const ::hal::OutputSignal& signal);

  template<KeyBit keyBit>
  void processKeyReleaseObserver(const KeyBitset keyBitset, KeyReleaseObserver* observer, std::function<void()> action);

  void togglePower();
  void powerOff();

  const ::hal::OutputSignal m_start;
  const ::hal::OutputSignal m_select;
  const ::hal::OutputSignal m_option;
  const ::hal::OutputSignal m_reset;
  const ::hal::OutputSignal m_power;
  const ::hal::PowerOnSequence m_powerOnSequence;

  KeyReleaseObserver m_powerOffKeyObserver;
  KeyReleaseObserver m_powerToggleKeyObserver;
  KeyReleaseObserver m_powerCycleKeyObserver;
  KeyReleaseObserver m_sbcBootObserver;
  KeyReleaseObserver m_d1EjectKeyObserver;
  KeyReleaseObserver m_d1RotateDiskKeyObserver;

  bool m_powerActive;
};

} } // namespace keyboard::console

#endif // ifndef keyboard_console_Controller_h

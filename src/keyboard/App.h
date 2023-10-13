#ifndef keyboard_app_h
#define keyboard_app_h

#include "hal/InputSignal.h"
#include "hal/misc.h"
#include "hal/OutputSignal.h"
#include "keyboard/Controller.h"
#include "keyboard/console/Controller.h"
#include "keyboard/pokey/Controller.h"

namespace keyboard {

class App {
public:
  App(::hal::OutputSignal start,
      ::hal::OutputSignal select,
      ::hal::OutputSignal option,
      ::hal::OutputSignal reset,
      ::hal::OutputSignal power,
      ::hal::PowerOnSequence powerOnSequence,
      ::hal::InputSignal k0,
      ::hal::InputSignal k5,
      ::hal::OutputSignal kr1,
      ::hal::OutputSignal kr2,
      ::hal::BusyWaitEq busyWaitEq);

  bool isKeyboardAttached() const {
    return m_keyboardAttachedCount != 0;
  }

  void onKeyboardAttached();
  void onKeyboardDetached();
  void onKeyboardInput(const ::keyboard::usb::InputReport& report);

  void schedule();

private:
  ::keyboard::console::Controller m_consoleController;
  ::keyboard::pokey::Controller m_pokeyController;
  ::keyboard::Controller m_keyboardController;

  unsigned int m_keyboardAttachedCount;

  ::keyboard::usb::InputReport m_currentInputReport;
};

} // namespace keyboard

#endif // ifndef keyboard_app_h

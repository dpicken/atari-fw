#ifndef app_app_h
#define app_app_h

#include "hardware/InputSignal.h"
#include "hardware/misc.h"
#include "hardware/OutputSignal.h"
#include "keyboard/Controller.h"
#include "keyboard/console/Controller.h"
#include "keyboard/pokey/Controller.h"

namespace app {

class App {
public:
  App(::hardware::OutputSignal start,
      ::hardware::OutputSignal select,
      ::hardware::OutputSignal option,
      ::hardware::OutputSignal reset,
      ::hardware::OutputSignal power,
      ::hardware::PowerOnSequence powerOnSequence,
      ::hardware::InputSignal k0,
      ::hardware::InputSignal k5,
      ::hardware::OutputSignal kr1,
      ::hardware::OutputSignal kr2,
      ::hardware::BusyWaitEq busyWaitEq);

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

} // namespace app

#endif // ifndef app_app_h

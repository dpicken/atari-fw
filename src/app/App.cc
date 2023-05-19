#include "App.h"

app::App::App(
    ::hardware::OutputSignal start,
    ::hardware::OutputSignal select,
    ::hardware::OutputSignal option,
    ::hardware::OutputSignal reset,
    ::hardware::OutputSignal power,
    ::hardware::PowerOnSequence powerOnSequence,
    ::hardware::InputSignal k0,
    ::hardware::InputSignal k5,
    ::hardware::OutputSignal kr1,
    ::hardware::OutputSignal kr2,
    ::hardware::BusyWaitEq busyWaitEq)
  : m_consoleController(start, select, option, reset, power, powerOnSequence)
  , m_pokeyController(k0, k5, kr1, kr2, busyWaitEq)
  , m_keyboardController(&m_consoleController, &m_pokeyController)
  , m_keyboardAttachedCount(0) {
}

void app::App::onKeyboardAttached() {
  ++m_keyboardAttachedCount;
}

void app::App::onKeyboardDetached() {
  onKeyboardInput({});
  schedule();
  --m_keyboardAttachedCount;
}

void app::App::onKeyboardInput(const ::keyboard::usb::InputReport& report) {
  m_currentInputReport = report;
}

void app::App::schedule() {
  if (m_keyboardAttachedCount > 0) {
    m_keyboardController.receiveInputReport(m_currentInputReport);
  }
}

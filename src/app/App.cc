#include "App.h"

app::App::App(
    ::hal::OutputSignal start,
    ::hal::OutputSignal select,
    ::hal::OutputSignal option,
    ::hal::OutputSignal reset,
    ::hal::OutputSignal power,
    ::hal::PowerOnSequence powerOnSequence,
    ::hal::InputSignal k0,
    ::hal::InputSignal k5,
    ::hal::OutputSignal kr1,
    ::hal::OutputSignal kr2,
    ::hal::BusyWaitEq busyWaitEq)
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

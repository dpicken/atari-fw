#include "Controller.h"

#include "util/type_traits.h"

keyboard::pokey::Controller::Controller(
    ::hardware::InputSignal k0,
    ::hardware::InputSignal k5,
    ::hardware::OutputSignal kr1,
    ::hardware::OutputSignal kr2)
  : m_k0(k0)
  , m_k5(k5)
  , m_kr1(kr1)
  , m_kr2(kr2) {
    m_kr1.deactivate();
    m_kr2.deactivate();
}

void keyboard::pokey::Controller::receiveInputReport(const InputReport& report) const {
  while (m_k5.isActive());
  while (!m_k5.isActive());

  bool k0Active = true;

  for (int debounceCount = 0; debounceCount != 2; ++debounceCount) {
    Traits::scan_code_value_type scanCode = Traits::ScanCodeEnd;
    while (scanCode-- != 0) {
      if (util::to_underlying(report.keyCode) == scanCode) {
        m_kr1.activate();
      }

      if (util::to_underlying(report.modifierCode) == scanCode) {
        m_kr2.activate();
      }

      while(m_k0.isActive() == k0Active);
      k0Active = !k0Active;

      m_kr1.deactivate();
      m_kr2.deactivate();
    }
  }
}

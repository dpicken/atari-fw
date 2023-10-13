#include "Controller.h"

#include "util/type_traits.h"

namespace {

constexpr hal::duration_us PokeyKeyScanTimeout = 250 * 1000;

} // namespace

keyboard::pokey::Controller::Controller(
    ::hal::InputSignal k0,
    ::hal::InputSignal k5,
    ::hal::OutputSignal kr1,
    ::hal::OutputSignal kr2,
    ::hal::BusyWaitEq busyWaitEq)
  : m_k0(k0)
  , m_k5(k5)
  , m_kr1(kr1)
  , m_kr2(kr2)
  , m_busyWaitEq(busyWaitEq) {
}

void keyboard::pokey::Controller::receiveInputReport(const InputReport& report) const {
  if (!m_busyWaitEq(m_k5, false, PokeyKeyScanTimeout)) {
    return;
  }
  if (!m_busyWaitEq(m_k5, true, PokeyKeyScanTimeout)) {
    return;
  }

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

      bool timedOut = !m_busyWaitEq(m_k0, !k0Active, PokeyKeyScanTimeout);
      k0Active = !k0Active;

      m_kr1.deactivate();
      m_kr2.deactivate();

      if (timedOut) {
        return;
      }
    }
  }
}

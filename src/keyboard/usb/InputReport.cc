#include "InputReport.h"

#include "util/type_traits.h"

keyboard::usb::InputReport::InputReport(ModifierBitset mbs, KeyCode k0)
  : modifierBitset(mbs)
  , reserved(0)
  , keyCodes{
      util::to_underlying(k0),
      util::to_underlying(KeyCode::NoEventIndicated),
      util::to_underlying(KeyCode::NoEventIndicated),
      util::to_underlying(KeyCode::NoEventIndicated),
      util::to_underlying(KeyCode::NoEventIndicated),
      util::to_underlying(KeyCode::NoEventIndicated)
    } {
}

keyboard::usb::InputReport::InputReport(ModifierBitset mbs)
  : InputReport(mbs, KeyCode::NoEventIndicated) {
}

keyboard::usb::InputReport::InputReport(ModifierBit mb, KeyCode k0)
  : InputReport(util::to_underlying(mb), k0) {
}

keyboard::usb::InputReport::InputReport(KeyCode k0)
  : InputReport(0, k0) {
}

keyboard::usb::InputReport::InputReport()
  : InputReport(0, KeyCode::NoEventIndicated) {
}

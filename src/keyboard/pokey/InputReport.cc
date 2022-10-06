#include "InputReport.h"

keyboard::pokey::InputReport::InputReport(ModifierCode m, KeyCode k)
  : modifierCode(m)
  , keyCode(k) {
}

keyboard::pokey::InputReport::InputReport(ModifierCode m)
  : InputReport(m, KeyCode::NoEventIndicated) {
}

keyboard::pokey::InputReport::InputReport(KeyCode k)
  : InputReport(ModifierCode::NoEventIndicated, k) {
}

keyboard::pokey::InputReport::InputReport()
  : InputReport(ModifierCode::NoEventIndicated, KeyCode::NoEventIndicated) {
}

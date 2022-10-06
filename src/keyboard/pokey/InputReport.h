#ifndef keyboard_pokey_InputReport_h
#define keyboard_pokey_InputReport_h

#include "KeyCode.h"
#include "ModifierCode.h"

namespace keyboard { namespace pokey {

struct InputReport {
  const ModifierCode modifierCode;
  const KeyCode keyCode;

  InputReport(ModifierCode m, KeyCode k);

  InputReport(ModifierCode m);

  InputReport(KeyCode k);

  InputReport();
};

} } // namespace keyboard::pokey

#endif // ifndef keyboard_pokey_InputReport_h

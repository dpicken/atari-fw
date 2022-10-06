#ifndef keyboard_pokey_ModifierCode_h
#define keyboard_pokey_ModifierCode_h

#include "Traits.h"

namespace keyboard { namespace pokey {

enum class ModifierCode : Traits::scan_code_value_type {
  Control = 0x00,
  Shift = 0x10,
  Break = 0x30,

  NoEventIndicated = Traits::ScanCodeEnd
};

} } // namespace keyboard::pokey

#endif // ifndef keyboard_pokey_ModifierCode_h

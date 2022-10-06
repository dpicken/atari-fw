#ifndef keyboard_usb_ModifierBit_h
#define keyboard_usb_ModifierBit_h

#include <cstdint>
#include <type_traits>

namespace keyboard { namespace usb {

enum class ModifierBit : std::uint8_t {
  LeftCtrl    = 1 << 0,
  LeftShift   = 1 << 1,
  LeftAlt     = 1 << 2,
  LeftGui     = 1 << 3,
  RightCtrl   = 1 << 4,
  RightShift  = 1 << 5,
  RightAlt    = 1 << 6,
  RightGui    = 1 << 7
};

using ModifierBitset = std::underlying_type<ModifierBit>::type;

constexpr inline ModifierBitset operator|(ModifierBit lhs, ModifierBit rhs) {
  return ModifierBitset(lhs) | ModifierBitset(rhs);
}

} } // namespace keyboard::usb

#endif // ifndef keyboard_usb_ModifierBit_h

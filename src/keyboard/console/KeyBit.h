#ifndef keyboard_console_KeyBit_h
#define keyboard_console_KeyBit_h

#include <cstdint>
#include <type_traits>

namespace keyboard { namespace console {

enum class KeyBit : std::uint8_t {
  Start   = 1 << 0,
  Select  = 1 << 1,
  Option  = 1 << 2,
  Reset   = 1 << 3,
  Power   = 1 << 4,
  Eject   = 1 << 5,
  D1RotateDisk = 1 << 6
};

using KeyBitset = std::underlying_type<KeyBit>::type;

constexpr inline KeyBitset operator|(KeyBitset lhs, KeyBit rhs) {
  return lhs | KeyBitset(rhs);
}

constexpr inline KeyBitset& operator|=(KeyBitset& lhs, KeyBit rhs) {
  lhs = (lhs | rhs);
  return lhs;
}

constexpr inline KeyBitset operator&(KeyBitset lhs, KeyBit rhs) {
  return lhs & KeyBitset(rhs);
}

} } // namespace keyboard::console

#endif // ifndef keyboard_console_KeyBit_h

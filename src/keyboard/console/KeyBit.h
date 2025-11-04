#ifndef keyboard_console_KeyBit_h
#define keyboard_console_KeyBit_h

#include <cstdint>
#include <type_traits>

namespace keyboard { namespace console {

enum class KeyBit {
  Start             = 1 << 0,
  Select            = 1 << 1,
  Option            = 1 << 2,
  Reset             = 1 << 3,
  PowerOff          = 1 << 4,
  PowerCycle        = 1 << 5,
  PowerToggle       = 1 << 6,
  SbcBoot           = 1 << 7,
  D1Eject           = 1 << 8,
  D1RotateDisk      = 1 << 9
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

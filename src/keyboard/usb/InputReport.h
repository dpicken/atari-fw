#ifndef keyboard_usb_InputReport_h
#define keyboard_usb_InputReport_h

#include "KeyCode.h"
#include "ModifierBit.h"
#include "util/type_traits.h"

#include <array>
#include <compare>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace keyboard { namespace usb {

struct InputReport {
  using KeyCodes = std::array<std::underlying_type<KeyCode>::type, 6>;

  ModifierBitset modifierBitset;
  std::uint8_t reserved;
  KeyCodes keyCodes;

  InputReport(ModifierBitset mbs, KeyCode k0);

  InputReport(ModifierBitset mbs);

  InputReport(ModifierBit mb, KeyCode k0);

  InputReport(KeyCode k0);

  InputReport();

  bool operator==(const InputReport&) const = default;

  struct Hash {
    std::size_t operator()(const InputReport& report) const {
      return (static_cast<size_t>(report.modifierBitset) << 8) | report.keyCodes[0];
    }
  };
};

} } // namespace keyboard::usb

#endif // ifndef keyboard_usb_InputReport_h

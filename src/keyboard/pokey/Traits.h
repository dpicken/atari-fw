#ifndef keyboard_pokey_Traits_h
#define keyboard_pokey_Traits_h

#include <cstdint>

namespace keyboard { namespace pokey {

struct Traits {
  typedef std::uint8_t scan_code_value_type;

  static const int ScanCodeBitCount = 6;
  static const scan_code_value_type ScanCodeEnd = 1 << ScanCodeBitCount;
};


} } // namespace keyboard::pokey

#endif // ifndef keyboard_pokey_Traits_h

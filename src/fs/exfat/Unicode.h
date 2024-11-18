#ifndef fs_exfat_Unicode_h
#define fs_exfat_Unicode_h

#include "util/PACKED.h"

#include <cstdint>
#include <iterator>
#include <string>

namespace fs { namespace exfat {

struct UTF16ToAsciiBackInserter {
  using string_type = std::string;

  using code_unit_type = std::uint16_t;
  using code_point_type = std::uint32_t;

  UTF16ToAsciiBackInserter(string_type& str);
  UTF16ToAsciiBackInserter& operator=(code_unit_type codeUnit);

  UTF16ToAsciiBackInserter& operator++();
  UTF16ToAsciiBackInserter& operator++(int);
  UTF16ToAsciiBackInserter& operator*();

private:
  void consumeCodeUnit(code_unit_type codeUnit);

  static bool isSurrogate(code_unit_type codeUnit);
  static bool isHighSurrogate(code_unit_type codeUnit);
  static bool isLowSurrogate(code_unit_type codeUnit);

  std::string* const m_str;

  code_unit_type m_previousCodeUnit;
};

template<typename StringBackInserterType, std::size_t ArraySizeV>
inline void appendUnicode(const util::PackedArray<std::uint16_t, ArraySizeV>& unicode, std::size_t maxCodeUnitCount, std::string& str) {
  unicode.copy(maxCodeUnitCount, StringBackInserterType(str));
}

} } // namespace fs::exfat

#endif // ifndef fs_exfat_Unicode_h

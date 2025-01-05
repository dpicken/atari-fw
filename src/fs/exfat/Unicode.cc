#include "Unicode.h"

#include "Configuration.h"

namespace {

inline char toAscii(std::uint32_t codePoint) {
  return (codePoint <= 0x7F) ? static_cast<char>(codePoint) : fs::exfat::Configuration::instance()->nonAsciiReplacementCharacter();
}

} // namespace

fs::exfat::UTF16ToAsciiBackInserter::UTF16ToAsciiBackInserter(string_type& str)
  : m_str(&str)
  , m_previousCodeUnit(0) {
}

fs::exfat::UTF16ToAsciiBackInserter& fs::exfat::UTF16ToAsciiBackInserter::operator=(code_unit_type codeUnit) {
  consumeCodeUnit(codeUnit);
  m_previousCodeUnit = codeUnit;
  return *this;
}

fs::exfat::UTF16ToAsciiBackInserter& fs::exfat::UTF16ToAsciiBackInserter::operator++() {
  return *this;
}

fs::exfat::UTF16ToAsciiBackInserter& fs::exfat::UTF16ToAsciiBackInserter::operator++(int) {
  return *this;
}

fs::exfat::UTF16ToAsciiBackInserter& fs::exfat::UTF16ToAsciiBackInserter::operator*() {
  return *this;
}

void fs::exfat::UTF16ToAsciiBackInserter::consumeCodeUnit(code_unit_type codeUnit) {
  if (!isSurrogate(codeUnit)) {
    m_str->push_back(toAscii(codeUnit));
  } else if (isHighSurrogate(m_previousCodeUnit) && isLowSurrogate(codeUnit)) {
    m_str->push_back(Configuration::instance()->nonAsciiReplacementCharacter());
  }
}

bool fs::exfat::UTF16ToAsciiBackInserter::isSurrogate(code_unit_type codeUnit) {
  return codeUnit >= 0xD800 && codeUnit <= 0xDFFF;
}

bool fs::exfat::UTF16ToAsciiBackInserter::isHighSurrogate(code_unit_type codeUnit) {
  return codeUnit >= 0xD800 && codeUnit <= 0xDBFF;
}

bool fs::exfat::UTF16ToAsciiBackInserter::isLowSurrogate(code_unit_type codeUnit) {
  return codeUnit >= 0xDC00 && codeUnit <= 0xDFFF;
}

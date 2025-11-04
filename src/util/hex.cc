#include "hex.h"

#include <array>

std::string util::toHexString(const std::uint8_t* begin, const std::uint8_t* end) {
  constexpr std::array<char, 16> hexDigits{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

  std::string str;
  for (auto it = begin; it != end; ++it) {
    str.push_back(hexDigits[(*it >> 4) & 0x0F]);
    str.push_back(hexDigits[(*it) & 0x0F]);
  }
  return str;
}

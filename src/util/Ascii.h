#ifndef util_Ascii_h
#define util_Ascii_h

#include <algorithm>
#include <string_view>

namespace util {

constexpr inline char asciiToLower(char c) {
  constexpr auto upper_to_lower_difference_v = 'a' - 'A';
  return c >= 'A' && c <= 'Z' ? c + upper_to_lower_difference_v : c;
}

constexpr inline int asciiCaseInsensitiveCompare(char c1, char c2) {
  return asciiToLower(c1) - asciiToLower(c2);
}

constexpr inline bool asciiCaseInsensitivelyEqual(char c1, char c2) {
  return asciiCaseInsensitiveCompare(c1, c2) == 0;
}

struct AsciiCaseInsensitivelyLess {
  bool operator()(const std::string_view& s1, const std::string_view& s2) const {
    return std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(), [](auto c1, auto c2) { return asciiCaseInsensitiveCompare(c1, c2) < 0; });
  }
};

struct AsciiCaseInsensitivelyEqual {
  bool operator()(const std::string_view& s1, const std::string_view& s2) const {
    return std::equal(s1.begin(), s1.end(), s2.begin(), s2.end(), [](auto c1, auto c2) { return asciiCaseInsensitiveCompare(c1, c2) == 0; });
  }
};

struct AsciiCaseInsensitivelyGreater {
  bool operator()(const std::string_view& s1, const std::string_view& s2) const {
    return !AsciiCaseInsensitivelyLess{}(s1, s2) && !AsciiCaseInsensitivelyEqual{}(s1, s2);
  }
};

} // namespace util

#endif // ifndef util_Ascii_h

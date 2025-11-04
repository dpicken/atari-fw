#ifndef util_hex_h
#define util_hex_h

#include <cstdint>
#include <string>

namespace util {

std::string toHexString(const std::uint8_t* begin, const std::uint8_t* end);

} // namespace util

#endif // ifndef util_hex_h

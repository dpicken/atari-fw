#ifndef util_crc_h
#define util_crc_h

#include <cstddef>
#include <cstdint>

namespace util {

  std::uint8_t crc7(const std::uint8_t* data, std::size_t size);

  std::uint16_t crc16(const std::uint8_t* data, std::size_t size);

} // namespace util

#endif // ifndef util_crc_h

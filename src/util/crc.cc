#include "crc.h"

std::uint8_t util::crc7(const std::uint8_t* data, std::size_t size) {
  constexpr const std::uint8_t polynomial = 0x89;
  std::uint8_t crc = 0;
  for (auto it = data, end = data + size; it != end; ++it) {
    crc ^= *it;
    for (int j = 0; j < 8; j++) {
      crc = (crc & 0x80) ? std::uint8_t((crc << 1) ^ (polynomial << 1)) : std::uint8_t(crc << 1);
    }
  }
  return crc >> 1;
}

std::uint16_t util::crc16(const std::uint8_t* data, std::size_t size) {
  std::uint16_t crc = 0;
  for (auto it = data, end = data + size; it != end; ++it) {
    crc = static_cast<std::uint16_t>(crc << 8) | (crc >> 8);
    crc ^= *it;
    crc ^= (crc & 0x00FF) >> 4;
    crc ^= (crc << 12);
    crc ^= (crc & 0x00FF) << 5;
  }
  return crc;
}

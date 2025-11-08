#include "checksum.h"

void sio::Checksum::update(const std::uint8_t* it, std::size_t byteCount) {
  for (auto end = it + byteCount; it != end; ++it) {
    auto prev = m_checksum;
    m_checksum += *it;
    if (m_checksum < prev) {
      ++m_checksum;
    }
  }
}

std::uint8_t sio::checksum(const std::uint8_t* it, std::size_t byteCount) {
  Checksum checksum;
  checksum.update(it, byteCount);
  return checksum.get();
}

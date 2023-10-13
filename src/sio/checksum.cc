#include "checksum.h"

std::uint8_t sio::checksum(const std::uint8_t* it, std::size_t byteCount) {
  std::uint8_t sum = 0;
  for (auto end = it + byteCount; it != end; ++it) {
    auto prev = sum;
    sum += *it;
    if (sum < prev) {
      ++sum;
    }
  }
  return sum;
}

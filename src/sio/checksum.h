#ifndef sio_checksum_h
#define sio_checksum_h

#include <cstddef>
#include <cstdint>

namespace sio {

  std::uint8_t checksum(const std::uint8_t* it, std::size_t byteCount);

  template<typename PackedType>
  std::uint8_t checksum(const PackedType& value) {
    return checksum(reinterpret_cast<const uint8_t*>(&value), sizeof(value));
  }

} // namespace sio

#endif // ifndef sio_checksum_h

#ifndef sio_checksum_h
#define sio_checksum_h

#include <cstddef>
#include <cstdint>

namespace sio {

  std::uint8_t checksum(const std::uint8_t* it, std::size_t byteCount);

} // namespace sio

#endif // ifndef sio_checksum_h

#ifndef sio_Frame_h
#define sio_Frame_h

#include "checksum.h"

#include "hal/Uart.h"
#include "util/PACKED.h"

namespace sio {

template<typename Data>
struct Frame {
  Frame() {
  }

  Frame(const Data& data)
    : m_data(data) {
  }

  const Data* data() const {
    return &m_data;
  }

  bool rx(const ::hal::Uart* uart) {
    if (!uart->rx(reinterpret_cast<std::uint8_t*>(this), sizeof(*this))) {
      return false;
    }
    if (m_checksum != checksum(reinterpret_cast<const std::uint8_t*>(&m_data), sizeof(m_data))) {
      return false;
    }
    return true;
  }

  bool tx(const ::hal::Uart* uart) {
    m_checksum = checksum(reinterpret_cast<std::uint8_t*>(&m_data), sizeof(m_data));
    return uart->tx(reinterpret_cast<const std::uint8_t*>(this), sizeof(*this));
  }

private:
  Data m_data;
  std::uint8_t m_checksum;
} PACKED;

} // namespace sio

#endif // ifndef sio_Frame_h
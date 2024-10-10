#ifndef hal_Spi_h
#define hal_Spi_h

#include "OutputSignal.h"

#include <cstddef>
#include <cstdint>

namespace hal {

struct Spi {
  typedef void (* const Rx)(std::uint8_t* buf, std::size_t byteCount);
  typedef void (* const Tx)(const std::uint8_t* buf, std::size_t byteCount);
  typedef void (* const SetBaudRate)(unsigned int baudRate);

  constexpr Spi(Rx rx, Tx tx, SetBaudRate setBaudRate)
    : m_rx(rx)
    , m_tx(tx)
    , setBaudRate(setBaudRate) {
  }


  template<typename PackedType>
  void rx(PackedType* value) const {
    m_rx(reinterpret_cast<std::uint8_t*>(value), sizeof(*value));
  }

  void rx(std::uint8_t* data, std::size_t size) const {
    m_rx(data, size);
  }

  template<typename PackedType>
  void tx(const PackedType* value) const {
    m_tx(reinterpret_cast<const std::uint8_t*>(value), sizeof(*value));
  }

private:
  const Rx m_rx;
  const Tx m_tx;

public:
  const SetBaudRate setBaudRate;
};

} // namespace hal

#endif // ifndef hal_Spi_h

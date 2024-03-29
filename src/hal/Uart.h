#ifndef hal_Uart_h
#define hal_Uart_h

#include <cstddef>
#include <cstdint>

namespace hal {

struct Uart {
  typedef bool (* const Rx)(std::uint8_t* buf, std::size_t byteCount);
  typedef void (* const Tx)(const std::uint8_t* buf, std::size_t byteCount);

  constexpr Uart(Rx rx, Tx tx)
    : m_rx(rx)
    , m_tx(tx) {
  }

  template<typename PackedType>
  bool rx(PackedType& value) const {
    return m_rx(reinterpret_cast<std::uint8_t*>(&value), sizeof(value));
  }

  template<typename PackedType>
  void tx(const PackedType& value) const {
    m_tx(reinterpret_cast<const std::uint8_t*>(&value), sizeof(value));
  }

  void tx(const std::uint8_t* data, std::size_t size) const {
    m_tx(data, size);
  }

private:
  const Rx m_rx;
  const Tx m_tx;
};

} // namespace hal

#endif // ifndef hal_Uart_h

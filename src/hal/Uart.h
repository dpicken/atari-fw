#ifndef hal_Uart_h
#define hal_Uart_h

#include <cstddef>
#include <cstdint>

namespace hal {

struct Uart {
  typedef bool (* const Rx)(std::uint8_t* buf, std::size_t byteCount);
  typedef bool (* const Tx)(const std::uint8_t* buf, std::size_t byteCount);

  constexpr Uart(Rx r, Tx t)
    : rx(r)
    , tx(t) {
  }

  void txByte(std::uint8_t b) const {
    tx(&b, 1);
  }

  const Rx rx;
  const Tx tx;
};

} // namespace hal

#endif // ifndef hal_Uart_h

#ifndef sio_Traits_h
#define sio_Traits_h

namespace sio {

struct Traits {
  static constexpr unsigned int uart_start_bit_count = 1;

  static constexpr unsigned int uart_data_bit_count = 8;

  static constexpr unsigned int uart_stop_bit_count = 1;

  static constexpr unsigned int uart_baud_rate = 19040;
};

} // namespace sio

#endif // ifndef sio_Traits_h

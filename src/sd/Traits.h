#ifndef sd_Traits_h
#define sd_Traits_h

#include "hal/duration_us.h"

namespace sd {

struct Traits {
  static constexpr std::uint8_t spi_null_byte = 0xFF;

  static constexpr unsigned int spi_init_baud_rate = 400 * 1000;
  static constexpr unsigned int spi_data_baud_rate = 25 * 1000 * 1000;

  static constexpr unsigned int rx_retry_count = 10;
  static constexpr ::hal::duration_us rx_retry_delay = 100 * 1000;
};

} // namespace sd

#endif // ifndef sd_Traits_h

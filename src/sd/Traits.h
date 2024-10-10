#ifndef sd_Traits_h
#define sd_Traits_h

#include "hal/duration_us.h"

#include <chrono>
#include <limits>

namespace sd {

struct Traits {
  static constexpr std::uint8_t spi_null_byte = 0xFF;

  static constexpr unsigned int spi_init_baud_rate = 400 * 1000;
  static constexpr unsigned int spi_data_baud_rate = 25 * 1000 * 1000;

  // ncs: Post chip-select.
  static constexpr unsigned int ncs_clock_cycle_count = 8;
  // ncr: Pre response rx.
  static constexpr unsigned int max_ncr_clock_cycle_count = 64;
  // ncx: Pre read-csd-data rx.
  static constexpr unsigned int max_ncx_clock_cycle_count = 64;
  // nac: Pre read-data-block rx.
  static constexpr auto tacc_max_duration = std::chrono::milliseconds(10) * 8;
  static constexpr auto tacc_max_clock_cycle_count = tacc_max_duration.count() * (spi_data_baud_rate / decltype(tacc_max_duration)::period::den);
  static constexpr auto nsac_max_clock_cycle_count = 100 * std::numeric_limits<std::uint8_t>::max();
  static constexpr unsigned int max_nac_clock_cycle_count = 100 * (tacc_max_clock_cycle_count + nsac_max_clock_cycle_count);
  // nrc: Inter response-next-command.
  static constexpr unsigned int min_nrc_clock_cycle_count = 1;
  // nec: Pre chip-deselect.
  static constexpr unsigned int nec_clock_cycle_count = 8;

  static constexpr ::hal::duration_us command_retry_delay = 100 * 1000;
  static constexpr unsigned int command_retry_count = 8;
  static constexpr unsigned int response_retry_count = 8;
  static constexpr unsigned int read_token_retry_count = 8;
};

} // namespace sd

#endif // ifndef sd_Traits_h

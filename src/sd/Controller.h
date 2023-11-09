#ifndef sd_Controller_h
#define sd_Controller_h

#include "SpiAccessor.h"

#include "BlockSize.h"

#include "hal/InputSignal.h"
#include "hal/misc.h"
#include "hal/Spi.h"

#include <array>
#include <optional>
#include <functional>

namespace sd {

class Controller {
public:
  static constexpr std::uint32_t block_byte_count = 512;
  static constexpr std::size_t max_read_byte_count = block_byte_count;

  using open_id_type = unsigned int;
  using optional_open_id_type = std::optional<open_id_type>;
  using Sink = std::function<void(std::uint8_t* data, std::size_t size)>;

  Controller(
      ::hal::InputSignal detect,
      ::hal::OutputSignal power,
      ::hal::OutputSignal cs,
      ::hal::Spi spi,
      ::hal::BusyWait busyWait);

  void poll();

  optional_open_id_type getOpenId() const {
    return m_currentOpenId;
  }

  bool read(open_id_type openId, std::uint64_t byteOffset, std::size_t byteCount, Sink sink);

private:
  static constexpr ::hal::duration_us sd_detect_debounce_time = 50 * 1000;

  void pollCardInserted();
  void pollCardEjected();

  bool initialize();
  bool cacheBlock(std::uint64_t byteOffset);

  const ::hal::InputSignal m_detect;
  const ::hal::OutputSignal m_power;
  const ::hal::BusyWait m_busyWait;
  const SpiAccessor m_accessor;

  open_id_type m_nextOpenId;

  optional_open_id_type m_currentOpenId;

  bool m_initialized;
  bool m_isSDSC;

  std::optional<uint32_t> m_cachedBlockAddress;
  std::array<std::uint8_t, BlockSize::byte_count> m_cachedBlock;
};

} // namespace sd

#endif // ifndef sd_Controller_h

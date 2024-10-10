#ifndef sd_Controller_h
#define sd_Controller_h

#include "Controller.fwd.h"

#include "Block.h"
#include "Card.h"
#include "SpiAccessor.h"

#include "fs/File.h"
#include "hal/InputSignal.h"
#include "hal/misc.h"
#include "hal/Spi.h"

#include <array>
#include <optional>
#include <string_view>

namespace sd {

class Controller {
public:
  using sink_type = ::fs::File::sink_type;

  Controller(
      ::hal::InputSignal detect,
      ::hal::OutputSignal power,
      ::hal::OutputSignal cs,
      ::hal::Spi spi,
      ::hal::BusyWait busyWait);

  void poll();

  bool read(Card* card, std::uint64_t byteOffset, std::size_t byteCount, sink_type sink);

  Card::ptr_type getCurrentCard() const {
    return m_currentCard;
  }

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

  std::string_view m_initializationFailureReason;
  bool m_isSDSC;

  Card::ptr_type m_currentCard;

  std::optional<uint32_t> m_cachedBlockAddress;
  Block m_cachedBlock;
};

} // namespace sd

#endif // ifndef sd_Controller_h

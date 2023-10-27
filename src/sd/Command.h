#ifndef sd_Command_h
#define sd_Command_h

#include "util/PACKED.h"

#include <cstdint>

namespace sd {

struct Command {
  enum class Cmd : std::uint8_t {
    GoIdleState = 0,
    SendIfCond = 8,
    SetBlocklen = 16,
    ReadSingleBlock = 17,
    AppCmd = 55,
    ReadOcr = 58
  };

  enum class ACmd : std::uint8_t {
    SdSendOpCond = 41
  };

  using argument_type = std::uint32_t;

  Command(Cmd cmd, argument_type argument);

  Command(ACmd acmd, argument_type argument);

private:
  Command(std::uint8_t index, argument_type argument);

  const std::uint8_t m_data[5];
  const std::uint8_t m_crc;
} PACKED;

} // namespace sd

#endif // ifndef sd_Command_h

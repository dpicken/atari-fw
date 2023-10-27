#include "Command.h"

#include "util/crc.h"
#include "util/type_traits.h"

sd::Command::Command(Cmd cmd, argument_type argument)
  : Command(util::to_underlying(cmd), argument) {
}
sd::Command::Command(ACmd acmd, argument_type argument)
  : Command(util::to_underlying(acmd), argument) {
}

sd::Command::Command(std::uint8_t index, argument_type argument)
  : m_data{
      static_cast<std::uint8_t>(0x40 | index),
      static_cast<std::uint8_t>(argument >> 24),
      static_cast<std::uint8_t>(argument >> 16),
      static_cast<std::uint8_t>(argument >>  8),
      static_cast<std::uint8_t>(argument)
    }
  , m_crc(static_cast<std::uint8_t>((::util::crc7(m_data, sizeof(m_data)) << 1) | 0x01)) {
}

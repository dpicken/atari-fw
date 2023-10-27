#include "SpiAccessor.h"

#include "Traits.h"

#include "util/crc.h"
#include "util/type_traits.h"

namespace {

void spi_8_clocks(const hal::Spi& spi) {
  spi.tx(sd::Traits::spi_null_byte);
}

struct ScopedChipSelect {
  ScopedChipSelect(const hal::OutputSignal& cs, const hal::Spi& spi)
    : m_cs(cs)
    , m_spi(spi) {
    spi_8_clocks(m_spi);
    m_cs.activate();
  }

  ~ScopedChipSelect() {
    m_cs.deactivate();
    spi_8_clocks(m_spi);
  }

private:
  const hal::OutputSignal& m_cs;
  const hal::Spi& m_spi;
};

} // namespace

sd::SpiAccessor::SpiAccessor(
    ::hal::OutputSignal cs,
    ::hal::Spi spi,
    ::hal::BusyWait busyWait)
  : m_cs(cs)
  , m_spi(spi)
  , m_busyWait(busyWait) {
}

void sd::SpiAccessor::initialize() const {
  constexpr ::hal::duration_us vdd_settle_time = 1 * 1000;
  m_busyWait(vdd_settle_time);

  constexpr unsigned int prepareClockTickCount = 74;
  for (unsigned int prepareClockTicks = 0; prepareClockTicks < prepareClockTickCount; prepareClockTicks += 8) {
    spi_8_clocks(m_spi);
  }
}

sd::Response::R1 sd::SpiAccessor::goIdleState() const {
  return execute(Command::Cmd::GoIdleState, 0);
}

sd::Response::R7 sd::SpiAccessor::sendIfCond(VHS vhs, std::uint8_t checkPattern) const {
  argument_type argument = (util::to_underlying(vhs) << 8) | checkPattern;
  return execute<sd::Response::R7>(Command::Cmd::SendIfCond, argument);
}

sd::Response::R1 sd::SpiAccessor::setBlocklen(argument_type byteCount) const {
  return execute(Command::Cmd::SetBlocklen, byteCount);
}

sd::Response::R1 sd::SpiAccessor::readSingleBlock(argument_type address) const {
  return execute(Command::Cmd::ReadSingleBlock, address);
}

sd::Response::R3 sd::SpiAccessor::readOcr() const {
  return execute<sd::Response::R3>(Command::Cmd::ReadOcr, 0);
}

sd::Response::R1 sd::SpiAccessor::sdSendOpCond(bool sdscSupportOnly) const {
  constexpr argument_type hcsBit = 0x40000000;
  argument_type argument = sdscSupportOnly ? 0x00000000 : hcsBit;
  return execute(Command::ACmd::SdSendOpCond, argument);
}

template<typename ResponseType>
ResponseType sd::SpiAccessor::execute(Command::Cmd cmd, Command::argument_type argument) const {
  return execute<ResponseType>(Command(cmd, argument));
}

template<typename ResponseType>
ResponseType sd::SpiAccessor::execute(Command::ACmd acmd, Command::argument_type argument) const {
  ResponseType response = execute<ResponseType>(Command(Command::Cmd::AppCmd, 0));
  if (!response.isSuccess() && !response.isInIdleStateOnly()) {
    return response;
  }

  return execute<ResponseType>(Command(acmd, argument));
}

template<typename ResponseType>
ResponseType sd::SpiAccessor::execute(Command command) const {
  ScopedChipSelect cs(m_cs, m_spi);

  m_spi.tx(command);

  ResponseType response;
  for (unsigned int retry = 0; retry != Traits::rx_retry_count; ++retry) {
    m_spi.rx(*response.r1());
    if (response.isValid()) {
      if constexpr (sizeof(ResponseType) > sizeof(Response::R1)) {
        m_spi.rx(response.data());
      }
      break;
    }
    m_busyWait(Traits::rx_retry_delay);
  }
  return response;
}

bool sd::SpiAccessor::rx(std::uint8_t* data, std::size_t byteCount) const {
  ScopedChipSelect cs(m_cs, m_spi);

  for (unsigned int retry = 0; retry != Traits::rx_retry_count; ++retry) {
    Response::ReadDataToken token;
    m_spi.rx(token);
    if (token.isError()) {
      return false;
    }

    if (token.isSuccess()) {
      m_spi.rx(data, byteCount);
      Response::CRC crc;
      m_spi.rx(crc);
      if (!crc.isEq(::util::crc16(data, byteCount))) {
        return false;
      }
      return true;
    }

    m_busyWait(Traits::rx_retry_delay);
  }
  return false;
}

#include "SpiAccessor.h"

#include "Traits.h"

#include "util/crc.h"
#include "util/type_traits.h"

#include <algorithm>

namespace {

void txNullByte(const hal::Spi& spi) {
  return spi.tx(&sd::Traits::spi_null_byte);
}

unsigned int cycleCountToByteCount(unsigned int cycleCount) {
  return std::max(cycleCount / 8, 1U);
}

void busyWaitClockCycles(unsigned int cycleCount, const hal::Spi& spi) {
  auto byteCount = cycleCountToByteCount(cycleCount);
  while (byteCount-- != 0) {
    txNullByte(spi);
  }
}

template<typename ByteType>
void tryBusyWaitRxNonNullByte(ByteType* byte, unsigned int maxCycleCount, const hal::Spi& spi) {
  static_assert (sizeof(*byte) == sizeof(std::uint8_t));
  auto byteCount = cycleCountToByteCount(maxCycleCount);
  while (byteCount-- != 0) {
    spi.rx(byte);
    if (*reinterpret_cast<const std::uint8_t*>(byte) != sd::Traits::spi_null_byte) {
      return;
    }
  }
}

struct ScopedChipSelect {
  ScopedChipSelect(const hal::OutputSignal& cs, const hal::Spi& spi)
    : m_cs(cs)
    , m_spi(spi) {
    m_cs.activate();
    busyWaitClockCycles(sd::Traits::ncs_clock_cycle_count, m_spi);
  }

  ~ScopedChipSelect() {
    busyWaitClockCycles(sd::Traits::nec_clock_cycle_count, m_spi);
    m_cs.deactivate();
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

void sd::SpiAccessor::setBaudRate(unsigned int baudRate) const {
  m_spi.setBaudRate(baudRate);
}

void sd::SpiAccessor::initialize() const {
  constexpr unsigned int prepareClockTickCount = 74;
  for (unsigned int prepareClockTicks = 0; prepareClockTicks < prepareClockTickCount; prepareClockTicks += 8) {
    txNullByte(m_spi);
  }
}

sd::Response::R1 sd::SpiAccessor::goIdleState() const {
  ScopedChipSelect cs(m_cs, m_spi);
  return execute(Command::Cmd::GoIdleState, 0);
}

sd::Response::R7 sd::SpiAccessor::sendIfCond(VHS vhs, std::uint8_t checkPattern) const {
  ScopedChipSelect cs(m_cs, m_spi);
  argument_type argument = (util::to_underlying(vhs) << 8) | checkPattern;
  return execute<Response::R7>(Command::Cmd::SendIfCond, argument);
}

sd::Response::R1 sd::SpiAccessor::setBlocklen(argument_type byteCount) const {
  ScopedChipSelect cs(m_cs, m_spi);
  return execute(Command::Cmd::SetBlocklen, byteCount);
}

sd::Response::R3 sd::SpiAccessor::readOcr() const {
  ScopedChipSelect cs(m_cs, m_spi);
  return execute<Response::R3>(Command::Cmd::ReadOcr, 0);
}

sd::Response::R1 sd::SpiAccessor::sdSendOpCond(bool sdscSupportOnly) const {
  ScopedChipSelect cs(m_cs, m_spi);
  constexpr argument_type hcsBit = 0x40000000;
  argument_type argument = sdscSupportOnly ? 0x00000000 : hcsBit;
  return execute(Command::ACmd::SdSendOpCond, argument);
}

bool sd::SpiAccessor::readCsd(CSD* csd) const {
  ScopedChipSelect cs(m_cs, m_spi);

  if (!sendCsd().isSuccess()) {
    return false;
  }

  if (!rxBlock(csd, Traits::max_ncx_clock_cycle_count)) {
    return false;
  }

  if (!csd->verifyCRC()) {
    return false;
  }

  return true;
}

sd::Response::R1 sd::SpiAccessor::sendCsd() const {
  return execute(Command::Cmd::SendCsd, 0);
}

bool sd::SpiAccessor::readBlock(Block* block, argument_type address) const {
  ScopedChipSelect cs(m_cs, m_spi);

  if (!readSingleBlock(address).isSuccess()) {
    return false;
  }

  if (!rxBlock(block, Traits::max_nac_clock_cycle_count)) {
    return false;
  }

  return true;
}

sd::Response::R1 sd::SpiAccessor::readSingleBlock(argument_type address) const {
  return execute(Command::Cmd::ReadSingleBlock, address);
}

template<typename ResponseType>
ResponseType sd::SpiAccessor::execute(Command::Cmd cmd, Command::argument_type argument) const {
  return execute<ResponseType>(Command(cmd, argument));
}

template<typename ResponseType>
ResponseType sd::SpiAccessor::execute(Command::ACmd acmd, Command::argument_type argument) const {
  ResponseType response = execute<Response::R1>(Command(Command::Cmd::AppCmd, 0));
  if (!response.isSuccess() && !response.isInIdleStateOnly()) {
    return response;
  }

  busyWaitClockCycles(Traits::min_nrc_clock_cycle_count, m_spi);

  return execute<ResponseType>(Command(acmd, argument));
}

template<typename ResponseType>
ResponseType sd::SpiAccessor::execute(Command command) const {
  m_spi.tx(&command);

  ResponseType response;
  tryBusyWaitRxNonNullByte(response.r1(), Traits::max_ncr_clock_cycle_count, m_spi);
  if constexpr (sizeof(ResponseType) > sizeof(Response::R1)) {
    if (response.isValid()) {
      m_spi.rx(&response.data());
    }
  }
  return response;
}

bool sd::SpiAccessor::rxBlock(std::uint8_t* data, std::size_t byteCount, unsigned int maxBusyWaitCycleCount) const {
  Response::ReadDataToken token;
  tryBusyWaitRxNonNullByte(&token, maxBusyWaitCycleCount, m_spi);
  if (!token.isSuccess()) {
    return false;
  }

  m_spi.rx(data, byteCount);

  Response::CRC crc;
  m_spi.rx(&crc);
  if (!crc.isEq(::util::crc16(data, byteCount))) {
    return false;
  }

  return true;
}

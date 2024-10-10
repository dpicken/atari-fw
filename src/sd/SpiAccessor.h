#ifndef sd_SpiAccessor_h
#define sd_SpiAccessor_h

#include "Block.h"
#include "Command.h"
#include "CSD.h"
#include "VHS.h"
#include "Response.h"

#include "hal/misc.h"
#include "hal/Spi.h"

#include <functional>

namespace sd {

class SpiAccessor {
public:
  using argument_type = Command::argument_type;

  SpiAccessor(
      ::hal::OutputSignal cs,
      ::hal::Spi spi,
      ::hal::BusyWait busyWait);

  void setBaudRate(unsigned int baudRate) const;

  void initialize() const;

  Response::R1 goIdleState() const;

  Response::R7 sendIfCond(VHS vhs, std::uint8_t checkPattern) const;

  Response::R1 setBlocklen(argument_type byteCount) const;

  Response::R3 readOcr() const;

  Response::R1 sdSendOpCond(bool sdscSupportOnly) const;

  bool readCsd(CSD* csd) const;
  bool readBlock(Block* block, argument_type address) const;

private:
  Response::R1 sendCsd() const;
  Response::R1 readSingleBlock(argument_type address) const;

  bool rxBlock(std::uint8_t* data, std::size_t byteCount, unsigned int maxCycleCount) const;

  template<typename PackedType>
  bool rxBlock(PackedType* value, unsigned int maxCycleCount) const {
    return rxBlock(reinterpret_cast<std::uint8_t*>(value), sizeof(*value), maxCycleCount);
  }

  template<typename ResponseType = Response::R1>
  ResponseType execute(Command::Cmd cmd, Command::argument_type argument) const;

  template<typename ResponseType = Response::R1>
  ResponseType execute(Command::ACmd acmd, Command::argument_type argument) const;

  template<typename ResponseType>
  ResponseType execute(Command command) const;

  const ::hal::OutputSignal m_cs;
  const ::hal::Spi m_spi;
  const ::hal::BusyWait m_busyWait;
};

} // namespace sd

#endif // ifndef sd_SpiAccessor_h

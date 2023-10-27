#include "Controller.h"

#include "Traits.h"

#include <algorithm>

sd::Controller::Controller(
    ::hal::InputSignal detect,
    ::hal::OutputSignal power,
    ::hal::OutputSignal cs,
    ::hal::Spi spi,
    ::hal::BusyWait busyWait)
  : m_detect(detect)
  , m_power(power)
  , m_busyWait(busyWait)
  , m_accessor(cs, spi, busyWait)
  , m_nextOpenId(0)
  , m_initialized(false)
  , m_isSDSC(false) {
  // HACK: The v1.1 hardware pulls detect to switched 3.3V.
  m_power.activate();
}

void sd::Controller::poll() {
  pollCardInserted();
  // HACK: Current build of the v1.1 hardware has a broken detect signal.
  //pollCardEjected();
}

void sd::Controller::pollCardInserted() {
  if (!m_detect.isActive() || m_currentOpenId) {
    return;
  }

  m_currentOpenId = m_nextOpenId++;

  // HACK: The v1.1 hardware pulls detect to switched 3.3V.
  m_power.deactivate();
  m_busyWait(1000 * 1000);

  m_initialized = initialize();
}

void sd::Controller::pollCardEjected() {
  if (m_detect.isActive() || !m_currentOpenId) {
    return;
  }

  m_cachedBlockAddress = std::nullopt;
  m_currentOpenId = std::nullopt;
}

bool sd::Controller::initialize() {
  // Part1_Physical_Layer_Simplified_Specification_Ver8.00.pdf, 6.4.1:

  m_power.activate();

  constexpr ::hal::duration_us power_up_delay = 2 * 1000;
  m_busyWait(power_up_delay);

  m_accessor.initialize();

  // Part1_Physical_Layer_Simplified_Specification_Ver8.00.pdf, 7.2.1:

  if (!m_accessor.goIdleState().isInIdleStateOnly()) {
    return false;
  }

  constexpr VHS vhs = VHS::range_2V7_3V6;
  constexpr std::uint8_t checkPattern = 0x45;

  auto ifcResponse = m_accessor.sendIfCond(vhs, checkPattern);
  if (ifcResponse.isInIdleStateOnly()) {
    if (!ifcResponse.isVHS(vhs)) {
      return false;
    }
    if (!ifcResponse.isCheckPattern(checkPattern)) {
      return false;
    }
  } else if (!ifcResponse.isIllegalCommand()) {
    return false;
  }

  const bool sdscSupportOnly = ifcResponse.isIllegalCommand();

  auto ocrResponse = m_accessor.readOcr();
  if (!ocrResponse.isInIdleStateOnly()) {
    return false;
  }
  if (!ocrResponse.is3V3Supported()) {
    return false;
  }

  Response::R1 socResponse;
  for (unsigned int retry = 0; retry != Traits::rx_retry_count; ++retry) {
    socResponse = m_accessor.sdSendOpCond(sdscSupportOnly);
    if (socResponse.isSuccess() || !socResponse.isInIdleStateOnly()) {
      break;
    }
    m_busyWait(Traits::rx_retry_delay);
  }
  if (!socResponse.isSuccess()) {
    return false;
  }

  ocrResponse = m_accessor.readOcr();
  if (!ocrResponse.isSuccess()) {
    return false;
  }

  if (!ocrResponse.isPowerUpComplete()) {
    return false;
  }

  m_isSDSC = ocrResponse.isSDSC();

  // Part1_Physical_Layer_Simplified_Specification_Ver8.00.pdf, 7.2.3:

  if (m_isSDSC) {
    if (!m_accessor.setBlocklen(BlockSize::byte_count).isSuccess()) {
      return false;
    }
  }

  return true;
}

bool sd::Controller::read(open_id_type openId, std::uint64_t byteOffset, std::size_t byteCount, Sink sink) {
  if (openId != m_currentOpenId) {
    return false;
  }

  if (!m_initialized) {
    return false;
  }

  while (byteCount != 0) {
    if (!cacheBlock(byteOffset)) {
      return false;
    }

    std::size_t skipByteCount = byteOffset - BlockSize::alignByteOffset(byteOffset);
    std::size_t sinkByteCount = std::min(m_cachedBlock.size() - skipByteCount, byteCount);
    sink(m_cachedBlock.data() + skipByteCount, sinkByteCount);

    byteOffset += sinkByteCount;
    byteCount -= sinkByteCount;
  }

  return true;;
}

bool sd::Controller::cacheBlock(std::uint64_t byteOffset) {
  auto blockAddress = static_cast<std::uint32_t>(BlockSize::byteOffsetToBlock(byteOffset));

  if (m_cachedBlockAddress == blockAddress) {
    return true;
  }

  std::uint32_t address = static_cast<std::uint32_t>(m_isSDSC ? BlockSize::alignByteOffset(blockAddress) : blockAddress);
  if (!m_accessor.readSingleBlock(address).isSuccess()) {
    return false;
  }

  m_cachedBlockAddress = std::nullopt;
  if (!m_accessor.rx(m_cachedBlock.data(), m_cachedBlock.size())) {
    return false;
  }
  m_cachedBlockAddress = blockAddress;

  return true;
}

#include "Controller.h"

#include "Traits.h"

#include "fs/automount/Manager.h"

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
  , m_isSDSC(false) {
}

void sd::Controller::poll() {
  pollCardInserted();
  pollCardEjected();
}

void sd::Controller::pollCardInserted() {
  if (m_currentCard) {
    return;
  }

  if (!m_detect.isActive()) {
    return;
  }

  m_busyWait(sd_detect_debounce_time);

  if (!m_detect.isActive()) {
    return;
  }

  m_power.activate();

  if (!initialize()) {
    m_currentCard = Card::makeInvalid(this, m_initializationFailureReason);
    ::fs::automount::Manager::instance()->onBlockDeviceAvailable(m_currentCard, m_currentCard->name());
    return;
  }

  CSD csd;
  if (!m_accessor.readCsd(&csd)) {
    m_currentCard = Card::makeInvalid(this, "read-csd");
    ::fs::automount::Manager::instance()->onBlockDeviceAvailable(m_currentCard, m_currentCard->name());
    return;
  }

  m_currentCard = Card::make(this, csd.getSize());
  ::fs::automount::Manager::instance()->onBlockDeviceAvailable(m_currentCard, m_currentCard->name());
}

void sd::Controller::pollCardEjected() {
  if (!m_currentCard) {
    return;
  }

  if (m_detect.isActive()) {
    return;
  }

  m_busyWait(sd_detect_debounce_time);

  if (m_detect.isActive()) {
    return;
  }

  ::fs::automount::Manager::instance()->onBlockDeviceUnavailable(m_currentCard);
  m_currentCard = nullptr;

  m_cachedBlockAddress = std::nullopt;
  m_power.deactivate();
}

bool sd::Controller::initialize() {
  m_initializationFailureReason = "";

  // Set initialization frequency.
  m_accessor.setBaudRate(Traits::spi_init_baud_rate);

  // Part1_Physical_Layer_Simplified_Specification_Ver8.00.pdf, 6.4.1:

  constexpr ::hal::duration_us power_up_delay = 2 * 1000;
  m_busyWait(power_up_delay);

  m_accessor.initialize();

  // Part1_Physical_Layer_Simplified_Specification_Ver8.00.pdf, 7.2.1:

  if (!m_accessor.goIdleState().isInIdleStateOnly()) {
    m_initializationFailureReason = "go-idle-state";
    return false;
  }

  constexpr VHS vhs = VHS::range_2V7_3V6;
  constexpr std::uint8_t checkPattern = 0x45;

  auto ifcResponse = m_accessor.sendIfCond(vhs, checkPattern);
  if (ifcResponse.isInIdleStateOnly()) {
    if (!ifcResponse.isVHS(vhs)) {
      m_initializationFailureReason = "send-if-cond:vhs";
      return false;
    }
    if (!ifcResponse.isCheckPattern(checkPattern)) {
      m_initializationFailureReason = "send-if-cond:check-pattern";
      return false;
    }
  } else if (!ifcResponse.isIllegalCommand()) {
    m_initializationFailureReason = "send-if-cond:not-illegal_command";
    return false;
  }

  const bool sdscSupportOnly = ifcResponse.isIllegalCommand();

  auto ocrResponse = m_accessor.readOcr();
  if (!ocrResponse.isInIdleStateOnly()) {
    m_initializationFailureReason = "read-ocr";
    return false;
  }
  if (!ocrResponse.is3V3Supported()) {
    m_initializationFailureReason = "read-ocr:3v3-not-supported";
    return false;
  }

  Response::R1 socResponse;
  for (unsigned int retry = 0; retry != Traits::command_retry_count; ++retry) {
    socResponse = m_accessor.sdSendOpCond(sdscSupportOnly);
    if (socResponse.isSuccess() || !socResponse.isInIdleStateOnly()) {
      break;
    }
    m_busyWait(Traits::command_retry_delay);
  }
  if (!socResponse.isSuccess()) {
    m_initializationFailureReason = "send-op-cond";
    return false;
  }

  ocrResponse = m_accessor.readOcr();
  if (!ocrResponse.isSuccess()) {
    m_initializationFailureReason = "read-ocr";
    return false;
  }

  if (!ocrResponse.isPowerUpComplete()) {
    m_initializationFailureReason = "power-up-incomplete";
    return false;
  }

  m_isSDSC = ocrResponse.isSDSC();

  // Part1_Physical_Layer_Simplified_Specification_Ver8.00.pdf, 7.2.3:

  if (m_isSDSC) {
    if (!m_accessor.setBlocklen(BlockSize.byteCount()).isSuccess()) {
      m_initializationFailureReason = "set-blocklen";
      return false;
    }
  }

  // Set post-initialization frequency.
  m_accessor.setBaudRate(Traits::spi_data_baud_rate);

  return true;
}

bool sd::Controller::read(Card* card, std::uint64_t byteOffset, std::size_t byteCount, sink_type sink) {
  if (card != m_currentCard.get()) {
    return false;
  }

  while (byteCount != 0) {
    if (!cacheBlock(static_cast<std::uint32_t>(BlockSize.byteOffsetToBlockAddress(byteOffset)))) {
      return false;
    }

    std::size_t skipByteCount = byteOffset - BlockSize.alignByteOffset(byteOffset);
    std::size_t sinkByteCount = std::min(byteCount, m_cachedBlock.size() - skipByteCount);

    sink(m_cachedBlock.data() + skipByteCount, sinkByteCount);

    byteOffset += sinkByteCount;
    byteCount -= sinkByteCount;
  }

  return true;
}

bool sd::Controller::cacheBlock(std::uint32_t blockAddress) {
  if (m_cachedBlockAddress == blockAddress) {
    return true;
  }

  m_cachedBlockAddress = std::nullopt;
  std::uint32_t address = static_cast<std::uint32_t>(m_isSDSC ? BlockSize.blockAddressToByteOffset(blockAddress) : blockAddress);
  if (!m_accessor.readBlock(&m_cachedBlock, address)) {
    return false;
  }
  m_cachedBlockAddress = blockAddress;

  return true;
}

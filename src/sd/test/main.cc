#include "sd/SpiAccessor.h"

#include <array>
#include <cstdlib>

namespace {

static bool cs;

static const std::uint8_t* rxIt;
static const std::uint8_t* rxEnd;

static const std::uint8_t* txIt;
static const std::uint8_t* txEnd;

hal::Spi spi(
    [](std::uint8_t* buf, std::size_t byteCount) {
      if (!cs) {
        return;
      }
      auto it = buf;
      auto end = buf + byteCount;
      for (; it != end && rxIt != rxEnd; ++it, ++rxIt) {
        *it = *rxIt;
      }
    },
    [](const std::uint8_t* buf, std::size_t byteCount) {
      if (!cs) {
        return;
      }
      auto it = buf;
      auto end = buf + byteCount;
      for (; it != end && txIt != txEnd; ++it, ++txIt) {
        if (*it != *txIt) {
          throw std::logic_error("unexpected tx");
        }
      }
    },
    [](unsigned int) {}
);

static constexpr std::array<std::uint8_t, 1> r1ImmediatelyIdle = {
  0x01
};

static constexpr std::array<std::uint8_t, 2> r1DelayedIdle = {
  0xFF,
  0x01
};

template<typename RxData>
void testGoIdleState(sd::SpiAccessor* spiAccessor, const RxData& rxData) {
  std::uint8_t expectedTxData[] = {
    0xFF,
    0x40 | 0,
    0x00,
    0x00,
    0x00,
    0x00,
    0x95,
    0xFF
  };

  txIt = expectedTxData;
  txEnd = txIt + sizeof(expectedTxData);

  rxIt = rxData.data();
  rxEnd = rxIt + rxData.size();

  auto r1 = spiAccessor->goIdleState();

  if (rxIt != rxEnd) {
    throw std::logic_error("rx underrun");
  }

  if (!r1.isInIdleStateOnly()) {
    throw std::logic_error("goIdleState failed");
  }
}

static constexpr std::array<std::uint8_t, 5> r7ImmediatelyIdle = {
  0x01,
  0x00,
  0x00,
  0x01,
  0xAA
};

static constexpr std::array<std::uint8_t, 6> r7DelayedIdle = {
  0xFF,
  0x01,
  0x00,
  0x00,
  0x01,
  0xAA
};

template<typename RxData>
void testSendIfCond(sd::SpiAccessor* spiAccessor, const RxData& rxData) {
  std::uint8_t expectedTxData[] = {
    0xFF,
    0x40 | 8,
    0x00,
    0x00,
    0x01,
    0xAA,
    0x87,
    0xFF
  };

  txIt = expectedTxData;
  txEnd = txIt + sizeof(expectedTxData);

  rxIt = rxData.data();
  rxEnd = rxIt + rxData.size();

  constexpr sd::VHS vhs = sd::VHS::range_2V7_3V6;
  constexpr std::uint8_t checkPattern = 0xAA;

  auto r7 = spiAccessor->sendIfCond(vhs, checkPattern);

  if (rxIt != rxEnd) {
    throw std::logic_error("rx underrun");
  }

  if (!r7.isInIdleStateOnly()) {
    throw std::logic_error("sendIfCond failed");
  }

  if (!r7.isVHS(vhs)) {
    throw std::logic_error("sendIfCond VHS not accepted");
  }

  if (!r7.isCheckPattern(checkPattern)) {
    throw std::logic_error("sendIfCond check pattern mismatch");
  }
}

static constexpr std::array<std::uint8_t, 5> r3ImmediatelyIdle = {
  0x01,
  0x00,
  0x10,
  0x00,
  0x00
};

static constexpr std::array<std::uint8_t, 6> r3DelayedIdle = {
  0xFF,
  0x01,
  0x00,
  0x10,
  0x00,
  0x00
};

template<typename RxData>
void testReadOcr(sd::SpiAccessor* spiAccessor, const RxData& rxData) {
  std::uint8_t expectedTxData[] = {
    0xFF,
    0x40 | 58,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFD,
    0xFF
  };

  txIt = expectedTxData;
  txEnd = txIt + sizeof(expectedTxData);

  rxIt = rxData.data();
  rxEnd = rxIt + rxData.size();

  auto r3 = spiAccessor->readOcr();

  if (rxIt != rxEnd) {
    throw std::logic_error("rx underrun");
  }

  if (!r3.is3V3Supported()) {
    throw std::logic_error("3V3 support failure");
  }
}

void testReadCsd(sd::SpiAccessor* spiAccessor) {
  std::uint8_t expectedTxData[] = {
    0xFF,
    0x40 | 9,
    0x00,
    0x00,
    0x00,
    0x00,
    0xAF,
    0xFF
  };

  txIt = expectedTxData;
  txEnd = txIt + sizeof(expectedTxData);

  std::array<std::uint8_t, 1 + 1 + 16 + 2> rxData;
  rxData[0] = 0x00;
  rxData[1] = 0xFE;
  std::fill(&rxData[2], &rxData[17], 0xDA);
  rxData[17] = 0x06 | 0x01;
  rxData[18] = 0xD0;
  rxData[19] = 0x38;

  rxIt = rxData.data();
  rxEnd = rxIt + rxData.size();

  sd::CSD csd;
  if (!spiAccessor->readCsd(&csd)) {
    throw std::logic_error("rx failed");
  }

  if (rxIt != rxEnd) {
    throw std::logic_error("rx underrun");
  }
}

void testReadBlock(sd::SpiAccessor* spiAccessor) {
  std::uint8_t expectedTxData[] = {
    0xFF,
    0x40 | 17,
    0x04,
    0x03,
    0x02,
    0x01,
    0x91,
    0xFF
  };

  txIt = expectedTxData;
  txEnd = txIt + sizeof(expectedTxData);

  std::array<std::uint8_t, 1 + 1 + 512 + 2> rxData;
  rxData[0] = 0x00;
  rxData[1] = 0xFE;
  std::fill(&rxData[2], &rxData[514], 0xDA);
  rxData[514] = 0x84;
  rxData[515] = 0xA9;

  rxIt = rxData.data();
  rxEnd = rxIt + rxData.size();

  sd::Block block;
  if (!spiAccessor->readBlock(&block, 0x04030201)) {
    throw std::logic_error("rx failed");
  }

  if (rxIt != rxEnd) {
    throw std::logic_error("rx underrun");
  }
}

void testAccessor() {
  sd::SpiAccessor spiAccessor(hal::OutputSignal([](){ cs = true; }, [](){ cs = false; }), spi, [](hal::duration_us){});

  testGoIdleState(&spiAccessor, r1ImmediatelyIdle);
  testGoIdleState(&spiAccessor, r1DelayedIdle);

  testSendIfCond(&spiAccessor, r7ImmediatelyIdle);
  testSendIfCond(&spiAccessor, r7DelayedIdle);

  testReadOcr(&spiAccessor, r3ImmediatelyIdle);
  testReadOcr(&spiAccessor, r3DelayedIdle);

  testReadCsd(&spiAccessor);

  testReadBlock(&spiAccessor);
}

}

int main(int, char**) {
  testAccessor();
  return EXIT_SUCCESS;
}

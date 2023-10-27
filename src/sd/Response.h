#ifndef sd_Response_h
#define sd_Response_h

#include "VHS.h"

#include "hal/Spi.h"
#include "util/PACKED.h"
#include "util/type_traits.h"

#include <array>
#include <cstddef>
#include <cstdint>

namespace sd {

namespace Response {

struct R1 {
  constexpr R1()
    : m_flags(0xFF) {
  }

  bool isValid() const {
    return (m_flags & 0x80) == 0x00;
  }

  bool isSuccess() const {
    return m_flags == 0x00;
  }

  bool isInIdleStateOnly() const {
    return m_flags == 0x01;
  }

  bool isIllegalCommand() const {
    return m_flags & 0x04;
  }

  R1* r1() {
    return this;
  }

private:
  std::uint8_t m_flags;
} PACKED;

template<std::size_t dataByteCount>
struct ResponseWithData : R1 {
  using data_array_type = std::array<std::uint8_t, dataByteCount>;

  data_array_type& data() {
    return m_data;
  }

  const data_array_type& data() const {
    return m_data;
  }

private:
  data_array_type m_data;
} PACKED;

struct R3 : ResponseWithData<4> {
  bool isPowerUpComplete() const {
    return data()[0] & 0x80;
  }

  bool isSDSC() const {
    return (data()[0] & 0x40) == 0;
  }

  bool is3V3Supported() const {
    constexpr std::uint8_t range_3V2_3V3 = 0x10;
    return data()[1] & range_3V2_3V3;
  }
} PACKED;

struct R7 : ResponseWithData<4> {
  bool isVHS(VHS vhs) const {
    return (data()[2] & 0x0F) == util::to_underlying(vhs);
  }

  bool isCheckPattern(std::uint8_t checkPattern) const {
    return data()[3] == checkPattern;
  }
} PACKED;

struct ReadDataToken {
  bool isValid() const {
    return m_token != 0xFF;
  }

  bool isSuccess() const {
    return m_token == 0xFE;
  }

  bool isError() const {
    return (m_token & 0xF1) == 0x01;
  }

private:
  std::uint8_t m_token;
} PACKED;

struct CRC {
  bool isEq(std::uint16_t crc) const {
    return m_data[0] == static_cast<std::uint8_t>(crc >> 8)
      && m_data[1] == static_cast<std::uint8_t>(crc);
  }

private:
  std::uint8_t m_data[2];
} PACKED;

} // namespace Response

} // namespace sd

#endif // ifndef sd_Response_h

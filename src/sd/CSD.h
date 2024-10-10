#ifndef sd_CSD_h
#define sd_CSD_h

#include "util/crc.h"
#include "util/PACKED.h"

#include <array>
#include <cstdint>

namespace sd {

struct CSD {
  unsigned int getVersion() const {
    return extract<unsigned int>(m_data, 127, 126);
  }

  std::uint64_t getSize() const {
    switch (getVersion()) {
      case 0:
        return V1Accessor::getSize(m_data);
      case 1:
        return V2Accessor::getSize(m_data);
    }
    return 0;
  }

  std::uint8_t crc() const {
    return extract<std::uint8_t>(m_data, 7, 1);
  }

  bool verifyCRC() const {
    return crc() == ::util::crc7(reinterpret_cast<const std::uint8_t*>(this), sizeof(*this) - sizeof(std::uint8_t));
  }

private:
  static constexpr auto bit_count_v = 128U;

  using word_type = std::uint8_t;

  static constexpr auto word_bit_count_v = sizeof(word_type) * 8;
  static constexpr auto word_top_bit_v = 1U << (word_bit_count_v - 1);

  using word_type_array = std::array<word_type, bit_count_v / word_bit_count_v>;

  template<typename value_type = std::uint64_t>
  static value_type extract(const word_type_array& data, unsigned int lastBit, unsigned int firstBit) {
    value_type result = 0;

    auto bitIndex = bit_count_v - 1;
    for (auto word : data) {
      for (auto rit = word_bit_count_v; rit != 0; --rit) {
        if (bitIndex <= lastBit && bitIndex >= firstBit) {
          bool bit = (word & word_top_bit_v) == word_top_bit_v;
          result <<= 1;
          result |= bit;
        }

        word <<= 1;
        --bitIndex;
      }
    }

    return result;
  }

  struct V1Accessor {
    static std::uint64_t readBlLen(const word_type_array& data) {
      return extract(data, 83, 80);
    }

    static std::uint64_t cSize(const word_type_array& data) {
      return extract(data, 73, 62);
    }

    static std::uint64_t cSizeMult(const word_type_array& data) {
      return extract(data, 49, 47);
    }

    static std::uint64_t getSize(const word_type_array& data) {
      auto mult = 1ULL << (cSizeMult(data) + 2);
      auto blockNr = (cSize(data) + 1) * mult;
      auto blockLen = 1ULL << readBlLen(data);
      return blockNr * blockLen;
    }
  };

  struct V2Accessor {
    static std::uint64_t cSize(const word_type_array& data) {
      return extract(data, 69, 48);
    }

    static std::uint64_t getSize(const word_type_array& data) {
      return (cSize(data) + 1) * (512 << 10);
    }
  };

  word_type_array m_data;
} PACKED;
static_assert (sizeof(CSD) == 16);

} // namespace sd

#endif // ifndef sd_CSD_h

#ifndef sd_CSD_h
#define sd_CSD_h

#include "BlockSize.h"

#include "util/PACKED.h"

#include <array>
#include <cstdint>

namespace sd {

struct CSD {
  std::uint64_t getSize() const {
    switch (getVersion()) {
      case 0:
        return V1Accessor::getSize(m_data);
      case 1:
        return V2Accessor::getSize(m_data);
    }
    return 0;
  }

private:
  static constexpr auto bit_count_v = 128;
  static constexpr auto size_v = bit_count_v / bit_count_v;

  using data_array_type = std::array<std::uint8_t, size_v>;

  unsigned int getVersion() const {
    return m_data[0] & 0x03;
  }

  struct V1Accessor {
    static unsigned int readBlLen(const data_array_type& data) {
      return
        ((static_cast<unsigned int>(data[10]) & 0x0F) << 0);
    }

    static std::uint32_t cSize(const data_array_type& data) {
      return
        ((static_cast<std::uint32_t>(data[9]) & 0x03) << 10) |
        ((static_cast<std::uint32_t>(data[8]) & 0xFF) << 2) |
        ((static_cast<std::uint32_t>(data[7]) & 0xC0) >> 6);
    }

    static unsigned int cSizeMult(const data_array_type& data) {
      return
        ((static_cast<unsigned int>(data[6]) & 0x03) << 1);
        ((static_cast<unsigned int>(data[5]) & 0x80) >> 7);
    }

    static std::uint64_t getSize(const data_array_type& data) {
      auto mult = static_cast<std::uint64_t>(1) << (cSizeMult(data) + 2);
      auto blockNr = static_cast<std::uint64_t>(cSize(data) + 1) * mult;
      auto blockLen = static_cast<std::uint64_t>(1) << readBlLen(data);
      return blockNr * blockLen;
    }
  };

  struct V2Accessor {
    static std::uint32_t cSize(const data_array_type& data) {
      return
        ((static_cast<std::uint32_t>(data[8]) & 0x3F) << 16) |
        ((static_cast<std::uint32_t>(data[7]) & 0xFF) << 8) |
        ((static_cast<std::uint32_t>(data[6]) & 0xFF) << 0);
    }

    static std::uint64_t getSize(const data_array_type& data) {
      return static_cast<std::uint64_t>(cSize(data) + 1) * BlockSize::byte_count;
    }
  };

  data_array_type m_data;
} PACKED;

} // namespace sd

#endif // ifndef sd_CSD_h

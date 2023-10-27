#ifndef sd_BlockSize_h
#define sd_BlockSize_h

#include <cstddef>
#include <cstdint>

namespace sd {

struct BlockSize {
  static constexpr unsigned int size_log2 = 9;
  static constexpr unsigned int byte_count = 1 << size_log2;
  static constexpr std::uint64_t byte_align_mask = ~(byte_count - 1);

  static std::uint64_t byteOffsetToBlock(std::uint64_t byteOffset) {
    return byteOffset >> size_log2;
  }

  static std::uint64_t blockToByteOffset(std::uint64_t block) {
    return block << size_log2;
  }

  static std::uint64_t alignByteOffset(std::uint64_t byteOffset) {
    return byteOffset & byte_align_mask;
  }
};

} // namespace sd

#endif // ifndef sd_BlockSize_h

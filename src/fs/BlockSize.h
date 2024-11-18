#ifndef fs_BlockSize_h
#define fs_BlockSize_h

#include <cstdint>

namespace fs {

class BlockSize {
public:
  using value_type = unsigned int;

  using block_count_type = std::uint64_t;
  using byte_count_type = std::uint64_t;

  static constexpr BlockSize fromSizeLog2(value_type sizeLog2) {
    return BlockSize(sizeLog2);
  }

  constexpr value_type sizeLog2() const {
    return m_sizeLog2;
  }

  constexpr byte_count_type byteCount() const {
    return static_cast<byte_count_type>(1) << m_sizeLog2;
  }

  constexpr byte_count_type alignByteOffset(byte_count_type byteOffset) const {
    return byteOffset & ~(byteCount() - 1);
  }

  constexpr byte_count_type blockAddressToByteOffset(block_count_type blockAddress) const {
    return static_cast<byte_count_type>(blockAddress) << m_sizeLog2;
  }

  constexpr byte_count_type blockCountToByteCount(block_count_type blockCount) const {
    return blockAddressToByteOffset(blockCount);
  }

  constexpr block_count_type byteOffsetToBlockAddress(byte_count_type byteOffset) const {
    return static_cast<block_count_type>(byteOffset >> m_sizeLog2);
  }

  constexpr block_count_type byteCountToBlockCount(byte_count_type byteCount) const {
    return byteCount == 0 ? static_cast<block_count_type>(0) : static_cast<block_count_type>((byteCount - 1) >> m_sizeLog2) + 1;
  }

private:
  constexpr BlockSize(value_type sizeLog2)
    : m_sizeLog2(sizeLog2) {
  }

  const value_type m_sizeLog2;
};

} // namespace fs

#endif // ifndef fs_BlockSize_h

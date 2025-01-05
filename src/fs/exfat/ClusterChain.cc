#include "ClusterChain.h"

fs::exfat::ClusterChain::ClusterChain() {
}

fs::exfat::ClusterChain::ClusterChain(const ::fs::FileSlice::impl_ptr_type& fat, logical_cluster_number begin)
  : m_fat(fat)
  , m_begin({0, begin}) {
}

fs::exfat::ClusterChain::entry_enumeration_type fs::exfat::ClusterChain::begin() {
  return m_begin;
}

fs::exfat::ClusterChain::entry_enumeration_type fs::exfat::ClusterChain::next(const entry_type& entry) {
  constexpr auto fat_eof_value = 0xFFFFFFFF;
  constexpr auto fat_entry_block_size = BlockSize::fromSizeLog2(2);

  logical_cluster_number next;
  if (!m_fat->read(fat_entry_block_size.blockAddressToByteOffset(entry.lcn()), &next)) {
    return std::nullopt;
  }

  if (next == fat_eof_value) {
    return std::nullopt;
  }

  return ClusterChainEntry(entry.index() + 1, next);
}

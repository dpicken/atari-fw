#include "ClusterChain.h"

fs::exfat::ClusterChain::ClusterChain() {
}

fs::exfat::ClusterChain::ClusterChain(const ::fs::FileSlice::impl_ptr_type& fat, logical_cluster_number lcn)
  : m_fat(fat)
  , m_begin(entry_type(0, lcn)) {
}

fs::exfat::ClusterChain::entry_enumeration_type fs::exfat::ClusterChain::begin() {
  return m_begin;
}

fs::exfat::ClusterChain::entry_enumeration_type fs::exfat::ClusterChain::next(const entry_type& entry) {
  constexpr auto fat_value_eof = 0xFFFFFFFFU;
  constexpr auto fat_value_bad_cluster = 0xFFFFFFF7U;
  constexpr auto fat_entry_block_size = BlockSize::fromSizeLog2(2);

  logical_cluster_number next;
  if (!m_fat->read(fat_entry_block_size.blockAddressToByteOffset(entry.lcn()), &next)) {
    return std::nullopt;
  }

  if (next == fat_value_eof || next == fat_value_bad_cluster) {
    return std::nullopt;
  }

  return entry_type(entry.index() + 1, next);
}

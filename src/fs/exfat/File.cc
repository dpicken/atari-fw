#include "File.h"

#include <algorithm>

fs::exfat::File::File(size_type size, cluster_enumerator_type&& clusterEnumerator, const ClusterHeap::impl_ptr_type& clusterHeap)
  : m_size(size)
  , m_clusterHeap(clusterHeap)
  , m_clusterEnumerator(std::move(clusterEnumerator)) {
}

fs::exfat::File::impl_ptr_type fs::exfat::File::make(size_type size, ClusterChain&& clusterChain, const ClusterHeap::impl_ptr_type& clusterHeap) {
  return std::make_shared<File>(size, ClusterChainEnumerator(std::move(clusterChain)), clusterHeap);
}

fs::exfat::File::impl_ptr_type fs::exfat::File::make(const ClusterAllocation& clusterAllocation, const ::fs::FileSlice::impl_ptr_type& fat, const ClusterHeap::impl_ptr_type& clusterHeap) {
  if (clusterAllocation.noFatChain()) {
    return std::make_shared<File>(clusterAllocation.byteCount(), ClusterRunEnumerator(ClusterRun(clusterAllocation.lcn(), clusterHeap->blockSize().byteCountToBlockCount(clusterAllocation.byteCount()))), clusterHeap);
  } else {
    return std::make_shared<File>(clusterAllocation.byteCount(), ClusterChainEnumerator(ClusterChain(fat, clusterAllocation.lcn())), clusterHeap);
  }
}

fs::File::block_size_type fs::exfat::File::blockSize() const {
  return m_clusterHeap->blockSize();
}

fs::File::size_type fs::exfat::File::size() {
  return m_size;
}

fs::FileSlice::impl_ptr_type fs::exfat::File::tryGetFat() const {
  const auto* clusterChainEnumerator = std::get_if<ClusterChainEnumerator>(&m_clusterEnumerator);
  if (clusterChainEnumerator == nullptr) {
    return nullptr;
  }
  return clusterChainEnumerator->container().fat();
}

bool fs::exfat::File::readImpl(size_type offset, buffer_size_type byteCount, const sink_type& sink) {
  return std::visit([this, &offset, &byteCount, &sink](auto&& clusterEnumerator) {
    auto clusterSize = m_clusterHeap->blockSize();
    while (byteCount != 0) {
      auto vcn = virtual_cluster_number{clusterSize.byteOffsetToBlockAddress(offset)};
      clusterEnumerator.reposition(vcn);
      if (!clusterEnumerator.isValid()) {
        return false;
      }

      auto clusterSliceOffset = offset - clusterSize.alignByteOffset(offset);
      auto clusterSliceByteCount = std::min(static_cast<BlockSize::byte_count_type>(byteCount), clusterSize.byteCount() - clusterSliceOffset);

      auto clusterHeapOffset = clusterSize.blockAddressToByteOffset(clusterEnumerator.entry().lcn()) + clusterSliceOffset;
      if (!m_clusterHeap->read(clusterHeapOffset, clusterSliceByteCount, sink)) {
        return false;
      }

      offset += clusterSliceByteCount;
      byteCount -= clusterSliceByteCount;
    }
    return true;
  }, m_clusterEnumerator);
}

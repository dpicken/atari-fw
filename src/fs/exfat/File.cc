#include "File.h"

#include <algorithm>

fs::exfat::File::File(size_type size, ClusterChain&& clusterChain, const ClusterHeap::impl_ptr_type& clusterHeap)
  : m_size(size)
  , m_clusterHeap(clusterHeap)
  , m_clusterChainEnumerator(std::move(clusterChain)) {
}

fs::exfat::File::impl_ptr_type fs::exfat::File::make(size_type size, ClusterChain&& clusterChain, const ClusterHeap::impl_ptr_type& clusterHeap) {
  return std::make_shared<File>(size, std::move(clusterChain), clusterHeap);
}

fs::File::block_size_type fs::exfat::File::blockSize() const {
  return m_clusterHeap->blockSize();
}

fs::File::size_type fs::exfat::File::size() {
  return m_size;
}

bool fs::exfat::File::readImpl(size_type offset, buffer_size_type byteCount, const sink_type& sink) {
  auto clusterSize = m_clusterHeap->blockSize();

  while (byteCount != 0) {
    auto vcn = static_cast<virtual_cluster_number>(clusterSize.byteOffsetToBlockAddress(offset));
    m_clusterChainEnumerator.reposition(vcn);
    if (!m_clusterChainEnumerator.isValid()) {
      return false;
    }

    auto clusterSliceOffset = offset - clusterSize.alignByteOffset(offset);
    auto clusterSliceByteCount = std::min(static_cast<BlockSize::byte_count_type>(byteCount), clusterSize.byteCount() - clusterSliceOffset);

    auto clusterHeapOffset = clusterSize.blockAddressToByteOffset(m_clusterChainEnumerator.entry().lcn()) + clusterSliceOffset;
    if (!m_clusterHeap->read(clusterHeapOffset, clusterSliceByteCount, sink)) {
      return false;
    }

    offset += clusterSliceByteCount;
    byteCount -= clusterSliceByteCount;
  }

  return true;
}

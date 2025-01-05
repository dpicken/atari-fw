#include "ClusterHeap.h"

fs::exfat::ClusterHeap::ClusterHeap(const ::fs::File::ptr_type& clusters, BlockSize clusterSize, size_type clusterCount)
  : m_clusters(clusters)
  , m_clusterSize(clusterSize)
  , m_size(m_clusterSize.blockCountToByteCount(clusterCount)) {
}

fs::exfat::ClusterHeap::impl_ptr_type fs::exfat::ClusterHeap::make(const ::fs::File::ptr_type& clusters, BlockSize clusterSize, size_type clusterCount) {
  return std::make_shared<ClusterHeap>(clusters, clusterSize, clusterCount);
}

fs::File::block_size_type fs::exfat::ClusterHeap::blockSize() const {
  return m_clusterSize;
}

fs::File::size_type fs::exfat::ClusterHeap::size() {
  return m_size;
}

bool fs::exfat::ClusterHeap::readImpl(size_type offset, buffer_size_type byteCount, const sink_type& sink) {
  auto baseOffset = m_clusterSize.blockCountToByteCount(2);
  if (offset < baseOffset) {
    return false;
  }
  return m_clusters->read(offset - baseOffset, byteCount, sink);
}

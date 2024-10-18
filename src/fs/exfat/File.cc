#include "File.h"

fs::exfat::File::File(ClusterChain&& clusterChain, size_type size)
  : m_clusterChainEnumerator(std::move(clusterChain))
  , m_size(size) {
}

fs::exfat::File::impl_ptr_type fs::exfat::File::make(ClusterChain&& clusterChain, size_type size) {
  return std::make_shared<File>(std::move(clusterChain), size);
}

fs::File::block_size_type fs::exfat::File::blockSize() const {
  return m_clusterChainEnumerator.container()->fat()->blockSize();
}

fs::File::size_type fs::exfat::File::size() {
  return m_size;
}

bool fs::exfat::File::readImpl(size_type offset, buffer_size_type byteCount, const sink_type& sink) {
  (void) offset;
  (void) byteCount;
  (void) sink;
  return false;
}

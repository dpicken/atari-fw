#ifndef fs_exfat_ClusterHeap_h
#define fs_exfat_ClusterHeap_h

#include "fs/File.h"

namespace fs { namespace exfat {

class ClusterHeap : public ::fs::File {
public:
  using impl_ptr_type = ptr<File>;

  ClusterHeap(const ::fs::File::ptr_type& clusters, BlockSize clusterSize, size_type clusterCount);

  static impl_ptr_type make(const ::fs::File::ptr_type& clusters, BlockSize clusterSize, size_type clusterCount);

  block_size_type blockSize() const override;

  size_type size() override;

protected:
  bool readImpl(size_type offset, buffer_size_type byteCount, const sink_type& sink) override;

private:
  const ::fs::File::ptr_type m_clusters;

  const BlockSize m_clusterSize;

  const size_type m_size;
};

} } // namespace fs::exfat

#endif // ifndef fs_exfat_ClusterHeap_h

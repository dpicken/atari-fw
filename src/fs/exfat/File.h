#ifndef fs_exfat_File_h
#define fs_exfat_File_h

#include "ClusterChain.h"
#include "ClusterChainEnumerator.h"
#include "ClusterHeap.h"

#include "fs/File.h"

namespace fs { namespace exfat {

class File : public ::fs::File {
public:
  using impl_ptr_type = ptr<File>;

  File(size_type size, ClusterChain&& clusterChain, const ClusterHeap::impl_ptr_type& clusterHeap);

  static impl_ptr_type make(size_type size, ClusterChain&& clusterChain, const ClusterHeap::impl_ptr_type& clusterHeap);

  block_size_type blockSize() const override;

  size_type size() override;

  const ClusterChain& clusterChain() const {
    return m_clusterChainEnumerator.container();
  }

  const ClusterHeap::impl_ptr_type& clusterHeap() const {
    return m_clusterHeap;
  }

protected:
  bool readImpl(size_type offset, buffer_size_type byteCount, const sink_type& sink) override;

private:
  const size_type m_size;
  const ClusterHeap::impl_ptr_type m_clusterHeap;

  ClusterChainEnumerator m_clusterChainEnumerator;
};

} } // namespace fs::exfat

#endif // ifndef fs_exfat_File_h

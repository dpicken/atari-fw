#ifndef fs_exfat_File_h
#define fs_exfat_File_h

#include "ClusterAllocation.h"
#include "ClusterChain.h"
#include "ClusterChainEnumerator.h"
#include "ClusterHeap.h"
#include "ClusterRunEnumerator.h"

#include "fs/File.h"

#include <variant>

namespace fs { namespace exfat {

class File : public ::fs::File {
public:
  using impl_ptr_type = ptr<File>;
  using cluster_enumerator_type = std::variant<ClusterChainEnumerator, ClusterRunEnumerator>;

  File(size_type size, cluster_enumerator_type&& cluster_enumerator_type, const ClusterHeap::impl_ptr_type& clusterHeap);

  static impl_ptr_type make(size_type size, ClusterChain&& clusterChain, const ClusterHeap::impl_ptr_type& clusterHeap);
  static impl_ptr_type make(const ClusterAllocation& clusterAllocation, const ::fs::FileSlice::impl_ptr_type& fat, const ClusterHeap::impl_ptr_type& clusterHeap);

  block_size_type blockSize() const override;

  size_type size() override;

  const ClusterHeap::impl_ptr_type& clusterHeap() const {
    return m_clusterHeap;
  }

  ::fs::FileSlice::impl_ptr_type tryGetFat() const;

protected:
  bool readImpl(size_type offset, buffer_size_type byteCount, const sink_type& sink) override;

private:
  const size_type m_size;
  const ClusterHeap::impl_ptr_type m_clusterHeap;

  cluster_enumerator_type m_clusterEnumerator;
};

} } // namespace fs::exfat

#endif // ifndef fs_exfat_File_h

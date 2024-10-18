#ifndef fs_exfat_File_h
#define fs_exfat_File_h

#include "ClusterChain.h"
#include "ClusterChainEnumerator.h"

#include "fs/File.h"

namespace fs { namespace exfat {

class File : ::fs::File {
public:
  using impl_ptr_type = ptr<File>;

  File(ClusterChain&& clusterChain, size_type size);

  static impl_ptr_type make(ClusterChain&& clusterChain, size_type size);

  block_size_type blockSize() const override;

  size_type size() override;

protected:
  bool readImpl(size_type offset, buffer_size_type byteCount, const sink_type& sink) override;

private:
  ClusterChainEnumerator m_clusterChainEnumerator;

  const size_type m_size;
};

} } // namespace fs::exfat

#endif // ifndef fs_exfat_File_h

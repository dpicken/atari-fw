#ifndef fs_exfat_ClusterChainEntry_h
#define fs_exfat_ClusterChainEntry_h

#include "AddressTypes.h"

namespace fs { namespace exfat {

struct ClusterChainEntry {
  using index_type = virtual_cluster_number;

  ClusterChainEntry(index_type index, logical_cluster_number lcn);

  index_type index() const {
    return m_index;
  }

  logical_cluster_number lcn() const {
    return m_lcn;
  }

private:
  index_type m_index;
  logical_cluster_number m_lcn;
};

} } // namespace fs::exfat

#endif // ifndef fs_exfat_ClusterChainEntry_h

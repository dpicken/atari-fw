#ifndef fs_exfat_ClusterRunEntry_h
#define fs_exfat_ClusterRunEntry_h

#include "AddressTypes.h"

namespace fs { namespace exfat {

class ClusterRunEntry {
public:
  using index_type = virtual_cluster_number;

  ClusterRunEntry(virtual_cluster_number vcn, logical_cluster_number lcn)
    : m_vcn(vcn)
    , m_lcn(lcn) {
  }

  virtual_cluster_number vcn() const {
    return m_vcn;
  }

  logical_cluster_number lcn() const {
    return m_lcn;
  }

  index_type index() const {
    return vcn();
  }

private:
  virtual_cluster_number m_vcn;
  logical_cluster_number m_lcn;
};

} } // namespace fs::exfat

#endif // ifndef fs_exfat_ClusterRunEntry_h

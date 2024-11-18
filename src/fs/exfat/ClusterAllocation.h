#ifndef fs_exfat_ClusterAllocation_h
#define fs_exfat_ClusterAllocation_h

#include "AddressTypes.h"

namespace fs { namespace exfat {

struct ClusterAllocation {
  ClusterAllocation(logical_cluster_number lcn, std::uint64_t byteCount, bool noFatChain)
    : m_lcn(lcn)
    , m_byteCount(byteCount)
    , m_noFatChain(noFatChain) {
  }

  logical_cluster_number lcn() const {
    return m_lcn;
  }

  std::uint64_t byteCount() const {
    return m_byteCount;
  }

  bool noFatChain() const {
    return m_noFatChain;
  }

private:
  logical_cluster_number m_lcn;
  std::uint64_t m_byteCount;
  bool m_noFatChain;
};

} } // namespace fs::exfat

#endif // ifndef fs_exfat_ClusterAllocation_h

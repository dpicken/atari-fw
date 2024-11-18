#ifndef fs_exfat_ClusterRun_h
#define fs_exfat_ClusterRun_h

#include "ClusterRunEntry.h"

#include "fs/BlockSize.h"

#include <optional>

namespace fs { namespace exfat {

class ClusterRun {
public:
  using entry_type = ClusterRunEntry;
  using entry_enumeration_type = std::optional<entry_type>;

  using block_count_type = BlockSize::block_count_type;

  ClusterRun();
  ClusterRun(logical_cluster_number lcn, block_count_type clusterCount);

  entry_enumeration_type begin() const {
    if (m_clusterCount == 0) {
      return std::nullopt;
    }
    return entry_type(0, m_lcn);
  }

  entry_enumeration_type next(const entry_type& entry) {
    if (entry.vcn() + 1 == m_clusterCount) {
      return std::nullopt;
    }
    return entry_type(entry.vcn() + 1, entry.lcn() + 1);
  }

private:
  const logical_cluster_number m_lcn;
  const block_count_type m_clusterCount;
};

} } // namespace fs::exfat

#endif // ifndef fs_exfat_ClusterRun_h

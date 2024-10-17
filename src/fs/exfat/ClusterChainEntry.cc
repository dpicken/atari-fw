#include "ClusterChainEntry.h"

::fs::exfat::ClusterChainEntry::ClusterChainEntry(index_type index, logical_cluster_number lcn)
  : m_index(index)
  , m_lcn(lcn) {
}

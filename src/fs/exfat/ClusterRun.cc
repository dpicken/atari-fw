#include "ClusterRun.h"

fs::exfat::ClusterRun::ClusterRun()
  : ClusterRun(0, 0) {
}

fs::exfat::ClusterRun::ClusterRun(logical_cluster_number lcn, block_count_type clusterCount)
  : m_lcn(lcn)
  , m_clusterCount(clusterCount) {
}

#include "LogicalDirectoryEntry.h"

fs::exfat::LogicalDirectoryEntry::LogicalDirectoryEntry(entry_type&& entry, FileAttributes fileAttributes, const ClusterAllocation& clusterAllocation, physical_index_type endPhysicalIndex)
  : m_entry(std::move(entry))
  , m_fileAttributes(fileAttributes)
  , m_clusterAllocation(clusterAllocation)
  , m_endPhysicalIndex(endPhysicalIndex) {
}

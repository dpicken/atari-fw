#ifndef fs_exfat_LogicalDirectoryEntry_h
#define fs_exfat_LogicalDirectoryEntry_h

#include "ClusterAllocation.h"
#include "PhysicalDirectoryEntry.h"

#include "fs/DirectoryEntry.h"

namespace fs { namespace exfat {

class LogicalDirectoryEntry {
public:
  using entry_type = ::fs::DirectoryEntry;

  using logical_index_type = entry_type::index_type;
  using physical_index_type = PhysicalDirectoryEntry::index_type;

  using index_type = logical_index_type;

  LogicalDirectoryEntry(entry_type&& entry, FileAttributes fileAttributes, const ClusterAllocation& clusterAllocation, physical_index_type endPhysicalIndex);

  const entry_type& standard() const {
    return m_entry;
  }

  FileAttributes fileAttributes() const {
    return m_fileAttributes;
  }

  const ClusterAllocation& clusterAllocation() const {
    return m_clusterAllocation;
  }

  logical_index_type index() const {
    return standard().index();
  }

  physical_index_type endPhysicalIndex() const {
    return m_endPhysicalIndex;
  }

private:
  entry_type m_entry;

  FileAttributes m_fileAttributes;

  ClusterAllocation m_clusterAllocation;

  physical_index_type m_endPhysicalIndex;
};

} } // namespace fs::exfat

#endif // ifndef fs_exfat_LogicalDirectoryEntry_h

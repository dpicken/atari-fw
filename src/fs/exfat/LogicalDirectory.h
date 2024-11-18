#ifndef fs_exfat_LogicalDirectory_h
#define fs_exfat_LogicalDirectory_h

#include "LogicalDirectoryEntry.h"
#include "PhysicalDirectoryEnumerator.h"

#include <optional>

namespace fs { namespace exfat {

class LogicalDirectory {
public:
  using entry_type = LogicalDirectoryEntry;
  using entry_enumeration_type = std::optional<entry_type>;

  LogicalDirectory(PhysicalDirectory&& directory);

  entry_enumeration_type begin();
  entry_enumeration_type next(const entry_type& entry);

  PhysicalDirectory* physicalDirectory();

private:
  entry_enumeration_type findNonExcluded(entry_type::logical_index_type logicalIndex);
  entry_enumeration_type find(entry_type::logical_index_type logicalIndex);

  PhysicalDirectoryEnumerator m_enumerator;
};

} } // namespace fs::exfat

#endif // ifndef fs_exfat_LogicalDirectory_h

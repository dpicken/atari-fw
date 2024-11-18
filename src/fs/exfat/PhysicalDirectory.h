#ifndef fs_exfat_PhysicalDirectory_h
#define fs_exfat_PhysicalDirectory_h

#include "PhysicalDirectoryEntry.h"
#include "File.h"

#include <optional>

namespace fs { namespace exfat {

class PhysicalDirectory {
public:
  using entry_type = PhysicalDirectoryEntry;
  using entry_enumeration_type = std::optional<entry_type>;

  explicit PhysicalDirectory(File::impl_ptr_type&& file);

  entry_enumeration_type begin();
  entry_enumeration_type next(const entry_type& entry);

  const File::impl_ptr_type& file() const { return m_file; }

private:
  entry_enumeration_type readEntry(entry_type::index_type index);

  File::impl_ptr_type m_file;
};

} } // namespace fs::exfat

#endif // ifndef fs_exfat_PhysicalDirectory_h

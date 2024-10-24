#ifndef fs_exfat_InternalDirectory_h
#define fs_exfat_InternalDirectory_h

#include "InternalDirectoryEntry.h"
#include "File.h"

#include <optional>

namespace fs { namespace exfat {

class InternalDirectory {
public:
  using entry_type = InternalDirectoryEntry;
  using entry_enumeration_type = std::optional<entry_type>;

  explicit InternalDirectory(File::impl_ptr_type&& file);

  entry_enumeration_type begin();
  entry_enumeration_type next(const entry_type& entry);

  const File::impl_ptr_type& file() const { return m_file; }

private:
  entry_enumeration_type readEntry(entry_type::index_type index);

  File::impl_ptr_type m_file;
};

} } // namespace fs::exfat

#endif // ifndef fs_exfat_InternalDirectory_h

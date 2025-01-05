#ifndef fs_exfat_PhysicalDirectoryEntry_h
#define fs_exfat_PhysicalDirectoryEntry_h

#include "DirectoryEntryTypes.h"

#include <cstdint>

namespace fs { namespace exfat {

class PhysicalDirectoryEntry {
public:
  using index_type = std::uint32_t;
  using value_type = UntypedDirectoryEntry;

  constexpr PhysicalDirectoryEntry(index_type index)
    : m_index(index) {
  }

  index_type index() const {
    return m_index;
  }

  value_type* generic() {
    return &m_value;
  }

  const value_type* generic() const {
    return &m_value;
  }

private:
  index_type m_index;

  value_type m_value;
};

} } // namespace fs::exfat

#endif // ifndef fs_exfat_PhysicalDirectoryEntry_h

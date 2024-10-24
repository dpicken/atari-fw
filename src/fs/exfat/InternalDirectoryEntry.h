#ifndef fs_exfat_InternalDirectoryEntry_h
#define fs_exfat_InternalDirectoryEntry_h

#include "GenericDirectoryEntry.h"

#include <cstdint>

namespace fs { namespace exfat {

class InternalDirectoryEntry {
public:
  using index_type = std::uint32_t;

  constexpr InternalDirectoryEntry(index_type index)
    : m_index(index) {
  }

  index_type index() const {
    return m_index;
  }

  GenericDirectoryEntry* generic() {
    return &m_value;
  }

  const GenericDirectoryEntry* generic() const {
    return &m_value;
  }

private:
  index_type m_index;

  GenericDirectoryEntry m_value;
};

} } // namespace fs::exfat

#endif // ifndef fs_exfat_InternalDirectoryEntry_h

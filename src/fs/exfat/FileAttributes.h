#ifndef fs_exfat_FileAttributes_h
#define fs_exfat_FileAttributes_h

#include "util/PACKED.h"
#include "util/type_traits.h"

#include <cstdint>

namespace fs { namespace exfat {

struct FileAttributes {
  using value_type = std::uint16_t;

  enum class Flag : value_type {
    ReadOnly  = 0x0001,
    Hidden    = 0x0002,
    System    = 0x0004,
    Reserved1 = 0x0008,
    Directory = 0x0010,
    Archive   = 0x0020,
    Reserved2 = 0x0040
  };

  constexpr FileAttributes()
    : m_flags(0) {
  }

  bool isSet(Flag flag) const {
    return m_flags & util::to_underlying(flag);
  }

private:
  value_type m_flags;
} PACKED;

} } // namespace fs::exfat

#endif // ifndef fs_exfat_FileAttributes_h

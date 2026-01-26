#ifndef fs_exfat_PhysicalDirectoryEnumerator_h
#define fs_exfat_PhysicalDirectoryEnumerator_h

#include "PhysicalDirectory.h"

#include "fs/Enumerator.h"

namespace fs { namespace exfat {

using PhysicalDirectoryEnumerator = Enumerator<PhysicalDirectory>;

template<typename DirectoryEntryType>
inline const DirectoryEntryType* findEntrySet(PhysicalDirectoryEnumerator& enumerator) {
  const DirectoryEntryType* entry = nullptr;
  while (enumerator.isValid() && (entry = enumerator.entry().generic()->to<DirectoryEntryType>()) == nullptr) {
    enumerator.next();
  }
  return entry;
}

template<typename DirectoryEntryType>
inline const DirectoryEntryType* nextEntryInSet(PhysicalDirectoryEnumerator& enumerator) {
  enumerator.next();
  if (!enumerator.isValid()) {
    return nullptr;
  }
  return enumerator.entry().generic()->to<DirectoryEntryType>();
}

} } // namespace fs::exfat

#endif // ifndef fs_exfat_PhysicalDirectoryEnumerator_h

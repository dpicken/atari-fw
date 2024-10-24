#include "InternalDirectory.h"

fs::exfat::InternalDirectory::InternalDirectory(File::impl_ptr_type&& file)
  : m_file(std::move(file)) {
}

fs::exfat::InternalDirectory::entry_enumeration_type fs::exfat::InternalDirectory::begin() {
  return readEntry(0);
}

fs::exfat::InternalDirectory::entry_enumeration_type fs::exfat::InternalDirectory::next(const entry_type& entry) {
  return readEntry(entry.index() + 1);
}

fs::exfat::InternalDirectory::entry_enumeration_type fs::exfat::InternalDirectory::readEntry(entry_type::index_type index) {
  entry_type entry(index);
  if (!m_file->read(entry.index() * sizeof(GenericDirectoryEntry), entry.generic())) {
    return std::nullopt;
  }
  if (entry.generic()->entryType().isEndOfDirectory()) {
    return std::nullopt;
  }
  return entry;
}

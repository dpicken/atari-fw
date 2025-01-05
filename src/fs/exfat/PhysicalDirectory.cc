#include "PhysicalDirectory.h"

fs::exfat::PhysicalDirectory::PhysicalDirectory(File::impl_ptr_type&& file)
  : m_file(std::move(file)) {
}

fs::exfat::PhysicalDirectory::entry_enumeration_type fs::exfat::PhysicalDirectory::begin() {
  return readEntry(0);
}

fs::exfat::PhysicalDirectory::entry_enumeration_type fs::exfat::PhysicalDirectory::next(const entry_type& entry) {
  return readEntry(entry.index() + 1);
}

fs::exfat::PhysicalDirectory::entry_enumeration_type fs::exfat::PhysicalDirectory::readEntry(entry_type::index_type index) {
  entry_type entry(index);
  if (!m_file->read(entry.index() * sizeof(entry_type::value_type), entry.generic())) {
    return std::nullopt;
  }
  if (entry.generic()->entryType().isEndOfDirectory()) {
    return std::nullopt;
  }
  return entry;
}

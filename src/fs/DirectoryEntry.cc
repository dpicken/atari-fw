#include "DirectoryEntry.h"

fs::DirectoryEntry::DirectoryEntry(name_type name, Type type, index_type index)
  : m_name(std::move(name))
  , m_type(type)
  , m_index(index) {
}

bool fs::operator==(const DirectoryEntry& lhs, const DirectoryEntry& rhs) {
  return lhs.name() == rhs.name()
      && lhs.type() == rhs.type()
      && lhs.index() == rhs.index();
}

bool fs::operator!=(const DirectoryEntry& lhs, const DirectoryEntry& rhs) {
  return !(lhs == rhs);
}

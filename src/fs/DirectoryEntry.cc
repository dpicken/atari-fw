#include "DirectoryEntry.h"

fs::DirectoryEntry::DirectoryEntry(name_type name, Type type, index_type index)
  : m_name(std::move(name))
  , m_type(type)
  , m_index(index) {
}
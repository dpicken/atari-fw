#include "Directory.h"

fs::ram::Directory::impl_ptr_type fs::ram::Directory::make() {
  return std::make_shared<Directory>();
}

fs::Directory::entry_type::Type fs::ram::Directory::toEntryType(const impl_entry_type& entry) {
  return std::get_if<ptr_type>(&entry) ? entry_type::Type::Directory : entry_type::Type::File;
}

fs::Directory::entry_enumeration_type fs::ram::Directory::begin() {
  auto it = m_map.begin();
  return it != m_map.end() ? entry_enumeration_type(std::in_place, it->first, toEntryType(it->second), 0) : std::nullopt;
}

fs::Directory::entry_enumeration_type fs::ram::Directory::next(const entry_type& entry) {
  auto it = m_map.upper_bound(entry.name());
  return it != m_map.end() ? entry_enumeration_type(std::in_place, it->first, toEntryType(it->second), entry.index() + 1) : std::nullopt;
}

template<typename PtrType>
PtrType fs::ram::Directory::open(const name_type& name) {
  auto it = m_map.find(name);
  if (it == m_map.end()) {
    return nullptr;
  }
  auto value = std::get_if<PtrType>(&it->second);
  if (value == nullptr) {
    return nullptr;
  }
  return *value;
}

fs::Directory::ptr_type fs::ram::Directory::openDirectory(const name_type& name) {
  return open<ptr_type>(name);
}

fs::Directory::file_type::ptr_type fs::ram::Directory::openFile(const name_type& name) {
  return open<file_type::ptr_type>(name);
}

void fs::ram::Directory::put(name_type name, ptr_type directory) {
  m_map.insert_or_assign(std::move(name), std::move(directory));
}

void fs::ram::Directory::put(name_type name, file_type::ptr_type file) {
  m_map.insert_or_assign(std::move(name), std::move(file));
}

void fs::ram::Directory::remove(const name_type& name) {
  m_map.erase(name);
}

#include "Directory.h"

#include "Configuration.h"

#include "SortingDirectory.h"

fs::exfat::Directory::Directory(LogicalDirectory&& directory, const ::fs::FileSlice::impl_ptr_type& fat)
  : m_fat(fat)
  , m_enumerator(std::move(directory)) {
}

fs::exfat::Directory::ptr_type fs::exfat::Directory::make(LogicalDirectory&& directory, const ::fs::FileSlice::impl_ptr_type& fat) {
  return SortingDirectory::make(std::make_shared<Directory>(std::move(directory), fat));
}

fs::exfat::Directory::ptr_type fs::exfat::Directory::tryMake(LogicalDirectory&& directory) {
  auto fat = directory.physicalDirectory()->file()->tryGetFat();
  if (fat == nullptr) {
    return nullptr;
  }
  return make(std::move(directory), fat);
}

fs::Directory::entry_enumeration_type fs::exfat::Directory::begin() {
  m_enumerator.reset();
  if (!m_enumerator.isValid()) {
    return std::nullopt;
  }
  return m_enumerator.entry().standard();
}

fs::Directory::entry_enumeration_type fs::exfat::Directory::next(const entry_type& entry) {
  m_enumerator.reposition(entry.index() + 1);
  if (!m_enumerator.isValid()) {
    return std::nullopt;
  }
  return m_enumerator.entry().standard();
}

fs::exfat::LogicalDirectory::entry_enumeration_type fs::exfat::Directory::find(const name_type& name) {
  m_enumerator.reset();
  for (m_enumerator.reset(); m_enumerator.isValid(); m_enumerator.next()) {
    auto& entry = m_enumerator.entry();
    if (Configuration::directory_entry_name_equal(entry.standard().name(), name)) {
      return entry;
    }
  }
  return std::nullopt;
}

fs::Directory::ptr_type fs::exfat::Directory::openDirectory(const name_type& name) {
  auto entry = find(name);
  if (!entry) {
    return nullptr;
  }
  if (!entry->standard().isDirectory()) {
    return nullptr;
  }
  return Directory::make(LogicalDirectory(PhysicalDirectory(File::make(entry->clusterAllocation(), m_fat, m_enumerator.container().physicalDirectory()->file()->clusterHeap()))), m_fat);
}

fs::Directory::file_type::ptr_type fs::exfat::Directory::openFile(const name_type& name) {
  auto entry = find(name);
  if (!entry) {
    return nullptr;
  }
  if (entry->standard().isDirectory()) {
    return nullptr;
  }
  return File::make(entry->clusterAllocation(), m_fat, m_enumerator.container().physicalDirectory()->file()->clusterHeap());
}

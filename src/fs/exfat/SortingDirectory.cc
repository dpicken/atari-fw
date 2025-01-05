#include "SortingDirectory.h"

#include "fs/DirectoryEnumerator.h"

fs::exfat::SortingDirectory::SortingDirectory(ptr_type&& directory)
  : m_directory(std::move(directory)) {
}

fs::Directory::ptr_type fs::exfat::SortingDirectory::make(ptr_type&& directory) {
  return std::make_shared<SortingDirectory>(std::move(directory));
}

fs::Directory::entry_enumeration_type fs::exfat::SortingDirectory::begin() {
  constexpr auto upperBound = "";
  refillWindow(upperBound, 0);
  return findNext(upperBound);
}

fs::Directory::entry_enumeration_type fs::exfat::SortingDirectory::next(const entry_type& entry) {
  refillWindow(entry.name(), entry.index() + 1);
  return findNext(entry.name());
}

fs::Directory::entry_enumeration_type fs::exfat::SortingDirectory::findNext(const name_type& upperBound) {
  auto it = m_sortedWindow.upper_bound(upperBound);
  if (it == m_sortedWindow.end()) {
    return std::nullopt;
  }
  return *it;
}

void fs::exfat::SortingDirectory::refillWindow(const name_type& upperBound, entry_type::index_type nextIndex) {
  if (m_sortedWindow.find(upperBound) != m_sortedWindow.end()
      && m_sortedWindow.upper_bound(upperBound) != m_sortedWindow.end()) {
    return;
  }

  m_upperBound = upperBound;
  std::size_t sortedWindowSize = 0;
  m_sortedWindow.clear();

  for (::fs::DirectoryEnumerator enumerator(m_directory); enumerator.isValid(); enumerator.next()) {
    if (Configuration::directory_entry_name_greater(enumerator.entry().name(), upperBound)) {
      auto& entry = enumerator.entry();
      sortedWindowSize += entry.size();
      m_sortedWindow.emplace(entry);
    }

    if (m_sortedWindow.size() > 1 && sortedWindowSize > Configuration::instance()->maxSortingDirectoryWindowSize()) {
      auto& entry = *m_sortedWindow.rbegin();
      sortedWindowSize -= entry.size();
      m_sortedWindow.erase(entry);
    }
  }

  for (auto& entry : m_sortedWindow) {
    const_cast<entry_type&>(entry).setIndex(nextIndex++);
  }
}

fs::Directory::ptr_type fs::exfat::SortingDirectory::openDirectory(const name_type& name) {
  return m_directory->openDirectory(name);
}

fs::Directory::file_type::ptr_type fs::exfat::SortingDirectory::openFile(const name_type& name) {
  return m_directory->openFile(name);
}

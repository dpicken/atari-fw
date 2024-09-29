#include "DirectoryEnumerator.h"

fs::DirectoryEnumerator::DirectoryEnumerator(directory_ptr_type directory)
  : m_directory(std::move(directory)) {
  reset();
}

bool fs::DirectoryEnumerator::isValid() const {
  return m_currentEntry.has_value();
}

const fs::DirectoryEnumerator::entry_type& fs::DirectoryEnumerator::entry() const {
  return *m_currentEntry;
}

void fs::DirectoryEnumerator::next() {
  m_currentEntry = m_directory->next(*m_currentEntry);
}

void fs::DirectoryEnumerator::reset() {
  m_currentEntry = m_directory->begin();
}

void fs::DirectoryEnumerator::reposition(index_type index) {
  if (!isValid() || entry().index() > index) {
    reset();
  }
  while (isValid() && entry().index() != index) {
    next();
  }
}

fs::DirectoryEnumerator::directory_type::ptr_type fs::DirectoryEnumerator::openDirectory() const {
  return m_directory->openDirectory(entry().name());
}

fs::DirectoryEnumerator::directory_type::file_type::ptr_type fs::DirectoryEnumerator::openFile() const {
  return m_directory->openFile(entry().name());
}

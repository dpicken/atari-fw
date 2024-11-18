#include "Configuration.h"

fs::exfat::Configuration::Configuration()
 : m_maxSortingDirectoryWindowSize(8 << 10)
 , m_nonAsciiReplacementChar('#')
 , m_excludeHiddenDirectoryEntries(false)
 , m_excludeSystemDirectoryEntries(false)
 , m_excludeUnixHiddenDirectoryEntries(false) {
}

fs::exfat::Configuration* fs::exfat::Configuration::instance() {
  static Configuration configuration;
  return &configuration;
}

fs::exfat::Configuration::size_type fs::exfat::Configuration::maxSortingDirectoryWindowSize() const {
  return m_maxSortingDirectoryWindowSize;
}

void fs::exfat::Configuration::maxSortingDirectoryWindowSize(size_type size) {
  m_maxSortingDirectoryWindowSize = size;
}

char fs::exfat::Configuration::nonAsciiReplacementCharacter() const {
  return m_nonAsciiReplacementChar;
}

void fs::exfat::Configuration::nonAsciiReplacementCharacter(char replacement) {
  m_nonAsciiReplacementChar = replacement;
}

bool fs::exfat::Configuration::excludeHiddenDirectoryEntries() const {
  return m_excludeHiddenDirectoryEntries;
}

void fs::exfat::Configuration::excludeHiddenDirectoryEntries(bool exclude) {
  m_excludeHiddenDirectoryEntries = exclude;
}

bool fs::exfat::Configuration::excludeSystemDirectoryEntries() const {
  return m_excludeSystemDirectoryEntries;
}

void fs::exfat::Configuration::excludeSystemDirectoryEntries(bool exclude) {
  m_excludeSystemDirectoryEntries = exclude;
}

bool fs::exfat::Configuration::excludeUnixHiddenDirectoryEntries() const {
  return m_excludeUnixHiddenDirectoryEntries;
}

void fs::exfat::Configuration::excludeUnixHiddenDirectoryEntries(bool exclude) {
  m_excludeUnixHiddenDirectoryEntries = exclude;
}

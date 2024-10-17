#include "Directory.h"

fs::exfat::Directory::impl_ptr_type fs::exfat::Directory::make() {
  return std::make_shared<Directory>();
}

fs::Directory::entry_enumeration_type fs::exfat::Directory::begin() {
  return std::nullopt;
}

fs::Directory::entry_enumeration_type fs::exfat::Directory::next(const entry_type& entry) {
  (void) entry;
  return std::nullopt;
}

fs::Directory::ptr_type fs::exfat::Directory::openDirectory(const name_type& name) {
  (void) name;
  return nullptr;
}

fs::Directory::file_type::ptr_type fs::exfat::Directory::openFile(const name_type& name) {
  (void) name;
  return nullptr;
}

#include "FileSystem.h"

fs::exfat::FileSystem::FileSystem(name_type&& label, InternalDirectory&& rootDirectory)
  : m_label(std::move(label))
  , m_rootDirectory(Directory::make(std::move(rootDirectory))) {
}

fs::exfat::FileSystem::impl_ptr_type fs::exfat::FileSystem::make(name_type&& label, InternalDirectory&& rootDirectory) {
  return std::make_unique<FileSystem>(std::move(label), std::move(rootDirectory));
}

const fs::FileSystem::name_type& fs::exfat::FileSystem::name() const {
  return m_label;
}

fs::FileSystem::directory_ptr_type fs::exfat::FileSystem::getRootDirectory() const {
  return m_rootDirectory;
}

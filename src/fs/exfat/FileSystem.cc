#include "FileSystem.h"

#include "fs/builtin/FileSystem.h"

fs::exfat::FileSystem::FileSystem(File::ptr_type device)
  : m_device(std::move(device))
  , m_rootDirectory(Directory::make()) {
}

fs::exfat::FileSystem::impl_ptr_type fs::exfat::FileSystem::tryMake(File::ptr_type device) {
  return std::make_unique<FileSystem>(std::move(device));
}

const fs::FileSystem::name_type& fs::exfat::FileSystem::name() const {
  static const name_type name = "";
  return name;
}

fs::FileSystem::directory_ptr_type fs::exfat::FileSystem::getRootDirectory() const {
  return m_rootDirectory;
}

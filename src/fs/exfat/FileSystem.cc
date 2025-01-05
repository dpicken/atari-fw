#include "FileSystem.h"

fs::exfat::FileSystem::FileSystem(name_type&& label, Directory::ptr_type&& rootDirectory)
  : m_label(std::move(label))
  , m_rootDirectory(std::move(rootDirectory)) {
}

fs::exfat::FileSystem::impl_ptr_type fs::exfat::FileSystem::tryMake(name_type&& label, PhysicalDirectory&& rootDirectory) {
  auto directory = Directory::tryMake(std::move(rootDirectory));
  if (!directory) {
    return nullptr;
  }
  return std::make_unique<FileSystem>(std::move(label), std::move(directory));
}

const fs::FileSystem::name_type& fs::exfat::FileSystem::name() const {
  return m_label;
}

fs::FileSystem::directory_ptr_type fs::exfat::FileSystem::getRootDirectory() const {
  return m_rootDirectory;
}

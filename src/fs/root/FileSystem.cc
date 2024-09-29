#include "FileSystem.h"

#include "fs/builtin/FileSystem.h"

fs::root::FileSystem::FileSystem()
  : m_rootDirectory(impl_directory_type::make()) {
  auto builtinFileSystem = ::fs::builtin::FileSystem::instance();
  m_rootDirectory->put(builtinFileSystem->name(), builtinFileSystem->getRootDirectory());
}

fs::root::FileSystem* fs::root::FileSystem::instance() {
  static FileSystem instance;
  return &instance;
}

const fs::FileSystem::name_type& fs::root::FileSystem::name() const {
  static const name_type name = "";
  return name;
}

fs::FileSystem::directory_ptr_type fs::root::FileSystem::getRootDirectory() const {
  return m_rootDirectory;
}

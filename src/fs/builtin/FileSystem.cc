#include "FileSystem.h"

#include "File.h"

#include "media/BuiltinAtrLibrary.h"

fs::builtin::FileSystem::FileSystem()
  : m_rootDirectory(impl_directory_type::make()) {
  populate();
}

fs::builtin::FileSystem* fs::builtin::FileSystem::instance() {
  static FileSystem instance;
  return &instance;
}

void fs::builtin::FileSystem::populate() {
  using builtin_atr_library = ::media::BuiltinAtrLibrary;
  for (builtin_atr_library::size_type index = 0; index != builtin_atr_library::getAtrCount(); ++index) {
    m_rootDirectory->put(
        std::string(builtin_atr_library::getAtrTitle(index)),
        File::make(builtin_atr_library::getAtrData(index), builtin_atr_library::getAtrSize(index)));
  }
}

const fs::FileSystem::name_type& fs::builtin::FileSystem::name() const {
  static const name_type name = "builtin";
  return name;
}

fs::FileSystem::directory_ptr_type fs::builtin::FileSystem::getRootDirectory() const {
  return m_rootDirectory;
}

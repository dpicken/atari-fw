#include "FileSystem.h"

#include "File.h"
#include "LibraryEnumerator.h"

#include <filesystem>

fs::builtin::FileSystem::FileSystem()
  : m_rootDirectory(impl_directory_type::make()) {
  populate();
}

fs::builtin::FileSystem* fs::builtin::FileSystem::instance() {
  static FileSystem instance;
  return &instance;
}

void fs::builtin::FileSystem::populate() {
  for (LibraryEnumerator enumerator(Library::instance()); enumerator.isValid(); enumerator.next()) {
    const auto& entry = enumerator.entry();
    std::filesystem::path path = entry.path;
    // TODO: ensure path.parent_path and place file there.
    m_rootDirectory->put(
        path.filename().string(),
        File::make(entry.data, entry.size));
  }
}

const fs::FileSystem::name_type& fs::builtin::FileSystem::name() const {
  static const name_type name = "builtin";
  return name;
}

fs::FileSystem::directory_ptr_type fs::builtin::FileSystem::getRootDirectory() const {
  return m_rootDirectory;
}

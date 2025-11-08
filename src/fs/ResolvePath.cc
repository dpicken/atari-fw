#include "ResolvePath.h"

#include "root/FileSystem.h"

fs::Directory::ptr_type fs::resolveDirectory(const std::filesystem::path& path) {
  static const auto rootDirectory = root::FileSystem::instance()->getRootDirectory();

  auto directory = rootDirectory;
  auto rpath = path.relative_path();
  for (auto it = rpath.begin(), end = rpath.end(); it != end && directory != nullptr; ++it) {
    directory = directory->openDirectory(*it);
  }

  return directory;
}

fs::File::ptr_type fs::resolveFile(const std::filesystem::path& path) {
  auto directory = resolveDirectory(path.parent_path());
  if (!directory) {
    return nullptr;
  }
  return directory->openFile(path.filename());
}

fs::File::ptr_type fs::resolveWellKnownFile(const std::string& name) {
  auto file = resolveFile(std::filesystem::path{"/sd"} / name);
  if (!file) {
    file = resolveFile(std::filesystem::path{"/builtin"} / name);
  }
  return file;
}

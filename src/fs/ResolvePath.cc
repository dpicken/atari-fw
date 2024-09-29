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

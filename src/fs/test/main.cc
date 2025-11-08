#include "fs/DirectoryEnumerator.h"
#include "fs/ResolvePath.h"
#include "fs/builtin/FileSystem.h"
#include "fs/root/FileSystem.h"

#include <cstdlib>
#include <set>
#include <stdexcept>

namespace {

void testRootFileSystemRootDirectory(const fs::Directory::ptr_type& directory) {
  if (directory == nullptr) {
    throw std::logic_error("root fs root directory is null");
  }

  std::set<fs::Directory::name_type> rootDirEntries;

  fs::DirectoryEnumerator dirEnumerator(directory);
  for (; dirEnumerator.isValid(); dirEnumerator.next()) {
    if (!dirEnumerator.entry().isDirectory()) {
      throw std::logic_error("root fs root directory: contains a non-directory entry");
    }
    if (dirEnumerator.openDirectory() == nullptr) {
      throw std::logic_error("root fs root directory: failed to open a directory");
    }
    if (dirEnumerator.openFile() != nullptr) {
      throw std::logic_error("root fs root directory: opened a file");
    }
    rootDirEntries.insert(dirEnumerator.entry().name());
  }

  std::set<std::string> expectedDirEntries;
  expectedDirEntries.insert("builtin");

  if (rootDirEntries != expectedDirEntries) {
    throw std::logic_error("root fs root directory entries inconsistent with expected dir entries");
  }
}

void testBuiltinFileSystemRootDirectory(const fs::Directory::ptr_type& directory) {
  if (directory == nullptr) {
    throw std::logic_error("builtin fs root directory is null");
  }

  std::set<fs::Directory::name_type> dirEntries;

  fs::DirectoryEnumerator dirEnumerator(directory);
  for (; dirEnumerator.isValid(); dirEnumerator.next()) {
    if (dirEnumerator.entry().isDirectory()) {
      throw std::logic_error("builtin fs root directory: contains a directory entry");
    }
    if (dirEnumerator.openDirectory() != nullptr) {
      throw std::logic_error("builtin fs root directory: opened a directory");
    }
    if (dirEnumerator.openFile() == nullptr) {
      throw std::logic_error("builtin fs root directory: failed to open a file");
    }
    dirEntries.insert(dirEnumerator.entry().name());
  }

  std::set<std::string> expectedDirEntries;
  expectedDirEntries.insert("!sbc-boot.atr");
  expectedDirEntries.insert("!sbc-boot.xex");
  expectedDirEntries.insert("!xex-loader.atr");

  if (dirEntries != expectedDirEntries) {
    throw std::logic_error("builtin fs root directory inconsistent with expected dir entries");
  }
}

} // namespace

int main(int, char**) {
  testRootFileSystemRootDirectory(fs::root::FileSystem::instance()->getRootDirectory());
  testBuiltinFileSystemRootDirectory(fs::builtin::FileSystem::instance()->getRootDirectory());

  testRootFileSystemRootDirectory(fs::resolveDirectory("/"));
  testBuiltinFileSystemRootDirectory(fs::resolveDirectory("/builtin"));

  return EXIT_SUCCESS;
}

#include "DirectoryEnumerator.h"

fs::DirectoryEnumerator::DirectoryEnumerator(Directory::ptr_type directory)
  : Enumerator<Directory::ptr_type>(std::move(directory)) {
}

fs::Directory::ptr_type fs::DirectoryEnumerator::openDirectory() const {
  return container()->openDirectory(entry().name());
}

fs::File::ptr_type fs::DirectoryEnumerator::openFile() const {
  return container()->openFile(entry().name());
}

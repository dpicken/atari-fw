#ifndef fs_DirectoryEnumerator_h
#define fs_DirectoryEnumerator_h

#include "Directory.h"
#include "Enumerator.h"
#include "File.h"

namespace fs {

class DirectoryEnumerator : public Enumerator<Directory::ptr_type> {
public:
  explicit DirectoryEnumerator(Directory::ptr_type directory);

  Directory::ptr_type openDirectory() const;

  File::ptr_type openFile() const;
};

} // namespace fs

#endif // ifndef fs_DirectoryEnumerator_h

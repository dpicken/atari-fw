#ifndef fs_DirectoryEnumerator_h
#define fs_DirectoryEnumerator_h

#include "Directory.h"

namespace fs {

class DirectoryEnumerator {
public:
  using directory_type = Directory;
  using directory_ptr_type = directory_type::ptr_type;
  using entry_type = directory_type::entry_type;
  using entry_enumeration_type = directory_type::entry_enumeration_type;
  using index_type = entry_type::index_type;

  explicit DirectoryEnumerator(directory_ptr_type directory);

  bool isValid() const;

  const entry_type& entry() const;

  void next();

  void reset();

  void reposition(index_type index);

  directory_type::ptr_type openDirectory() const;
  directory_type::file_type::ptr_type openFile() const;

private:
  directory_ptr_type m_directory;

  entry_enumeration_type m_currentEntry;
};

} // namespace fs

#endif // ifndef fs_DirectoryEnumerator_h

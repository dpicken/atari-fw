#ifndef fs_FileSystem_h
#define fs_FileSystem_h

#include "Directory.h"
#include "File.h"

#include <memory>

namespace fs {

class FileSystem {
public:
  template<typename FileSystemType>
  using ptr = std::unique_ptr<FileSystemType>;

  using ptr_type = ptr<FileSystem>;

  using directory_type = Directory;
  using directory_ptr_type = directory_type::ptr_type;

  using file_type = File;
  using file_ptr_type = file_type::ptr_type;

  using name_type = directory_type::name_type;

  virtual ~FileSystem();

  virtual const name_type& name() const = 0;

  virtual directory_ptr_type getRootDirectory() const = 0;
};

} // namespace fs

#endif // ifndef fs_FileSystem_h

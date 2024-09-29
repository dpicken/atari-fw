#ifndef fs_builtin_FileSystem_h
#define fs_builtin_FileSystem_h

#include "fs/FileSystem.h"
#include "fs/ram/Directory.h"

namespace fs { namespace builtin {

class FileSystem : public ::fs::FileSystem {
public:
  static FileSystem* instance();

  const name_type& name() const override;

  directory_ptr_type getRootDirectory() const override;

private:
  FileSystem();

  using impl_directory_type = ::fs::ram::Directory;

  void populate();

  const impl_directory_type::impl_ptr_type m_rootDirectory;
};

} } // namespace fs::builtin

#endif // ifndef fs_builtin_FileSystem_h

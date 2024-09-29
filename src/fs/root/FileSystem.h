#ifndef fs_root_FileSystem_h
#define fs_root_FileSystem_h

#include "fs/FileSystem.h"
#include "fs/ram/Directory.h"

namespace fs { namespace root {

class FileSystem : public ::fs::FileSystem {
public:
  using impl_directory_type = ::fs::ram::Directory;

  FileSystem();

  static FileSystem* instance();

  const name_type& name() const override;

  directory_ptr_type getRootDirectory() const override;

  impl_directory_type* getRootDirectoryImpl() { return m_rootDirectory.get(); }

private:
  const impl_directory_type::impl_ptr_type m_rootDirectory;
};

} } // namespace fs::root

#endif // ifndef fs_root_FileSystem_h

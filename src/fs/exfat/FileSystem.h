#ifndef fs_exfat_FileSystem_h
#define fs_exfat_FileSystem_h

#include "Directory.h"

#include "fs/FileSystem.h"

namespace fs { namespace exfat {

class FileSystem : public ::fs::FileSystem {
public:
  using impl_ptr_type = ptr<FileSystem>;

  using impl_directory_type = Directory;

  FileSystem(File::ptr_type device);

  static impl_ptr_type tryMake(File::ptr_type device);

  const name_type& name() const override;

  directory_ptr_type getRootDirectory() const override;

private:
  const File::ptr_type m_device;

  const impl_directory_type::impl_ptr_type m_rootDirectory;
};

} } // namespace fs::exfat

#endif // ifndef fs_exfat_FileSystem_h

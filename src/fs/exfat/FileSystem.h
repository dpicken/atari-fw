#ifndef fs_exfat_FileSystem_h
#define fs_exfat_FileSystem_h

#include "Directory.h"
#include "PhysicalDirectory.h"

#include "fs/FileSystem.h"

namespace fs { namespace exfat {

class FileSystem : public ::fs::FileSystem {
public:
  using impl_ptr_type = ptr<FileSystem>;

  FileSystem(name_type&& label, Directory::ptr_type&& rootDirectory);

  static impl_ptr_type tryMake(name_type&& label, PhysicalDirectory&& rootDirectory);

  const name_type& name() const override;

  directory_ptr_type getRootDirectory() const override;

private:
  const name_type m_label;
  const Directory::ptr_type m_rootDirectory;
};

} } // namespace fs::exfat

#endif // ifndef fs_exfat_FileSystem_h

#ifndef fs_exfat_Directory_h
#define fs_exfat_Directory_h

#include "ClusterAllocation.h"
#include "File.h"
#include "LogicalDirectory.h"
#include "LogicalDirectoryEnumerator.h"

#include "fs/Directory.h"
#include "fs/FileSlice.h"

namespace fs { namespace exfat {

class Directory : public ::fs::Directory {
public:
  using impl_ptr_type = ptr<Directory>;

  Directory(LogicalDirectory&& directory, const ::fs::FileSlice::impl_ptr_type& fat);

  static ptr_type tryMake(LogicalDirectory&& directory);
  static ptr_type make(LogicalDirectory&& directory, const ::fs::FileSlice::impl_ptr_type& fat);

  entry_enumeration_type begin() override;
  entry_enumeration_type next(const entry_type& entry) override;

  ptr_type openDirectory(const name_type& name) override;
  file_type::ptr_type openFile(const name_type& name) override;

private:
  LogicalDirectory::entry_enumeration_type find(const name_type& name);

  const ::fs::FileSlice::impl_ptr_type m_fat;

  LogicalDirectoryEnumerator m_enumerator;
};

} } // namespace fs::exfat

#endif // ifndef fs_exfat_Directory_h

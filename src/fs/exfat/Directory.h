#ifndef fs_exfat_Directory_h
#define fs_exfat_Directory_h

#include "fs/Directory.h"

namespace fs { namespace exfat {

class Directory : public ::fs::Directory {
public:
  using impl_ptr_type = ptr<Directory>;

  static impl_ptr_type make();

  entry_enumeration_type begin() override;
  entry_enumeration_type next(const entry_type& entry) override;

  ptr_type openDirectory(const name_type& name) override;

  file_type::ptr_type openFile(const name_type& name) override;

private:
};

} } // namespace fs::exfat

#endif // ifndef fs_exfat_Directory_h

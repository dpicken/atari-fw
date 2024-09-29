#ifndef fs_Directory_h
#define fs_Directory_h

#include "DirectoryEntry.h"
#include "File.h"

#include <memory>
#include <optional>

namespace fs {

class Directory {
public:
  template<typename DirectoryType>
  using ptr = std::shared_ptr<DirectoryType>;

  using ptr_type = ptr<Directory>;

  using entry_type = DirectoryEntry;
  using entry_enumeration_type = std::optional<entry_type>;
  using name_type = entry_type::name_type;

  using file_type = File;

  virtual ~Directory();

  virtual entry_enumeration_type begin() = 0;
  virtual entry_enumeration_type next(const entry_type& entry) = 0;

  virtual ptr_type openDirectory(const name_type& name) = 0;
  virtual file_type::ptr_type openFile(const name_type& name) = 0;
};

} // namespace fs

#endif // ifndef fs_Directory_h

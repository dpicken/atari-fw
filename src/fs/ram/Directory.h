#ifndef fs_ram_Directory_h
#define fs_ram_Directory_h

#include "fs/Directory.h"

#include <map>
#include <variant>

namespace fs { namespace ram {

class Directory : public ::fs::Directory {
public:
  using impl_ptr_type = ptr<Directory>;

  static impl_ptr_type make();

  entry_enumeration_type begin() override;
  entry_enumeration_type next(const entry_type& entry) override;

  ptr_type openDirectory(const name_type& name) override;

  file_type::ptr_type openFile(const name_type& name) override;

  void put(name_type name, ptr_type directory);
  void put(name_type name, file_type::ptr_type file);
  void remove(const name_type& name);

private:
  using impl_entry_type = std::variant<ptr_type, file_type::ptr_type>;
  using map_type = std::map<name_type, impl_entry_type>;

  static entry_type::Type toEntryType(const impl_entry_type& entry);

  template<typename PtrType>
  PtrType open(const name_type& name);

  map_type m_map;
};

} } // namespace fs::ram

#endif // ifndef fs_ram_Directory_h

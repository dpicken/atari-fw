#ifndef fs_exfat_SortingDirectory_h
#define fs_exfat_SortingDirectory_h

#include "Configuration.h"

#include "fs/Directory.h"

#include <set>
#include <type_traits>

namespace fs { namespace exfat {

class SortingDirectory : public ::fs::Directory {
public:
  using impl_ptr_type = ptr<SortingDirectory>;

  SortingDirectory(ptr_type&& directory);

  static ptr_type make(ptr_type&& directory);

  entry_enumeration_type begin() override;
  entry_enumeration_type next(const entry_type& entry) override;

  ptr_type openDirectory(const name_type& name) override;
  file_type::ptr_type openFile(const name_type& name) override;

private:
  struct set_entry_compare_type {
    bool operator()(const entry_type& e1, const entry_type& e2) const {
      return Configuration::directory_entry_name_less(e1.name(), e2.name());
    }
    bool operator()(const entry_type& e1, const name_type& e2) const {
      return Configuration::directory_entry_name_less(e1.name(), e2);
    }
    bool operator()(const name_type& e1, const entry_type& e2) const {
      return Configuration::directory_entry_name_less(e1, e2.name());
    }
    using is_transparent = std::true_type;
  };
  using set_type = std::set<entry_type, set_entry_compare_type>;

  void refillWindow(const name_type& sortUpperBound, entry_type::index_type nextIndex);
  entry_enumeration_type findNext(const name_type& upperBound);

  const ptr_type m_directory;

  name_type m_upperBound;

  set_type m_sortedWindow;
};

} } // namespace fs::exfat

#endif // ifndef fs_exfat_SortingDirectory_h

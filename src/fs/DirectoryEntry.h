#ifndef fs_DirectoryEntry_h
#define fs_DirectoryEntry_h

#include <cstdint>
#include <string>

namespace fs {

class DirectoryEntry {
public:
  using name_type = std::string;
  enum class Type { Directory, File };
  using index_type = std::uint32_t;

  DirectoryEntry(name_type name, Type type, index_type index);

  const name_type& name() const {
    return m_name;
  }

  Type type() const {
    return m_type;
  }

  bool isDirectory() const {
    return type() == Type::Directory;
  }

  index_type index() const {
    return m_index;
  }

private:
  name_type m_name;
  Type m_type;
  index_type m_index;
};

} // namespace fs

#endif // ifndef fs_DirectoryEntry_h

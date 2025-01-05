#ifndef fs_exfat_Configuration_h
#define fs_exfat_Configuration_h

#include "Unicode.h"

#include "fs/Directory.h"
#include "util/Ascii.h"

#include <cstddef>

namespace fs { namespace exfat {

struct StaticConfiguration {
  using directory_entry_name_back_inserter_type = UTF16ToAsciiBackInserter;

  static constexpr auto directory_entry_name_less = util::AsciiCaseInsensitivelyLess{};
  static constexpr auto directory_entry_name_equal = util::AsciiCaseInsensitivelyEqual{};
  static constexpr auto directory_entry_name_greater = util::AsciiCaseInsensitivelyGreater{};
};

class Configuration : public StaticConfiguration {
public:
  using size_type = std::size_t;

  static Configuration* instance();

  size_type maxSortingDirectoryWindowSize() const;
  void maxSortingDirectoryWindowSize(size_type size);

  char nonAsciiReplacementCharacter() const;
  void nonAsciiReplacementCharacter(char replacement);

  bool excludeHiddenDirectoryEntries() const;
  void excludeHiddenDirectoryEntries(bool exclude);

  bool excludeSystemDirectoryEntries() const;
  void excludeSystemDirectoryEntries(bool exclude);

  bool excludeUnixHiddenDirectoryEntries() const;
  void excludeUnixHiddenDirectoryEntries(bool exclude);

private:
  Configuration();

  size_type m_maxSortingDirectoryWindowSize;
  char m_nonAsciiReplacementChar;
  bool m_excludeHiddenDirectoryEntries;
  bool m_excludeSystemDirectoryEntries;
  bool m_excludeUnixHiddenDirectoryEntries;
};

} } // namespace fs::exfat

#endif // ifndef fs_exfat_Configuration_h

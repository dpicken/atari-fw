#ifndef fs_builtin_Library_h
#define fs_builtin_Library_h

#include <cstdint>
#include <cstddef>
#include <optional>
#include <string_view>

namespace fs { namespace builtin {

struct Library {
  struct entry_type {
    std::string_view path;
    std::size_t size;
    const std::uint8_t* data;

    using index_type = std::size_t;
    index_type index;
  };
  using entry_enumeration_type = std::optional<entry_type>;

  static const Library* instance();

  entry_enumeration_type begin() const;
  entry_enumeration_type next(const entry_type& entry) const;

private:
  constexpr std::size_t entryCount() const {
    return sizeof(s_paths) / sizeof(*s_paths);
  }

  static constexpr const char* s_paths[] = {
    #include "Library.gen.paths.h"
  };

  static constexpr std::size_t s_sizes[] = {
    #include "Library.gen.sizes.h"
  };

  static constexpr std::uint8_t s_data[] = {
    #include "Library.gen.data.h"
  };
};

} } // namespace fs::builtin

#endif // ifndef fs_builtin_Library_h

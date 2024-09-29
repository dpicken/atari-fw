#ifndef media_BuiltinAtrLibrary_h
#define media_BuiltinAtrLibrary_h

#include <cstdint>
#include <cstddef>
#include <memory>
#include <string_view>

namespace media {

struct BuiltinAtrLibrary {
  using size_type = std::size_t;

  static constexpr std::size_t getAtrCount() {
    return sizeof(index) / sizeof(*index);
  }

  static constexpr const std::uint8_t* getAtrData(size_type atrIndex) {
    std::size_t offset = 0;
    for (size_type i = 0; i != atrIndex; ++i) {
      offset += index[i];
    }
    return &data[offset];
  }

  static constexpr std::size_t getAtrSize(size_type atrIndex) {
    return index[atrIndex];
  }

  static constexpr std::string_view getAtrTitle(size_type atrIndex) {
    return std::string_view(title[atrIndex]);
  }

private:
  static constexpr std::uint8_t data[] = {
    #include "BuiltinAtrLibrary.gen.data.h"
  };

  static constexpr std::size_t index[] = {
    #include "BuiltinAtrLibrary.gen.index.h"
  };

  static constexpr const char* title[] = {
    #include "BuiltinAtrLibrary.gen.title.h"
  };
};

} // namespace media

#endif // ifndef media_BuiltinAtrLibrary_h

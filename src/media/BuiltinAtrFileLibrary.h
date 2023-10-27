#ifndef media_BuiltinAtrFileLibrary_h
#define media_BuiltinAtrFileLibrary_h

#include "io/RomFile.h"

#include <cstdint>
#include <cstddef>
#include <memory>

namespace media {

struct BuiltinAtrFileLibrary {
  static constexpr std::size_t getAtrCount() {
    return sizeof(index) / sizeof(*index);
  }

  static constexpr const std::uint8_t* getAtrData(unsigned int atrIndex) {
    std::size_t offset = 0;
    for (unsigned int i = 0; i != atrIndex; ++i) {
      offset += index[i];
    }
    return &data[offset];
  }

  static constexpr std::size_t getAtrSize(unsigned int atrIndex) {
    return index[atrIndex];
  }

  static constexpr const char* getAtrTitle(unsigned int atrIndex) {
    return title[atrIndex];
  }

  static std::unique_ptr<::io::RomFile> makeRomFile(unsigned int atrIndex) {
    return std::make_unique<io::RomFile>(getAtrData(atrIndex), getAtrSize(atrIndex));
  }

private:
  static constexpr std::uint8_t data[] = {
    #include "BuiltinAtrFileLibrary.gen.data.h"
  };

  static constexpr std::size_t index[] = {
    #include "BuiltinAtrFileLibrary.gen.index.h"
  };

  static constexpr const char* title[] = {
    #include "BuiltinAtrFileLibrary.gen.title.h"
  };
};

} // namespace media

#endif // ifndef media_BuiltinAtrFileLibrary_h

#ifndef media_DiskLibraryAtr_h
#define media_DiskLibraryAtr_h

namespace media {

struct DiskLibraryAtr {
  static constexpr const std::uint8_t* getAtrData(std::size_t diskIndex) {
    std::size_t offset = 0;
    for (unsigned int i = 0; i != diskIndex; ++i) {
      offset += index[i];
    }
    return &data[offset];
  }

  static constexpr std::size_t getAtrSize(std::size_t diskIndex) {
    return index[diskIndex];
  }

  static constexpr const char* getAtrTitle(std::size_t diskIndex) {
    return title[diskIndex];
  }

  static constexpr std::size_t getAtrCount() {
    return sizeof(index) / sizeof(*index);
  }

private:
  static constexpr std::uint8_t data[] = {
    #include "DiskLibraryAtr.gen.data.h"
  };

  static constexpr std::size_t index[] = {
    #include "DiskLibraryAtr.gen.index.h"
  };

  static constexpr const char* title[] = {
    #include "DiskLibraryAtr.gen.title.h"
  };
};

} // namespace media

#endif // ifndef media_DiskLibraryAtr_h

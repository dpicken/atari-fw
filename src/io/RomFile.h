#ifndef io_RomFile_h
#define io_RomFile_h

#include "File.h"

namespace io {

class RomFile : public File {
public:
  constexpr RomFile(const std::uint8_t* data, std::size_t size)
    : m_data(data)
    , m_size(size) {
  }

  constexpr size_type size() const override {
    return m_size;
  }

  bool read(size_type offset, std::size_t byteCount, const sink_type& sink) const override;

private:
  const std::uint8_t* const m_data;
  const std::size_t m_size;
};

} // namespace io

#endif // ifndef io_RomFile_h

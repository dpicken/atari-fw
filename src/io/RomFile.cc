#include "RomFile.h"

bool io::RomFile::read(size_type offset, std::size_t byteCount, const sink_type& sink) const {
  if (offset + byteCount > m_size) {
    return false;
  }
  sink(m_data + offset, byteCount);
  return true;
}

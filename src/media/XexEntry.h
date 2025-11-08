#ifndef media_XexEntry_h
#define media_XexEntry_h

#include "fs/File.h"
#include "util/PACKED.h"

#include <cstdint>

namespace media {

struct XexEntry {
  struct segment_header_type {
    std::uint16_t loadAddressBegin;
    std::uint16_t loadAddressLast;
  } PACKED;

  using size_type = ::fs::File::size_type;

  using index_type = std::uint32_t;

  XexEntry(const segment_header_type& segmentHeader, size_type segmentDataOffset, index_type index);

  const segment_header_type& segmentHeader() const {
    return m_segmentHeader;
  }

  size_type segmentDataOffset() const {
    return m_segmentDataOffset;
  }

  std::size_t segmentDataSize() const {
    return (std::size_t{m_segmentHeader.loadAddressLast} + 1) - m_segmentHeader.loadAddressBegin;
  }

  size_type segmentDataEnd() const {
    return m_segmentDataOffset + segmentDataSize();
  }

  index_type index() const {
    return m_index;
  }

private:
  segment_header_type m_segmentHeader;
  size_type m_segmentDataOffset;
  index_type m_index;
};

} // namespace media

#endif // ifndef media_XexEntry_h

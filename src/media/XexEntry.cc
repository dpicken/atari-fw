#include "XexEntry.h"

media::XexEntry::XexEntry(const segment_header_type& segmentHeader, size_type segmentDataOffset, index_type index)
  : m_segmentHeader(segmentHeader)
  , m_segmentDataOffset(segmentDataOffset)
  , m_index(index) {
}

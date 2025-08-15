#include "Xex.h"

media::Xex::Xex(::fs::File::ptr_type xexFile)
  : m_xexFile(std::move(xexFile)) {
}

media::Xex::entry_enumeration_type media::Xex::begin() {
  return readEntry(0, 0);
}

media::Xex::entry_enumeration_type media::Xex::next(const entry_type& entry) {
  return readEntry(entry.segmentDataEnd(), entry.index() + 1);
}

media::Xex::entry_enumeration_type media::Xex::readEntry(size_type offset, entry_type::index_type index) {
  if (m_xexFile == nullptr) {
    return std::nullopt;
  }

  std::uint16_t binaryHeader;
  if (!m_xexFile->read(offset, &binaryHeader)) {
    return std::nullopt;
  }

  if (offset == 0 && binaryHeader != 0xFFFF) {
    return std::nullopt;
  }

  if (binaryHeader == 0xFFFF) {
    offset += sizeof(binaryHeader);
  }

  XexEntry::segment_header_type segmentHeader;
  if (!m_xexFile->read(offset, &segmentHeader)) {
    return std::nullopt;
  }

  auto segmentDataOffset = offset + sizeof(segmentHeader);

  entry_type entry(segmentHeader, segmentDataOffset, index);
  if (entry.segmentDataEnd() > m_xexFile->size()) {
    return std::nullopt;
  }

  return entry;
}

bool media::Xex::readSegmentData(const entry_type& entry, const sink_type& sink) {
  return m_xexFile->read(entry.segmentDataOffset(), entry.segmentDataSize(), sink);
}

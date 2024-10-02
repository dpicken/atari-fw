#include "FileSlice.h"


fs::FileSlice::FileSlice(ptr_type file, size_type offset, size_type size)
  : m_file(std::move(file))
  , m_offset(offset)
  , m_size(size) {
}

fs::FileSlice::impl_ptr_type fs::FileSlice::make(ptr_type file, size_type offset, size_type size) {
  if (offset + size > file->size()) {
    return nullptr;
  }
  return std::make_shared<FileSlice>(file, offset, size);
}

fs::File::size_type fs::FileSlice::size() {
  return m_size;
}

fs::File::size_type fs::FileSlice::offset() const {
  return m_offset;
}

bool fs::FileSlice::readImpl(size_type offset, buffer_size_type byteCount, const sink_type& sink) {
  return m_file->read(m_offset + offset, byteCount, sink);
}

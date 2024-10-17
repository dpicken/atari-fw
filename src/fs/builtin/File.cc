#include "File.h"

fs::builtin::File::File(const buffer_value_type* data, buffer_size_type size)
  : m_data(data)
  , m_size(size) {
}

fs::builtin::File::impl_ptr_type fs::builtin::File::make(const buffer_value_type* data, buffer_size_type size) {
  return std::make_shared<File>(data, size);
}

fs::File::block_size_type fs::builtin::File::blockSize() const {
  constexpr auto blockSize = BlockSize::fromSizeLog2(0);
  return blockSize;
}

fs::File::size_type fs::builtin::File::size() {
  return m_size;
}

bool fs::builtin::File::readImpl(size_type offset, buffer_size_type byteCount, const sink_type& sink) {
  sink(m_data + offset, byteCount);
  return true;
}

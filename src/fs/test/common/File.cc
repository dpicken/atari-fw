#include "File.h"

fs::test::common::File::File(const buffer_value_type* data, buffer_size_type size, BlockSize blockSize)
  : m_data(data)
  , m_size(size)
  , m_blockSize(blockSize) {
}

fs::test::common::File::impl_ptr_type fs::test::common::File::make(const buffer_value_type* data, buffer_size_type size, BlockSize blockSize) {
  return std::make_shared<File>(data, size, blockSize);
}

fs::File::block_size_type fs::test::common::File::blockSize() const {
  return m_blockSize;
}

fs::File::size_type fs::test::common::File::size() {
  return m_size;
}

bool fs::test::common::File::readImpl(size_type offset, buffer_size_type byteCount, const sink_type& sink) {
  sink(m_data + offset, byteCount);
  return true;
}

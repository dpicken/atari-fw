#include "File.h"

#include <thread>
#include <vector>

fs::test::common::File::File(const std::filesystem::path& path, size_type size, BlockSize blockSize)
  : m_blockSize(blockSize)
  , m_size(size)
  , m_file(path, std::ios::in | std::ios::binary) {
}

fs::test::common::File::impl_ptr_type fs::test::common::File::make(const std::filesystem::path& path, size_type size, BlockSize blockSize) {
  auto file = std::make_shared<File>(path, size, blockSize);
  if (!file->m_file) {
    return nullptr;
  }
  return file;
}

fs::test::common::File::impl_ptr_type fs::test::common::File::make(const std::filesystem::path& path, BlockSize blockSize) {
  return make(path, std::filesystem::file_size(path), blockSize);
}

fs::File::block_size_type fs::test::common::File::blockSize() const {
  return m_blockSize;
}

fs::File::size_type fs::test::common::File::size() {
  return m_size;
}

bool fs::test::common::File::readImpl(size_type offset, buffer_size_type byteCount, const sink_type& sink) {
  m_file.clear();

  if (!m_file.seekg(static_cast<std::streamoff>(offset)).good()) {
    return false;
  }

  std::vector<buffer_value_type> buffer(byteCount, 0);
  if (!m_file.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamoff>(buffer.size())).good()) {
    return false;
  }

  sink(buffer.data(), buffer.size());

  return true;
}

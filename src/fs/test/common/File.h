#ifndef fs_test_common_File_h
#define fs_test_common_File_h

#include "fs/File.h"

#include <filesystem>
#include <fstream>

namespace fs { namespace test { namespace common {

class File : public ::fs::File {
public:
  using impl_ptr_type = ptr<File>;

  File(const std::filesystem::path& path, size_type size, BlockSize blockSize);

  static impl_ptr_type make(const std::filesystem::path& path, size_type size, BlockSize blockSize);
  static impl_ptr_type make(const std::filesystem::path& path, BlockSize blockSize);

  block_size_type blockSize() const override;

  size_type size() override;

protected:
  bool readImpl(size_type offset, buffer_size_type byteCount, const sink_type& sink) override;

private:
  const BlockSize m_blockSize;
  const size_type m_size;

  std::fstream m_file;
};

} } } // namespace fs::test::common

#endif // ifndef fs_test_common_File_h

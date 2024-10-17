#ifndef fs_test_common_File_h
#define fs_test_common_File_h

#include "fs/File.h"

namespace fs { namespace test { namespace common {

class File : public ::fs::File {
public:
  using impl_ptr_type = ptr<File>;

  File(const buffer_value_type* data, buffer_size_type size, BlockSize blockSize);

  static impl_ptr_type make(const buffer_value_type* data, buffer_size_type size, BlockSize blockSize);

  block_size_type blockSize() const override;

  size_type size() override;

protected:
  bool readImpl(size_type offset, buffer_size_type byteCount, const sink_type& sink) override;

private:
  const buffer_value_type* m_data;
  const buffer_size_type m_size;
  const BlockSize m_blockSize;
};

} } } // namespace fs::test::common

#endif // ifndef fs_test_common_File_h

#ifndef fs_FileSlice_h
#define fs_FileSlice_h

#include "File.h"

namespace fs {

class FileSlice : public File {
public:
  using impl_ptr_type = ptr<FileSlice>;

  FileSlice(ptr_type file, size_type offset, size_type size);

  static impl_ptr_type tryMake(ptr_type file, size_type offset, size_type size);

  block_size_type blockSize() const override;

  size_type size() override;

  size_type offset() const;

private:
  bool readImpl(size_type offset, buffer_size_type byteCount, const sink_type& sink) override;

  const ptr_type m_file;

  const size_type m_offset;

  const size_type m_size;
};

} // namespace fs

#endif // ifndef fs_FileSlice_h

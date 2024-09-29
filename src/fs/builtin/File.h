#ifndef fs_builtin_File_h
#define fs_builtin_File_h

#include "fs/File.h"

namespace fs { namespace builtin {

class File : public ::fs::File {
public:
  using impl_ptr_type = ptr<File>;

  File(const buffer_value_type* data, buffer_size_type size);

  static impl_ptr_type make(const buffer_value_type* data, buffer_size_type size);

  size_type size() override;

protected:
  bool readImpl(size_type offset, buffer_size_type byteCount, const sink_type& sink) override;

private:
  const buffer_value_type* m_data;
  const buffer_size_type m_size;
};

} } // namespace fs::builtin

#endif // ifndef fs_builtin_File_h

#ifndef fs_File_h
#define fs_File_h

#include "BlockSize.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>

namespace fs {

class File {
public:
  template<typename FileType>
  using ptr = std::shared_ptr<FileType>;

  using ptr_type = ptr<File>;

  using block_size_type = BlockSize;
  using size_type = std::uint64_t;

  using buffer_value_type = std::uint8_t;
  using buffer_size_type = std::size_t;
  using sink_type = std::function<void(const buffer_value_type* data, buffer_size_type size)>;

  virtual ~File();

  virtual block_size_type blockSize() const = 0;

  virtual size_type size() = 0;

  bool read(size_type offset, buffer_size_type byteCount, const sink_type& sink) {
    if (offset + byteCount > size()) {
      return false;
    }
    return readImpl(offset, byteCount, sink);
  }


  bool read(size_type offset, buffer_value_type* buffer, buffer_size_type bufferSize) {
    return read(offset, bufferSize, [&buffer](const buffer_value_type* data, buffer_size_type size) {
      buffer = std::copy(data, data + size, buffer);
    });
  }

  template<typename PackedType>
  bool read(size_type offset, PackedType* value) {
    return read(offset, reinterpret_cast<buffer_value_type*>(value), sizeof(*value));
  }

protected:
  virtual bool readImpl(size_type offset, buffer_size_type byteCount, const sink_type& sink) = 0;
};

} // namespace fs

#endif // ifndef fs_File_h

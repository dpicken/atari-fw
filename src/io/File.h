#ifndef io_File_h
#define io_File_h

#include "Sink.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>

namespace io {

class File {
public:
  using sink_type = Sink;

  virtual ~File();

  using size_type = std::uint64_t;

  virtual size_type size() const = 0;

  virtual bool read(size_type offset, std::size_t byteCount, const sink_type& sink) const = 0;

  template<typename PackedType>
  bool read(size_type offset, PackedType* value) {
    std::size_t sinkOffset = 0;
    return read(offset, sizeof(*value), makeCopyToSink(value, sinkOffset));
  }

private:
  template<typename PackedType>
  Sink makeCopyToSink(PackedType* value, std::size_t& sinkOffset) {
    return [value, &sinkOffset](const std::uint8_t* data, std::size_t size) {
      std::copy(data, data + size, reinterpret_cast<std::uint8_t*>(value) + sinkOffset);
      sinkOffset += size;
    };
  }
};

} // namespace io

#endif // ifndef io_File_h

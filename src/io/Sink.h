#ifndef io_Sink_h
#define io_Sink_h

#include <cstddef>
#include <cstdint>
#include <functional>

namespace io {

  using Sink = std::function<void(const std::uint8_t* data, std::size_t size)>;

} // namespace io

#endif // ifndef io_Sink_h

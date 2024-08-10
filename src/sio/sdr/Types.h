#ifndef sio_sdr_Types_h
#define sio_sdr_Types_h

#include "util/PACKED.h"

#include <cstdint>

#ifndef BUILD_MOS
#include <algorithm>
#include <string_view>
#endif

namespace sio { namespace sdr {

using bool_type = std::uint8_t;

using unsigned_int_type = std::uint16_t;

using size_type = std::uint16_t;

template<typename PackedType, size_type CapacityV>
struct Vector {
  using value_type = PackedType;
  using size_type = decltype(CapacityV);

  static constexpr auto capacity_v = CapacityV;

  Vector() : m_size(0) {}

#ifndef BUILD_MOS
  Vector(const value_type* data, size_type size) : m_size(size) {
    std::copy_n(data, m_size, m_buffer);
  }
#endif

  bool full() const {
    return m_size == capacity_v;
  }

  template<typename... Args>
  value_type& emplace_back(Args... args) {
    return m_buffer[m_size++] = value_type(args...);
  }

  const value_type* begin() const {
    return m_buffer;
  }

  const value_type* end() const {
    return m_buffer + m_size;
  }

  const size_type size() const {
    return m_size;
  }

private:
  size_type m_size;
  value_type m_buffer[capacity_v];
} PACKED;

template<size_type CapacityV>
struct String : Vector<char, CapacityV> {
  using base_type = Vector<char, CapacityV>;
  using value_type = typename base_type::value_type;

  static constexpr auto capacity_v = base_type::capacity_v;

  String() {}

#ifndef BUILD_MOS
  explicit String(const std::string_view& value) : base_type(value.data(), std::min(size_type(value.size()), capacity_v)) {}
#endif

  const value_type* data() const {
    return this->begin();
  }
} PACKED;

} } // namespace sio::sdr

#endif // ifndef sio_sdr_Types_h

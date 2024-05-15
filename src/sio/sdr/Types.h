#ifndef sio_sdr_Types_h
#define sio_sdr_Types_h

#include "util/PACKED.h"

#include <cstdint>
#include <string>
#include <string_view>

namespace sio { namespace sdr {

using bool_type = std::uint8_t;

using size_type = std::uint8_t;

using unsigned_int_type = std::uint16_t;

template<size_type MaxSize>
struct String {
  explicit String(const std::string& value) : m_size(static_cast<size_type>(value.copy(m_data, sizeof(m_data)))) {}
  std::string_view view() const { return std::string_view(m_data, m_size); }
private:
  size_type m_size;
  char m_data[MaxSize];
} PACKED;

template<typename PackedType, size_type MaxSize>
struct Array {
  Array() : m_size(0) {}
  bool full() const { return m_size == MaxSize; }
  void push_back(const PackedType& entry) { m_data[m_size++] = entry; }
  const PackedType* begin() const { return m_data; }
  const PackedType* end() const { return m_data + m_size; }
private:
  size_type m_size;
  PackedType m_data[MaxSize];
} PACKED;

} } // namespace sio::sdr

#endif // ifndef sio_sdr_Types_h

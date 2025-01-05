#ifndef util_PACKED_h
#define util_PACKED_h

#include <algorithm>
#include <cstddef>

#define PACKED __attribute__((packed))

namespace util {

template<typename T, std::size_t SizeV>
struct PackedArray {
  using value_type = T;
  using size_type = decltype(SizeV);

  static constexpr size_type size_v = SizeV;

  template<typename DestinationIteratorType>
  void copy(size_type maxCount, DestinationIteratorType destination) const {
    for (auto index = size_type{0}, end = std::min(size(), maxCount); index != end; ++index) {
      destination = m_elements[index];
    }
  }

  constexpr size_type size() const {
    return size_v;
  }

private:
  value_type m_elements[size_v];
} PACKED;

} // namespace util

#endif // ifndef util_PACKED_h

#ifndef util_type_traits_h
#define util_type_traits_h

#include <type_traits>

namespace util {

template<typename E>
constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
  return static_cast<typename std::underlying_type<E>::type>(e);
}

} // namespace util

#endif // ifndef util_type_traits_h

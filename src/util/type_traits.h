#ifndef util_type_traits_h
#define util_type_traits_h

#include <memory>
#include <type_traits>

namespace util {

template<typename E>
constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
  return static_cast<typename std::underlying_type<E>::type>(e);
}

template<typename T>
struct is_shared_ptr : std::false_type {
};

template<typename T>
struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {
};

template<typename T>
constexpr bool is_shared_ptr_v = is_shared_ptr<T>::value;

template<typename T>
struct remove_shared_ptr : std::type_identity<T> {
};

template<typename T>
struct remove_shared_ptr<std::shared_ptr<T>> : std::type_identity<T> {
};

template<typename T>
using remove_shared_ptr_t = typename remove_shared_ptr<T>::type;

} // namespace util

#endif // ifndef util_type_traits_h

#ifndef fs_Enumerator_h
#define fs_Enumerator_h

#include "util/type_traits.h"

#include <optional>
#include <type_traits>

namespace fs {

template<typename ContainerValueType>
class Enumerator {
public:
  using container_value_type = ContainerValueType;
  using container_type = std::conditional_t<util::is_shared_ptr_v<container_value_type>, util::remove_shared_ptr_t<container_value_type>, std::remove_pointer_t<container_value_type>>;
  static constexpr bool container_value_type_needs_dereference = util::is_shared_ptr_v<container_value_type> || std::is_pointer_v<ContainerValueType>;

  using entry_type = typename container_type::entry_type;
  using entry_enumeration_type = std::optional<entry_type>;
  using index_type = typename entry_type::index_type;

  explicit Enumerator(container_value_type&& container)
    : m_container(std::move(container)) {
    reset();
  }

  explicit Enumerator(const container_value_type& container)
    : m_container(container) {
    reset();
  }

  bool isValid() const {
    return m_currentEntry.has_value();
  }

  const entry_type& entry() const {
    return *m_currentEntry;
  }

  const container_value_type& container() const {
    return m_container;
  }

  container_value_type& container() {
    return m_container;
  }

  void reset() {
    if constexpr (container_value_type_needs_dereference) {
      m_currentEntry = m_container->begin();
    } else {
      m_currentEntry = m_container.begin();
    }
  }

  void next() {
    if constexpr (container_value_type_needs_dereference) {
      m_currentEntry = m_container->next(*m_currentEntry);
    } else {
      m_currentEntry = m_container.next(*m_currentEntry);
    }
  }

  void reposition(index_type index) {
    if (!isValid() || entry().index() > index) {
      reset();
    }
    while (isValid() && entry().index() != index) {
      next();
    }
  }

private:
  container_value_type m_container;

  entry_enumeration_type m_currentEntry;
};

} // namespace fs

#endif // ifndef fs_Enumerator_h

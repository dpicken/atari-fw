#include "Library.h"

const fs::builtin::Library* fs::builtin::Library::instance() {
  static const Library instance;
  return &instance;
}

fs::builtin::Library::entry_enumeration_type fs::builtin::Library::begin() const {
  entry_type::index_type index = 0;
  return entry_type{
    .path = s_paths[index],
    .size = s_sizes[index],
    .data = s_data,
    .index = index
  };
}

fs::builtin::Library::entry_enumeration_type fs::builtin::Library::next(const entry_type& entry) const {
  auto index = entry.index + 1;
  if (index == entryCount()) {
    return std::nullopt;
  }

  return entry_type {
    .path = s_paths[index],
    .size = s_sizes[index],
    .data = entry.data + entry.size,
    .index = index
  };
}

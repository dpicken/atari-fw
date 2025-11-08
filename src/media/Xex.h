#ifndef media_Xex_h
#define media_Xex_h

#include "XexEntry.h"

#include "fs/File.h"

#include <optional>

namespace media {

class Xex {
public:
  using size_type = ::fs::File::size_type;
  using sink_type = ::fs::File::sink_type;

  using entry_type = XexEntry;
  using entry_enumeration_type = std::optional<entry_type>;

  explicit Xex(::fs::File::ptr_type xexFile = nullptr);

  entry_enumeration_type begin();
  entry_enumeration_type next(const entry_type& entry);

  bool readSegmentData(const entry_type& entry, const sink_type& sink);

private:
  entry_enumeration_type readEntry(size_type offset, entry_type::index_type index);

  ::fs::File::ptr_type m_xexFile;
};

} // namespace media

#endif // ifndef media_Xex_h

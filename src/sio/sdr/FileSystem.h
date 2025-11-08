#ifndef sio_sdr_FileSystem_h
#define sio_sdr_FileSystem_h

#include "Types.h"

#include <limits>
#include <utility>

namespace sio { namespace sdr {

using CurrentDirPath = String<36>;

struct DirEntry {
  using name_type = String<36>;
  using index_type = unsigned_int_type;

  constexpr DirEntry() {
  }

#ifndef BUILD_MOS
  DirEntry(const std::string& name, bool isDirectory, unsigned int index)
    : m_name(name)
    , m_isDirectory(isDirectory)
    , m_index(static_cast<index_type>(index)) {
  }
#endif

  const name_type& name() const {
    return m_name;
  }

  bool isDirectory() const {
    return m_isDirectory;
  }

  index_type index() const {
    return m_index;
  }

private:
  name_type m_name;
  bool_type m_isDirectory;
  index_type m_index;
} PACKED;

struct DirEntryPage {
  using vector_type = Vector<DirEntry, 18>;
  using dir_entry_type = vector_type::value_type;

  static constexpr auto max_entry_count = vector_type::capacity_v;

  constexpr DirEntryPage()
    : m_eos(true) {
  }

  bool empty() const {
    return m_entries.empty();
  }

  bool full() const {
    return m_entries.full();
  }

  template<typename... Args>
  dir_entry_type& emplace_back(Args... args) {
    return m_entries.emplace_back(std::forward<Args>(args)...);
  }

  void setEOS(bool eos) {
    m_eos = eos;
  }

  const dir_entry_type* begin() const {
    return m_entries.begin();
  }

  const dir_entry_type* end() const {
    return m_entries.end();
  }

  const dir_entry_type& operator[](vector_type::size_type index) const {
    return m_entries[index];
  }

  bool eos() const {
    return m_eos;
  }

private:
  bool_type m_eos;
  vector_type m_entries;
} PACKED;

struct XexEntry {
#ifndef BUILD_MOS
  constexpr XexEntry()
    : m_eos(true)
    , m_segmentAddressBegin(0)
    , m_segmentAddressLast(0) {
  }
#endif

#ifndef BUILD_MOS
  XexEntry(std::uint16_t segmentAddressBegin, std::uint16_t segmentAddressLast)
    : m_eos(false)
    , m_segmentAddressBegin(segmentAddressBegin)
    , m_segmentAddressLast(segmentAddressLast) {
  }
#endif

  static constexpr XexEntry makeEOS() { return XexEntry{}; }

  bool eos() const {
    return m_eos;
  }

  std::uint16_t segmentAddressBegin() const {
    return m_segmentAddressBegin;
  }

  std::uint16_t segmentAddressLast() const {
    return m_segmentAddressLast;
  }

  std::uint16_t segmentDataSize() const {
    return (m_segmentAddressLast + 1) - m_segmentAddressBegin;
  }

private:
  bool_type m_eos;
  std::uint16_t m_segmentAddressBegin;
  std::uint16_t m_segmentAddressLast;
} PACKED;

} } // namespace sio::sdr

#endif // ifndef sio_sdr_FileSystem_h

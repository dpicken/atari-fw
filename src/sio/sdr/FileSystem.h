#ifndef sio_sdr_FileSystem_h
#define sio_sdr_FileSystem_h

#include "Types.h"

namespace sio { namespace sdr {

struct DirEntryInfo {
  using value_type = unsigned_int_type;

  explicit DirEntryInfo(value_type value) : m_value(value) {}

  DirEntryInfo(value_type index, bool isDirectory) {
    m_value = (index & IndexMask);
    if (isDirectory) {
      m_value |= IsDirectoryBit;
    }
  }

  value_type index() const { return m_value & IndexMask; }
  bool isDirectory() const { return m_value & IsDirectoryBit; }

private:
  static constexpr value_type IsDirectoryBit = 1U << ((sizeof(value_type) * 8) - 1);
  static constexpr value_type IndexMask = IsDirectoryBit - 1;
  value_type m_value;
} PACKED;

struct DirEntry {
  DirEntry(const std::string& name, const DirEntryInfo& info) : m_displayName(name), m_info(info) {}

  const std::string_view displayName() const { return m_displayName.view(); }

  const DirEntryInfo& info() const { return m_info; }
private:
  String<36> m_displayName;
  DirEntryInfo m_info;
} PACKED;

struct DirPage {
  bool full() const { return m_entries.full(); }
  void push_back(const DirEntry& entry) { m_entries.push_back(entry); }
  const DirEntry* begin() const { return m_entries.begin(); }
  const DirEntry* end() const { return m_entries.end(); }
  void setEOS() { m_eos = 1; }
  bool eos() const { return m_eos != 0; }
private:
  Array<DirEntry, 18> m_entries;
  std::uint8_t m_eos;
};

} } // namespace sio::sdr

#endif // ifndef sio_sdr_FileSystem_h

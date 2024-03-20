#ifndef sio_FileSystem_h
#define sio_FileSystem_h

#include "Device.h"

#include "util/PACKED.h"

#include <cstddef>
#include <cstdint>
#include <string_view>

namespace sio {

class FileSystem : public Device {
public:
  FileSystem(::hal::Uart uart, ::hal::BusyWait busyWait);

  void handle(const Command* command) override;

private:
  template<std::size_t MaxSize>
  struct String {
    std::string_view view() const { return std::string_view(m_data, m_size); }
  private:
    std::uint8_t m_size;
    char m_data[MaxSize];
  } PACKED;

  template<typename PackedType, std::size_t MaxSize>
  struct Array {
    const PackedType* begin() const { return m_data; }
    const PackedType* end() const { return m_data + m_size; }
    bool isFull() const { return m_size == MaxSize; }
  private:
    std::uint8_t m_size;
    PackedType m_data[MaxSize];
  } PACKED;

  struct DirEntryId {
    explicit DirEntryId(std::uint16_t id) : m_id(id) {}
    bool isDirectory() const { return m_id & 0x8000; }
    std::uint16_t index() const { return m_id & 0x7FFF; }
  private:
    std::uint16_t m_id;
  } PACKED;

  struct DirEntry {
    const DirEntryId& id() const { return m_id; }
    const std::string_view displayName() const { return m_displayName.view(); }
  private:
    DirEntryId m_id;
    String<22> m_displayName;
  } PACKED;

  struct DirPage {
    const DirEntry* begin() const { return m_entries.begin(); }
    const DirEntry* end() const { return m_entries.end(); }
    bool eof() const { return !m_entries.isFull(); }
  private:
    Array<DirEntry, 20> m_entries;
  } PACKED;

  void handleCurrentDirChangeUp();
  void handleCurrentDirChangeDown(DirEntryId dirEntryId);
  void handleCurrentDirSetFilter();

  void handleCurrentDirReadNextPage();
  void handleCurrentDirReadPreviousPage();

  void handleCurrentFileSet(DirEntryId dirEntryId);
  void handleCurrentFileApply(std::uint8_t deviceId);
};

} // namespace sio

#endif // ifndef sio_FileSystem_h

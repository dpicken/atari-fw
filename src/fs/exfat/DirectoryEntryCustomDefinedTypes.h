#ifndef fs_exfat_DirectoryEntryCustomDefinedTypes_h
#define fs_exfat_DirectoryEntryCustomDefinedTypes_h

#include "Configuration.h"
#include "FileAttributes.h"
#include "Unicode.h"

#include <array>
#include <cstdint>
#include <string>

#include "util/PACKED.h"

namespace fs { namespace exfat {

struct VolumeLabelDirectoryEntryCustomDefined {
  std::uint8_t characterCount() const {
    return m_characterCount;
  }

  const auto& volumeLabel() const {
    return m_volumeLabel;
  }

  std::string toAscii() const {
    std::string ascii;
    appendUnicode<Configuration::directory_entry_name_back_inserter_type>(volumeLabel(), characterCount(), ascii);
    return ascii;
  }

private:
  std::uint8_t m_characterCount;
  util::PackedArray<std::uint16_t, 11> m_volumeLabel;
  std::array<std::uint8_t, 8> m_reserved;
} PACKED;

struct FileDirectoryEntryCustomDefined {
  FileAttributes fileAttributes() const {
    return m_fileAttributes;
  }

private:
  FileAttributes m_fileAttributes;
  std::uint16_t m_reserved1;
  std::uint32_t m_createTimestamp;
  std::uint32_t m_lastModifiedTimestamp;
  std::uint32_t m_accessedTimestamp;
  std::uint8_t m_create10msIncrement;
  std::uint8_t m_lastModified10msIncrement;
  std::uint8_t m_createUtcOffset;
  std::uint8_t m_lastModifiedUtcOffset;
  std::uint8_t m_lastAccessedUtcOffset;
  std::array<std::uint8_t, 7> m_reserved2;
} PACKED;

struct StreamExtensionDirectoryEntryCustomDefined {
  std::uint8_t nameLength() const {
    return m_nameLength;
  }

  std::uint64_t validDataLength() const {
    return m_validDataLength;
  }

private:
  std::uint8_t m_reserved1;
  std::uint8_t m_nameLength;
  std::uint16_t m_nameHash;
  std::uint16_t m_reserved2;
  std::uint64_t m_validDataLength;
  std::array<std::uint8_t, 4> m_reserved3;
} PACKED;

struct FileNameDirectoryEntryCustomDefined {
  const auto& fileName() const {
    return m_fileName;
  }

private:
  util::PackedArray<std::uint16_t, 15> m_fileName;
} PACKED;

} } // namespace fs::exfat

#endif // ifndef fs_exfat_DirectoryEntryCustomDefinedTypes_h

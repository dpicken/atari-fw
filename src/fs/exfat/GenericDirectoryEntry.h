#ifndef fs_exfat_GenericDirectoryEntry_h
#define fs_exfat_GenericDirectoryEntry_h

#include <array>
#include <cstdint>
#include <string>

#include "util/PACKED.h"

namespace fs { namespace exfat {

struct EntryType {
  bool isEndOfDirectory() const {
    return m_value == 0x00;
  }

  bool isInUse() const {
    return m_value >= 0x81;
  }

  unsigned int typeCode() const {
    return (m_value >> 0) & 0x1F;
  }

  enum class TypeImportance { Critical, Benign };

  TypeImportance typeImportance() const {
    return TypeImportance{(m_value >> 5) & 0x01};
  }

  bool isCritical() const {
    return typeImportance() == TypeImportance::Critical;
  }

  bool isBenign() const {
    return typeImportance() == TypeImportance::Benign;
  }

  enum class TypeCategory { Primary, Secondary };

  TypeCategory typeCategory() const {
    return TypeCategory{(m_value >> 6) & 0x01};
  }

  bool isPrimary() const {
    return typeCategory() == TypeCategory::Primary;
  }

  bool isSecondary() const {
    return typeCategory() == TypeCategory::Secondary;
  }

  bool isVolumeLabel() const {
    return isCritical() && isPrimary() && typeCode() == 3;
  }

private:
  std::uint8_t m_value;
} PACKED;
static_assert(sizeof(EntryType) == 1);

struct GenericPrimaryDirectoryEntry {
  EntryType m_entryType;
  std::uint8_t m_secondaryCount;
  std::uint16_t m_setChecksum;
  std::uint16_t m_generalPrimaryFlags;
  std::array<std::uint8_t, 14> m_customDefined;
  std::uint32_t m_firstCluster;
  std::uint64_t m_dataLength;
} PACKED;
static_assert(sizeof(GenericPrimaryDirectoryEntry) == 32);

struct GenericSecondaryDirectoryEntry {
  EntryType m_entryType;
  std::uint8_t m_generalSecondaryFlags;
  std::array<std::uint8_t, 18> m_customDefined;
  std::uint32_t m_firstCluster;
  std::uint64_t m_dataLength;
} PACKED;
static_assert(sizeof(GenericSecondaryDirectoryEntry) == 32);

struct VolumeLabelDirectoryEntry {
  std::string ascii(char nonAsciiReplacementChar = '#') const {
    std::string str;
    for (auto index = 0U; index != m_characterCount; ++index) {
      if ((m_volumeLabel[index] > 0x7F) == 0) {
        str += m_volumeLabel[index] & 0x7F;
      } else {
        str += nonAsciiReplacementChar;
      }
    }
    return str;
  }
private:
  EntryType m_entryType;
  std::uint8_t m_characterCount;
  std::array<std::uint16_t, 11> m_volumeLabel;
  std::array<std::uint8_t, 8> m_reserved;
} PACKED;
static_assert(sizeof(VolumeLabelDirectoryEntry) == 32);

struct GenericDirectoryEntry {
  const EntryType& entryType() const {
    return m_entryType;
  }

  const GenericPrimaryDirectoryEntry* toGenericPrimary() const {
    return reinterpret_cast<const GenericPrimaryDirectoryEntry*>(
        m_entryType.isPrimary() ? this : nullptr);
  }

  const GenericSecondaryDirectoryEntry* toGenericSecondary() const {
    return reinterpret_cast<const GenericSecondaryDirectoryEntry*>(
        m_entryType.isSecondary() ? this : nullptr);
  }

  const VolumeLabelDirectoryEntry* toVolumeLabel() const {
    return reinterpret_cast<const VolumeLabelDirectoryEntry*>(
        m_entryType.isVolumeLabel() ? this : nullptr);
  }

private:
  EntryType m_entryType;
  std::array<std::uint8_t, 19> m_customDefined;
  std::uint32_t m_firstCluster;
  std::uint64_t m_dataLength;
} PACKED;
static_assert(sizeof(GenericDirectoryEntry) == 32);

} } // namespace fs::exfat

#endif // ifndef fs_exfat_GenericDirectoryEntry_h

#ifndef fs_exfat_GenericDirectoryEntry_h
#define fs_exfat_GenericDirectoryEntry_h

#include <array>
#include <cstdint>

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

  enum class TypeCategory { Primary, Secondary };

  TypeCategory typeCategory() const {
    return TypeCategory{(m_value >> 6) & 0x01};
  }

private:
  std::uint8_t m_value;
} PACKED;

using GenericDirectoryEntryCustomDefinedTypeWithAllocation = std::array<std::uint8_t, 19>;
using GenericDirectoryEntryCustomDefinedTypeWithoutAllocation = std::array<std::uint8_t, 31>;

template<typename CustomDefinedType = GenericDirectoryEntryCustomDefinedTypeWithAllocation, bool HasAllocation = sizeof(CustomDefinedType) == sizeof(GenericDirectoryEntryCustomDefinedTypeWithAllocation)>
struct GenericDirectoryEntry;

template<typename CustomDefinedType>
struct GenericDirectoryEntry<CustomDefinedType, false> {
  const EntryType& entryType() const {
    return m_entryType;
  }

  const CustomDefinedType& customDefined() const {
    return m_customDefined;
  }

private:
  EntryType m_entryType;
  CustomDefinedType m_customDefined;
} PACKED;

template<typename CustomDefinedType>
struct GenericDirectoryEntry<CustomDefinedType, true> : GenericDirectoryEntry<CustomDefinedType, false> {
  std::uint32_t firstCluster() const {
    return m_firstCluster;
  }

  std::uint64_t dataLength() const {
    return m_dataLength;
  }

private:
  std::uint32_t m_firstCluster;
  std::uint64_t m_dataLength;
} PACKED;

static_assert(sizeof(GenericDirectoryEntry<GenericDirectoryEntryCustomDefinedTypeWithAllocation>) == 32);
static_assert(sizeof(GenericDirectoryEntry<GenericDirectoryEntryCustomDefinedTypeWithoutAllocation>) == 32);

} } // namespace fs::exfat

#endif // ifndef fs_exfat_GenericDirectoryEntry_h

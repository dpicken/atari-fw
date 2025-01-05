#ifndef fs_exfat_GenericDirectoryEntryCustomDefinedTypes_h
#define fs_exfat_GenericDirectoryEntryCustomDefinedTypes_h

#include <array>
#include <cstdint>

#include "util/PACKED.h"

namespace fs { namespace exfat {

struct GenericPrimaryDirectoryEntrySecondaryCount {
  std::uint8_t secondaryCount() const {
    return m_secondaryCount;
  }

private:
  std::uint8_t m_secondaryCount;
} PACKED;

struct GenericPrimaryDirectoryEntrySetChecksum {
  std::uint16_t setChecksum() const {
    return m_setChecksum;
  }

private:
  std::uint16_t m_setChecksum;
} PACKED;

struct GenericPrimaryDirectoryEntryGeneralPrimaryFlags {
  bool allocationPossible() const {
    return m_generalPrimaryFlags & 0x0001;
  }

  bool noFatChain() const {
    return m_generalPrimaryFlags & 0x0002;
  }

private:
  std::uint16_t m_generalPrimaryFlags;
} PACKED;

template<typename CustomDefinedType>
struct GenericPrimaryDirectoryEntryCustomDefinedField {
  const CustomDefinedType& customDefined() const {
    return m_customDefined;
  }

private:
  CustomDefinedType m_customDefined;
} PACKED;

template<typename CustomDefinedType, std::size_t OverrideSizeV>
struct GenericPrimaryDirectoryEntryCustomDefinedBase;

template<typename CustomDefinedType>
struct GenericPrimaryDirectoryEntryCustomDefinedBase<CustomDefinedType, 0>
  : GenericPrimaryDirectoryEntrySecondaryCount
  , GenericPrimaryDirectoryEntrySetChecksum
  , GenericPrimaryDirectoryEntryGeneralPrimaryFlags
  , GenericPrimaryDirectoryEntryCustomDefinedField<CustomDefinedType> {
} PACKED;

template<typename CustomDefinedType>
struct GenericPrimaryDirectoryEntryCustomDefinedBase<CustomDefinedType, sizeof(GenericPrimaryDirectoryEntryGeneralPrimaryFlags)>
  : GenericPrimaryDirectoryEntrySecondaryCount
  , GenericPrimaryDirectoryEntrySetChecksum
  , GenericPrimaryDirectoryEntryCustomDefinedField<CustomDefinedType> {
} PACKED;

template<typename CustomDefinedType>
struct GenericPrimaryDirectoryEntryCustomDefinedBase<CustomDefinedType, sizeof(GenericPrimaryDirectoryEntryGeneralPrimaryFlags) + sizeof(GenericPrimaryDirectoryEntrySetChecksum)>
  : GenericPrimaryDirectoryEntrySecondaryCount
  , GenericPrimaryDirectoryEntryCustomDefinedField<CustomDefinedType> {
} PACKED;

template<typename CustomDefinedType>
struct GenericPrimaryDirectoryEntryCustomDefinedBase<CustomDefinedType, sizeof(GenericPrimaryDirectoryEntryGeneralPrimaryFlags) + sizeof(GenericPrimaryDirectoryEntrySetChecksum) + sizeof(GenericPrimaryDirectoryEntrySecondaryCount)>
  : GenericPrimaryDirectoryEntryCustomDefinedField<CustomDefinedType> {
} PACKED;

static constexpr auto gpde_overrideable_fields_size = sizeof(GenericPrimaryDirectoryEntryGeneralPrimaryFlags) + sizeof(GenericPrimaryDirectoryEntrySetChecksum) + sizeof(GenericPrimaryDirectoryEntrySecondaryCount);

template<typename CustomDefinedType, bool HasAllocationV>
struct gpdecd_override_size;

template<typename CustomDefinedType>
struct gpdecd_override_size<CustomDefinedType, true>
  : std::integral_constant<std::size_t, sizeof(CustomDefinedType) - (sizeof(GenericDirectoryEntryCustomDefinedTypeWithAllocation) - gpde_overrideable_fields_size)> {
};

template<typename CustomDefinedType>
struct gpdecd_override_size<CustomDefinedType, false>
  : std::integral_constant<std::size_t, sizeof(CustomDefinedType) - (sizeof(GenericDirectoryEntryCustomDefinedTypeWithoutAllocation) - gpde_overrideable_fields_size)> {
};

template<typename CustomDefinedType, bool HasAllocationV>
static constexpr auto gpdecd_override_size_v = gpdecd_override_size<CustomDefinedType, HasAllocationV>::value;

template<typename CustomDefinedType, bool HasAllocationV>
using GenericPrimaryDirectoryEntryCustomDefined = GenericPrimaryDirectoryEntryCustomDefinedBase<CustomDefinedType, gpdecd_override_size_v<CustomDefinedType, HasAllocationV>>;

template<typename CustomDefinedType>
struct GenericSecondaryDirectoryEntryCustomDefined {
  bool allocationPossible() const {
    return m_generalSecondaryFlags & 0x0001;
  }

  bool noFatChain() const {
    return m_generalSecondaryFlags & 0x0002;
  }

  const CustomDefinedType& customDefined() const {
    return m_customDefined;
  }

private:
  std::uint8_t m_generalSecondaryFlags;
  CustomDefinedType m_customDefined;
} PACKED;

} } // namespace fs::exfat

#endif // ifndef fs_exfat_GenericDirectoryEntryCustomDefinedTypes_h

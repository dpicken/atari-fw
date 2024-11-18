#ifndef fs_exfat_DirectoryEntryTypes_h
#define fs_exfat_DirectoryEntryTypes_h

#include "DirectoryEntryCustomDefinedTypes.h"
#include "GenericDirectoryEntryTypes.h"

namespace fs { namespace exfat {

using VolumeLabelDirectoryEntry
    = GenericPrimaryDirectoryEntry<EntryType::TypeImportance::Critical, 3, VolumeLabelDirectoryEntryCustomDefined, false>;
using FileDirectoryEntry
    = GenericPrimaryDirectoryEntry<EntryType::TypeImportance::Critical, 5, FileDirectoryEntryCustomDefined, false>;

using StreamExtensionDirectoryEntry
    = GenericSecondaryDirectoryEntry<EntryType::TypeImportance::Critical, 0, StreamExtensionDirectoryEntryCustomDefined, true>;
using FileNameDirectoryEntry
    = GenericSecondaryDirectoryEntry<EntryType::TypeImportance::Critical, 1, FileNameDirectoryEntryCustomDefined, false>;

struct UntypedDirectoryEntry : GenericDirectoryEntry<> {
  template<typename DirectoryEntryType>
  const DirectoryEntryType* to() const {
    static_assert(sizeof(DirectoryEntryType) == sizeof(*this));
    if (DirectoryEntryType::type_category_v != entryType().typeCategory()) {
      return nullptr;
    }
    if (DirectoryEntryType::type_importance_v != entryType().typeImportance()) {
      return nullptr;
    }
    if (DirectoryEntryType::type_code_v != entryType().typeCode()) {
      return nullptr;
    }
    return reinterpret_cast<const DirectoryEntryType*>(this);
  }

  template<typename DirectoryEntryType>
  auto toCustomDefined() const {
    auto typed = to<DirectoryEntryType>();
    return (typed != nullptr) ? &typed->customDefined().customDefined() : nullptr;
  }
} PACKED;

} } // namespace fs::exfat

#endif // ifndef fs_exfat_DirectoryEntryTypes_h

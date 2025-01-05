#ifndef fs_exfat_GenericDirectoryEntryTypes_h
#define fs_exfat_GenericDirectoryEntryTypes_h

#include "GenericDirectoryEntry.h"
#include "GenericDirectoryEntryCustomDefinedTypes.h"

namespace fs { namespace exfat {

template<EntryType::TypeCategory TypeCategoryV, EntryType::TypeImportance TypeImportanceV, unsigned int TypeCodeV, typename GenericDirectoryEntryType>
struct TypedGenericDirectoryEntry : GenericDirectoryEntryType {
  static constexpr auto type_category_v = TypeCategoryV;
  static constexpr auto type_importance_v = TypeImportanceV;
  static constexpr auto type_code_v = TypeCodeV;
} PACKED;

template<EntryType::TypeImportance TypeImportanceV, unsigned int TypeCodeV, typename CustomDefinedType, bool HasAllocationV>
using GenericPrimaryDirectoryEntry = TypedGenericDirectoryEntry<EntryType::TypeCategory::Primary, TypeImportanceV, TypeCodeV, GenericDirectoryEntry<GenericPrimaryDirectoryEntryCustomDefined<CustomDefinedType, HasAllocationV>, HasAllocationV>>;

template<EntryType::TypeImportance TypeImportanceV, unsigned int TypeCodeV, typename CustomDefinedType, bool HasAllocationV>
using GenericSecondaryDirectoryEntry = TypedGenericDirectoryEntry<EntryType::TypeCategory::Secondary, TypeImportanceV, TypeCodeV, GenericDirectoryEntry<GenericSecondaryDirectoryEntryCustomDefined<CustomDefinedType>, HasAllocationV>>;

} } // namespace fs::exfat

#endif // ifndef fs_exfat_GenericDirectoryEntryTypes_h

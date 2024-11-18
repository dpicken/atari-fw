#include "LogicalDirectory.h"

fs::exfat::LogicalDirectory::LogicalDirectory(PhysicalDirectory&& directory)
  : m_enumerator(std::move(directory)) {
}

fs::exfat::LogicalDirectory::entry_enumeration_type fs::exfat::LogicalDirectory::begin() {
  m_enumerator.reset();
  return findNonExcluded(0);
}

fs::exfat::LogicalDirectory::entry_enumeration_type fs::exfat::LogicalDirectory::next(const entry_type& entry) {
  m_enumerator.reposition(entry.endPhysicalIndex());
  return findNonExcluded(entry.standard().index() + 1);
}

fs::exfat::LogicalDirectory::entry_enumeration_type fs::exfat::LogicalDirectory::findNonExcluded(entry_type::logical_index_type logicalIndex) {
  for (auto entry = find(logicalIndex); m_enumerator.isValid(); entry = find(logicalIndex)) {
    auto fileAttributes = entry->fileAttributes();
    if (fileAttributes.isSet(FileAttributes::Flag::Hidden) && Configuration::instance()->excludeHiddenDirectoryEntries()) {
      continue;
    }
    if (fileAttributes.isSet(FileAttributes::Flag::System) && Configuration::instance()->excludeSystemDirectoryEntries()) {
      continue;
    }
    if (entry->standard().name()[0] == '.' && Configuration::instance()->excludeUnixHiddenDirectoryEntries()) {
      continue;
    }
    return entry;
  }
  return std::nullopt;
}

fs::exfat::LogicalDirectory::entry_enumeration_type fs::exfat::LogicalDirectory::find(entry_type::logical_index_type logicalIndex) {
  auto fileDirectoryEntry = ::fs::exfat::find<FileDirectoryEntry>(m_enumerator);
  if (!fileDirectoryEntry) {
    return std::nullopt;
  }

  auto fileAttributes = fileDirectoryEntry->customDefined().customDefined().fileAttributes();

  ::fs::DirectoryEntry::Type type = fileAttributes.isSet(FileAttributes::Flag::Directory)
      ? ::fs::DirectoryEntry::Type::Directory
      : ::fs::DirectoryEntry::Type::File;

  auto streamExtensionEntry = ::fs::exfat::next<StreamExtensionDirectoryEntry>(m_enumerator);
  if (streamExtensionEntry == nullptr) {
    return std::nullopt;
  }

  if (!streamExtensionEntry->customDefined().allocationPossible()) {
    return std::nullopt;
  }

  auto clusterAllocation = ClusterAllocation(streamExtensionEntry->firstCluster(), streamExtensionEntry->customDefined().customDefined().validDataLength(), streamExtensionEntry->customDefined().noFatChain());

  auto nameLength = streamExtensionEntry->customDefined().customDefined().nameLength();
  if (nameLength == 0) {
    return std::nullopt;
  }

  auto lastPhysicalIndex = m_enumerator.entry().index();

  std::string name;
  name.reserve(nameLength);
  while (name.size() != nameLength) {
    const auto fileNameEntry = ::fs::exfat::next<FileNameDirectoryEntry>(m_enumerator);
    if (fileNameEntry == nullptr) {
      return std::nullopt;
    }
    appendUnicode<Configuration::directory_entry_name_back_inserter_type>(fileNameEntry->customDefined().customDefined().fileName(), nameLength - name.size(), name);
    lastPhysicalIndex = m_enumerator.entry().index();
  }

  return LogicalDirectoryEntry({std::move(name), type, logicalIndex}, fileAttributes, clusterAllocation, lastPhysicalIndex + 1);
}

fs::exfat::PhysicalDirectory* fs::exfat::LogicalDirectory::physicalDirectory() {
  return &m_enumerator.container();
}

#include "GPT.h"

#include "fs/FileSlice.h"
#include "fs/ram/Directory.h"
#include "util/PACKED.h"

#include <algorithm>
#include <array>
#include <cstdint>

namespace {

struct GPT {
  enum class TableType {
    Primary,
    Backup
  };

  struct header_type {
    static constexpr unsigned int max_supported_partition_count_v = 16;

    template<TableType TableTypeV>
    bool read(const ::fs::File::ptr_type& device) {
      (void)device;
      return false;
    }

    bool isValid(const ::fs::File::ptr_type& device) const {
      (void)device;
      return false;
    }

    unsigned int partitionCount() const {
      return std::min(numberOfPartitionEntries, max_supported_partition_count_v);
    }

  private:
    std::array<char, 8> signature;
    std::uint32_t revision;
    std::uint32_t headerSize;
    std::uint32_t headerCRC32;
    std::uint32_t reserved;
    std::uint64_t myLBA;
    std::uint64_t alternateLBA;
    std::uint64_t firstUsableLBA;
    std::uint64_t lastUsableLBA;
    std::array<std::uint8_t, 16> diskGUID;
    std::uint64_t partitionEntryLBA;
    std::uint32_t numberOfPartitionEntries;
    std::uint32_t sizeOfPartitionEntry;
    std::uint32_t partitionEntryArrayCRC32;
  } PACKED;
  static_assert(sizeof(header_type) == 92);

  struct partition_entry_type {
    bool read(const ::fs::File::ptr_type& device, const header_type& header, unsigned int index) {
      (void)device; (void)header; (void)index;
      return false;
    }

    bool isExFat() const {
      return false;
    }

    std::uint64_t offset(const ::fs::File::ptr_type& device) const {
      return device->blockSize().blockAddressToByteOffset(startingLBA);
    }

    std::uint64_t size(const ::fs::File::ptr_type& device) const {
      return device->blockSize().blockCountToByteCount((endingLBA + 1) - startingLBA);
    }

    std::string name() const {
      return "";
    }

  private:
    std::array<std::uint8_t, 16> partitionTypeGUID;
    std::array<std::uint8_t, 16> uniquePartitionGUID;
    std::uint64_t startingLBA;
    std::uint64_t endingLBA;
    std::uint64_t attributes;
    std::array<std::uint8_t, 72> partitionName;
  } PACKED;
  static_assert(sizeof(partition_entry_type) == 128);
};

} // namespace

fs::Directory::ptr_type fs::gpt::tryMakeDirectory(const ::fs::File::ptr_type& device) {
  GPT::header_type header;
  if (!header.read<GPT::TableType::Primary>(device)) {
    return nullptr;
  }

  if (!header.isValid(device)) {
    return nullptr;
  }

  auto directory = ::fs::ram::Directory::make();
  for (auto index = 0U; index != header.partitionCount(); ++index) {
    GPT::partition_entry_type partitionEntry;
    if (!partitionEntry.read(device, header, index)) {
      continue;
    }

    if (!partitionEntry.isExFat()) {
      continue;
    }

    auto partition = FileSlice::tryMake(device, partitionEntry.offset(device), partitionEntry.size(device));
    if (partition == nullptr) {
      continue;
    }

    directory->put(partitionEntry.name(), std::move(partition));
  }

  return directory;
}

#include "GPT.h"

#include "fs/FileSlice.h"
#include "fs/ram/Directory.h"
#include "util/hex.h"
#include "util/PACKED.h"

#include <array>
#include <cstdint>

namespace {

struct GPT {
  struct header_base_type {
    unsigned int partitionCount() const {
      return numberOfPartitionEntries;
    }

    std::uint64_t partitionIndexToByteOffset(const ::fs::File::ptr_type& device, std::uint32_t index) const {
      return device->blockSize().blockAddressToByteOffset(partitionEntryLBA) + (index * sizeOfPartitionEntry);
    }

  protected:
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
  static_assert(sizeof(header_base_type) == 92);

  enum class PartitionTable {
    Primary,
    Backup
  };

  template<PartitionTable PartitionTableV = PartitionTable::Primary>
  struct header_type : header_base_type {
    static constexpr auto partition_table_v = PartitionTableV;

    static constexpr decltype(signature) signature_v{'E', 'F', 'I', ' ', 'P', 'A', 'R', 'T'};
    static constexpr std::uint32_t revision_1_0_v = 0x00010000;
    static constexpr unsigned int max_supported_partition_count_v = 128;

    bool read(const ::fs::File::ptr_type& device) {
      return device->read(device->blockSize().blockAddressToByteOffset(calculateMyLBA(device)), this);
    }

    bool isValid(const ::fs::File::ptr_type& device) const {
      return signature == signature_v
          && revision == revision_1_0_v
          && headerSize >= sizeof(*this) && headerSize <= device->blockSize().byteCount()
          && headerCRC32 == calculateHeaderCRC32()
          && myLBA == calculateMyLBA(device)
          && numberOfPartitionEntries <= max_supported_partition_count_v
          && sizeOfPartitionEntry <= device->blockSize().byteCount()
          && partitionEntryArrayCRC32 == calculatePartitionEntryArrayCRC32();
    }

  private:
    std::uint32_t calculateHeaderCRC32() const {
      // TODO: Implement.
      return headerCRC32;
    }

    std::uint64_t calculateMyLBA(const ::fs::File::ptr_type& device) const {
      if constexpr(partition_table_v == PartitionTable::Primary) {
        return 1U;
      } else {
        static_assert(partition_table_v == PartitionTable::Backup);
        return device->blockSize().blockCountToByteCount(device->size()) - 1U;
      }
    }

    std::uint32_t calculatePartitionEntryArrayCRC32() const {
      // TODO: Implement.
      return partitionEntryArrayCRC32;
    }
  };

  struct partition_entry_type {
    using guid_type = std::array<std::uint8_t, 16>;

    static constexpr guid_type partition_type_microsoft_basic_data{0xA2, 0xA0, 0xD0, 0xEB, 0xE5, 0xB9, 0x33, 0x44, 0x87, 0xC0, 0x68, 0xB6, 0xB7, 0x26, 0x99, 0xC7};

    bool read(const ::fs::File::ptr_type& device, const header_base_type& header, unsigned int index) {
      return device->read(header.partitionIndexToByteOffset(device, index), this);
    }

    bool isPartitionTypeMicrosoftBasicData() const {
      return partitionTypeGUID == partition_type_microsoft_basic_data;
    }

    std::uint64_t offset(const ::fs::File::ptr_type& device) const {
      return device->blockSize().blockAddressToByteOffset(startingLBA);
    }

    std::uint64_t size(const ::fs::File::ptr_type& device) const {
      return device->blockSize().blockCountToByteCount((endingLBA + 1) - startingLBA);
    }

    std::string name() const {
      return util::toHexString(uniquePartitionGUID.begin(), uniquePartitionGUID.end());
    }

  private:
    guid_type partitionTypeGUID;
    guid_type uniquePartitionGUID;
    std::uint64_t startingLBA;
    std::uint64_t endingLBA;
    std::uint64_t attributes;
    std::array<std::uint16_t, 36> partitionName;
  } PACKED;
  static_assert(sizeof(partition_entry_type) == 128);
};

} // namespace

fs::Directory::ptr_type fs::gpt::tryMakeDirectory(const ::fs::File::ptr_type& device) {
  GPT::header_type<GPT::PartitionTable::Primary> header;
  if (!header.read(device)) {
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

    if (!partitionEntry.isPartitionTypeMicrosoftBasicData()) {
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

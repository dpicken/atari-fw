#include "MBR.h"

#include "fs/FileSlice.h"
#include "fs/ram/Directory.h"
#include "util/PACKED.h"

#include <array>
#include <cstdint>

namespace {

struct MBR {
  struct partition_entry_type {
    bool isPartitionTypeMicrosoft() const {
      return partitionType == 0x07;
    }

    std::uint64_t offset(const ::fs::File::ptr_type& device) const {
      return device->blockSize().blockAddressToByteOffset(firstSectorLBA);
    }

    std::uint64_t size(const ::fs::File::ptr_type& device) const {
      return device->blockSize().blockCountToByteCount(sectorCount);
    }

  private:
    using chs_address_type = std::array<std::uint8_t, 3>;

    std::uint8_t status;
    chs_address_type firstSector;
    std::uint8_t partitionType;
    chs_address_type lastSector;
    std::uint32_t firstSectorLBA;
    std::uint32_t sectorCount;
  } PACKED;

  using partition_table_type = std::array<partition_entry_type, 4>;

  bool isValid() const {
    return bootSignature0 == 0x55 && bootSignature1 == 0xAA;
  }

  const partition_table_type& partitionTable() const {
    return partitionEntries;
  }

private:
  std::array<std::uint8_t, 446> bootstrapCode;
  partition_table_type partitionEntries;
  std::uint8_t bootSignature0;
  std::uint8_t bootSignature1;
} PACKED;
static_assert (sizeof(MBR) == 512);

} // namespace

fs::Directory::ptr_type fs::mbr::tryMakeDirectory(const ::fs::File::ptr_type& device) {
  MBR mbr;
  if (!device->read(0, &mbr)) {
    return nullptr;
  }

  if (!mbr.isValid()) {
    return nullptr;
  }

  auto directory = ::fs::ram::Directory::make();

  for (auto index = 0U; index != mbr.partitionTable().size(); ++index) {
    const auto& partitionEntry = mbr.partitionTable()[index];
    if (!partitionEntry.isPartitionTypeMicrosoft()) {
      continue;
    }

    auto partition = FileSlice::tryMake(device, partitionEntry.offset(device), partitionEntry.size(device));
    if (partition == nullptr) {
      continue;
    }

    directory->put(Directory::name_type(1, static_cast<char>('0' + index)), std::move(partition));
  }

  return directory;
}

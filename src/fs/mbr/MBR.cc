#include "MBR.h"

#include "fs/FileSlice.h"
#include "fs/ram/Directory.h"
#include "util/PACKED.h"

#include <array>
#include <cstdint>

namespace {

struct MBR {
  struct partition_entry_type {
    using chs_address_type = std::array<std::uint8_t, 3>;
    std::uint8_t status;
    chs_address_type firstSector;
    std::uint8_t partitionType;
    chs_address_type lastSector;
    std::uint32_t firstSectorLBA;
    std::uint32_t sectorCount;
    bool isExFat() const {
      return partitionType == 0x07;
    }
    std::uint64_t offset() const {
      return firstSectorLBA * sizeof(MBR);
    }
    std::uint64_t size() const {
      return sectorCount * sizeof(MBR);
    }
  };
  static constexpr unsigned int partition_entry_count_v = 4;

  bool isValid() const {
    return bootSignature0 == 0x55 && bootSignature1 == 0xAA;
  }

  partition_entry_type partitionEntry(unsigned int index) const {
    return partitionEntries[index];
  }

private:
  std::array<std::uint8_t, 446> bootstrapCode;
  partition_entry_type partitionEntries[partition_entry_count_v];
  std::uint8_t bootSignature0;
  std::uint8_t bootSignature1;
} PACKED;
static_assert (sizeof(MBR) == 512);

} // namespace

fs::Directory::ptr_type fs::mbr::tryMakeDirectory(const ::fs::File::ptr_type& disk) {
  MBR mbr;
  if (!disk->read(0, &mbr)) {
    return nullptr;
  }

  if (!mbr.isValid()) {
    return nullptr;
  }

  auto directory = ::fs::ram::Directory::make();
  for (auto index = 0U; index != MBR::partition_entry_count_v; ++index) {
    auto partitionEntry = mbr.partitionEntry(index);

    if (!partitionEntry.isExFat()) {
      continue;
    }

    auto partition = FileSlice::make(disk, partitionEntry.offset(), partitionEntry.size());
    if (partition == nullptr) {
      continue;
    }

    directory->put(Directory::name_type(1, static_cast<char>('0' + index)), std::move(partition));
  }

  return directory;
}

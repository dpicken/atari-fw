#ifndef fs_exfat_BootSector_h
#define fs_exfat_BootSector_h

#include "fs/BlockSize.h"
#include "fs/File.h"
#include "fs/FileSlice.h"

#include <array>
#include <cstdint>

namespace fs { namespace exfat {

struct BootSector {
  using file_system_name_type = std::array<char, 8>;

  bool isSupported() const {
    return bootSignature == 0xAA55
        && fileSystemName == file_system_name_type{'E', 'X', 'F', 'A', 'T', ' ', ' ', ' '}
        && fileSystemRevision[1] == 1 && fileSystemRevision[0] == 0;
  }

  bool isValid(const ::fs::File::ptr_type& volume) const {
    //const auto* const partition = dynamic_cast<::fs::FileSlice*>(volume.get());
    const ::fs::FileSlice* const partition = nullptr;
    const auto sectorSize = volume->blockSize();
    const auto clusterSize = BlockSize::fromSizeLog2(sectorSize.sizeLog2() + sectorsPerClusterShift);
    return isSupported()
        && bytesPerSectorShift == sectorSize.sizeLog2()
        && sectorsPerClusterShift <= (25 - bytesPerSectorShift)
        && (partitionOffset == 0 || partition == nullptr || sectorSize.blockAddressToByteOffset(partitionOffset) == partition->offset())
        && sectorSize.blockCountToByteCount(volumeLength) <= volume->size()
        && fatOffset >= 24 && fatOffset <= (clusterHeapOffset - (fatLength * numberOfFats))
        && fatLength <= ((clusterHeapOffset - fatOffset) / numberOfFats)
        && clusterHeapOffset >= (fatOffset + (fatLength * numberOfFats)) && sectorSize.blockCountToByteCount(clusterHeapOffset) <= (sectorSize.blockCountToByteCount(volumeLength) - clusterSize.blockCountToByteCount(clusterCount))
        && clusterSize.blockCountToByteCount(clusterCount) <= sectorSize.blockCountToByteCount(volumeLength - clusterHeapOffset)
        && firstClusterOfRootDirectory >= 2 && firstClusterOfRootDirectory <= (clusterCount + 1)
        && numberOfFats >= 1 && numberOfFats <= 2;
  }

  bool activeFatBit() const {
    return volumeFlags & 0x0001;
  }

  std::uint32_t activeFatOffset() const {
    return activeFatBit() == false ? fatOffset : fatOffset + fatLength;
  }

  BlockSize clusterSize(const ::fs::File::ptr_type& volume) const {
    return BlockSize::fromSizeLog2(volume->blockSize().sizeLog2() + sectorsPerClusterShift);
  }

  std::array<std::uint8_t, 3> jumpBoot;
  file_system_name_type fileSystemName;
  std::array<std::uint8_t, 53> mustBeZero;
  std::uint64_t partitionOffset;
  std::uint64_t volumeLength;
  std::uint32_t fatOffset;
  std::uint32_t fatLength;
  std::uint32_t clusterHeapOffset;
  std::uint32_t clusterCount;
  std::uint32_t firstClusterOfRootDirectory;
  std::uint32_t volumeSerialNumber;
  std::array<std::uint8_t, 2> fileSystemRevision;
  std::uint16_t volumeFlags;
  std::uint8_t bytesPerSectorShift;
  std::uint8_t sectorsPerClusterShift;
  std::uint8_t numberOfFats;
  std::uint8_t driveSelect;
  std::uint8_t percentInUse;
  std::array<std::uint8_t, 7> reserved;
  std::array<std::uint8_t, 390> bootCode;
  std::uint16_t bootSignature;
  //std::array<std::uint8_t, volume-block-size - 512> excessSpace;
};
static_assert (sizeof(BootSector) == 512);

} } // namespace fs::exfat

#endif // ifndef fs_exfat_BootSector_h

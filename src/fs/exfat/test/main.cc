#include "fs/exfat/BootSector.h"
#include "fs/exfat/Volume.h"

#include "fs/FileSlice.h"
#include "fs/mbr/MBR.h"
#include "fs/test/common/File.h"
#include "fs/test/common/images.h"

#include <cstdlib>
#include <stdexcept>

namespace {

void testBootSector() {
  auto blockSize = fs::BlockSize::fromSizeLog2(9);
  auto device = fs::test::common::File::make(fs::test::common::sd_card_3921920_512B_sectors_mbr_exfat_boot_region_macos, blockSize.blockCountToByteCount(3'921'920), blockSize);
  auto partition = fs::FileSlice::tryMake(device, blockSize.blockAddressToByteOffset(2048), blockSize.blockCountToByteCount(3919872));

  if (partition == nullptr) {
    throw std::logic_error("failed to slice first partition");
  }

  fs::exfat::BootSector bootSector;
  if (!partition->read(0, &bootSector)) {
    throw std::logic_error("failed to read boot sector");
  }

  if (!bootSector.isSupported()) {
    throw std::logic_error("boot sector is-supported failed");
  }

  if (!bootSector.isValid(partition)) {
    throw std::logic_error("boot sector is-valid failed");
  }
}

void testVolume() {
  auto blockSize = fs::BlockSize::fromSizeLog2(9);
  auto device = fs::test::common::File::make(fs::test::common::sd_card_3921920_512B_sectors_mbr_exfat_boot_region_macos, blockSize.blockCountToByteCount(3'921'920), blockSize);
  auto partition = fs::FileSlice::tryMake(device, blockSize.blockAddressToByteOffset(2048), blockSize.blockCountToByteCount(3919872));

  fs::exfat::Volume volume(partition);

  auto activeFat = volume.tryGetActiveFat();
  if (!activeFat) {
    throw std::logic_error("failed to get active fat");
  }

  if (activeFat->size() != blockSize.blockCountToByteCount(512)) {
    throw std::logic_error("unexpected fat size");
  }

  auto clusterHeap = volume.tryGetClusterHeap();
  if (!clusterHeap) {
    throw std::logic_error("failed to get cluster heap");
  }

  if (clusterHeap->size() != blockSize.blockCountToByteCount(3919232)) {
    throw std::logic_error("unexpected cluster heap size");
  }
}

} // namespace

int main(int, char**) {
  testBootSector();
  testVolume();
  return EXIT_SUCCESS;
}

#include "fs/exfat/BootSector.h"
#include "fs/exfat/ClusterChainEnumerator.h"
#include "fs/exfat/InternalDirectoryEnumerator.h"
#include "fs/exfat/Volume.h"

#include "fs/FileSlice.h"
#include "fs/mbr/MBR.h"
#include "fs/test/common/File.h"

#include <algorithm>
#include <cstdlib>
#include <cstdint>
#include <stdexcept>

namespace {

static constexpr std::uint8_t image[] = {
#include "fs/test/data/sd-card-3921920-512B-sectors-mbr_exfat_boot_and_fat_region_and_three_clusters-macos.data"
};
static constexpr auto blockSize = fs::BlockSize::fromSizeLog2(9);
static constexpr auto imageSize = blockSize.blockCountToByteCount(3'921'920);

void testBootSector() {
  auto device = fs::test::common::File::make(image, imageSize, blockSize);
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
  auto device = fs::test::common::File::make(image, imageSize, blockSize);
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

void testClusterChain() {
  auto device = fs::test::common::File::make(image, imageSize, blockSize);
  auto partition = fs::FileSlice::tryMake(device, blockSize.blockAddressToByteOffset(2048), blockSize.blockCountToByteCount(3919872));

  fs::exfat::Volume volume(partition);

  fs::exfat::ClusterChainEnumerator clusterChainEnumerator(volume.getRootDirectoryClusterChain());
  if (!clusterChainEnumerator.isValid()) {
    throw std::logic_error("failed to enumerator root directory cluster chain");
  }

  if (clusterChainEnumerator.entry().index() != 0) {
    throw std::logic_error("unexpected root directory cluster chain - invalid index");
  }

  if (clusterChainEnumerator.entry().lcn() != 4) {
    throw std::logic_error("unexpected root directory cluster chain - invalid logical cluster number");
  }

  clusterChainEnumerator.next();
  if (clusterChainEnumerator.isValid()) {
    throw std::logic_error("unexpected root directory cluster chain - chain size");
  }
}

void testFile() {
  auto device = fs::test::common::File::make(image, imageSize, blockSize);
  auto partition = fs::FileSlice::tryMake(device, blockSize.blockAddressToByteOffset(2048), blockSize.blockCountToByteCount(3919872));

  fs::exfat::Volume volume(partition);

  auto file = volume.tryGetRootDirectoryFile();
  if (!file) {
    throw std::logic_error("failed to get root directory file");
  }

  std::array<std::uint8_t, 32768> firstCluster;
  if (!file->read(0, &firstCluster)) {
    throw std::logic_error("failed to read root directory - first cluster");
  }

  if (firstCluster[0] != 0x83
      && firstCluster[1] != 0x0a
      && firstCluster[2] != 't'
      && firstCluster[3] != 0x00
      && firstCluster[4] != 'e'
      && firstCluster[5] != 0x00
      && firstCluster[6] != 's'
      && firstCluster[7] != 0x00
      && firstCluster[8] != 't'
      && firstCluster[9] != 0x00
      && firstCluster[10] != '-'
      && firstCluster[11] != 0x00
      && firstCluster[12] != 'e'
      && firstCluster[13] != 0x00
      && firstCluster[14] != 'x'
      && firstCluster[15] != 0x00
      && firstCluster[16] != 'f'
      && firstCluster[17] != 0x00
      && firstCluster[18] != 'a'
      && firstCluster[19] != 0x00
      && firstCluster[20] != 't'
      && firstCluster[21] != 0x00) {
    throw std::logic_error("unexpected root directory data - first cluster");
  }

  std::array<std::uint8_t, 16> firstClusterSlice;
  if (!file->read(2, &firstClusterSlice)) {
    throw std::logic_error("failed to read root directory - slice of first cluster");
  }

  if (!std::equal(firstClusterSlice.begin(), firstClusterSlice.end(), firstCluster.begin() + 2)) {
    throw std::logic_error("unexpected root directory data - slice of first cluster");
  }
}

void testVolumeLabel() {
  auto device = fs::test::common::File::make(image, imageSize, blockSize);
  auto partition = fs::FileSlice::tryMake(device, blockSize.blockAddressToByteOffset(2048), blockSize.blockCountToByteCount(3919872));

  fs::exfat::Volume volume(partition);

  auto file = volume.tryGetRootDirectoryFile();
  if (!file) {
    throw std::logic_error("failed to get root directory file");
  }

  for (fs::exfat::InternalDirectoryEnumerator enumerator(fs::exfat::InternalDirectory(std::move(file))); enumerator.isValid(); enumerator.next()) {
    auto volumeLabel = enumerator.entry().generic()->toVolumeLabel();
    if (volumeLabel) {
      if (volumeLabel->ascii() != "test-exfat") {
        throw std::logic_error("unexpected volume label");
      }
      return;
    }
  }

  throw std::logic_error("failed to find volume label");
}

void testFileSystem() {
  auto device = fs::test::common::File::make(image, imageSize, blockSize);
  auto partition = fs::FileSlice::tryMake(device, blockSize.blockAddressToByteOffset(2048), blockSize.blockCountToByteCount(3919872));

  fs::exfat::Volume volume(partition);

  auto fs = volume.tryMakeFileSystem();
  if (!fs) {
    throw std::logic_error("failed to get file system");
  }

  if (fs->name() != "test-exfat") {
    throw std::logic_error("file system has unexpected name");
  }
}

} // namespace

int main(int, char**) {
  testBootSector();
  testVolume();
  testClusterChain();
  testFile();
  testVolumeLabel();
  testFileSystem();
  return EXIT_SUCCESS;
}

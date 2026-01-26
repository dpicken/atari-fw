#include "fs/exfat/BootSector.h"
#include "fs/exfat/ClusterChainEnumerator.h"
#include "fs/exfat/Configuration.h"
#include "fs/exfat/PhysicalDirectoryEnumerator.h"
#include "fs/exfat/Volume.h"

#include "fs/DirectoryEnumerator.h"
#include "fs/FileSlice.h"
#include "fs/mbr/MBR.h"
#include "fs/test/common/File.h"

#include <cstdlib>
#include <stdexcept>
#include <vector>

namespace {

void testBootSector(const fs::File::ptr_type& partition) {
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

void testVolume(const fs::File::ptr_type& device, const fs::File::ptr_type& partition, std::uint64_t clusterHeapBlockCount) {
  fs::exfat::Volume volume(partition);

  auto activeFat = volume.tryGetActiveFat();
  if (!activeFat) {
    throw std::logic_error("failed to get active fat");
  }

  if (activeFat->size() != device->blockSize().blockCountToByteCount(512)) {
    throw std::logic_error("unexpected fat size");
  }

  auto clusterHeap = volume.tryGetClusterHeap();
  if (!clusterHeap) {
    throw std::logic_error("failed to get cluster heap");
  }

  if (clusterHeap->size() != device->blockSize().blockCountToByteCount(clusterHeapBlockCount)) {
    throw std::logic_error("unexpected cluster heap size");
  }
}

void testClusterChain(const fs::File::ptr_type& partition) {
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

void testFile(const fs::File::ptr_type& partition) {
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

void testVolumeLabel(const fs::File::ptr_type& partition) {
  fs::exfat::Volume volume(partition);

  auto file = volume.tryGetRootDirectoryFile();
  if (!file) {
    throw std::logic_error("failed to get root directory file");
  }

  for (fs::exfat::PhysicalDirectoryEnumerator enumerator(fs::exfat::PhysicalDirectory(std::move(file))); enumerator.isValid(); enumerator.next()) {
    auto volumeLabel = enumerator.entry().generic()->toCustomDefined<fs::exfat::VolumeLabelDirectoryEntry>();
    if (volumeLabel) {
      if (volumeLabel->toAscii() != "test-exfat") {
        throw std::logic_error("unexpected volume label");
      }
      return;
    }
  }

  throw std::logic_error("failed to find volume label");
}

void testFileSystem(const fs::File::ptr_type& partition) {
  fs::exfat::Volume volume(partition);

  auto fs = volume.tryMakeFileSystem();
  if (!fs) {
    throw std::logic_error("failed to get file system");
  }

  if (fs->name() != "test-exfat") {
    throw std::logic_error("file system has unexpected name");
  }
}

void testDirectory(const ::fs::Directory::ptr_type& directory, const std::vector<fs::DirectoryEntry>& expectedEntries) {
  std::vector<::fs::DirectoryEntry> entries;
  for (::fs::DirectoryEnumerator enumerator(directory); enumerator.isValid(); enumerator.next()) {
    entries.push_back(enumerator.entry());
  }

  if (entries != expectedEntries) {
    throw std::logic_error("directory inconsistent with expected entries");
  }
}

void testFile(const ::fs::File::ptr_type& file, const std::vector<std::uint8_t>& expectedData) {
  if (file->size() != expectedData.size()) {
    throw std::logic_error("file size inconsistent with expected data");
  }

  std::vector<std::uint8_t> data(expectedData.size());
  if (!file->read(0, data.data(), data.size())) {
    throw std::logic_error("failed to read file");
  }

  if (data != expectedData) {
    throw std::logic_error("file data inconsistent with expected data");
  }
}

void testRootDirectory(const fs::File::ptr_type& partition) {
  fs::exfat::Volume volume(partition);

  auto fs = volume.tryMakeFileSystem();
  if (!fs) {
    throw std::logic_error("failed to get file system");
  }

  auto rootDirectory = fs->getRootDirectory();

  fs::exfat::Configuration::instance()->excludeHiddenDirectoryEntries(false);
  testDirectory(rootDirectory, {
    {".fseventsd", fs::DirectoryEntry::Type::Directory, 0},
    {".Spotlight-V100", fs::DirectoryEntry::Type::Directory, 1},
    {"test-dir", fs::DirectoryEntry::Type::Directory, 2}
  });

  fs::exfat::Configuration::instance()->excludeHiddenDirectoryEntries(true);
  testDirectory(rootDirectory, {
    {"test-dir", fs::DirectoryEntry::Type::Directory, 0}
  });

  fs::exfat::Configuration::instance()->excludeHiddenDirectoryEntries(false);
  testDirectory(rootDirectory, {
    {".fseventsd", fs::DirectoryEntry::Type::Directory, 0},
    {".Spotlight-V100", fs::DirectoryEntry::Type::Directory, 1},
    {"test-dir", fs::DirectoryEntry::Type::Directory, 2}
  });

  auto subDirectory = rootDirectory->openDirectory("test-dir");
  if (!subDirectory) {
    throw std::logic_error("failed to open sub-directory");
  }

  testDirectory(subDirectory, {
    {"test-file", fs::DirectoryEntry::Type::File, 0}
  });

  auto file = subDirectory->openFile("test-file");
  if (!file) {
    throw std::logic_error("failed to open file");
  }
  testFile(file, {'t', 'e', 's', 't', '-', 'd', 'a', 't', 'a', '\n'});
}

void test(const std::filesystem::path& devicePath, fs::BlockSize deviceBlockSize, std::uint64_t deviceBlockCount, std::uint64_t partitionBlockOffset, std::uint64_t partitionBlockCount, std::uint64_t clusterHeapBlockCount) {
  auto device  = fs::test::common::File::make(devicePath, deviceBlockSize.blockCountToByteCount(deviceBlockCount), deviceBlockSize);
  if (device == nullptr) {
    throw std::logic_error("device not found");
  }

  auto partition = fs::FileSlice::tryMake(device, device->blockSize().blockAddressToByteOffset(partitionBlockOffset), device->blockSize().blockCountToByteCount(partitionBlockCount));
  if (partition == nullptr) {
    throw std::logic_error("failed to slice first partition");
  }

  testBootSector(partition);
  testVolume(device, partition, clusterHeapBlockCount);
  testClusterChain(partition);
  testFile(partition);
  testVolumeLabel(partition);
  testFileSystem(partition);
  testRootDirectory(partition);
}

} // namespace

int main(int, char**) {
  test("image/sd-card-3921920-512B-sectors-mbr_exfat_head-macos.img", fs::BlockSize::fromSizeLog2(9), 3'921'920, 2048, 3'919'872, 3'919'232);
  test("image/sd-card-3921920-512B-sectors-gpt_exfat_head-macos.img", fs::BlockSize::fromSizeLog2(9), 3'921'920, 2048, 3'917'824, 3'917'184);
  return EXIT_SUCCESS;
}

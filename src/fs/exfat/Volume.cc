#include "Volume.h"

#include "ClusterChainEnumerator.h"
#include "File.h"
#include "PhysicalDirectory.h"
#include "PhysicalDirectoryEnumerator.h"

#include "fs/FileSlice.h"

fs::exfat::Volume::Volume(const file_ptr_type& volume)
  : m_volume(volume)
  , m_bootSector(tryReadBootSector(m_volume)) {
}

fs::exfat::Volume::boot_sector_type fs::exfat::Volume::tryReadBootSector(const file_ptr_type& volume) {
  for (auto bootRegion : {tryGetMainBootRegion(volume), tryGetBackupBootRegion(volume)}) {
    if (!isBootRegionValid(bootRegion)) {
      continue;
    }

    auto bootSector = tryReadBootSector(volume, bootRegion);
    if (!bootSector) {
      continue;
    }

    return bootSector;
  }
  return std::nullopt;
}

fs::exfat::Volume::boot_sector_type fs::exfat::Volume::tryReadBootSector(const file_ptr_type& volume, const file_ptr_type& bootRegion) {
  BootSector bootSector;
  if (!bootRegion->read(0, &bootSector)) {
    return std::nullopt;
  }

  if (!bootSector.isValid(volume)) {
    return std::nullopt;
  }

  return bootSector;
}

fs::exfat::Volume::file_ptr_type fs::exfat::Volume::tryGetMainBootRegion(const file_ptr_type& volume) {
  auto blockSize = volume->blockSize();
  return ::fs::FileSlice::tryMake(volume, blockSize.blockAddressToByteOffset(0), blockSize.blockCountToByteCount(12));
}

fs::exfat::Volume::file_ptr_type fs::exfat::Volume::tryGetBackupBootRegion(const file_ptr_type& volume) {
  auto blockSize = volume->blockSize();
  return ::fs::FileSlice::tryMake(volume, blockSize.blockAddressToByteOffset(12), blockSize.blockCountToByteCount(12));
}

bool fs::exfat::Volume::isBootRegionValid(const file_ptr_type& bootRegion) {
  // TODO: Validate boot region checksum.
  (void) bootRegion;
  return true;
}

fs::FileSlice::impl_ptr_type fs::exfat::Volume::tryGetActiveFat() const {
  if (!m_bootSector) {
    return nullptr;
  }
  auto blockSize = m_volume->blockSize();
  return ::fs::FileSlice::tryMake(m_volume, blockSize.blockAddressToByteOffset(m_bootSector->activeFatOffset()), blockSize.blockCountToByteCount(m_bootSector->fatLength));
}

fs::exfat::ClusterHeap::impl_ptr_type fs::exfat::Volume::tryGetClusterHeap() const {
  if (!m_bootSector) {
    return nullptr;
  }
  auto clusterSize = m_bootSector->clusterSize(m_volume);
  auto clusters = ::fs::FileSlice::tryMake(m_volume, m_volume->blockSize().blockAddressToByteOffset(m_bootSector->clusterHeapOffset), clusterSize.blockCountToByteCount(m_bootSector->clusterCount));
  if (!clusters) {
    return nullptr;
  }
  return ClusterHeap::make(clusters, clusterSize, m_bootSector->clusterCount);
}

fs::exfat::ClusterChain fs::exfat::Volume::getRootDirectoryClusterChain() const {
  if (!m_bootSector) {
    return ClusterChain();
  }
  auto fat = tryGetActiveFat();
  if (!fat) {
    return ClusterChain();
  }
  return ClusterChain(fat, m_bootSector->firstClusterOfRootDirectory);
}

fs::exfat::File::impl_ptr_type fs::exfat::Volume::tryGetRootDirectoryFile() const {
  auto clusterHeap = tryGetClusterHeap();
  if (!clusterHeap) {
    return nullptr;
  }

  auto clusterChain = getRootDirectoryClusterChain();
  ::fs::File::size_type size = 0;
  for (ClusterChainEnumerator enumerator(clusterChain); enumerator.isValid(); enumerator.next()) {
    size += clusterHeap->blockSize().byteCount();
  }

  return File::make(size, std::move(clusterChain), clusterHeap);
}

fs::exfat::FileSystem::impl_ptr_type fs::exfat::Volume::tryMakeFileSystem() const {
  auto rootDirectoryFile = tryGetRootDirectoryFile();
  if (!rootDirectoryFile) {
    return nullptr;
  }

  PhysicalDirectory rootDirectory(std::move(rootDirectoryFile));
  PhysicalDirectoryEnumerator rootDirectoryEnumerator(rootDirectory);
  auto volumeLabelEntry = find<VolumeLabelDirectoryEntry>(rootDirectoryEnumerator);
  return FileSystem::tryMake(volumeLabelEntry ? volumeLabelEntry->customDefined().customDefined().toAscii() : "", std::move(rootDirectory));
}

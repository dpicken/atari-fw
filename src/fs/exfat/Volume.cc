#include "Volume.h"

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

fs::exfat::Volume::boot_sector_type fs::exfat::Volume::tryReadBootSector(const file_ptr_type& volume, const file_slice_ptr_type& bootRegion) {
  BootSector bootSector;
  if (!bootRegion->read(0, &bootSector)) {
    return std::nullopt;
  }

  if (!bootSector.isValid(volume)) {
    return std::nullopt;
  }

  return bootSector;
}

fs::exfat::Volume::file_slice_ptr_type fs::exfat::Volume::tryGetMainBootRegion(const file_ptr_type& volume) {
  auto blockSize = volume->blockSize();
  return ::fs::FileSlice::tryMake(volume, blockSize.blockAddressToByteOffset(0), blockSize.blockCountToByteCount(12));
}

fs::exfat::Volume::file_slice_ptr_type fs::exfat::Volume::tryGetBackupBootRegion(const file_ptr_type& volume) {
  auto blockSize = volume->blockSize();
  return ::fs::FileSlice::tryMake(volume, blockSize.blockAddressToByteOffset(12), blockSize.blockCountToByteCount(12));
}

bool fs::exfat::Volume::isBootRegionValid(const file_slice_ptr_type& bootRegion) {
  // TODO: Validate boot region checksum.
  (void) bootRegion;
  return true;
}

fs::exfat::Volume::file_slice_ptr_type fs::exfat::Volume::tryGetActiveFat() const {
  if (!m_bootSector) {
    return nullptr;
  }
  auto blockSize = m_volume->blockSize();
  return ::fs::FileSlice::tryMake(m_volume, blockSize.blockAddressToByteOffset(m_bootSector->activeFatOffset()), blockSize.blockCountToByteCount(m_bootSector->fatLength));
}

fs::exfat::Volume::file_slice_ptr_type fs::exfat::Volume::tryGetClusterHeap() const {
  if (!m_bootSector) {
    return nullptr;
  }
  auto clusterSize = m_bootSector->clusterSize(m_volume);
  return ::fs::FileSlice::tryMake(m_volume, m_volume->blockSize().blockAddressToByteOffset(m_bootSector->clusterHeapOffset), clusterSize.blockCountToByteCount(m_bootSector->clusterCount));
}

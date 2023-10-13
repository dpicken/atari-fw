#include "DiskLibrary.h"

#include "Atr.h"
#include "DiskLibraryAtr.h"

media::DiskLibrary::DiskLibrary() {
  for (std::size_t diskIndex = 0; diskIndex != DiskLibraryAtr::getAtrCount(); ++diskIndex) {
    auto disk = makeAtr(DiskLibraryAtr::getAtrData(diskIndex), DiskLibraryAtr::getAtrSize(diskIndex));
    if (disk != nullptr) {
      push(std::move(disk));
    }
  }
}

media::DiskLibrary& media::DiskLibrary::instance() {
  static DiskLibrary instance;
  return instance;
}

void media::DiskLibrary::push(disk_ptr disk) {
  if (disk == nullptr) {
    return;
  }
  m_disks.push_back(std::move(disk));
}

media::DiskLibrary::disk_ptr media::DiskLibrary::pop() {
  if (m_disks.empty()) {
    return nullptr;
  }

  auto disk = std::move(m_disks.front());
  m_disks.pop_front();
  return disk;
}

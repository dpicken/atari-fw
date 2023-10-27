#include "DiskLibrary.h"

#include "BuiltinAtrFileLibrary.h"

#include "Atr.h"

media::DiskLibrary::DiskLibrary() {
  addBuiltinAtrFileLibrary();
}

void media::DiskLibrary::addBuiltinAtrFileLibrary() {
  for (unsigned int atrIndex = 0; atrIndex != BuiltinAtrFileLibrary::getAtrCount(); ++atrIndex) {
    auto disk = makeAtr(BuiltinAtrFileLibrary::makeRomFile(atrIndex));
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

#include "media/Atr.h"
#include "media/DiskLibrary.h"
#include "media/DiskLibraryAtr.h"

#include <cstdlib>
#include <list>
#include <sstream>
#include <stdexcept>

namespace {

void testDisk(std::unique_ptr<media::Disk> disk) {
  for (auto it = disk->getBeginSectorAddress(), end = disk->getEndSectorAddress(); it != end; ++it) {
    if (!disk->hasSector(it)) {
      throw std::logic_error("unreadable sector");
    }
    disk->readSector(it, [](const std::uint8_t* sector, std::size_t sectorSize) {
        (void) sector;
        if (sectorSize != 128) {
          throw std::logic_error("unexpected sector size");
        }
    });
  }
}

void testDiskLibraryAtr() {
  std::list<const char*> badTitles;
  for (std::size_t diskIndex = 0; diskIndex != media::DiskLibraryAtr::getAtrCount(); ++diskIndex) {
    auto disk = media::makeAtr(media::DiskLibraryAtr::getAtrData(diskIndex), media::DiskLibraryAtr::getAtrSize(diskIndex));
    if (disk != nullptr) {
      try {
        testDisk(std::move(disk));
      } catch (...) {
        badTitles.push_back(media::DiskLibraryAtr::getAtrTitle(diskIndex));
      }
    } else {
      badTitles.push_back(media::DiskLibraryAtr::getAtrTitle(diskIndex));
    }
  }
  if (!badTitles.empty()) {
    std::ostringstream os;
    os << "bad atr images:";
    for (auto badTitle : badTitles) {
      os << " " << badTitle;
    }
    throw std::logic_error(os.str());
  }
}

void testDiskLibrary() {
  unsigned int diskCount = 0;
  for (auto disk = media::DiskLibrary::instance().pop(); disk != nullptr; disk = media::DiskLibrary::instance().pop()) {
    testDisk(std::move(disk));
    ++diskCount;
  }
  if (diskCount == 0) {
    throw std::logic_error("empty disk library");
  }
}

} // namespace

int main(int, char**) {
  testDiskLibraryAtr();
  testDiskLibrary();
  return EXIT_SUCCESS;
}

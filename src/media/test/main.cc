#include "media/Atr.h"
#include "media/BuiltinAtrFileLibrary.h"
#include "media/DiskLibrary.h"

#include "io/RomFile.h"

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
    bool readOK = disk->readSector(it, [&disk, it](const std::uint8_t* sector, std::size_t sectorSize) {
      (void)sector;
      switch (disk->getDensity()) {
        case media::Disk::Density::Single:
        case media::Disk::Density::Enhanced:
          if (sectorSize != 128) {
            throw std::logic_error("unexpected sector size");
          }
          break;

        case media::Disk::Density::Double:
          if (it >= 1 && it <= 3) {
            if (sectorSize != 128) {
              throw std::logic_error("unexpected sector size");
            }
          } else {
            if (sectorSize != 256) {
              throw std::logic_error("unexpected sector size");
            }
          }
          break;
      }
    });
    if (!readOK) {
      throw std::logic_error("sector read failed");
    }
  }
}

void testBuiltinAtrFileLibrary() {
  std::list<std::string_view> badTitles;
  for (unsigned int atrIndex = 0; atrIndex != media::BuiltinAtrFileLibrary::getAtrCount(); ++atrIndex) {
    auto disk = media::makeAtr(media::BuiltinAtrFileLibrary::makeRomFile(atrIndex));
    if (disk != nullptr) {
      try {
        testDisk(std::move(disk));
      } catch (...) {
        badTitles.push_back(media::BuiltinAtrFileLibrary::getAtrTitle(atrIndex));
      }
    } else {
      badTitles.push_back(media::BuiltinAtrFileLibrary::getAtrTitle(atrIndex));
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
  testBuiltinAtrFileLibrary();
  testDiskLibrary();
  return EXIT_SUCCESS;
}

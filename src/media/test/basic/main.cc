#include "media/Atr.h"

#include "fs/DirectoryEnumerator.h"
#include "fs/builtin/FileSystem.h"

#include <cstdlib>
#include <list>
#include <sstream>
#include <stdexcept>

namespace {

void testDisk(const media::Disk::ptr_type& disk) {
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

void testBuiltinAtrLibrary() {
  std::list<std::string_view> badTitles;
  for (fs::DirectoryEnumerator enumerator(::fs::builtin::FileSystem::instance()->getRootDirectory()); enumerator.isValid(); enumerator.next()) {
    const auto& entry = enumerator.entry();
    if (!entry.isFile()) {
      continue;
    }
    if (!entry.name().ends_with(".atr")) {
      continue;
    }
    auto disk = media::makeAtr(enumerator.openFile());
    if (disk != nullptr) {
      try {
        testDisk(disk);
      } catch (...) {
        badTitles.push_back(entry.name());
      }
    } else {
      badTitles.push_back(entry.name());
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

} // namespace

int main(int, char**) {
  testBuiltinAtrLibrary();
  return EXIT_SUCCESS;
}

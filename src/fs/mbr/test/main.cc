#include "fs/mbr/MBR.h"

#include "fs/DirectoryEnumerator.h"
#include "fs/FileSlice.h"
#include "fs/test/common/File.h"

#include <cstdlib>
#include <stdexcept>

namespace {

static constexpr std::uint8_t image[] = {
#include "fs/test/data/sd-card-3921920-512B-sectors-mbr_exfat_boot_and_fat_region_and_three_clusters-macos.data"
};
static constexpr auto blockSize = fs::BlockSize::fromSizeLog2(9);
static constexpr auto imageSize = blockSize.blockCountToByteCount(3'921'920);

void testMBR() {
  auto device = fs::test::common::File::make(image, imageSize, blockSize);

  auto directory = fs::mbr::tryMakeDirectory(device);
  if (directory == nullptr) {
    throw std::logic_error("failed to decode mbr");
  }

  fs::DirectoryEnumerator dirEnumerator(directory);
  if (!dirEnumerator.isValid()) {
    throw std::logic_error("failed to find first partition");
  }

  auto partition = dirEnumerator.openFile();
  if (partition == nullptr) {
    throw std::logic_error("failed to open first partition");
  }

  if (partition->size() != blockSize.blockCountToByteCount(3'919'872)) {
    throw std::logic_error("partition 0: unexpected size");
  }

  if (dynamic_cast<fs::FileSlice*>(partition.get())->offset() != blockSize.blockCountToByteCount(2048)) {
    throw std::logic_error("partition 0: unexpected offset");
  }

  dirEnumerator.next();
  if (dirEnumerator.isValid()) {
    throw std::logic_error("unexpected second partition");
  }
}

} // namespace

int main(int, char**) {
  testMBR();
  return EXIT_SUCCESS;
}

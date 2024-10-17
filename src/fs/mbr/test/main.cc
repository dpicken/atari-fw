#include "fs/mbr/MBR.h"

#include "fs/DirectoryEnumerator.h"
#include "fs/FileSlice.h"
#include "fs/test/common/File.h"
#include "fs/test/common/images.h"

#include <cstdlib>
#include <stdexcept>

namespace {

void testMBR() {
  auto blockSize = fs::BlockSize::fromSizeLog2(9);
  auto device = fs::test::common::File::make(fs::test::common::sd_card_3921920_512B_sectors_mbr_exfat_boot_region_macos, blockSize.blockCountToByteCount(3'921'920), blockSize);

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

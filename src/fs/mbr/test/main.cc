#include "fs/mbr/MBR.h"

#include "fs/DirectoryEnumerator.h"
#include "fs/FileSlice.h"
#include "fs/test/common/File.h"

#include <cstdlib>
#include <stdexcept>

namespace {

fs::File::ptr_type makeDevice() {
  constexpr auto devicePath = "image/sd-card-3921920-512B-sectors-mbr_exfat_head-macos.img";
  constexpr auto deviceBlockSize = fs::BlockSize::fromSizeLog2(9);
  return fs::test::common::File::make(devicePath, deviceBlockSize.blockCountToByteCount(3921920), deviceBlockSize);
}

void testMBR() {
  auto device = makeDevice();
  if (device == nullptr) {
    throw std::logic_error("device not found");
  }

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

  if (partition->size() != device->blockSize().blockCountToByteCount(3'919'872)) {
    throw std::logic_error("partition 0: unexpected size");
  }

  if (dynamic_cast<fs::FileSlice*>(partition.get())->offset() != device->blockSize().blockCountToByteCount(2048)) {
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

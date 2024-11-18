#include "fs/automount/Manager.h"
#include "fs/DirectoryEnumerator.h"
#include "fs/ResolvePath.h"
#include "fs/test/common/File.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

namespace {

fs::test::common::File::impl_ptr_type makeDevice(const std::filesystem::path& devicePath) {
  constexpr auto deviceBlockSize = fs::BlockSize::fromSizeLog2(9);
  return fs::test::common::File::make(devicePath, deviceBlockSize.blockCountToByteCount(3921920), deviceBlockSize);
}

void testEnumeratorDirectory(const std::filesystem::path& directoryPath) {
  auto directory = fs::resolveDirectory(directoryPath);
  if (!directory) {
    throw std::logic_error("failed to resolve directory");
  }

  for (fs::DirectoryEnumerator enumerator(directory); enumerator.isValid(); enumerator.next()) {
    std::cout << enumerator.entry().name() << "\n";
  }
}

} // namespace

int main(int argc, char** argv) {
  if (argc > 3) {
    std::cerr << "Usage: " << argv[0] << " [image] [directory]" << std::endl;
    return EXIT_FAILURE;
  }

  const auto imagePath = (argc > 1) ? argv[1] : "image/sd-card-3921920-512B-sectors-mbr_exfat_boot_and_fat_region_and_three_clusters-macos.img";
  const auto directoryPath = (argc > 2) ? argv[2] : "image:test-exfat";

  fs::automount::Manager::instance()->onBlockDeviceAvailable(makeDevice(imagePath), "image");

  testEnumeratorDirectory(directoryPath);
  return EXIT_SUCCESS;
}

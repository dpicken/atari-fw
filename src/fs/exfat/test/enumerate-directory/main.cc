#include "fs/automount/Manager.h"
#include "fs/DirectoryEnumerator.h"
#include "fs/ResolvePath.h"
#include "fs/root/FileSystem.h"
#include "fs/test/common/File.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

namespace {

std::filesystem::path mountDevice(const std::filesystem::path& devicePath) {
  constexpr auto assumedDeviceBlockSize = fs::BlockSize::fromSizeLog2(9);
  constexpr auto assumedDeviceByteCount = assumedDeviceBlockSize.blockCountToByteCount(134'217'728);
  auto device = fs::test::common::File::make(devicePath, assumedDeviceByteCount, assumedDeviceBlockSize);
  if (device == nullptr) {
    throw std::logic_error(devicePath.string() + ": failed to open device");
  }

  const std::string deviceName = "test-device";
  fs::automount::Manager::instance()->onBlockDeviceAvailable(device, deviceName);
  return deviceName;
}

std::filesystem::path mountTestDevice() {
  constexpr auto deviceBlockSize = fs::BlockSize::fromSizeLog2(9);
  auto device = fs::test::common::File::make("image/sd-card-3921920-512B-sectors-mbr_exfat_boot_and_fat_region_and_three_clusters-macos.img", deviceBlockSize.blockCountToByteCount(3921920), deviceBlockSize);
  const std::string deviceName = "test-device";
  fs::automount::Manager::instance()->onBlockDeviceAvailable(device, deviceName);
  return deviceName;
}

void testEnumerateDirectory(const std::filesystem::path& directoryPath) {
  auto directory = fs::resolveDirectory(directoryPath);
  if (!directory) {
    throw std::logic_error(directoryPath.string() + ": failed to resolve directory");
  }

  for (fs::DirectoryEnumerator enumerator(directory); enumerator.isValid(); enumerator.next()) {
    std::cout << enumerator.entry().name() << "\n";
  }
}

} // namespace

int main(int argc, char** argv) {
  if (argc > 3) {
    std::cerr << "Usage: " << argv[0] << " [exfat-image [exfat-directory]]" << std::endl;
    return EXIT_FAILURE;
  }

  const auto mountPointPath = (argc > 1) ? mountDevice(argv[1]) : mountTestDevice();
  if (argc > 2) {
    std::filesystem::path exFatDirectoryPath = std::filesystem::path(argv[2]).relative_path();
    if (exFatDirectoryPath.empty()) {
      testEnumerateDirectory(mountPointPath);
    } else {
      testEnumerateDirectory(mountPointPath / exFatDirectoryPath.relative_path());
    }
  } else {
    testEnumerateDirectory(mountPointPath);
  }

  return EXIT_SUCCESS;
}

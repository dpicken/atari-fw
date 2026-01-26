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
  auto device = fs::test::common::File::make("image/sd-card-3921920-512B-sectors-mbr_exfat_head-macos.img", deviceBlockSize.blockCountToByteCount(3921920), deviceBlockSize);
  const std::string deviceName = "test-device";
  fs::automount::Manager::instance()->onBlockDeviceAvailable(device, deviceName);
  return deviceName;
}

void enumerateDirectory(const fs::Directory::ptr_type& directory) {
  for (fs::DirectoryEnumerator enumerator(directory); enumerator.isValid(); enumerator.next()) {
    std::cout << (enumerator.entry().isDirectory() ? 'd' : '-') << ' ' << enumerator.entry().name() << "\n";
  }
}

void readFile(const fs::File::ptr_type& file) {
  const auto dumper = [](const fs::File::buffer_value_type* data, fs::File::buffer_size_type size) {
    std::cout.write(reinterpret_cast<const char*>(data), static_cast<std::streamsize>(size));
    std::cout.flush();
  };

  if (!file->read(0, file->size(), dumper)) {
    throw std::logic_error("failed to read file");
  }
}

void read(const std::filesystem::path& path) {
  auto parent = fs::resolveDirectory(path.parent_path());
  if (!parent) {
    throw std::invalid_argument("failed to resolve parent");
  }
  if (!path.has_filename()) {
    enumerateDirectory(parent);
  } else if (auto directory = parent->openDirectory(path.filename())) {
    enumerateDirectory(directory);
  } else if (auto file = parent->openFile(path.filename())) {
    readFile(file);
  } else {
    throw std::invalid_argument("failed to resolve child");
  }
}

} // namespace

int main(int argc, char** argv) {
  if ((argc > 1 && std::string(argv[1]) == "--help") || argc > 3) {
    std::cerr << "Usage: " << argv[0] << " [exfat-image [path]]" << std::endl;
    return EXIT_FAILURE;
  }

  const auto mountPointPath = (argc > 1) ? mountDevice(argv[1]) : mountTestDevice();
  if (argc > 2) {
    std::filesystem::path exFatFilePath = std::filesystem::path(argv[2]).relative_path();
    read(mountPointPath / exFatFilePath.relative_path());
  } else {
    read(mountPointPath);
  }

  return EXIT_SUCCESS;
}

#include "sio/checksum.h"
#include "sio/DiskDrive.h"
#include "sio/Frame.h"
#include "sio/sdr/FileSystem.h"

#include "fs/builtin/File.h"
#include "media/Atr.h"
#include "media/BuiltinAtrLibrary.h"

#include <cstdlib>
#include <cstdint>
#include <stdexcept>

namespace {

static const std::uint8_t* rxIt;
static const std::uint8_t* rxEnd;

static const std::uint8_t* txIt;
static const std::uint8_t* txEnd;

hal::Uart uart(
    [](std::uint8_t* buf, std::size_t byteCount) {
      auto it = buf;
      auto end = buf + byteCount;
      for (; it != end && rxIt != rxEnd; ++it, ++rxIt) {
        *it = *rxIt;
      }
      return it == end;
    },
    [](const std::uint8_t* buf, std::size_t byteCount) {
      auto it = buf;
      auto end = buf + byteCount;
      for (; it != end && txIt != txEnd; ++it, ++txIt) {
        if (*it != *txIt) {
          throw std::logic_error("unexpected tx");
        }
      }
      if (it != end) {
        throw std::logic_error("tx underrun");
      }
    }
);

hal::BusyWait busyWait = [](hal::duration_us timeoutDurationUS) {
  (void) timeoutDurationUS;
};

void testDiskDriveCommand(sio::DiskDrive* diskDrive) {
  sio::Frame<sio::Device::Command> commandFrame;
  if (!commandFrame.rx(&uart)) {
    throw std::logic_error("rx failed");
  }

  diskDrive->handle(commandFrame.data());

  if (txIt != txEnd) {
    throw std::logic_error("tx overrun");
  }
}

void testDiskDriveStatus() {
  static constexpr std::uint8_t command[] = {
    0x31, 'S', 0x00, 0x00, // Status command
    0x84 // CRC
  };

  rxIt = command;
  rxEnd = rxIt + sizeof(command);

  std::uint8_t expectedResult[] = {
    'A', // Ack
    'C', // Complete
    0x00, 0xFF, 0xE0, 0x00, // Status result
    0xE0 // Checksum
  };

  auto disk = media::makeAtr(fs::builtin::File::make(media::BuiltinAtrLibrary::getAtrData(0), media::BuiltinAtrLibrary::getAtrSize(0)));
  switch (disk->getDensity()) {
    case media::Disk::Density::Single:
      break;
    case media::Disk::Density::Enhanced:
      expectedResult[2] |= 0x80;
      expectedResult[6] = 0x61;
      break;
    case media::Disk::Density::Double:
      expectedResult[2] |= 0x20;
      expectedResult[6] = 0x20;
      break;
  }

  txIt = expectedResult;
  txEnd = txIt + sizeof(expectedResult);

  sio::DiskDrive diskDrive(uart, busyWait);;
  diskDrive.insert(std::move(disk));

  testDiskDriveCommand(&diskDrive);
}

void testDiskDriveRead() {
  static constexpr std::uint8_t command[] = {
    0x31, 'R', 0x01, 0x00, // Read command
    0x84 // CRC
  };

  rxIt = command;
  rxEnd = rxIt + sizeof(command);

  std::uint8_t expectedResult[131];

  auto atrData = media::BuiltinAtrLibrary::getAtrData(0);

  expectedResult[0] = 'A'; // Ack
  expectedResult[1] = 'C'; // Complete
  std::copy(atrData + 16, atrData + 16 + 128, expectedResult + 2); // Sector 1
  expectedResult[130] = sio::checksum(expectedResult + 2, 128); // Checksum

  txIt = expectedResult;
  txEnd = txIt + sizeof(expectedResult);

  sio::DiskDrive diskDrive(uart, busyWait);
  auto disk = media::makeAtr(fs::builtin::File::make(media::BuiltinAtrLibrary::getAtrData(0), media::BuiltinAtrLibrary::getAtrSize(0)));
  diskDrive.insert(std::move(disk));

  testDiskDriveCommand(&diskDrive);
}

void testDiskDrive() {
  testDiskDriveStatus();
  testDiskDriveRead();
}

void testFileSystemStringTruncation() {
  using path_type = sio::sdr::CurrentDirPath;

  const std::string testString(path_type::capacity_v + 1, 'A');

  auto rightTruncated = path_type(testString);
  if (std::string(rightTruncated.data(), rightTruncated.size()) != (std::string(path_type::capacity_v - 3, 'A') + std::string(3, '.')) ){
    throw std::logic_error("unexpected right-truncated string");
  }

  auto leftTruncated = path_type(testString, path_type::left_truncate);
  if (std::string(leftTruncated.data(), leftTruncated.size()) != (std::string(3, '.') + std::string(path_type::capacity_v - 3, 'A'))) {
    throw std::logic_error("unexpected left-truncated string");
  }
}

} // namespace

int main(int, char**) {
  testDiskDrive();
  testFileSystemStringTruncation();
  return EXIT_SUCCESS;
}

#include "sio/checksum.h"
#include "sio/DiskDrive.h"
#include "sio/FileSystem.h"
#include "sio/Frame.h"
#include "sio/sdr/FileSystem.h"

#include "fs/builtin/FileSystem.h"
#include "media/Atr.h"

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

void testDeviceCommand(sio::Device* device) {
  sio::Frame<sio::Device::Command> commandFrame;
  if (!commandFrame.rx(&uart)) {
    throw std::logic_error("rx failed");
  }

  device->handle(commandFrame.data());

  if (txIt != txEnd) {
    throw std::logic_error("tx overrun");
  }
}

void testDiskDriveStatus() {
  constexpr std::uint8_t command[] = {
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

  auto disk = media::makeAtr(fs::builtin::FileSystem::instance()->getRootDirectory()->openFile("!sbc-boot.atr"));
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
  diskDrive.insert(disk);

  testDeviceCommand(&diskDrive);
}

void testDiskDriveRead(sio::DiskDrive* diskDrive, const media::Disk::ptr_type& expectedDisk) {
  constexpr std::uint8_t command[] = {
    0x31, 'R', 0x01, 0x00, // Read command
    0x84 // CRC
  };

  rxIt = command;
  rxEnd = rxIt + sizeof(command);

  std::uint8_t expectedResult[131];

  expectedDisk->readSector(1, [&expectedResult](const std::uint8_t* sector, std::size_t sectorSize) {
    std::copy_n(sector, sectorSize, expectedResult + 2);
  });

  expectedResult[0] = 'A'; // Ack
  expectedResult[1] = 'C'; // Complete
  expectedResult[130] = sio::checksum(expectedResult + 2, 128); // Checksum

  txIt = expectedResult;
  txEnd = txIt + sizeof(expectedResult);

  testDeviceCommand(diskDrive);
}

void testDiskDriveRead() {
  sio::DiskDrive diskDrive(uart, busyWait);

  auto disk = media::makeAtr(fs::builtin::FileSystem::instance()->getRootDirectory()->openFile("!xex-loader.atr"));
  diskDrive.insert(disk);

  testDiskDriveRead(&diskDrive, disk);
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

void testFileSystemGetCurrentDir() {
  constexpr std::uint8_t command[] = {
    0x81, 0x00, 0x00, 0x00, // GetCurrentDir command
    0x81 // CRC
  };

  rxIt = command;
  rxEnd = rxIt + sizeof(command);

  std::uint8_t expectedResult[] = {
    'A', // Ack
    'C', // Complete
    0x01, 0x00, '/', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Path
    0x30 // Checksum
  };

  txIt = expectedResult;
  txEnd = txIt + sizeof(expectedResult);

  sio::DiskDrive diskDrive(uart, busyWait);
  sio::FileSystem fileSystem(uart, busyWait, &diskDrive);

  testDeviceCommand(&fileSystem);
}

void testSelectDirEntry(sio::FileSystem* fileSystem, std::uint16_t index) {
  std::uint8_t command[] = {
    0x81, 0x03, static_cast<std::uint8_t>(index), static_cast<std::uint8_t>(index >> 8), // SelectDirEntry command
    0x00 // CRC
  };
  command[4] = sio::checksum(command, 4); // CRC

  rxIt = command;
  rxEnd = rxIt + sizeof(command);

  std::uint8_t expectedResult[] = {
    'A', // Ack
    'C'  // Complete
  };

  txIt = expectedResult;
  txEnd = txIt + sizeof(expectedResult);

  testDeviceCommand(fileSystem);
}

void testFileSystemReadXexSegmentEntry(sio::FileSystem* fileSystem, std::uint16_t index, std::uint16_t expectedLoadAddress, std::uint16_t expectedLoadAddressLast) {
  std::uint8_t command[] = {
    0x81, 0x04, static_cast<std::uint8_t>(index), static_cast<std::uint8_t>(index >> 8), // ReadXexSegmentEntry command
    0x00 // CRC
  };
  command[4] = sio::checksum(command, 4); // CRC

  rxIt = command;
  rxEnd = rxIt + sizeof(command);
  rxIt = command;
  rxEnd = rxIt + sizeof(command);

  std::uint8_t expectedResult[] = {
    'A', // Ack
    'C', // Complete
    0x00, // EOS
    static_cast<std::uint8_t>(expectedLoadAddress), static_cast<std::uint8_t>(expectedLoadAddress >> 8), // segmentAddressBegin
    static_cast<std::uint8_t>(expectedLoadAddressLast), static_cast<std::uint8_t>(expectedLoadAddressLast >> 8), // segmentAddressLast
    0x00 // Checksum
  };
  expectedResult[7] = sio::checksum(expectedResult + 2, 5); // CRC

  txIt = expectedResult;
  txEnd = txIt + sizeof(expectedResult);

  testDeviceCommand(fileSystem);
}

void testFileSystemReadXexSegmentEntryEOS(sio::FileSystem* fileSystem, std::uint16_t index) {
  std::uint8_t command[] = {
    0x81, 0x04, static_cast<std::uint8_t>(index), static_cast<std::uint8_t>(index >> 8), // ReadXexSegmentEntry command
    0x00 // CRC
  };
  command[4] = sio::checksum(command, 4); // CRC

  rxIt = command;
  rxEnd = rxIt + sizeof(command);
  rxIt = command;
  rxEnd = rxIt + sizeof(command);

  constexpr std::uint8_t expectedResult[] = {
    'A', // Ack
    'C', // Complete
    0x01, // EOS
    0x00, 0x00, // segmentAddressBegin
    0x00, 0x00, // segmentAddressLast
    0x01 // Checksum
  };

  txIt = expectedResult;
  txEnd = txIt + sizeof(expectedResult);

  testDeviceCommand(fileSystem);
}

void testFileSystemReadXexSegmentData(sio::FileSystem* fileSystem, const ::fs::File::ptr_type& xexFile, ::fs::File::size_type offset, ::fs::File::buffer_size_type size) {
  constexpr std::uint8_t command[] = {
    0x81, 0x05, 0x00, 0x00, // ReadXexSegmentData command
    0x86 // CRC
  };

  rxIt = command;
  rxEnd = rxIt + sizeof(command);

  std::vector<std::uint8_t> expectedResult(2 + size + 1);
  expectedResult[0] = 'A'; // Ack
  expectedResult[1] = 'C'; // Complete
  auto dest = expectedResult.data() + 2;
  xexFile->read(offset, size, [&dest](const ::fs::File::buffer_value_type* data, ::fs::File::buffer_size_type readSize) {
    dest = std::copy(data, data + readSize, dest);
  });
  *expectedResult.rbegin() = sio::checksum(expectedResult.data() + 2, size); // CRC

  txIt = expectedResult.data();
  txEnd = txIt + expectedResult.size();

  testDeviceCommand(fileSystem);
}

void testFileSystemXexLoader() {
  sio::DiskDrive diskDrive(uart, busyWait);
  sio::FileSystem fileSystem(uart, busyWait, &diskDrive);

  testSelectDirEntry(&fileSystem, 0); // /builtin
  testSelectDirEntry(&fileSystem, 1); // /builtin/!sbc-boot.xex

  auto disk = media::makeAtr(fs::builtin::FileSystem::instance()->getRootDirectory()->openFile("!xex-loader.atr"));
  testDiskDriveRead(&diskDrive, disk);

  auto xexFile = fs::builtin::FileSystem::instance()->getRootDirectory()->openFile("!sbc-boot.xex");
  ::fs::File::size_type xexFileOffset = 2;

  // TODO: Check /builtin/!sbc-boot.xex is served.
  std::uint16_t segmentIndex = 0;
  testFileSystemReadXexSegmentEntry(&fileSystem, segmentIndex++, 0x02E0, 0x02E1);
  xexFileOffset += 4;
  std::uint16_t dataSize = (0x02E1 + 1) - 0x02E0;
  testFileSystemReadXexSegmentData(&fileSystem, xexFile, xexFileOffset, dataSize);
  xexFileOffset += dataSize;

  testFileSystemReadXexSegmentEntry(&fileSystem, segmentIndex++, 0x2000, 0x4BFB);
  xexFileOffset += 4;
  dataSize = (0x4BFB + 1) - 0x2000;
  testFileSystemReadXexSegmentData(&fileSystem, xexFile, xexFileOffset, dataSize);
  xexFileOffset += dataSize;

  testFileSystemReadXexSegmentEntryEOS(&fileSystem, segmentIndex++);
}

void testFileSystem() {
  testFileSystemGetCurrentDir();
  testFileSystemXexLoader();
}

} // namespace

int main(int, char**) {
  testDiskDrive();
  testFileSystem();
  testFileSystemStringTruncation();
  return EXIT_SUCCESS;
}

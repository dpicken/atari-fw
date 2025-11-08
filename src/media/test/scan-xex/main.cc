#include "media/XexEnumerator.h"

#include "fs/test/common/File.h"

#include <cstdlib>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace {

fs::File::ptr_type makeXexFile(const std::filesystem::path& xexPath) {
  constexpr auto blockSize = fs::BlockSize::fromSizeLog2(0);
  return fs::test::common::File::make(xexPath, blockSize);
}

std::string toString(const media::XexEntry::segment_header_type& segmentHeader) {
  std::ostringstream oss;
  oss << std::hex << std::uppercase << std::setfill('0')
      << "["
      << "0x" << std::setw(4) << segmentHeader.loadAddressBegin << ", "
      << "0x" << std::setw(4) << segmentHeader.loadAddressLast
      << "]"
      << " "
      << (segmentHeader.loadAddressLast < 0x02E0 || segmentHeader.loadAddressBegin > 0x02E1 ? '-' : 'R')
      << (segmentHeader.loadAddressLast < 0x02E2 || segmentHeader.loadAddressBegin > 0x02E3 ? '-' : 'I');
  return oss.str();
}

std::string toString(const media::XexEntry& entry, fs::File::size_type segmentOffset) {
  std::ostringstream oss;
  oss << std::setfill('0')
      << std::setw(4) << entry.index() << ": "
      << std::setw(8) << std::hex << segmentOffset << std::dec << ": "
      << toString(entry.segmentHeader());
  return oss.str();
}

void scanXex(const std::filesystem::path& xexPath) {
  auto xexFile = makeXexFile(xexPath);
  if (!xexFile) {
    throw std::invalid_argument(xexPath.string() + ": not found");
  }

  fs::File::size_type segmentOffset = 0;
  for (media::XexEnumerator xexEnumerator(media::Xex{xexFile}); xexEnumerator.isValid(); xexEnumerator.next()) {
    std::cout << toString(xexEnumerator.entry(), segmentOffset) << std::endl;
    segmentOffset = xexEnumerator.entry().segmentDataEnd();
  }

  if (segmentOffset == 0) {
    throw std::invalid_argument(xexPath.string() + ": no segments");
  }

  if (segmentOffset > xexFile->size()) {
    throw std::invalid_argument(xexPath.string() + ": final segment end (" + std::to_string(segmentOffset) + ") beyond EOF (" + std::to_string(xexFile->size()) + ")");
  }

  if (segmentOffset < xexFile->size()) {
    throw std::invalid_argument(xexPath.string() + ": final segment end (" + std::to_string(segmentOffset) + ") before EOF (" + std::to_string(xexFile->size()) + ")");
  }
}

} // namespace

int main(int argc, char** argv) {
  if (argc > 2) {
    std::cerr << "Usage: " << argv[0] << " xex-file" << std::endl;
    return EXIT_FAILURE;
  }

  const auto xexPath = (argc > 1) ? argv[1] : "builtin/!sbc-boot.xex";

  scanXex(xexPath);

  return EXIT_SUCCESS;
}

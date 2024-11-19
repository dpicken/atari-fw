#include "media/Atr.h"
#include "media/BuiltinAtrLibrary.h"

#include "fs/test/common/File.h"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <stdexcept>

namespace {

fs::File::ptr_type makeAtrFile(const std::filesystem::path& atrPath) {
  constexpr auto blockSize = fs::BlockSize::fromSizeLog2(0);
  return fs::test::common::File::make(atrPath, blockSize);
}

void testLoadAtr(const std::filesystem::path& atrPath) {
  auto atrFile = makeAtrFile(atrPath);
  if (!atrFile) {
    throw std::invalid_argument(atrPath.string() + ": not found");
  }

  if (!media::makeAtr(atrFile)) {
    throw std::logic_error(atrPath.string() + ": failed to load");
  }
}

} // namespace

int main(int argc, char** argv) {
  if (argc > 2) {
    std::cerr << "Usage: " << argv[0] << " [atr-file]" << std::endl;
    return EXIT_FAILURE;
  }

  const auto atrPath = (argc > 1) ? argv[1] : "atr/!sbc-boot.atr";

  testLoadAtr(atrPath);

  return EXIT_SUCCESS;
}

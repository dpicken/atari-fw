#ifndef sd_BlockSize_h
#define sd_BlockSize_h

#include "fs/BlockSize.h"

#include <cstddef>
#include <cstdint>

namespace sd {

static constexpr auto BlockSize = ::fs::BlockSize::fromSizeLog2(9);

} // namespace sd

#endif // ifndef sd_BlockSize_h

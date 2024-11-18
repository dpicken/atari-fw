#ifndef sd_Block_h
#define sd_Block_h

#include "BlockSize.h"

#include <array>
#include <cstdint>

namespace sd {

using Block = std::array<std::uint8_t, BlockSize.byteCount()>;

} // namespace sd

#endif // ifndef sd_Block_h

#ifndef media_Atr_h
#define media_Atr_h

#include "Disk.h"

#include <cstdint>
#include <memory>

namespace media {

  std::unique_ptr<Disk> makeAtr(const std::uint8_t* data, std::size_t size);

} // namespace media

#endif // ifndef media_Atr_h

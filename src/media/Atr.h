#ifndef media_Atr_h
#define media_Atr_h

#include "Disk.h"

#include "io/File.h"

#include <memory>

namespace media {

  std::unique_ptr<Disk> makeAtr(std::unique_ptr<::io::File> atrFile);

} // namespace media

#endif // ifndef media_Atr_h

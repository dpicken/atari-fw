#ifndef media_Atr_h
#define media_Atr_h

#include "Disk.h"

#include "fs/File.h"

#include <memory>

namespace media {

  std::unique_ptr<Disk> makeAtr(::fs::File::ptr_type atrFile);

} // namespace media

#endif // ifndef media_Atr_h

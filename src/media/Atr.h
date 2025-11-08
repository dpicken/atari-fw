#ifndef media_Atr_h
#define media_Atr_h

#include "Disk.h"

#include "fs/File.h"

#include <memory>

namespace media {

  Disk::ptr_type makeAtr(::fs::File::ptr_type atrFile);

} // namespace media

#endif // ifndef media_Atr_h

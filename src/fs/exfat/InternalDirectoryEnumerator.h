#ifndef fs_exfat_InternalDirectoryEnumerator_h
#define fs_exfat_InternalDirectoryEnumerator_h

#include "InternalDirectory.h"

#include "fs/Enumerator.h"

namespace fs { namespace exfat {

using InternalDirectoryEnumerator = Enumerator<InternalDirectory>;

} } // namespace fs::exfat

#endif // ifndef fs_exfat_InternalDirectoryEnumerator_h

#ifndef fs_exfat_LogicalDirectoryEnumerator_h
#define fs_exfat_LogicalDirectoryEnumerator_h

#include "LogicalDirectory.h"

#include "fs/Enumerator.h"

namespace fs { namespace exfat {

using LogicalDirectoryEnumerator = Enumerator<LogicalDirectory>;

} } // namespace fs::exfat

#endif // ifndef fs_exfat_LogicalDirectoryEnumerator_h

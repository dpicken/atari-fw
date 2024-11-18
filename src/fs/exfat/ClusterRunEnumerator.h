#ifndef fs_exfat_ClusterRunEnumerator_h
#define fs_exfat_ClusterRunEnumerator_h

#include "ClusterRun.h"

#include "fs/Enumerator.h"

namespace fs { namespace exfat {

using ClusterRunEnumerator = Enumerator<ClusterRun>;

} } // namespace fs::exfat

#endif // ifndef fs_exfat_ClusterRunEnumerator_h

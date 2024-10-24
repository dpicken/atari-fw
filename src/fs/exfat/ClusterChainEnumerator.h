#ifndef fs_exfat_ClusterChainEnumerator_h
#define fs_exfat_ClusterChainEnumerator_h

#include "ClusterChain.h"

#include "fs/Enumerator.h"

namespace fs { namespace exfat {

using ClusterChainEnumerator = ::fs::Enumerator<ClusterChain>;

} } // namespace fs::exfat

#endif // ifndef fs_exfat_ClusterChainEnumerator_h

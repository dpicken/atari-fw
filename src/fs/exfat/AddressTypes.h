#ifndef fs_exfat_AddressTypes_h
#define fs_exfat_AddressTypes_h

#include <cstdint>

namespace fs { namespace exfat {

using cluster_number = std::uint32_t;

using logical_cluster_number = cluster_number;
using virtual_cluster_number = cluster_number;

} } // namespace fs::exfat

#endif // ifndef fs_exfat_AddressTypes_h

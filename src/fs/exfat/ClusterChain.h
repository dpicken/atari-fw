#ifndef fs_exfat_ClusterChain_h
#define fs_exfat_ClusterChain_h

#include "AddressTypes.h"
#include "ClusterChainEntry.h"

#include "fs/File.h"

namespace fs { namespace exfat {

class ClusterChain {
public:
  using entry_type = ClusterChainEntry;
  using entry_enumeration_type = std::optional<entry_type>;

  ClusterChain();
  ClusterChain(const ::fs::File::ptr_type& fat, logical_cluster_number begin);

  entry_enumeration_type begin();
  entry_enumeration_type next(const entry_type& entry);

  const ::fs::File::ptr_type& fat() const;

private:
  const ::fs::File::ptr_type m_fat;
  const entry_enumeration_type m_begin;
};

} } // namespace fs::exfat

#endif // ifndef fs_exfat_ClusterChain_h

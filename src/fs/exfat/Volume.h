#ifndef fs_exfat_Volume_h
#define fs_exfat_Volume_h

#include "BootSector.h"

#include "fs/File.h"
#include "fs/FileSlice.h"

namespace fs { namespace exfat {

class Volume {
public:
  using file_ptr_type = File::ptr_type;
  using file_slice_ptr_type = FileSlice::impl_ptr_type;

  Volume(const file_ptr_type& volume);

  file_slice_ptr_type tryGetActiveFat() const;

  file_slice_ptr_type tryGetClusterHeap() const;

private:
  using boot_sector_type = std::optional<BootSector>;

  static boot_sector_type tryReadBootSector(const file_ptr_type& volume);
  static boot_sector_type tryReadBootSector(const file_ptr_type& volume, const file_slice_ptr_type& bootRegion);

  static file_slice_ptr_type tryGetMainBootRegion(const file_ptr_type& volume);
  static file_slice_ptr_type tryGetBackupBootRegion(const file_ptr_type& volume);
  static bool isBootRegionValid(const file_slice_ptr_type& bootRegion);

  const file_ptr_type m_volume;

  const boot_sector_type m_bootSector;
};

} } // namespace fs::exfat

#endif // ifndef fs_exfat_Volume_h

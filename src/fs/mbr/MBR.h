#ifndef fs_mbr_MBR_h
#define fs_mbr_MBR_h

#include "fs/Directory.h"
#include "fs/File.h"

namespace fs { namespace mbr {

::fs::Directory::ptr_type tryMakeDirectory(const ::fs::File::ptr_type& device);

} } // namespace fs::mbr

#endif // ifndef fs_mbr_MBR_h

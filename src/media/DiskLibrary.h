#ifndef media_DiskLibrary_h
#define media_DiskLibrary_h

#include "Disk.h"

#include <deque>
#include <memory>

namespace media {

class DiskLibrary {
public:
  using disk_ptr = std::unique_ptr<Disk>;

  DiskLibrary(const DiskLibrary&) = delete;
  DiskLibrary& operator=(const DiskLibrary) = delete;

  static DiskLibrary& instance();

  void reset();

  void push(disk_ptr disk);

  disk_ptr pop();

private:
  DiskLibrary();

  void addBuiltinAtrFileLibrary();

  std::deque<disk_ptr> m_disks;
};

} // namespace media

#endif // ifndef media_DiskLibrary_h

#ifndef fs_automount_Manager_h
#define fs_automount_Manager_h

#include "fs/Directory.h"
#include "fs/File.h"

#include <map>

namespace fs { namespace automount {

class Manager {
public:
  using file_ptr_type = ::fs::File::ptr_type;

  using name_type = ::fs::Directory::name_type;

  static Manager* instance();

  void onBlockDeviceAvailable(file_ptr_type device, name_type name);

  void onBlockDeviceUnavailable(const file_ptr_type& device);

private:
  Manager();

  using device_map = std::map<file_ptr_type, name_type>;

  device_map m_devices;
};

} } // namespace fs::automount

#endif // ifndef fs_automount_Manager_h

#ifndef fs_automount_Manager_h
#define fs_automount_Manager_h

#include "fs/FileSystem.h"

#include <map>
#include <vector>

namespace fs { namespace automount {

class Manager {
public:
  using file_ptr_type = ::fs::File::ptr_type;

  using name_type = ::fs::Directory::name_type;

  static Manager* instance();

  void onBlockDeviceAvailable(file_ptr_type device, name_type name);

  void onBlockDeviceUnavailable(const file_ptr_type& device);

private:
  using file_system_container = std::vector<FileSystem::ptr_type>;
  using device_map = std::map<file_ptr_type, std::pair<name_type, file_system_container>>;

  Manager();

  static file_system_container tryMakeFileSystems(const file_ptr_type& device);
  static void tryMakeFileSystem(const file_ptr_type& device, file_system_container& fileSystems);
  static ::fs::Directory::ptr_type makeDirectory(const file_system_container& fileSystems);

  device_map m_devices;
};

} } // namespace fs::automount

#endif // ifndef fs_automount_Manager_h

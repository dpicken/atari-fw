#include "Manager.h"

#include "fs/root/FileSystem.h"

fs::automount::Manager::Manager() = default;

fs::automount::Manager* fs::automount::Manager::instance() {
  static Manager instance;
  return &instance;
}

void fs::automount::Manager::onBlockDeviceAvailable(file_ptr_type device, name_type name) {
  auto [it, inserted] = m_devices.emplace(std::move(device), std::move(name));
  if (!inserted) {
    return;
  }

  ::fs::root::FileSystem::instance()->getRootDirectoryImpl()->put(it->second, it->first);
}

void fs::automount::Manager::onBlockDeviceUnavailable(const file_ptr_type& device) {
  auto it = m_devices.find(device);
  if (it == m_devices.end()) {
    return;
  }

  ::fs::root::FileSystem::instance()->getRootDirectoryImpl()->remove(it->second);
  m_devices.erase(it);
}

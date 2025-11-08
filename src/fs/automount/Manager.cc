#include "Manager.h"

#include "fs/DirectoryEnumerator.h"
#include "fs/exfat/Volume.h"
#include "fs/gpt/GPT.h"
#include "fs/mbr/MBR.h"
#include "fs/ram/Directory.h"
#include "fs/root/FileSystem.h"

fs::automount::Manager::Manager() = default;

fs::automount::Manager* fs::automount::Manager::instance() {
  static Manager instance;
  return &instance;
}

fs::automount::Manager::file_system_container fs::automount::Manager::tryMakeFileSystems(const file_ptr_type& device) {
  file_system_container fileSystems;

  for (DirectoryEnumerator dirEnumerator(::fs::mbr::tryMakeDirectory(device)); dirEnumerator.isValid(); dirEnumerator.next()) {
    tryMakeFileSystem(dirEnumerator.openFile(), fileSystems);
  }
  if (!fileSystems.empty()) {
    return fileSystems;
  }

  for (DirectoryEnumerator dirEnumerator(::fs::gpt::tryMakeDirectory(device)); dirEnumerator.isValid(); dirEnumerator.next()) {
    tryMakeFileSystem(dirEnumerator.openFile(), fileSystems);
  }
  if (!fileSystems.empty()) {
    return fileSystems;
  }

  tryMakeFileSystem(device, fileSystems);
  return fileSystems;
}

void fs::automount::Manager::tryMakeFileSystem(const file_ptr_type& device, file_system_container& fileSystems) {
  ::fs::exfat::Volume volume(device);
  auto fs = volume.tryMakeFileSystem();
  if (!fs) {
    return;
  }

  fileSystems.emplace_back(std::move(fs));
}

::fs::Directory::ptr_type fs::automount::Manager::makeDirectory(const file_system_container& fileSystems) {
  if (fileSystems.size() == 1) {
    return fileSystems.front()->getRootDirectory();
  }

  auto directory = ::fs::ram::Directory::make();
  for (auto& fileSystem : fileSystems) {
    directory->put(fileSystem->name(), fileSystem->getRootDirectory());
  }
  return directory;
}

void fs::automount::Manager::onBlockDeviceAvailable(file_ptr_type device, name_type name) {
  auto fileSystems = tryMakeFileSystems(device);

  if (fileSystems.empty()) {
    name += ": no supported file systems";
  }

  auto [it, inserted] = m_devices.emplace(std::move(device), std::make_pair(std::move(name), std::move(fileSystems)));
  if (!inserted) {
    return;
  }

  ::fs::root::FileSystem::instance()->getRootDirectoryImpl()->put(it->second.first, makeDirectory(it->second.second));
}

void fs::automount::Manager::onBlockDeviceUnavailable(const file_ptr_type& device) {
  auto it = m_devices.find(device);
  if (it == m_devices.end()) {
    return;
  }

  ::fs::root::FileSystem::instance()->getRootDirectoryImpl()->remove(it->second.first);

  m_devices.erase(it);
}

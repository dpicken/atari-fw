#ifndef fs_ResolvePath_h
#define fs_ResolvePath_h

#include "Directory.h"
#include "File.h"

#include <filesystem>

namespace fs {

Directory::ptr_type resolveDirectory(const std::filesystem::path& path);

File::ptr_type resolveFile(const std::filesystem::path& path);

} // namespace fs

#endif // ifndef fs_ResolvePath_h

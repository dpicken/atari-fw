#ifndef fs_gpt_GPT_h
#define fs_gpt_GPT_h

#include "fs/Directory.h"
#include "fs/File.h"

namespace fs { namespace gpt {

::fs::Directory::ptr_type tryMakeDirectory(const ::fs::File::ptr_type& device);

} } // namespace fs::gpt

#endif // ifndef fs_gpt_GPT_h

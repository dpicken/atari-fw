#ifndef fs_builtin_LibraryEnumerator_h
#define fs_builtin_LibraryEnumerator_h

#include "Library.h"

#include "fs/Enumerator.h"

namespace fs { namespace builtin {

using LibraryEnumerator = ::fs::Enumerator<const Library*>;

} } // namespace fs::builtin

#endif // ifndef fs_builtin_LibraryEnumerator_h

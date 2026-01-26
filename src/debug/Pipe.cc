#include "Pipe.h"

debug::Pipe::Pipe() {
}

debug::Pipe& debug::Pipe::instance() {
  static Pipe instance;
  return instance;
}

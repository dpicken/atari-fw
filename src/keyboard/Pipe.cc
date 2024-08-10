#include "Pipe.h"

keyboard::Pipe::Pipe() {
}

keyboard::Pipe& keyboard::Pipe::instance() {
  static Pipe instance;
  return instance;
}

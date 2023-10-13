#include "Pipe.h"

sio::Pipe::Pipe() {
}

sio::Pipe& sio::Pipe::instance() {
  static Pipe instance;
  return instance;
}

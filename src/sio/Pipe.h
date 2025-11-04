#ifndef sio_Pipe_h
#define sio_Pipe_h

#include "thread/Pipe.h"

namespace sio {

class Pipe {
public:
  static Pipe& instance();

  enum class Message : unsigned int {
    Null,
    SbcBoot,
    D1Eject,
    D1RotateDisk
  };

  bool tryPush(Message message) {
    return m_pipe.tryPush(message);
  }

  Message tryPop() {
    return m_pipe.tryPop();
  }

private:
  Pipe();

  ::thread::Pipe<Message> m_pipe;
};

} // namespace sio

#endif // ifndef sio_Pipe_h

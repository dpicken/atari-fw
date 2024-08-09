#ifndef keyboard_Pipe_h
#define keyboard_Pipe_h

#include "thread/Pipe.h"

namespace keyboard {

class Pipe {
public:
  static Pipe& instance();

  enum class Message : unsigned int {
    Null,
    PowerOff,
    PowerCycle,
    Reset
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

} // namespace keyboard

#endif // ifndef keyboard_Pipe_h

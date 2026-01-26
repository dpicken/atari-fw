#ifndef debug_Pipe_h
#define debug_Pipe_h

#include "thread/Pipe.h"

namespace debug {

class Pipe {
public:
  static Pipe& instance();

  enum class Message : unsigned int {
    Null,
    IndicateDebug
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

} // namespace debug

#endif // ifndef debug_Pipe_h
